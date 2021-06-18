#ifndef BLE_BLEDATASTRUCTURES_H_
#define BLE_BLEDATASTRUCTURES_H_

#include <stdint.h>

typedef enum
{
	CMD_ID_BLE_QUERY_3230 = (uint16_t)0x3230,
	CMD_ID_BLE_QUERY_3232 = (uint16_t)0x3232,
	CMD_ID_BLE_QUERY_3234 = (uint16_t)0x3234,
	CMD_ID_BLE_QUERY_3132 = (uint16_t)0x3132,
	CMD_ID_BLE_QUERY_3332 = (uint16_t)0x3332,
	CMD_ID_BLE_QUERY_3630 = (uint16_t)0x3630,
	CMD_ID_BLE_QUERY_3632 = (uint16_t)0x3632,
	CMD_ID_BLE_QUERY_3530 = (uint16_t)0x3530,
	CMD_ID_BLE_QUERY_3532 = (uint16_t)0x3532
} t_ble_cmd_id_query;

typedef enum
{
	CMD_ID_BLE_RESPONSE_3231 = (uint16_t)0x3231,
	CMD_ID_BLE_RESPONSE_3131 = (uint16_t)0x3131,
	CMD_ID_BLE_RESPONSE_3331 = (uint16_t)0x3331,
	CMD_ID_BLE_RESPONSE_3631 = (uint16_t)0x3631,
	CMD_ID_BLE_RESPONSE_3531 = (uint16_t)0x3531
} t_ble_cmd_id_response;


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

typedef struct
{
	t_3231_uint16_status_bit_field u16_status_bit_fld;

	int8_t FunSpeed;
	int8_t SleepTimerSettings;
	uint16_t TimeLeftToReplaceFilter;
	uint16_t TimeLaftToAntibacterialLayerExpire;

	uint16_t  Filter_CCM;
} t_3230_ble_set_parameters_queri;
// --------------------------------------------------------------------------------
typedef struct
{
	// null data
} t_3232_ble_request_parameters_queri;
// --------------------------------------------------------------------------------
typedef struct
{
	// null data
} t_3234_ble_set_parameters_nonvolatile_mem_query;
// --------------------------------------------------------------------------------
typedef struct
{
	// null data
} t_3132_ble_test_query;
// --------------------------------------------------------------------------------
typedef struct
{
	// null data
} t_3332_ble_request_information_on_device_query;
// --------------------------------------------------------------------------------
typedef struct
{
	uint64_t device_time;
} t_3630_ble_set_device_time_query;
// --------------------------------------------------------------------------------
typedef struct
{
	// null data
} t_3632_ble_request_device_time_query;
// --------------------------------------------------------------------------------
typedef struct
{
	// TODO дописать
} t_3530_ble__query;
// --------------------------------------------------------------------------------



#endif /* BLE_BLEDATASTRUCTURES_H_ */
