#include "ProtocolDataStructures.h"


t_3531_request_indication_and_auto_settings_response Data_3531;
t_data_struct UartResponse_3531;



void ProtocolDataStructuresInit(void)
{
	UartResponse_3531.cmd_id = CMD_ID_3531;
	UartResponse_3531.size = DATA_SIZE_3531;
	UartResponse_3531.p_data = (void*)&Data_3531;

}
