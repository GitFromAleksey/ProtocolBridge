#include "protocol.h"



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
	uint8_t rx_data_len = prot->rx_buf_cnt;
	t_packet *packet;

	if(rx_data_len == T_PAKET_LEN)
	{
		packet = (t_packet *)&prot->packet_buf[0];

		if(packet->cmd == CMD_0)
		{
			if(packet->crc == ProtocolCrcCalk(&prot->packet_buf[0], T_PAKET_LEN-CRC8_LEN))
			{
				printf("packet->cmd = %X\n", packet->cmd);
				printf("packet->data_0 = %X\n", packet->data_0);
				printf("packet->data_1 = %X\n", packet->data_1);
				printf("packet->data_2 = %X\n", packet->data_2);
				printf("packet->crc = %X\n", packet->crc);
				return true;
			}
		}
	}

	return false;
}
// ----------------------------------------------------------------------------
void ProtocolRun(t_protocol *prot)
{
	uint8_t tmp = 0;
	if(prot == NULL) return;

	if(prot->uart_get_byte(&tmp))
	{
		if(tmp == PACKET_START) // найдено начало пакета
		{
			prot->find_start_of_packet = true;
			prot->rx_buf_cnt = 0;
		}
	}

	if(prot->find_start_of_packet) // приём данных, так как найдено начало пакета
	{
		while(prot->uart_get_byte(&tmp))
		{
			prot->packet_buf[prot->rx_buf_cnt++] = tmp;

			if(prot->rx_buf_cnt >= RX_BUF_CNT_MAX) // превышен размер буфера, значит не было пакета
			{
				prot->rx_buf_cnt = 0;
				prot->find_start_of_packet = false; // поиск пакета заново
				return;
			}

			if(ProtocolStructureFind(prot)) // поиск данных в пакете
			{

			}
		}
	}
}
// ----------------------------------------------------------------------------
