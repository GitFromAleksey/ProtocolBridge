#include <stdio.h>
#include <stdbool.h>
//#include <stdlib.h>
#include "../uart_moc/uart.h"
//#include "../protocol/protocol.h"
#include "../Interface/Interface.h"
#include "../protocol/UartInterfaceImpl.h"


i_Interface Interface;


void Setup(void)
{
	UartInit();

	UART_InterfaceInit_TEST(&Interface, UartGetByte, UartSentData);
}

int main(void)
{
//	uint8_t tmp = 0;
	int thread_counter = 10;
	printf("Run main()!!!\n");

	Setup();

	while(thread_counter-- > 0)
	{
		if(Interface.procRun)
			Interface.procRun();
	}

}
