#include <string.h>
#include "uart.h"
#include "../protocol/ProtocolDataStructures.h"


#define BUF_SIZE	100u

uint8_t buf_cnt = 0;
uint8_t buf[BUF_SIZE];


t_3531_request_indication_and_auto_settings_response Response3531 = {0};

// ----------------------------------------------------------------------------
uint8_t CrcXorCalk(const uint8_t *data, uint8_t size)
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
void CopyDataToRxBuffer(uint8_t *rx_buf, uint16_t buf_size, uint16_t cmd_id, uint8_t *data, uint16_t data_size)
{
	for(int i = 0; i < buf_size; ++i)
	{
		rx_buf[i] = 0;
	}


	uint16_t buf_cnt = 0;
	uint16_t tmp;

	buf[buf_cnt] = HEAD;
	buf_cnt += sizeof(HEAD); // 1

	tmp = cmd_id;
	memcpy(&buf[buf_cnt], &tmp, sizeof(tmp));
	buf_cnt += sizeof(tmp); // 2

	memcpy(&buf[buf_cnt], &Response3531, sizeof(t_3531_request_indication_and_auto_settings_response));
	buf_cnt += sizeof(t_3531_request_indication_and_auto_settings_response); // 44

	tmp = CrcXorCalk(buf, buf_cnt);
	memcpy(&buf[buf_cnt], &tmp, sizeof(tmp));
}
// ----------------------------------------------------------------------------
void UartInit(void)
{
	Response3531.PM2_5_value_for_Red_indication = 0x3412;
	Response3531.PM2_5_value_for_Yellow_indication = 0x7856;
	Response3531.PM2_5_value_for_Green_indication = 0x1290;
	Response3531.PM2_5_value_for_Motor_Off = 0x5634;
	Response3531.PM2_5_value_for_1_speed = 0x9078;
	Response3531.PM2_5_value_for_2_speed = 0x3412;
	Response3531.PM2_5_value_for_3_speed = 0x7856;
	Response3531.PM2_5_value_for_4_speed = 0x1290;
	Response3531.PM2_5_value_for_5_speed = 0x5634;
	Response3531.PM2_5_value_for_6_speed = 0x9078;
	Response3531.PM2_5_hysteresis_setting = 0x3412;
	Response3531.PM2_5_measurement_time_setting = 0x7856;
	Response3531.VOC_level_for_Red_indication = 0x1290;
	Response3531.VOC_level_for_Yellow_indication = 0x5634;
	Response3531.VOC_level_for_Green_indication = 0x9078;
	Response3531.VOC_level_for_Motor_Off = 0x3412;
	Response3531.VOC_level_for_1_speed = 0x7856;
	Response3531.VOC_level_for_2_speed = 0x1290;
	Response3531.VOC_level_for_3_speed = 0x5634;
	Response3531.VOC_level_for_4_speed = 0x9078;
	Response3531.VOC_level_for_5_speed = 0x3412;
	Response3531.VOC_level_for_6_speed = 0x7856;


	CopyDataToRxBuffer(buf, BUF_SIZE, CMD_ID_3531, (uint8_t *)&Response3531, sizeof(t_3531_request_indication_and_auto_settings_response));

}
// ----------------------------------------------------------------------------
bool UartRxDataAvailable(void)
{
	return true;
}
// ----------------------------------------------------------------------------
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
