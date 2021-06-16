#include <string.h>
#include "uart.h"
#include "../protocol/ProtocolDataStructures.h"


#define BUF_SIZE	100u

uint8_t buf_cnt = 0;
uint8_t buf[BUF_SIZE];


t_3531_request_indication_and_auto_settings_response Response3531 = {0};



void UartInit(void)
{
	int i = 1;
	uint16_t tmp;

	Response3531.PM2_5_value_for_Red_indication = 0x1112;
	Response3531.PM2_5_value_for_Yellow_indication = 0x1314;
	Response3531.PM2_5_hysteresis_setting = 1;
	Response3531.PM2_5_measurement_time_setting = 2;
	Response3531.PM2_5_value_for_Motor_Off = 0x13;
	Response3531.VOC_level_for_6_speed = 0x1516;


	for(int i = 0; i < BUF_SIZE; ++i)
	{
		buf[i] = 0;
	}

	buf[i] = HEAD;
	i += sizeof(HEAD);

	tmp = CMD_ID_3531;
	memcpy(&buf[i], &tmp, sizeof(tmp));
	i += sizeof(tmp);

	memcpy(&buf[i], &Response3531, sizeof(t_3531_request_indication_and_auto_settings_response));
	i += sizeof(t_3531_request_indication_and_auto_settings_response);

	tmp = 0xDD; // crc
	memcpy(&buf[i], &tmp, sizeof(tmp));

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
