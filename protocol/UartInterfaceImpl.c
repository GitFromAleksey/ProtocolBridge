#include "UartInterfaceImpl.h"
//#include "BLE ������ ������"

//#include "UART ������ ������"
//#include "UART ������ � ��������� ������"
//#include "UART ������ � ������� �������� ������"

// ���������� ���������� Interface.h


// ----------------------------------------------------------------------------
void UART_GetData (uint32_t ble_cmd_id, uint8_t *data)
{

}
// ----------------------------------------------------------------------------
uint32_t UART_SetData (uint8_t *data) // return ble_cmd_id
{
	return 0;
}
// ----------------------------------------------------------------------------
void UART_InterfaceInit(i_Interface *interface)
{
	interface->getData = UART_GetData;
	interface->setData = UART_SetData;
}
// ----------------------------------------------------------------------------
