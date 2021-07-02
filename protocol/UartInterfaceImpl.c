#include <string.h>
#include "protocol\UartInterfaceImpl.h"
#include "protocol\protocol.h"
#include "protocol\ProtocolDataStructures.h"
#include "BleDataStructures.h"


// реализаци¤ интерфейса Interface.h
#define SEND_BUF_SIZE   1050u //
static uint8_t SendBuf[SEND_BUF_SIZE];

static i_Interface *m_Interface;
static t_protocol UART_Protocol;


// ----------------------------------------------------------------------------
void SendUartData(uint16_t cmd_id, uint8_t *data_guery)
{
  ProtocolDataStructuresSendDataFromBle(cmd_id, data_guery);
}
// ----------------------------------------------------------------------------
void SendEventBleData(uint32_t ble_cmd_id, uint8_t *data, uint16_t size) // TODO планируется взамен BleSendAnsverCallback
{
  t_uart_error_evt evt;
  
  evt.evt_id = EVT_DATA;
  evt.cmd_id = ble_cmd_id;
  evt.data = data;
  evt.data_size = size;
  
  m_Interface->uartErrorHandler(&evt);
} 
// ----------------------------------------------------------------------------
uint16_t MakeResponse(uint8_t *buf, uint32_t response_cmd_id, uint8_t *data,
                      uint16_t data_size)
{
  t_BleAnsver *ble_ansver = (t_BleAnsver *)buf;
  
  ble_ansver->response_cmd_id = response_cmd_id;
  
  memcpy(&ble_ansver->data, data, data_size);
  
  return (data_size + sizeof(ble_ansver->response_cmd_id));
}
// ----------------------------------------------------------------------------
uint16_t MakeResponse_3231(uint8_t *buf)
{
  return MakeResponse(buf, 0, (uint8_t*)&ResponseData_3231,
                      sizeof(t_3231_ble_accept_parameters_command_response));
}
// ----------------------------------------------------------------------------
static void ConvertQuery_3230(uint32_t ble_cmd_id, uint8_t *data)
{
  if(CMD_ID_BLE_QUERY_3230 != ble_cmd_id)
          return;

  uint8_t tmp;
  t_3230_ble_set_parameters_query *ble_query = 
                                        (t_3230_ble_set_parameters_query*)data;
  t_3230_set_parameters_query *uart_query = 
                                         (t_3230_set_parameters_query *)SendBuf;
  
  //memcpy(uart_query, ble_query, DATA_SIZE_3230);
  memcpy(uart_query, ble_query, sizeof(t_3230_ble_set_parameters_query));

//	union
//	{
//		uint16_t u16_status_word;
//		t_3230_uint16_status_bit_field u16_status_bit_fld;
//	} u_status_bit_field;
//
//	int8_t FunSpeed;
//	int8_t SleepTimerSettings;
//	uint16_t TimeLeftToReplaceFilter;
//	uint16_t TimeLaftToAntibacterialLayerExpire;
//
//	union
//	{
//		uint8_t u8_service_byte;
//		t_3230_uint8_service_bit_field u8_service_bit_field;
//	} u_service_bit_field;
        
//	switch(ble_query->u16_status_bit_fld.OperationOfLightIndication)
//	{
//		case 0: // 00 - off
//			uart_query->u_status_bit_field.u16_status_bit_fld.OperationOfLightIndication = 1;
//			break;
//		case 1: // 01 Ц lower limit;
//			uart_query->u_status_bit_field.u16_status_bit_fld.OperationOfLightIndication = 0;
//			break;
//		case 2: // 10 Ц medium
//			uart_query->u_status_bit_field.u16_status_bit_fld.OperationOfLightIndication = 2;
//			break;
//		case 3: // 11 Ц maximum
//			uart_query->u_status_bit_field.u16_status_bit_fld.OperationOfLightIndication = 3;
//			break;
//		default:
//			break;
//	}

//	tmp = 100/6;

//	if(ble_query->FunSpeed < tmp)
//		uart_query->FunSpeed = 0;
//	else if( (ble_query->FunSpeed >= tmp) && (ble_query->FunSpeed < (2*tmp)) )
//		uart_query->FunSpeed = 1;
//	else if( (ble_query->FunSpeed >= (2*tmp)) && (ble_query->FunSpeed < (3*tmp)) )
//		uart_query->FunSpeed = 2;
//	else if( (ble_query->FunSpeed >= (3*tmp)) && (ble_query->FunSpeed < (4*tmp)) )
//		uart_query->FunSpeed = 3;
//	else if( (ble_query->FunSpeed >= (4*tmp)) && (ble_query->FunSpeed < (5*tmp)) )
//		uart_query->FunSpeed = 4;
//	else if( (ble_query->FunSpeed >= (5*tmp)) && (ble_query->FunSpeed < (6*tmp)) )
//		uart_query->FunSpeed = 5;
//	else if( ble_query->FunSpeed >= (6*tmp) )
//		uart_query->FunSpeed = 6;

//	uart_query->u_service_bit_field.u8_service_bit_field.ParingState = 0; // TODO нужно решить каким задавать этот параметр

//        t_3234_set_parameters_nonvolatile_mem_query *_3234 = 
//          (t_3234_set_parameters_nonvolatile_mem_query *)uart_query;
          
  SendUartData( CMD_ID_3234, SendBuf);
        
  uint16_t data_len = MakeResponse_3231(SendBuf);
  SendEventBleData(CMD_ID_BLE_RESPONSE_3231, SendBuf, data_len);
}
// ----------------------------------------------------------------------------
static void ConvertQuery_3232(uint32_t ble_cmd_id, uint8_t *data)
{
  if(CMD_ID_BLE_QUERY_3232 != ble_cmd_id)
    return;

  uint16_t data_len = MakeResponse_3231(SendBuf);
  SendEventBleData(CMD_ID_BLE_RESPONSE_3231, SendBuf, data_len);
}
// ----------------------------------------------------------------------------
static void ConvertQuery_3234(uint32_t ble_cmd_id, uint8_t *data)
{
  if(CMD_ID_BLE_QUERY_3234 != ble_cmd_id)
    return;

  SendUartData( CMD_ID_3232, NULL);

  uint16_t data_len = MakeResponse_3231(SendBuf);
  SendEventBleData(CMD_ID_BLE_RESPONSE_3231, SendBuf, data_len);
}
// ----------------------------------------------------------------------------
// это тестовый запрос. По UART посылать ничего не надо. Нужно только ответить BLE
static void ConvertQuery_3132(uint32_t ble_cmd_id, uint8_t *data)
{
  if(CMD_ID_BLE_QUERY_3132 != ble_cmd_id)
    return;

  t_3131_ble_test_response* ble_response = (t_3131_ble_test_response* )SendBuf;
  
  for(int i = 0; i < 1024; ++i)
  {
    ble_response->RandomeData[i] = i;
  }

  uint16_t size = MakeResponse(SendBuf, 0, (uint8_t*)ble_response, 
                               sizeof(t_3131_ble_test_response));
  
  SendEventBleData(CMD_ID_BLE_RESPONSE_3131, SendBuf, size);
}
// ----------------------------------------------------------------------------
static void ConvertQuery_3332(uint32_t ble_cmd_id, uint8_t *data)
{
  if(CMD_ID_BLE_QUERY_3332 != ble_cmd_id)
    return;

  t_3331_ble_receive_information_on_device_response * ble_response = 
    (t_3331_ble_receive_information_on_device_response *) SendBuf;

// копируем как есть так как структуры данных BLE и UART одинаковые
  memcpy(ble_response, &ResponseData_3331, 
          sizeof(t_3331_ble_receive_information_on_device_response));

  uint16_t size = MakeResponse(SendBuf, 0, (uint8_t*)ble_response, 
                     sizeof(t_3331_ble_receive_information_on_device_response));

  SendEventBleData(CMD_ID_BLE_RESPONSE_3331, SendBuf, size);  
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

  uint16_t size = MakeResponse(SendBuf, 0, (uint8_t*)ble_response, 
                               sizeof(t_3631_ble__response));

  SendEventBleData(CMD_ID_BLE_RESPONSE_3631, SendBuf, size);
}
// ----------------------------------------------------------------------------
static void ConvertQuery_3632(uint32_t ble_cmd_id, uint8_t *data)
{
  if(CMD_ID_BLE_QUERY_3632 != ble_cmd_id)
    return;
    
  t_3631_ble__response *ble_response =
    (t_3631_ble__response *)SendBuf;

  ble_response->DeviceTime = 0;

  uint16_t size = MakeResponse(SendBuf, 0, (uint8_t*)ble_response, 
                               sizeof(t_3631_ble__response));

  SendEventBleData(CMD_ID_BLE_RESPONSE_3631, SendBuf, size);
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

  uint16_t size = MakeResponse(SendBuf, 0, (uint8_t*)ble_response, 
                               sizeof(t_3631_ble__response));

  SendEventBleData(CMD_ID_BLE_RESPONSE_3631, SendBuf, size);
}
// ----------------------------------------------------------------------------
static void ConvertQuery_3532(uint32_t ble_cmd_id, uint8_t *data)
{
  if(CMD_ID_BLE_QUERY_3532 != ble_cmd_id)
    return;

  uint16_t size = MakeResponse(SendBuf, 0, (uint8_t*)&ResponseData_3531, 
              sizeof(t_3531_ble_request_indication_and_auto_settings_response));
  
  SendEventBleData(CMD_ID_BLE_RESPONSE_3631, SendBuf, size);    
}
// ----------------------------------------------------------------------------
void UART_SendData (uint32_t ble_cmd_id, uint8_t *data)
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
}
// ----------------------------------------------------------------------------
void UART_ErrorCallback(t_protocol_errors error)
{
  t_uart_error_evt evt;

  evt.cmd_id = EVT_ERROR;
  
  m_Interface->uartErrorHandler(&evt);
}
// ----------------------------------------------------------------------------
void UART_Run(void)
{
  static uint8_t pair = 0;
  
  if(pair != ResponseData_3231.u_service_bit_fld.u8_service_bit_fld.Pairing)
  {
    if(pair == 0)
    {
      t_uart_error_evt evt;
      evt.evt_id = EVT_PAIRING;
      m_Interface->uartErrorHandler(&evt);
    }
    pair = ResponseData_3231.u_service_bit_fld.u8_service_bit_fld.Pairing;
  }
  
  ProtocolRun(&UART_Protocol);
}
// ----------------------------------------------------------------------------
void UART_InterfaceInit(i_Interface *interface,
                        uint32_t (*get_time_ms)(void),
                        bool (*uart_get_byte)(uint8_t *data),
                        void (*uart_sent_data)(uint8_t *data, uint8_t size))
{
  t_protocol init;

  init.get_time_ms = get_time_ms;
  init.uart_get_byte = uart_get_byte;
  init.uart_sent_data = uart_sent_data;
  init.uart_errorCallback = UART_ErrorCallback;

  ProtocolInit(&UART_Protocol, &init);

  interface->uartSendData = UART_SendData;
  interface->procRun = UART_Run;

  m_Interface = interface;
}
// ----------------------------------------------------------------------------
