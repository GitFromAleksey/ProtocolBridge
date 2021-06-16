#include <stdio.h>
#include <stdbool.h>
//#include <stdlib.h>
#include "../protocol/protocol.h"
#include "../uart_moc/uart.h"


t_protocol Protocol;

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
//		printf("thread_counter = %u\n", thread_counter);
		ProtocolRun(&Protocol);
		for(int i = 0; i < 100; ++i)
		{
			printf("%X,", (unsigned int)Protocol.packet_buf[i]);
		}
		printf("\r");
	}

}
