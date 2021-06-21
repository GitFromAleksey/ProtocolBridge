#include <stdio.h>
#include <stdbool.h>
//#include <stdlib.h>
#include "../uart_moc/uart.h"
//#include "../protocol/protocol.h"
#include "../Interface/Interface.h"
#include "../protocol/UartInterfaceImpl.h"

#include "../BLE/BleDataStructures.h"

i_Interface Interface;

t_3230_ble_set_parameters_query ble_query;


uint32_t TimeMs = 0;
uint32_t GetTimeMs(void)
{
	return TimeMs;
}


void Setup(void)
{
	UartInit();

	UART_InterfaceInit(&Interface, GetTimeMs, UartGetByte, UartSentData);
}

int main(void)
{
//	uint8_t tmp = 0;
	int thread_counter = 10;
	printf("Run main()!!!\n");

	Setup();

	Interface.SetData(CMD_ID_BLE_QUERY_3230, (uint8_t*)&ble_query);

	while(thread_counter-- > 0)
	{
		TimeMs += 10;
		if(Interface.procRun)
			Interface.procRun();
	}

}
