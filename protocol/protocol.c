#include "protocol.h"
#include "ProtocolDataStructures.h"



// ----------------------------------------------------------------------------
void ProtocolInit(t_protocol *prot)
{
	prot->find_start_of_packet = false;
	prot->rx_buf_cnt = RX_BUF_CNT_MAX;
}
// ----------------------------------------------------------------------------
uint8_t ProtocolCrcCalk(uint8_t *data, uint8_t size) // TODO реализавать
{
	uint8_t res = CRC8_TEST;

	return res;
}
// ----------------------------------------------------------------------------
bool ProtocolStructureFind(t_protocol *prot)
{
	bool res = false;
	t_uart_data_struct *p_uart_packet = NULL;

	if(prot->rx_buf_cnt > sizeof(t_uart_data_struct))
	{
		p_uart_packet = &prot->packet_buf[0];

		if(p_uart_packet->header == HEAD) // проверяем на всякий случай
		{
			switch(p_uart_packet->cmd_id)
			{
				case CMD_ID_3230:
					printf("Find CMD_ID_3230 = 0x%X\n", CMD_ID_3230);
					break;
				case CMD_ID_3231:
					printf("Find CMD_ID_3231 = 0x%X\n", CMD_ID_3231);
					break;
				case CMD_ID_3232:
					printf("Find CMD_ID_3232 = 0x%X\n", CMD_ID_3232);
					break;
				case CMD_ID_3234:
					printf("Find CMD_ID_3234 = 0x%X\n", CMD_ID_3234);
					break;
				case CMD_ID_3332:
					printf("Find CMD_ID_3332 = 0x%X\n", CMD_ID_3332);
					break;
				case CMD_ID_3331:
					printf("Find CMD_ID_3331 = 0x%X\n", CMD_ID_3331);
					break;
				case CMD_ID_3530:
					printf("Find CMD_ID_3530 = 0x%X\n", CMD_ID_3530);
					break;
				case CMD_ID_3532:
					printf("Find CMD_ID_3532 = 0x%X\n", CMD_ID_3532);
					break;
				case CMD_ID_3531:
					printf("Find CMD_ID_3531 = 0x%X\n", CMD_ID_3531);
					break;
				default:
					printf("CMD not find\n");
					break;
			}

			res = true;
		}
	}

	return res;
}
// ----------------------------------------------------------------------------
void ProtocolFindPacket(t_protocol *prot)
{
	uint8_t tmp = 0;
	static t_uart_data_struct *p_uart_packet = NULL;
	if(prot == NULL) return;

	if(prot->uart_get_byte(&tmp)) // найдено начало пакета
	{
		if(tmp == HEAD)
		{
			prot->rx_buf_cnt = 0;
			prot->packet_buf[prot->rx_buf_cnt] = tmp;
			prot->find_start_of_packet = true;
			++prot->rx_buf_cnt;

printf("\nнайдено начало пакета = %X;", prot->packet_buf[0]);
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
	uint8_t tmp = 0;
	if(prot == NULL) return;

	ProtocolFindPacket(prot);
}
// ----------------------------------------------------------------------------
