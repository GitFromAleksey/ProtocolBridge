#include "protocol.h"
#include "ProtocolDataStructures.h"

#define HEAD	(uint8_t)0x23

#pragma pack(push, 1)
typedef struct
{
	uint8_t header;
	uint16_t cmd_id; // TODO ��� ���������� ��������� � ProtocolDataStructures ��� ��� ��� ��� ���� ���������������
	uint8_t first_data_byte;
} t_uart_data_struct;
#pragma pack(pop)

#define TX_BUF_SIZE	100u
static uint8_t tx_buf[TX_BUF_SIZE] = {0};

// ----------------------------------------------------------------------------
void ProtocolInit(t_protocol *prot)
{
	prot->find_start_of_packet = false;
	prot->rx_buf_cnt = RX_BUF_CNT_MAX;

	ProtocolDataStructuresInit();
}
// ----------------------------------------------------------------------------
uint8_t ProtocolCrcXorCalk(const uint8_t *data, uint8_t size)
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

		if(p_uart_packet->header == HEAD) // ��������� �� ������ ������. ����� � �� ���������
		{
			data_size = ProtocolDataStructuresGetDataSize(p_uart_packet->cmd_id);
			// ���������� ������� ������������ crc. ��� �� - ������ �������� ������
			data_size +=	sizeof(p_uart_packet->cmd_id) +
							sizeof(p_uart_packet->header);

			if(data_size == prot->rx_buf_cnt) // ����� �������� ������ ��������� � ����������� ������
			{
				crc = ((uint8_t*)p_uart_packet)[data_size];

				// ������� crc
				if(crc == ProtocolCrcXorCalk(&p_uart_packet->header, data_size))
				{
					// ��� ��� crc � �������, �� ���������� ��� ������� ��������� �������� ������
					ProtocolDataStructuresParse(&p_uart_packet->first_data_byte, p_uart_packet->cmd_id);
					res = true;
					printf("\ncrc = ok\n");
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

	if(prot->uart_get_byte(&tmp)) // ������� ������ ������
	{
		if(tmp == HEAD)
		{
			prot->rx_buf_cnt = 0;
			prot->packet_buf[prot->rx_buf_cnt] = tmp;
			prot->find_start_of_packet = true;
			++prot->rx_buf_cnt;
		}
	}

	if(prot->find_start_of_packet) // ���� ������, ��� ��� ������� ������ ������
	{
		while(prot->uart_get_byte(&tmp)) // �������� ������ ���� ����
		{
			prot->packet_buf[prot->rx_buf_cnt++] = tmp;

			if(ProtocolStructureFind(prot) || prot->rx_buf_cnt >= RX_BUF_CNT_MAX) // �������� ������ ������, ������ �� ���� ������
			{
				prot->rx_buf_cnt = 0;
				prot->find_start_of_packet = false; // ����� ������ ������
			}
		}
	}
}
// ----------------------------------------------------------------------------
void ProtocolPeriodicalRequestSend(t_protocol *prot) // TODO ��� �������� ������������ ������ �� ��������
{
	t_uart_data_struct *uart_data;

	uart_data = tx_buf;
	uart_data->header = HEAD;

	uint16_t tx_data_size = ProtocolDataStructuresGetNextRequest( &uart_data->cmd_id, TX_BUF_SIZE-sizeof(uart_data->header));

	printf("\ntx_data_size = %u", tx_data_size);

	uint16_t crc_position = tx_data_size + sizeof(uart_data->header);

	tx_buf[crc_position] = ProtocolCrcXorCalk(&uart_data->header, crc_position);

	tx_data_size = crc_position + 1;
	printf("\nheader = %X", uart_data->header);
	printf("\ncmd_id = %X", uart_data->cmd_id);
	printf("\ncrc = %X", tx_buf[crc_position]);
	printf("\nall bytes size = %u", tx_data_size);
	printf("\n");

	for(int i = 0; i < tx_data_size; ++i)
	{
		printf("%X:", tx_buf[i]);
	}

	prot->uart_sent_data(tx_buf, tx_data_size);
}
// ----------------------------------------------------------------------------
void ProtocolRun(t_protocol *prot)
{
	if(prot == NULL) return;

	//ProtocolFindPacket(prot);
	ProtocolPeriodicalRequestSend(prot);
}
// ----------------------------------------------------------------------------
