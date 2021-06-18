#ifndef PROTOCOL_PROTOCOL_C_
#define PROTOCOL_PROTOCOL_C_

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>


#define RX_BUF_SIZE		100u
#define RX_BUF_CNT_MAX	(RX_BUF_SIZE - 1)

typedef struct
{
	bool find_start_of_packet;
	int8_t rx_buf_cnt;
	uint8_t packet_buf[RX_BUF_SIZE];

	bool (*uart_get_byte)(uint8_t *data);
	void (*uart_sent_data)(uint8_t *data, uint8_t size);
} t_protocol;

void ProtocolInit(t_protocol *prot);
void ProtocolRun(void);//void ProtocolRun(t_protocol *prot);

#endif /* PROTOCOL_PROTOCOL_C_ */
