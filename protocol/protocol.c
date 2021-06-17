#include "protocol.h"
#include "ProtocolDataStructures.h"



// ----------------------------------------------------------------------------
void ProtocolInit(t_protocol *prot)
{
	prot->find_start_of_packet = false;
	prot->rx_buf_cnt = RX_BUF_CNT_MAX;
}
// ----------------------------------------------------------------------------
uint8_t ProtocolCrcXorCalk(const uint8_t *data, uint8_t size) // TODO реализавать
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
bool ProtocolStructureFind(t_protocol *prot)
{
	bool res = false;
	int16_t data_size = 0;
	uint8_t crc = 0;
	t_uart_data_struct *p_uart_packet = NULL;

	if(prot->rx_buf_cnt > sizeof(t_uart_data_struct))
	{
		p_uart_packet = (t_uart_data_struct *)&prot->packet_buf[0];

		if(p_uart_packet->header == HEAD) // проверяем на всякий случай. можно и не проверять
		{
			data_size = ProtocolDataStructuresGetDataSize(p_uart_packet->cmd_id);
			// вычисление позиции расположения crc. оно же - размер принятых данных
			data_size +=	sizeof(p_uart_packet->cmd_id) +
							sizeof(p_uart_packet->header);

			if(data_size == prot->rx_buf_cnt) // длина принятых данных совпадает с вычисленной длиной
			{
				crc = ((uint8_t*)p_uart_packet)[data_size];

				// рассчёт crc
				if(crc == ProtocolCrcXorCalk(&p_uart_packet->header, data_size))
				{
					res = true;
					printf("crc = ok\n");
				}
			}
		}
	}

	return res;
}
// ----------------------------------------------------------------------------
void ProtocolFindPacket(t_protocol *prot)
{
	uint8_t tmp = 0;
	if(prot == NULL) return;

	if(prot->uart_get_byte(&tmp)) // найдено начало пакета
	{
		if(tmp == HEAD)
		{
			prot->rx_buf_cnt = 0;
			prot->packet_buf[prot->rx_buf_cnt] = tmp;
			prot->find_start_of_packet = true;
			++prot->rx_buf_cnt;
		}
	}

	if(prot->find_start_of_packet) // приём данных, так как найдено начало пакета
	{
		while(prot->uart_get_byte(&tmp)) // забираем данные пока есть
		{
			prot->packet_buf[prot->rx_buf_cnt++] = tmp;

			if(ProtocolStructureFind(prot) || prot->rx_buf_cnt >= RX_BUF_CNT_MAX) // превышен размер буфера, значит не было пакета
			{
				prot->rx_buf_cnt = 0;
				prot->find_start_of_packet = false; // поиск пакета заново
			}
		}
	}
}
// ----------------------------------------------------------------------------
void ProtocolRun(t_protocol *prot)
{
	if(prot == NULL) return;

	ProtocolFindPacket(prot);
}
// ----------------------------------------------------------------------------
