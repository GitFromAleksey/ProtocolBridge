#ifndef PROTOCOL_PROTOCOLDATASTRUCTURES_H_
#define PROTOCOL_PROTOCOLDATASTRUCTURES_H_

#include <stdint.h>


typedef enum // TODO cmd_id дл€ удобства нужно разделить на два энума дл€ query и request
{
	CMD_ID_3230 = (uint16_t)0x3230,
	CMD_ID_3231 = (uint16_t)0x3231,
	CMD_ID_3232 = (uint16_t)0x3232,
	CMD_ID_3234 = (uint16_t)0x3234,
	CMD_ID_3332 = (uint16_t)0x3332,
	CMD_ID_3331 = (uint16_t)0x3331,
	CMD_ID_3530 = (uint16_t)0x3530,
	CMD_ID_3532 = (uint16_t)0x3532,
	CMD_ID_3531 = (uint16_t)0x3531
} t_cmd_id;


// ---------------------------------------------------------------------
// RESPONSES
// ---------------------------------------------------------------------
typedef struct
{
	uint16_t DeviceOnOff				: 1;
	uint16_t SoundIndicationOnOff		: 1;
	uint16_t LightIndicationStatus		: 2;
	uint16_t ChildLock					: 1;
	uint16_t reserve_0					: 2;
	uint16_t SourseOfLastCommand		: 1;
	uint16_t FilterOveraged				: 1;
	uint16_t ControlViaMaFlag			: 1;
	uint16_t AutomaticModeFlag			: 1;
	uint16_t ActiveTimerFlag			: 1;
	uint16_t AntibacterialLayerExpire	: 1;
	uint16_t reserve					: 3;
} t_3231_uint16_status_bit_field;

typedef struct
{
	uint8_t Pairing		: 1;
	uint8_t reserved	: 7;
} t_3231_uint8_service_bit_field;

typedef struct
{
	t_3231_uint16_status_bit_field u16_status_bit_fld;

	int8_t MaximumNumbersOfSpeeds;
	int8_t FanSpeed;
	int8_t SleepTimerCurrentValue;
	int16_t PM2_5_CurrentValue;
	int16_t VOC_CurrentValues;
	int32_t HardwareRunningTimeCounter;
	int32_t FanRunningTimeCounter;
	int32_t FilterRunningTimeCounter;
	int32_t AntibacterialLayerTimeCounter;
	uint16_t FilterCCM;
	int32_t ErrorBitField;

	t_3231_uint8_service_bit_field u8_service_bit_fld;
} t_3231_accept_parameters_response;

#define DATA_SIZE_3231	(uint16_t)(sizeof(t_3231_accept_parameters_response))
// ---------------------------------------------------------------------
//#pragma pack(push, 1)
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
} t_3531_request_indication_and_auto_settings_response; // Request indication and Auto settings
//#pragma pack(pop)

#define DATA_SIZE_3531	(uint16_t)(sizeof(t_3531_request_indication_and_auto_settings_response))
// ---------------------------------------------------------------------
typedef struct
{
	int8_t ModeOfOperation;		// 1 Ц main application operating;
	int16_t DeviceType;			// 0x8012
	int16_t SubDeviceType;		// 0x0000 (int 16) - "Tion IQ 200", 0x0001 (int 16) - "Tion IQ 400"
	int16_t SoftwareVertion;	// 0x0000 .. 0xFFFF
	int16_t BoardVersion;		// 0x0000 .. 0xFFFF
	int8_t ReserveArray[16];	//
} t_3331_receive_information_on_device_response; // Receive information on device

#define DATA_SIZE_3331	(uint16_t)(sizeof(t_3331_receive_information_on_device_response))
// ---------------------------------------------------------------------
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
} t_3530_set_indication_and_auto_settings_response; // Set indication and Auto settings

#define DATA_SIZE_3530	(uint16_t)(sizeof(t_3530_set_indication_and_auto_settings_response))

// ---------------------------------------------------------------------
// QUERYES
// ---------------------------------------------------------------------
#pragma pack(push, 1)
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
} t_3230_uint16_status_bit_field;
#pragma pack(pop)

typedef struct
{
	uint8_t ParingState	: 2;
	uint8_t reserve		: 6;
} t_3230_uint8_service_bit_field;

#pragma pack(push, 1)
typedef struct
{
	union
	{
		uint16_t u16_status_word;
		t_3230_uint16_status_bit_field u16_status_bit_fld;
	};

	int8_t FunSpeed;
	int8_t SleepTimerSettings;
	uint16_t TimeLeftToReplaceFilter;
	uint16_t TimeLaftToAntibacterialLayerExpire;

	union
	{
		uint8_t u8_service_byte;
		t_3230_uint8_service_bit_field u8_service_bit_field;
	};
} t_3230_set_parameters_query; // "Set parameters" command.  After the command has been sent a Response command of "Accept parameters" must be received
#pragma pack(pop)

#define DATA_SIZE_3230	(uint16_t)(sizeof(t_3230_set_parameters_query))
// ---------------------------------------------------------------------
typedef struct
{
	// null data
} t_3232_request_parameters_query;

#define DATA_SIZE_3232	(uint16_t)(sizeof(t_3232_request_parameters_query))
// ---------------------------------------------------------------------
typedef struct
{
	// null data
} t_3234_set_parameters_nonvolatile_mem_query; // Set parameters to be saved in non-volatile memory

#define DATA_SIZE_3234	(uint16_t)(sizeof(t_3234_set_parameters_nonvolatile_mem_query))
// ---------------------------------------------------------------------
typedef struct // 23 32 33 DD
{
	// null data
} t_3332_request_information_on_device_query; // Request information on device

#define DATA_SIZE_3332	(uint16_t)(sizeof(t_3332_request_information_on_device_query))
// ---------------------------------------------------------------------
typedef struct
{
	// null data
} t_3532_request_indication_and_auto_settings_query;

#define DATA_SIZE_3532	(uint16_t)(sizeof(t_3532_request_indication_and_auto_settings_query))
// ---------------------------------------------------------------------

typedef struct // эта структура дл€ создани€ списка экземпл€ров структур данных
// это нужно дл€ реализации автоматического копировани€ данных в экземпл€ры
{
	t_cmd_id cmd_id; //
	uint16_t data_size;	// размер структуры данных
	void *p_data;	// указатель на структуру данных
	void *p_next_item;	// указатель на следующий объект
} t_data_list_struct;

// экземпл€ры структур дл€ хранени€ данных, полученых от UART устройства
extern t_3231_accept_parameters_response					ResponseData_3231;
extern t_3331_receive_information_on_device_response		ResponseData_3331;
extern t_3530_set_indication_and_auto_settings_response		ResponseData_3530;
extern t_3531_request_indication_and_auto_settings_response	ResponseData_3531;


int16_t ProtocolDataStructuresGetDataSize(uint16_t cmd_id);
void ProtocolDataStructuresParse(uint8_t *data, uint16_t cmd_id);
uint16_t ProtocolDataStructuresGetNextRequest(uint8_t *data, uint16_t size);
void ProtocolDataStructuresInit(void);

#endif /* PROTOCOL_PROTOCOLDATASTRUCTURES_H_ */
