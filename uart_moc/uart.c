#include <string.h>
#include "uart.h"
#include "../protocol/ProtocolDataStructures.h"

#define HEAD	(uint8_t)0x23

#define BUF_SIZE	100u

static uint8_t buf_cnt = 0;
static uint8_t buf[BUF_SIZE];


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
void CopyDataToRxBuffer(uint8_t *rx_buf, uint16_t buf_size, t_cmd_id cmd_id, uint8_t *data, uint16_t data_size)
{
printf("cmd_id = 0x%X, data_size = %u\n", cmd_id, data_size);

	for(int i = 0; i < buf_size; ++i)
	{
		rx_buf[i] = 0;
	}


	uint16_t buf_cnt = 0;
	uint16_t tmp;
	uint8_t crc;

	rx_buf[buf_cnt] = HEAD;
	buf_cnt += sizeof(HEAD); // 1

	tmp = cmd_id;
	memcpy(&rx_buf[buf_cnt], &tmp, sizeof(tmp));
	buf_cnt += sizeof(tmp); // 2

	memcpy(&rx_buf[buf_cnt], &data, data_size);
	buf_cnt += data_size; // 44

	crc = CrcXorCalk(rx_buf, buf_cnt);
printf("crc = %X; crc position = %u\n", crc, buf_cnt);
	memcpy(&rx_buf[buf_cnt], &crc, sizeof(crc));

	for(int i = 0; i <= buf_cnt; ++i)
//		printf("%X(%u):", rx_buf[i], i);
		printf("%X:", rx_buf[i]);
}
// ----------------------------------------------------------------------------
void Receive_CMD_ID_3531(void)
{
	t_3531_request_indication_and_auto_settings_response Response3531 = {0};

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

	CopyDataToRxBuffer(buf, BUF_SIZE, CMD_ID_3531, (uint8_t *)&Response3531, DATA_SIZE_3531);
}
void Receive_CMD_ID_3530(void)
{
	t_3530_set_indication_and_auto_settings_response Response3530 = {0};
	CopyDataToRxBuffer(buf, BUF_SIZE, CMD_ID_3530, (uint8_t *)&Response3530, DATA_SIZE_3530);
}
void Receive_CMD_ID_3331(void)
{
	t_3331_receive_information_on_device_response Response3331 = {0};
	CopyDataToRxBuffer(buf, BUF_SIZE, CMD_ID_3331, (uint8_t *)&Response3331, DATA_SIZE_3331);
}
void Receive_CMD_ID_3231(void)
{
	t_3231_accept_parameters_response Response3231 = {0};
	CopyDataToRxBuffer(buf, BUF_SIZE, CMD_ID_3231, (uint8_t *)&Response3231, DATA_SIZE_3231);
}
// ----------------------------------------------------------------------------
void UartInit(void)
{
//	Receive_CMD_ID_3531();
//	Receive_CMD_ID_3530();
//	Receive_CMD_ID_3331();
	Receive_CMD_ID_3231();
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
