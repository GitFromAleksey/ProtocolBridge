#include "protocol.h"
#include "ProtocolDataStructures.h"

#define HEAD	(uint8_t)0x23

#define PERIODICAL_REQUEST_TIM_MS   100000u
//#define ANSVER_WAIT_TIMEOUT_VAL     PERIODICAL_REQUEST_TIM_MS * 10000

static uint32_t PreviousRequestTimeMs;

#pragma pack(push, 1)
typedef struct
{
	uint8_t header;
	uint16_t cmd_id; // TODO это желательно перенести в ProtocolDataStructures так как это его зона ответственности
	uint8_t first_data_byte;
} t_uart_data_struct;
#pragma pack(pop)

#define UART_DATA_STRUCT_SIZE   sizeof(t_uart_data_struct)

#define TX_BUF_SIZE   100u
static uint8_t tx_buf[TX_BUF_SIZE] = {0};

t_protocol UART_Protocol;

// ----------------------------------------------------------------------------
void ProtocolInit(t_protocol *prot)
{
  UART_Protocol.error_reg = NULL_ERROR;
  UART_Protocol.lost_data_counter = 0;

	PreviousRequestTimeMs = 0;

	UART_Protocol.find_start_of_packet = false;
	UART_Protocol.rx_buf_cnt = 0;

	UART_Protocol.get_time_ms = prot->get_time_ms;
	UART_Protocol.uart_get_byte = prot->uart_get_byte;
	UART_Protocol.uart_sent_data = prot->uart_sent_data;
  UART_Protocol.uart_errorCallback = prot->uart_errorCallback;

	ProtocolDataStructuresInit();
}
// ----------------------------------------------------------------------------
static uint8_t ProtocolCrcXorCalk(const uint8_t *data, uint8_t size)
{
	const uint8_t *pucStr = data;
	uint8_t usStartValue = 0xFF;

	if(size)
	{
		do
		{
		  usStartValue = usStartValue ^ *pucStr++;
		} while(--size);
	}

	return usStartValue;
}
// ----------------------------------------------------------------------------
static bool ProtocolStructureFind(t_protocol *prot)
{
	bool res = false;
	int16_t data_size = 0;
	uint8_t crc = 0;
	t_uart_data_struct *p_uart_packet = NULL;

	if(prot->rx_buf_cnt > UART_DATA_STRUCT_SIZE)
	{
		p_uart_packet = (t_uart_data_struct *)&prot->packet_buf[0];

		if(p_uart_packet->header == HEAD) // проверяем на всякий случай. можно и не проверять
		{
			data_size = ProtocolDataStructuresGetDataSize(p_uart_packet->cmd_id);
      if(data_size < 0) // неправильный cmd_id
        return true;
			// вычисление позиции расположения crc. оно же - размер принятых данных
			data_size += sizeof(p_uart_packet->cmd_id) +
							sizeof(p_uart_packet->header);

			if( prot->rx_buf_cnt == data_size ) // длина принятых данных совпадает с вычисленной длиной
			{
				crc = ((uint8_t*)p_uart_packet)[data_size-1];

				// рассчёт crc
				if(crc == ProtocolCrcXorCalk(&p_uart_packet->header, data_size-1))
				{
          UART_Protocol.lost_data_counter = 0;
					// так как crc в порядке, то отправляем для разбора структуры принятых данных
					ProtocolDataStructuresParse(&p_uart_packet->first_data_byte, p_uart_packet->cmd_id);
					res = true;
				}
			}
		}
	}

	return res;
}
// ----------------------------------------------------------------------------
static void ProtocolQueryReceive(t_protocol *prot)
{
	uint8_t tmp = 0;
	if(prot == NULL) return;

  if(!prot->find_start_of_packet)
  {
    if(prot->uart_get_byte(&tmp))
    {
      if(tmp == HEAD) // найдено начало пакета
      {
        prot->rx_buf_cnt = 0;
        prot->packet_buf[prot->rx_buf_cnt] = tmp;
        prot->find_start_of_packet = true;
        ++prot->rx_buf_cnt;
      }
    }
  }
  else
  {
    while(prot->uart_get_byte(&tmp)) // забираем данные пока есть
    {
      prot->packet_buf[prot->rx_buf_cnt++] = tmp;

      //if(ProtocolStructureFind(prot) || prot->rx_buf_cnt >= RX_BUF_CNT_MAX) // превышен размер буфера, значит не было пакета
      if(prot->rx_buf_cnt >= RX_BUF_CNT_MAX) // превышен размер буфера, значит не было пакета
      {
        prot->rx_buf_cnt = 0;
        prot->find_start_of_packet = false; // поиск пакета заново
      }
    }
    if(ProtocolStructureFind(prot)) // если пакет нашелся или это не пакет
    {
      prot->rx_buf_cnt = 0;
      prot->find_start_of_packet = false; // поиск пакета заново
    }
  }
}
// ----------------------------------------------------------------------------
static void ProtocolRequestSend(t_protocol *prot)
{
	t_uart_data_struct *uart_data;

	uart_data = (t_uart_data_struct*)tx_buf;
	uart_data->header = HEAD;

	uint16_t tx_data_size = ProtocolDataStructuresGetNextRequest( (uint8_t*)&uart_data->cmd_id, TX_BUF_SIZE-sizeof(uart_data->header));

	uint16_t crc_position = tx_data_size + sizeof(uart_data->header);

	tx_buf[crc_position] = ProtocolCrcXorCalk(&uart_data->header, crc_position);

	tx_data_size = crc_position + 1;

	prot->uart_sent_data(tx_buf, tx_data_size);
}
// ----------------------------------------------------------------------------
void ProtocolRun(void)
{
  uint32_t time_diff = UART_Protocol.get_time_ms() - PreviousRequestTimeMs;
  ProtocolQueryReceive(&UART_Protocol);

  if(UART_Protocol.lost_data_counter > 10)
  {
    if( (UART_Protocol.error_reg & LINK_ERROR) != LINK_ERROR)
    {
      UART_Protocol.error_reg |= LINK_ERROR;
      UART_Protocol.uart_errorCallback(LINK_ERROR);
    }
  }
  else
    UART_Protocol.error_reg &= ~LINK_ERROR;

  if( time_diff > PERIODICAL_REQUEST_TIM_MS )
  {
    PreviousRequestTimeMs = UART_Protocol.get_time_ms();
    ProtocolRequestSend(&UART_Protocol);
    ++UART_Protocol.lost_data_counter;
  }
}
// ----------------------------------------------------------------------------
