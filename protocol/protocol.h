#ifndef PROTOCOL_PROTOCOL_C_
#define PROTOCOL_PROTOCOL_C_

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define PACKET_START	(uint8_t)0x23

#define CRC8_TEST		(uint8_t)0xDD
#define CRC8_LEN		sizeof(CRC8_TEST)

#define CMD_0			(uint16_t)0x3132

//#pragma pack(push, 1)
typedef struct
{
	uint16_t cmd;
	uint8_t data_0;
	uint16_t data_1;
	uint32_t data_2;
	uint8_t crc;
} t_packet;
//#pragma pack(pop)
#define T_PAKET_LEN		sizeof(t_packet)


#define RX_BUF_SIZE		50u
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
void ProtocolRun(t_protocol *prot);

#endif /* PROTOCOL_PROTOCOL_C_ */
