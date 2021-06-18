#include "UartInterfaceImpl.h"
#include "protocol.h"
#include "ProtocolDataStructures.h"
//#include "BLE ������ ������"

//#include "UART ������ ������"
//#include "UART ������ � ��������� ������"
//#include "UART ������ � ������� �������� ������"

// ���������� ���������� Interface.h


// ----------------------------------------------------------------------------
void UART_SetData (uint32_t ble_cmd_id, uint8_t *data)
{
	// TODO ��� ������ ���� ����� �� �������� ������ � UART
}
// ----------------------------------------------------------------------------
uint32_t UART_GetData (uint8_t *data) // return ble_cmd_id
{
	// TODO ��� ������ ���� ������ ������ �� ��������� ProtocolDataStructures
	return 0;
}
// ----------------------------------------------------------------------------
void UART_InterfaceInit_TEST(i_Interface *interface,
								bool (*uart_get_byte)(uint8_t *data),
								void (*uart_sent_data)(uint8_t *data, uint8_t size))
{
	t_protocol prot;

	prot.uart_get_byte = uart_get_byte;
	prot.uart_sent_data = uart_sent_data;

	ProtocolInit(&prot);

	interface->SetData = UART_SetData;
	interface->GetData = UART_GetData;
	interface->procRun = ProtocolRun;
}
// ----------------------------------------------------------------------------
