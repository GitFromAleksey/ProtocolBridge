#ifndef BLE_BLEDATASTRUCTURES_H_
#define BLE_BLEDATASTRUCTURES_H_

#include <stdint.h>

typedef enum
{
	CMD_ID_BLE_QUERY_3230 = (uint32_t)0x3230,
	CMD_ID_BLE_QUERY_3232 = (uint32_t)0x3232,
	CMD_ID_BLE_QUERY_3234 = (uint32_t)0x3234,
	CMD_ID_BLE_QUERY_3132 = (uint32_t)0x3132,
	CMD_ID_BLE_QUERY_3332 = (uint32_t)0x3332,
	CMD_ID_BLE_QUERY_3630 = (uint32_t)0x3630,
	CMD_ID_BLE_QUERY_3632 = (uint32_t)0x3632,
	CMD_ID_BLE_QUERY_3530 = (uint32_t)0x3530,
	CMD_ID_BLE_QUERY_3532 = (uint32_t)0x3532
} t_ble_cmd_id_query;

typedef enum
{
	CMD_ID_BLE_RESPONSE_3231 = (uint32_t)0x3231,
	CMD_ID_BLE_RESPONSE_3131 = (uint32_t)0x3131,
	CMD_ID_BLE_RESPONSE_3331 = (uint32_t)0x3331,
	CMD_ID_BLE_RESPONSE_3631 = (uint32_t)0x3631,
	CMD_ID_BLE_RESPONSE_3531 = (uint32_t)0x3531
} t_ble_cmd_id_response;

// ---------------------------------------------------------------------
// QUERYES
// --------------------------------------------------------------------------------
typedef struct
{
	uint16_t DeviceOnOff:							1;
	uint16_t SoundIndicationOnOff:					1;
	uint16_t OperationOfLightIndication:			2;
	uint16_t ChildLock:								1;
	uint16_t reserve_0:								2;
	uint16_t LastSourceOfCommand:					1;
	uint16_t FactorySettingsReset:					1;
	uint16_t ResetingOfErrorCounter:				1;
	uint16_t ResetingOfFilterReplaceTimeCounter:	1;
	uint16_t ControlViaMaFlag:						1;
	uint16_t AutomaticModeFlag:						1;
	uint16_t SleepTimerOperationFlag:				1;
	uint16_t reserve_1:								1;
	uint16_t reserve_2:								1;
} t_3230_ble_status_bit_field;

#pragma pack(push, 1)
typedef struct
{
	t_3230_ble_status_bit_field u16_status_bit_fld;

	int8_t FunSpeed;
	int8_t SleepTimerSettings;
	uint16_t TimeLeftToReplaceFilter;
	uint16_t TimeLaftToAntibacterialLayerExpire;

	uint16_t  Filter_CCM;
} t_3230_ble_set_parameters_query;
#pragma pack(pop)
// --------------------------------------------------------------------------------
#pragma pack(push, 1)
typedef struct
{
	void *p_void; // пустые данные для того, чтобы компилятор не ругался
} t_3232_ble_request_parameters_query;
#pragma pack(pop)
// --------------------------------------------------------------------------------
#pragma pack(push, 1)
typedef struct
{
	void *p_void; // пустые данные для того, чтобы компилятор не ругался
} t_3234_ble_set_parameters_nonvolatile_mem_query;
#pragma pack(pop)
// --------------------------------------------------------------------------------
#pragma pack(push, 1)
typedef struct
{
	void *p_void; // пустые данные для того, чтобы компилятор не ругался
} t_3132_ble_test_query;
#pragma pack(pop)
// --------------------------------------------------------------------------------
#pragma pack(push, 1)
typedef struct
{
	void *p_void; // пустые данные для того, чтобы компилятор не ругался
} t_3332_ble_request_information_on_device_query;
#pragma pack(pop)
// --------------------------------------------------------------------------------
#pragma pack(push, 1)
typedef struct
{
	uint64_t device_time;
} t_3630_ble_set_device_time_query;
#pragma pack(pop)
// --------------------------------------------------------------------------------
#pragma pack(push, 1)
typedef struct
{
	void *p_void; // пустые данные для того, чтобы компилятор не ругался
} t_3632_ble_request_device_time_query;
#pragma pack(pop)
// --------------------------------------------------------------------------------
#pragma pack(push, 1)
typedef struct
{
	uint16_t PM2_5_value_for_Red_indication;
	uint16_t PM2_5_value_for_Yellow_indication;
	uint16_t PM2_5_value_for_Green_indication;
	uint16_t PM2_5_value_for_Motor_Off;
	uint16_t PM2_5_value_for_1_speed;
	uint16_t PM2_5_value_for_2_speed;
	uint16_t PM2_5_value_for_3_speed;
	uint16_t PM2_5_value_for_4_speed;
	uint16_t PM2_5_value_for_5_speed;
	uint16_t PM2_5_value_for_6_speed;
	uint16_t PM2_5_hysteresis_setting;
	uint16_t PM2_5_measurement_time_setting;
	uint16_t VOC_level_for_Red_indication;
	uint16_t VOC_level_for_Yellow_indication;
	uint16_t VOC_level_for_Green_indication;
	uint16_t VOC_level_for_Motor_Off;
	uint16_t VOC_level_for_1_speed;
	uint16_t VOC_level_for_2_speed;
	uint16_t VOC_level_for_3_speed;
	uint16_t VOC_level_for_4_speed;
	uint16_t VOC_level_for_5_speed;
	uint16_t VOC_level_for_6_speed;
} t_3530_ble_set_indication_and_auto_settings_query;
#pragma pack(pop)
// --------------------------------------------------------------------------------
#pragma pack(push, 1)
typedef struct
{
	void *p_void; // пустые данные для того, чтобы компилятор не ругался
} t_3532_ble_request_indication_and_auto_settings_query;
#pragma pack(pop)
// ---------------------------------------------------------------------
// RESPONSES
// ---------------------------------------------------------------------

typedef struct
{
  uint16_t DeviceOnOff                : 1;
  uint16_t SoundIndicationOnOff       : 1;
  uint16_t LightIndicationStatus      : 2;
  uint16_t ChildLock                  : 1;
  uint16_t reserve_0                  : 2;
  uint16_t SourceOfLastCommand        : 1;
  uint16_t IsFilterOveraged           : 1;
  uint16_t IsControlViaMA             : 1;
  uint16_t IsAutomaticMode            : 1;
  uint16_t IsActiveTimer              : 1;
  uint16_t IsAntibacterialLayerExpire : 1;
  uint16_t reserve_1                  : 3;
} t_3231_ble_status_bit_field;

#pragma pack(push, 1)
typedef struct
{
	union
	{
		uint16_t                    u16_status_word;
		t_3231_ble_status_bit_field u16_status_bit_field;
	} u_status_bit_field;
  
  int8_t    MaximumNumberOfSpeeds;
  int8_t    FanSpeed;
  int8_t    SleepTimerCurrentValue;
  int16_t   PM2_5_CurrentValues;
  int16_t   VOC_CurrentValues;
  int32_t   HardwareRunningTimeCounter;
  int32_t   FanRunningTimeCounter;
  int32_t   FilterRunningTimeCounter;
  int32_t   AntibacterialLayerTimeCounter;
  uint16_t  Filter_CCM;
  int32_t   ErrorBitField;
} t_3231_ble_accept_parameters_command_response;
#pragma pack(pop)
// ---------------------------------------------------------------------
#pragma pack(push, 1)
typedef struct
{
	uint8_t RandomeData[1024]; // Random test data of 1024 byte.
} t_3131_ble_test_response;
#pragma pack(pop)
// ---------------------------------------------------------------------
#pragma pack(push, 1)
typedef struct
{
	int8_t ModeOfOperation;		// 1 � main application operating;
	int16_t DeviceType;			// 0x8012
	int16_t SubDeviceType;		// 0x0000 (int 16) - "Tion IQ 200", 0x0001 (int 16) - "Tion IQ 400"
	int16_t SoftwareVertion;	// 0x0000 .. 0xFFFF
	int16_t BoardVersion;		// 0x0000 .. 0xFFFF
	int8_t ReserveArray[16];	//
} t_3331_ble_receive_information_on_device_response;
#pragma pack(pop)
// ---------------------------------------------------------------------
#pragma pack(push, 1)
typedef struct
{
	uint64_t DeviceTime; // Time in unix-time format
} t_3631_ble__response;
#pragma pack(pop)
// ---------------------------------------------------------------------
#pragma pack(push, 1)
typedef struct
{
	uint16_t PM2_5_value_for_Red_indication;
	uint16_t PM2_5_value_for_Yellow_indication;
	uint16_t PM2_5_value_for_Green_indication;
	uint16_t PM2_5_value_for_Motor_Off;
	uint16_t PM2_5_value_for_1_speed;
	uint16_t PM2_5_value_for_2_speed;
	uint16_t PM2_5_value_for_3_speed;
	uint16_t PM2_5_value_for_4_speed;
	uint16_t PM2_5_value_for_5_speed;
	uint16_t PM2_5_value_for_6_speed;
	uint16_t PM2_5_hysteresis_setting;
	uint16_t PM2_5_measurement_time_setting;
	uint16_t VOC_level_for_Red_indication;
	uint16_t VOC_level_for_Yellow_indication;
	uint16_t VOC_level_for_Green_indication;
	uint16_t VOC_level_for_Motor_Off;
	uint16_t VOC_level_for_1_speed;
	uint16_t VOC_level_for_2_speed;
	uint16_t VOC_level_for_3_speed;
	uint16_t VOC_level_for_4_speed;
	uint16_t VOC_level_for_5_speed;
	uint16_t VOC_level_for_6_speed;
} t_3531_ble_request_indication_and_auto_settings_response;
#pragma pack(pop)
// ---------------------------------------------------------------------
// структура для ответа для BLE
#pragma pack(push, 1)
typedef struct
{
  uint32_t response_cmd_id; // идентификатор пакета, присланный в запросе от BLE(пока не используется)
  uint8_t data; // указатель на структуру данных ответа для BLE
} t_BleAnsver;
#pragma pack(pop)

#endif /* BLE_BLEDATASTRUCTURES_H_ */
