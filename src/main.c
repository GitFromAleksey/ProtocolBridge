#include <stdio.h>
#include <stdbool.h>
//#include <stdlib.h>
#include "../uart_moc/uart.h"
//#include "../protocol/protocol.h"
#include "../Interface/Interface.h"
#include "../protocol/UartInterfaceImpl.h"

#include "../BLE/BleDataStructures.h"

i_Interface Interface;




uint32_t TimeMs = 0;
uint32_t GetTimeMs(void)
{
	return TimeMs;
}

void BleDataCallback(uint32_t ble_cmd_id, uint8_t *data)
{
	printf("\nBleDataCallback(ble_cmd_id = %X\n", ble_cmd_id);
}


void Setup(void)
{
	UartInit();

	Interface.bleGetDataCallback = BleDataCallback;

	UART_InterfaceInit(&Interface, GetTimeMs, UartGetByte, UartSentData);
}

int main(void)
{
//	uint8_t tmp = 0;
	int thread_counter = 10;
	printf("Run main()!!!\n");

	Setup();

	t_3230_ble_set_parameters_query ble_query;
	Interface.uartSendData(CMD_ID_BLE_QUERY_3230, (uint8_t*)&ble_query);

	while(thread_counter-- > 0)
	{
		TimeMs += 10;
		if(Interface.procRun)
			Interface.procRun();
	}

}
