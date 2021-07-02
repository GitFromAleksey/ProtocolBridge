#ifndef INTERFACE_INTERFACE_H_
#define INTERFACE_INTERFACE_H_

#include <stdint.h>

typedef enum
{
  EVT_NONE      = (uint8_t)0,
  EVT_ERROR     = (uint8_t)1,
  EVT_DATA      = (uint8_t)2,
  EVT_PAIRING   = (uint8_t)3
} t_event_id;


typedef struct
{
  t_event_id evt_id;
  uint16_t cmd_id;
  uint16_t data_size;
  uint8_t *data;
} t_uart_error_evt;

typedef struct
{
	void (*uartSendData)		(uint32_t ble_cmd_id, uint8_t *data); // отправкаданных со стороны BLE

	void (*procRun) (void);
	void (*uartErrorHandler)(t_uart_error_evt *evt);
} i_Interface;


#endif /* INTERFACE_INTERFACE_H_ */
