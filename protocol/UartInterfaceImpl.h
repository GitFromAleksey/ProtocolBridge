#ifndef PROTOCOL_UARTINTERFACEIMPL_H_
#define PROTOCOL_UARTINTERFACEIMPL_H_

#include <stdint.h>
#include <stdbool.h>
#include "../Interface/Interface.h"


void UART_InterfaceInit_TEST(i_Interface *interface,
								uint32_t (*get_time_ms)(void),
								bool (*uart_get_byte)(uint8_t *data),
								void (*uart_sent_data)(uint8_t *data, uint8_t size));


#endif /* PROTOCOL_UARTINTERFACEIMPL_H_ */
