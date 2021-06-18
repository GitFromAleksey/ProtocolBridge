#include <stdio.h>
#include <stdbool.h>
//#include <stdlib.h>
#include "../uart_moc/uart.h"
#include "../protocol/protocol.h"
#include "../Interface/Interface.h"

t_protocol Protocol;

i_Interface Interface;



void Setup(void)
{
	UartInit();

	Protocol.uart_get_byte = UartGetByte;
	Protocol.uart_sent_data = UartSentData;

	ProtocolInit(&Protocol);
}

int main(void)
{
	uint8_t tmp = 0;
	int thread_counter = 10;
	printf("Hi!!!\n");

	Setup();

	while(thread_counter-- > 0)
	{
//		if(Interface.procRun)
//			Interface.procRun();

//		ProtocolRun(&Protocol);

	}

}
