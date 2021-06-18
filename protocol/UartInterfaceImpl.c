#include "UartInterfaceImpl.h"
//#include "BLE формат данных"

//#include "UART формат данных"
//#include "UART доступ к хранилищу данных"
//#include "UART доступ к очереди отправки данных"

// реализация интерфейса Interface.h


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
