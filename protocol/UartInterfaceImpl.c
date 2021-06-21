#include <string.h>
#include "UartInterfaceImpl.h"
#include "protocol.h"
#include "ProtocolDataStructures.h"
#include "../BLE/BleDataStructures.h"
//#include "BLE ������ ������"

//#include "UART ������ ������"
//#include "UART ������ � ��������� ������"
//#include "UART ������ � ������� �������� ������"

// ���������� ���������� Interface.h
#define SEND_BUF_SIZE	100u
static uint8_t SendBuf[SEND_BUF_SIZE];


// ----------------------------------------------------------------------------
void SendUartData(uint16_t cmd_id, uint8_t *data_guery)
{
	// TODO �������� ������ ����������
printf("\nSendUartData(cmd_id = %X", cmd_id);
	ProtocolDataStructuresSendDataFromBle(cmd_id, data_guery);

}
// ----------------------------------------------------------------------------
static void ConvertQuery_3230(uint32_t ble_cmd_id, uint8_t *data)
{
	if(CMD_ID_BLE_QUERY_3230 != ble_cmd_id)
		return;

	printf("ConvertQuery_3230(ble_cmd_id = %X", ble_cmd_id);

	uint8_t tmp;
	t_3230_ble_set_parameters_query *ble_query = (t_3230_ble_set_parameters_query*)data;
	t_3230_set_parameters_query *uart_query = (t_3230_set_parameters_query *)SendBuf;

	memcpy(uart_query, ble_query, DATA_SIZE_3230);

	switch(ble_query->u16_status_bit_fld.OperationOfLightIndication)
	{
		case 0: // 00 - off
			uart_query->u16_status_bit_fld.OperationOfLightIndication = 1;
			break;
		case 1: // 01 � lower limit;
			uart_query->u16_status_bit_fld.OperationOfLightIndication = 0;
			break;
		case 2: // 10 � medium
			uart_query->u16_status_bit_fld.OperationOfLightIndication = 2;
			break;
		case 3: // 11 � maximum
			uart_query->u16_status_bit_fld.OperationOfLightIndication = 3;
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

	uart_query->u8_service_bit_field.ParingState = 0; // TODO ����� ������ ����� �������� ���� ��������

	SendUartData( CMD_ID_3230, SendBuf);
}
// ----------------------------------------------------------------------------
static void ConvertQuery_3232(uint32_t ble_cmd_id, uint8_t *data)
{
	// TODO �����������
	//	SendUartData( (uint8_t *), );
}
// ----------------------------------------------------------------------------
static void ConvertQuery_3234(uint32_t ble_cmd_id, uint8_t *data)
{
	// TODO �����������
	//	SendUartData( (uint8_t *), );
}
// ----------------------------------------------------------------------------
static void ConvertQuery_3132(uint32_t ble_cmd_id, uint8_t *data)
{
	// TODO �����������
	//	SendUartData( (uint8_t *), );
}
// ----------------------------------------------------------------------------
static void ConvertQuery_3332(uint32_t ble_cmd_id, uint8_t *data)
{
	// TODO �����������
	//	SendUartData( (uint8_t *), );
}
// ----------------------------------------------------------------------------
static void ConvertQuery_3630(uint32_t ble_cmd_id, uint8_t *data)
{
	// TODO �����������
	//	SendUartData( (uint8_t *), );
}
// ----------------------------------------------------------------------------
static void ConvertQuery_3632(uint32_t ble_cmd_id, uint8_t *data)
{
	// TODO �����������
	//	SendUartData( (uint8_t *), );
}
// ----------------------------------------------------------------------------
static void ConvertQuery_3530(uint32_t ble_cmd_id, uint8_t *data)
{
	// TODO �����������
	//	SendUartData( (uint8_t *), );
}
// ----------------------------------------------------------------------------
static void ConvertQuery_3532(uint32_t ble_cmd_id, uint8_t *data)
{
	// TODO �����������
	//	SendUartData( (uint8_t *), );
}
// ----------------------------------------------------------------------------
void UART_SetData (uint32_t ble_cmd_id, uint8_t *data)
{
printf("\nUART_SetData(cmd = %X)\n", ble_cmd_id);
	// TODO ��� ������ ���� ����� �� �������� ������ � UART
	ConvertQuery_3230( ble_cmd_id, data);
//	ConvertQuery_3232( ble_cmd_id, data);
//	ConvertQuery_3234( ble_cmd_id, data);
//	ConvertQuery_3132( ble_cmd_id, data);
//	ConvertQuery_3332( ble_cmd_id, data);
//	ConvertQuery_3630( ble_cmd_id, data);
//	ConvertQuery_3632( ble_cmd_id, data);
//	ConvertQuery_3530( ble_cmd_id, data);
//	ConvertQuery_3532( ble_cmd_id, data);
}
// ----------------------------------------------------------------------------
uint32_t UART_GetData (uint8_t *data) // return ble_cmd_id
{
	// TODO ��� ������ ���� ������ ������ �� ��������� ProtocolDataStructures
	// �� ���� �������� ����� ����� ������, ������������ �� ��� BLE � ���������
//	ResponseData_3231;
//	ResponseData_3331;
//	ResponseData_3530;
//	ResponseData_3531;

	return 0;
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

	ProtocolInit(&prot);

	interface->SetData = UART_SetData;
	interface->GetData = UART_GetData;
	interface->procRun = ProtocolRun;

}
// ----------------------------------------------------------------------------
