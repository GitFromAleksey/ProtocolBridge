#include <string.h>
#include "uart.h"
#include "../protocol/ProtocolDataStructures.h"


#define BUF_SIZE	100u

uint8_t buf_cnt = 0;
uint8_t buf[BUF_SIZE];


t_3531_request_indication_and_auto_settings_response Response3531 = {0};



void UartInit(void)
{
	int i = 0;
	uint16_t tmp;

//	Response3531.PM2_5_value_for_Red_indication = 0x1234;
//	Response3531.PM2_5_value_for_Yellow_indication = 0x5678;
//	Response3531.PM2_5_value_for_Green_indication = 0x9012;
//	Response3531.PM2_5_value_for_Motor_Off = 0x3456;
//	Response3531.PM2_5_value_for_1_speed = 0x7890;
//	Response3531.PM2_5_value_for_2_speed = 0x1234;
//	Response3531.PM2_5_value_for_3_speed = 0x5678;
//	Response3531.PM2_5_value_for_4_speed = 0x9012;
//	Response3531.PM2_5_value_for_5_speed = 0x3456;
//	Response3531.PM2_5_value_for_6_speed = 0x7890;
//	Response3531.PM2_5_hysteresis_setting = 0x1234;
//	Response3531.PM2_5_measurement_time_setting = 0x5678;
//	Response3531.VOC_level_for_Red_indication = 0x9012;
//	Response3531.VOC_level_for_Yellow_indication = 0x3456;
//	Response3531.VOC_level_for_Green_indication = 0x7890;
//	Response3531.VOC_level_for_Motor_Off = 0x1234;
//	Response3531.VOC_level_for_1_speed = 0x5678;
//	Response3531.VOC_level_for_2_speed = 0x9012;
//	Response3531.VOC_level_for_3_speed = 0x3456;
//	Response3531.VOC_level_for_4_speed = 0x7890;
//	Response3531.VOC_level_for_5_speed = 0x1234;
//	Response3531.VOC_level_for_6_speed = 0x5678;

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


	for(int i = 0; i < BUF_SIZE; ++i)
	{
		buf[i] = 0;
	}

	buf[i] = HEAD;
	i += sizeof(HEAD); // 1

	tmp = CMD_ID_3531;
	memcpy(&buf[i], &tmp, sizeof(tmp));
	i += sizeof(tmp); // 2
printf("uart: sizeof(t_3531_request_indication_and_auto_settings_response) = %u\n", sizeof(t_3531_request_indication_and_auto_settings_response));
	memcpy(&buf[i], &Response3531, sizeof(t_3531_request_indication_and_auto_settings_response));
	i += sizeof(t_3531_request_indication_and_auto_settings_response); // 44

	tmp = 0xDD; // crc
	memcpy(&buf[i], &tmp, sizeof(tmp));

	for(int ii = 0; ii < (i+1); ++ii)
		printf("%X(%u):",buf[ii],ii);
	printf("\n");
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
