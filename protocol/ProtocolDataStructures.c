#include "ProtocolDataStructures.h"


t_3531_request_indication_and_auto_settings_response Data_3531;
t_data_struct UartResponse_3531;


// ----------------------------------------------------------------------------
int16_t ProtocolDataStructuresGetDataSize(uint16_t cmd_id)
{
	int16_t result = -1;

	switch(cmd_id)
	{
		case CMD_ID_3230:
//			printf("Find CMD_ID_3230 = 0x%X\n", CMD_ID_3230);
			break;
		case CMD_ID_3231:
			result = DATA_SIZE_3231;
			break;
		case CMD_ID_3232:
//			printf("Find CMD_ID_3232 = 0x%X\n", CMD_ID_3232);
			break;
		case CMD_ID_3234:
//			printf("Find CMD_ID_3234 = 0x%X\n", CMD_ID_3234);
			break;
		case CMD_ID_3332:
//			printf("Find CMD_ID_3332 = 0x%X\n", CMD_ID_3332);
			break;
		case CMD_ID_3331:
			result = DATA_SIZE_3331;
			break;
		case CMD_ID_3530:
			result = DATA_SIZE_3530;
			break;
		case CMD_ID_3532:
//			printf("Find CMD_ID_3532 = 0x%X\n", CMD_ID_3532);
			break;
		case CMD_ID_3531:
			result = DATA_SIZE_3531;
			break;
		default:
			result = -1;
			break;
	}

	return result;
}
// ----------------------------------------------------------------------------
void ProtocolDataStructuresParse(uint8_t *data, uint16_t size)
{

}
// ----------------------------------------------------------------------------
void ProtocolDataStructuresInit(void)
{
	UartResponse_3531.cmd_id = CMD_ID_3531;
	UartResponse_3531.size = DATA_SIZE_3531;
	UartResponse_3531.p_data = (void*)&Data_3531;

}
// ----------------------------------------------------------------------------
