#include "protocol.h"
#include "ProtocolDataStructures.h"

#define HEAD	(uint8_t)0x23

#define PERIODICAL_REQUEST_TIM_MS   500u // пауза между отправкой запросов
//#define ANSVER_WAIT_TIMEOUT_VAL     PERIODICAL_REQUEST_TIM_MS * 10000
//uint16_t CUR_CMD_ID = 0; // временная для проверки
//uint16_t ANS_CMD_ID = 0; // временная для проверки

static uint32_t PreviousRequestTimeMs;

#pragma pack(push, 1)
typedef struct
{
	uint8_t header;
	uint16_t cmd_id; // это желательно перенести в ProtocolDataStructures так как это его зона ответственности
	uint8_t first_data_byte;
} t_uart_data_struct;
#pragma pack(pop)

#define UART_DATA_STRUCT_SIZE   sizeof(t_uart_data_struct)

#define TX_BUF_SIZE   100u
static uint8_t tx_buf[TX_BUF_SIZE] = {0};

//t_protocol UART_Protocol; // перенесено в protocol.c

// ----------------------------------------------------------------------------
void ProtocolInit(t_protocol *uart_protocol, t_protocol *init)
{
  uart_protocol->error_reg = NULL_ERROR;
  uart_protocol->lost_data_counter = 0;

  PreviousRequestTimeMs = 0;

  uart_protocol->find_start_of_packet = false;
  uart_protocol->rx_buf_cnt = 0;

  uart_protocol->get_time_ms = init->get_time_ms;
  uart_protocol->uart_get_byte = init->uart_get_byte;
  uart_protocol->uart_sent_data = init->uart_sent_data;
  uart_protocol->uart_errorCallback = init->uart_errorCallback;

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
//		ANS_CMD_ID = p_uart_packet->cmd_id;
      data_size = ProtocolDataStructuresGetDataSize(p_uart_packet->cmd_id);
      if(data_size < 0) // неправильный cmd_id
        return true;
      // вычисление позиции расположения crc. оно же - размер принятых данных
      data_size += sizeof(p_uart_packet->cmd_id) +
                                      sizeof(p_uart_packet->header);

      if( prot->rx_buf_cnt == data_size+1 ) // длина принятых данных совпадает с вычисленной длиной
      {
        crc = ((uint8_t*)p_uart_packet)[data_size];

        // рассчёт crc
        if(crc == ProtocolCrcXorCalk(&p_uart_packet->header, data_size))
        {
          prot->lost_data_counter = 0;
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
//CUR_CMD_ID = uart_data->cmd_id;
//ANS_CMD_ID = 0;
  uint16_t crc_position = tx_data_size + sizeof(uart_data->header);

  tx_buf[crc_position] = ProtocolCrcXorCalk(&uart_data->header, crc_position);

  tx_data_size = crc_position + 1;

  prot->uart_sent_data(tx_buf, tx_data_size);
}
// ----------------------------------------------------------------------------
void ProtocolRun(t_protocol *prot)
{
  uint32_t time_diff = prot->get_time_ms() - PreviousRequestTimeMs;
  ProtocolQueryReceive(prot);

  if(prot->lost_data_counter > 10)
  {
    if( (prot->error_reg & LINK_ERROR) != LINK_ERROR)
    {
      prot->error_reg |= LINK_ERROR;
      prot->uart_errorCallback(LINK_ERROR);
    }
  }
  else
    prot->error_reg &= ~LINK_ERROR;

  if( time_diff > PERIODICAL_REQUEST_TIM_MS )
  {
    PreviousRequestTimeMs = prot->get_time_ms();
    ProtocolRequestSend(prot);
    ++prot->lost_data_counter;
  }
}
// ----------------------------------------------------------------------------
