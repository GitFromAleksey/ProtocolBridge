#include <string.h>
#include "UartInterfaceImpl.h"
#include "protocol.h"
#include "ProtocolDataStructures.h"
#include "../BLE/BleDataStructures.h"


// реализаци¤ интерфейса Interface.h
#define SEND_BUF_SIZE   1050u //
static uint8_t SendBuf[SEND_BUF_SIZE];

static void (*BleGetDataCallback)	(uint32_t ble_cmd_id, uint8_t *data);
static void (*UartErrorHandler)(t_uart_error_evt *evt);

// ----------------------------------------------------------------------------
void SendUartData(uint16_t cmd_id, uint8_t *data_guery)
{
	ProtocolDataStructuresSendDataFromBle(cmd_id, data_guery);
}
// ----------------------------------------------------------------------------
void MakeResponse_3231(uint8_t *data)
{
// формирование ответа 3231 для BLE 
  t_3231_ble_accept_parameters_command_response *ble_response =
    (t_3231_ble_accept_parameters_command_response *)data;

// поля одинаковые, поэетому просто копируем
  memcpy(ble_response, &ResponseData_3231, 
          sizeof(t_3231_ble_accept_parameters_command_response));
}
// ----------------------------------------------------------------------------
static void ConvertQuery_3230(uint32_t ble_cmd_id, uint8_t *data)
{
	if(CMD_ID_BLE_QUERY_3230 != ble_cmd_id)
		return;

	uint8_t tmp;
	t_3230_ble_set_parameters_query *ble_query = (t_3230_ble_set_parameters_query*)data;
	t_3230_set_parameters_query *uart_query = (t_3230_set_parameters_query *)SendBuf;

	memcpy(uart_query, ble_query, DATA_SIZE_3230);

	switch(ble_query->u16_status_bit_fld.OperationOfLightIndication)
	{
		case 0: // 00 - off
			uart_query->u_status_bit_field.u16_status_bit_fld.OperationOfLightIndication = 1;
			break;
		case 1: // 01 Ц lower limit;
			uart_query->u_status_bit_field.u16_status_bit_fld.OperationOfLightIndication = 0;
			break;
		case 2: // 10 Ц medium
			uart_query->u_status_bit_field.u16_status_bit_fld.OperationOfLightIndication = 2;
			break;
		case 3: // 11 Ц maximum
			uart_query->u_status_bit_field.u16_status_bit_fld.OperationOfLightIndication = 3;
			break;
		default:
			break;
	}

	tmp = 100/6;

	if(ble_query->FunSpeed < tmp)
		uart_query->FunSpeed = 0;
	else if( (ble_query->FunSpeed >= tmp) && (ble_query->FunSpeed < (2*tmp)) )
		uart_query->FunSpeed = 1;
	else if( (ble_query->FunSpeed >= (2*tmp)) && (ble_query->FunSpeed < (3*tmp)) )
		uart_query->FunSpeed = 2;
	else if( (ble_query->FunSpeed >= (3*tmp)) && (ble_query->FunSpeed < (4*tmp)) )
		uart_query->FunSpeed = 3;
	else if( (ble_query->FunSpeed >= (4*tmp)) && (ble_query->FunSpeed < (5*tmp)) )
		uart_query->FunSpeed = 4;
	else if( (ble_query->FunSpeed >= (5*tmp)) && (ble_query->FunSpeed < (6*tmp)) )
		uart_query->FunSpeed = 5;
	else if( ble_query->FunSpeed >= (6*tmp) )
		uart_query->FunSpeed = 6;

	uart_query->u_service_bit_field.u8_service_bit_field.ParingState = 0; // TODO нужно решить каким задавать этот параметр

	SendUartData( CMD_ID_3230, SendBuf);

  MakeResponse_3231(SendBuf);
  BleGetDataCallback(CMD_ID_BLE_RESPONSE_3231, SendBuf); // TODO реализовать ответ для BLE в его формате
}
// ----------------------------------------------------------------------------
static void ConvertQuery_3232(uint32_t ble_cmd_id, uint8_t *data)
{
  if(CMD_ID_BLE_QUERY_3232 != ble_cmd_id)
    return;

//  t_3232_ble_request_parameters_query *ble_query =
//    (t_3232_ble_request_parameters_query*)data;
//  t_3232_request_parameters_query *uart_query =
//    (t_3232_request_parameters_query *)SendBuf;

  SendUartData( CMD_ID_3232, NULL);

  MakeResponse_3231(SendBuf);
  BleGetDataCallback(CMD_ID_BLE_RESPONSE_3231, SendBuf);
}
// ----------------------------------------------------------------------------
static void ConvertQuery_3234(uint32_t ble_cmd_id, uint8_t *data)
{
  if(CMD_ID_BLE_QUERY_3234 != ble_cmd_id)
    return;

//  t_3234_ble_set_parameters_nonvolatile_mem_query *ble_query =
//    (t_3234_ble_set_parameters_nonvolatile_mem_query*)data;
//  t_3234_set_parameters_nonvolatile_mem_query *uart_query =
//    (t_3234_set_parameters_nonvolatile_mem_query *)SendBuf;

  SendUartData( CMD_ID_3232, NULL);

  MakeResponse_3231(SendBuf);
  BleGetDataCallback(CMD_ID_BLE_RESPONSE_3231, SendBuf);
}
// ----------------------------------------------------------------------------
// это тестовый запрос. По UART посылать ничего не надо. Нужно только ответить BLE
static void ConvertQuery_3132(uint32_t ble_cmd_id, uint8_t *data)
{
  if(CMD_ID_BLE_QUERY_3132 != ble_cmd_id)
    return;

//  t_3132_ble_test_query *ble_query =
//    (t_3132_ble_test_query*)data; // пустые данные

  t_3131_ble_test_response* ble_response = (t_3131_ble_test_response* )SendBuf;
  
  for(int i = 0; i < 1024; ++i)
  {
    ble_response->RandomeData[i] = i;
  }

  BleGetDataCallback(CMD_ID_BLE_RESPONSE_3131, SendBuf);
}
// ----------------------------------------------------------------------------
static void ConvertQuery_3332(uint32_t ble_cmd_id, uint8_t *data)
{
  if(CMD_ID_BLE_QUERY_3332 != ble_cmd_id)
    return;

//  t_3332_ble_request_information_on_device_query *ble_query =
//    (t_3332_ble_request_information_on_device_query*)data;
//    
//  t_3234_set_parameters_nonvolatile_mem_query *uart_query =
//    (t_3234_set_parameters_nonvolatile_mem_query *)SendBuf;

  SendUartData( CMD_ID_3232, NULL);

  t_3331_ble_receive_information_on_device_response * ble_response = 
    (t_3331_ble_receive_information_on_device_response *) SendBuf;

// копируем как есть так как структуры данных BLE и UART одинаковые
  memcpy(ble_response, &ResponseData_3331, 
          sizeof(t_3331_ble_receive_information_on_device_response));

  BleGetDataCallback(CMD_ID_BLE_RESPONSE_3331, SendBuf);
}
// ----------------------------------------------------------------------------
// тут ничего отсылать и отвечат не надо вроде
static void ConvertQuery_3630(uint32_t ble_cmd_id, uint8_t *data)
{
  if(CMD_ID_BLE_QUERY_3630 != ble_cmd_id)
    return;

  t_3630_ble_set_device_time_query *ble_query =
    (t_3630_ble_set_device_time_query*)data;

  t_3631_ble__response *ble_response =
    (t_3631_ble__response *)SendBuf;

  ble_response->DeviceTime = ble_query->device_time;

  BleGetDataCallback(CMD_ID_BLE_RESPONSE_3631, SendBuf);
}
// ----------------------------------------------------------------------------
static void ConvertQuery_3632(uint32_t ble_cmd_id, uint8_t *data)
{
  if(CMD_ID_BLE_QUERY_3632 != ble_cmd_id)
    return;

//  t_3632_ble_request_device_time_query *ble_query =
//    (t_3632_ble_request_device_time_query*)data; // тут дата пустая
    
  t_3631_ble__response *ble_response =
    (t_3631_ble__response *)SendBuf;

  ble_response->DeviceTime = 0;

  BleGetDataCallback(CMD_ID_BLE_RESPONSE_3631, SendBuf);
}
// ----------------------------------------------------------------------------
static void ConvertQuery_3530(uint32_t ble_cmd_id, uint8_t *data)
{
  if(CMD_ID_BLE_QUERY_3530 != ble_cmd_id)
    return;

  t_3530_ble_set_indication_and_auto_settings_query *ble_query =
    (t_3530_ble_set_indication_and_auto_settings_query *)data;
    
  t_3530_set_indication_and_auto_settings_response *uart_query =
    (t_3530_set_indication_and_auto_settings_response *)SendBuf;
    
  memcpy(uart_query, ble_query, DATA_SIZE_3530);
    
  SendUartData( CMD_ID_3530, SendBuf);

  t_3631_ble__response *ble_response = (t_3631_ble__response *)SendBuf;
// тут просто копируем принятые байты обратно в ответ
  memcpy(ble_response, uart_query, sizeof(t_3631_ble__response));

  BleGetDataCallback(CMD_ID_BLE_RESPONSE_3531, SendBuf);
}
// ----------------------------------------------------------------------------
static void ConvertQuery_3532(uint32_t ble_cmd_id, uint8_t *data)
{
  if(CMD_ID_BLE_QUERY_3532 != ble_cmd_id)
    return;

//  data // пустая
  
  SendUartData( CMD_ID_3532, SendBuf);

  t_3531_ble_request_indication_and_auto_settings_response *ble_request = 
  (t_3531_ble_request_indication_and_auto_settings_response *)SendBuf;

// просто копируем хранящиеся данные в ответ для BLE так как структуры одинаковые
  memcpy(ble_request, &ResponseData_3531, DATA_SIZE_3531);

  BleGetDataCallback(CMD_ID_BLE_RESPONSE_3531, SendBuf); // TODO возможно ответ должен быть такой
}
// ----------------------------------------------------------------------------
void UART_SetData (uint32_t ble_cmd_id, uint8_t *data)
{
	ConvertQuery_3230( ble_cmd_id, data);
	ConvertQuery_3232( ble_cmd_id, data);
	ConvertQuery_3234( ble_cmd_id, data);
	ConvertQuery_3132( ble_cmd_id, data);
	ConvertQuery_3332( ble_cmd_id, data);
	ConvertQuery_3630( ble_cmd_id, data);
	ConvertQuery_3632( ble_cmd_id, data);
	ConvertQuery_3530( ble_cmd_id, data);
	ConvertQuery_3532( ble_cmd_id, data);

	BleGetDataCallback(ble_cmd_id, data); // TODO тест
}
// ----------------------------------------------------------------------------
void UART_ErrorCallback(t_protocol_errors error)
{
  t_uart_error_evt evt;
  evt.test = 10;
  UartErrorHandler(&evt);

}
// ----------------------------------------------------------------------------
void UART_InterfaceInit(i_Interface *interface,
                        uint32_t (*get_time_ms)(void),
                        bool (*uart_get_byte)(uint8_t *data),
                        void (*uart_sent_data)(uint8_t *data, uint8_t size))
{
  t_protocol prot;

  prot.get_time_ms = get_time_ms;
  prot.uart_get_byte = uart_get_byte;
  prot.uart_sent_data = uart_sent_data;
  prot.uart_errorCallback = UART_ErrorCallback;

  ProtocolInit(&prot);

  interface->uartSendData = UART_SetData;
  interface->procRun = ProtocolRun;

  BleGetDataCallback = interface->bleGetDataCallback;
  UartErrorHandler = interface->uartErrorHandler;
}
// ----------------------------------------------------------------------------
