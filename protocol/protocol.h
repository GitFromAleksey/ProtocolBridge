#ifndef PROTOCOL_PROTOCOL_C_
#define PROTOCOL_PROTOCOL_C_

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>


#define RX_BUF_SIZE		100u
#define RX_BUF_CNT_MAX	(RX_BUF_SIZE - 1)


typedef enum
{
  NULL_ERROR = 0,
  LINK_ERROR = (1<<0)
} t_protocol_errors;

typedef struct
{
  t_protocol_errors error_reg;
  uint8_t lost_data_counter;

  bool find_start_of_packet;
  int8_t rx_buf_cnt;
  uint8_t packet_buf[RX_BUF_SIZE];

  uint32_t (*get_time_ms)(void);

  bool (*uart_get_byte)(uint8_t *data);
  void (*uart_sent_data)(uint8_t *data, uint8_t size);
  void (*uart_errorCallback)(t_protocol_errors error);
} t_protocol;


void ProtocolInit(t_protocol *uart_protocol, t_protocol *init);
void ProtocolRun(t_protocol *prot);//void ProtocolRun(t_protocol *prot);

#endif /* PROTOCOL_PROTOCOL_C_ */
