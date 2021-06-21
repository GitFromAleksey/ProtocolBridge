#ifndef INTERFACE_INTERFACE_H_
#define INTERFACE_INTERFACE_H_

#include <stdint.h>


typedef struct
{
	void (*uartSendData)			(uint32_t ble_cmd_id, uint8_t *data);
	void (*bleGetDataCallback)	(uint32_t ble_cmd_id, uint8_t *data);

	void (*procRun) (void);
} i_Interface;


#endif /* INTERFACE_INTERFACE_H_ */
