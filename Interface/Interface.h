#ifndef INTERFACE_INTERFACE_H_
#define INTERFACE_INTERFACE_H_

#include <stdint.h>


typedef struct
{
	void		(*getData) (uint32_t ble_cmd_id, uint8_t *data);
	uint32_t	(*setData) (uint8_t *data); // return ble_cmd_id

	void (*procRun) (void);
} i_Interface;


#endif /* INTERFACE_INTERFACE_H_ */
