#include <string.h>
#include "UartInterfaceImpl.h"
#include "protocol.h"
#include "ProtocolDataStructures.h"
#include "../BLE/BleDataStructures.h"
//#include "BLE формат данных"

//#include "UART формат данных"
//#include "UART доступ к хранилищу данных"
//#include "UART доступ к очереди отправки данных"

// реализаци¤ интерфейса Interface.h
#define SEND_BUF_SIZE   100u
static uint8_t SendBuf[SEND_BUF_SIZE];

static void (*BleGetDataCallback)	(uint32_t ble_cmd_id, uint8_t *data);
static void (*UartErrorHandler)(t_uart_error_evt *evt);

// ----------------------------------------------------------------------------
void SendUartData(uint16_t cmd_id, uint8_t *data_guery)
{
	ProtocolDataStructuresSendDataFromBle(cmd_id, data_guery);
}
// ----------------------------------------------------------------------------
static void ConvertQuery_3230(uint32_t ble_cmd_id, uint8_t *data)
{
	if(CMD_ID_BLE_QUERY_3230 != ble_cmd_id)
		return;

//	printf("ConvertQuery_3230(ble_cmd_id = %X", ble_cmd_id);

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
}
// ----------------------------------------------------------------------------
static void ConvertQuery_3232(uint32_t ble_cmd_id, uint8_t *data)
{
	// TODO реализовать
	//	SendUartData( (uint8_t *), );
}
// ----------------------------------------------------------------------------
static void ConvertQuery_3234(uint32_t ble_cmd_id, uint8_t *data)
{
	// TODO реализовать
	//	SendUartData( (uint8_t *), );
}
// ----------------------------------------------------------------------------
static void ConvertQuery_3132(uint32_t ble_cmd_id, uint8_t *data)
{
	// TODO реализовать
	//	SendUartData( (uint8_t *), );
}
// ----------------------------------------------------------------------------
static void ConvertQuery_3332(uint32_t ble_cmd_id, uint8_t *data)
{
	// TODO реализовать
	//	SendUartData( (uint8_t *), );
}
// ----------------------------------------------------------------------------
static void ConvertQuery_3630(uint32_t ble_cmd_id, uint8_t *data)
{
	// TODO реализовать
	//	SendUartData( (uint8_t *), );
}
// ----------------------------------------------------------------------------
static void ConvertQuery_3632(uint32_t ble_cmd_id, uint8_t *data)
{
	// TODO реализовать
	//	SendUartData( (uint8_t *), );
}
// ----------------------------------------------------------------------------
static void ConvertQuery_3530(uint32_t ble_cmd_id, uint8_t *data)
{
	// TODO реализовать
	//	SendUartData( (uint8_t *), );
}
// ----------------------------------------------------------------------------
static void ConvertQuery_3532(uint32_t ble_cmd_id, uint8_t *data)
{
	// TODO реализовать
	//	SendUartData( (uint8_t *), );
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
