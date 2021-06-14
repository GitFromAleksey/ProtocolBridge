#include "uart.h"

typedef struct
{
	uint16_t cmd;
	uint8_t data_0;
	uint16_t data_1;
	uint32_t data_2;
	uint8_t crc;
} t_packet;


#define BUF_SIZE	40u

uint8_t buf_cnt = 0;
uint8_t buf[BUF_SIZE];

t_packet pack;

void UartInit(void)
{
	buf_cnt = 0;
	pack.cmd = 0x3132;
	pack.data_0 = 0x01;
	pack.data_1 = 0x0102;
	pack.data_2 = 0x01020304;
	pack.crc = 0xDD;

	for(int i = 0; i < BUF_SIZE; ++i)
	{
		buf[i] = i;
	}

	buf[2] = 0x23;
	*((t_packet*)&buf[3]) = pack;

}

bool UartRxDataAvailable(void)
{
	return true;
}

bool UartGetByte(uint8_t *data)
{
	*data = buf[buf_cnt];

	++buf_cnt;
	if(buf_cnt == BUF_SIZE)
	{
		buf_cnt = 0;
		return false;
	}
	else
		return true;
}
// ----------------------------------------------------------------------------
void UartSentData(uint8_t *data, uint8_t size)
{

}
// ----------------------------------------------------------------------------
