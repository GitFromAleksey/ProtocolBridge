#ifndef UART_MOC_UART_H_
#define UART_MOC_UART_H_

#include <stdint.h>
#include <stdbool.h>

void UartInit(void);
bool UartRxDataAvailable(void);
bool UartGetByte(uint8_t *data);
void UartSentData(uint8_t *data, uint8_t size);

#endif /* UART_MOC_UART_H_ */
