#include <stddef.h>
#include <string.h>
#include "ProtocolDataStructures.h"

// ----------------------------------------------------------------------------

// экземпл€ры структур дл€ хранени€ данных, полученых от UART устройства
t_3231_accept_parameters_response						ResponseData_3231;
t_3331_receive_information_on_device_response			ResponseData_3331;
t_3530_set_indication_and_auto_settings_response		ResponseData_3530;
t_3531_request_indication_and_auto_settings_response	ResponseData_3531;

// элементы списка ответов
static t_data_list_struct	ResponseListItem_3231;
static t_data_list_struct	ResponseListItem_3331;
static t_data_list_struct	ResponseListItem_3530;
static t_data_list_struct	ResponseListItem_3531;

static t_data_list_struct * ResponsesList; // нециклический список ответов
// ----------------------------------------------------------------------------

// экземпл€ры структыр данных запросов
static t_3230_set_parameters_query							Query_3230;
static t_3232_request_parameters_query						Query_3232;
static t_3234_set_parameters_nonvolatile_mem_query			Query_3234;
static t_3332_request_information_on_device_query			Query_3332;
static t_3532_request_indication_and_auto_settings_query	Query_3532;
// элементы списка запросов
static t_data_list_struct	QueryListItem_3230;
static t_data_list_struct	QueryListItem_3232;
static t_data_list_struct	QueryListItem_3234;
static t_data_list_struct	QueryListItem_3332;
static t_data_list_struct	QueryListItem_3532;

static t_data_list_struct * QueryesList; // циклический список запросов


#define SIZE_QUERY_FROM_BLE	50u
uint8_t QueryBleBuf[SIZE_QUERY_FROM_BLE];
static t_data_list_struct QueryFromBle; //
bool BleQueryNeedToSend = false;

// ----------------------------------------------------------------------------
int16_t ProtocolDataStructuresGetDataSize(uint16_t cmd_id)
{
	int16_t result = -1;

	switch(cmd_id)
	{
	// QUERYES
		case CMD_ID_3231:
			result = DATA_SIZE_3231;
			break;
		case CMD_ID_3331:
			result = DATA_SIZE_3331;
			break;
		case CMD_ID_3530:
			result = DATA_SIZE_3530;
			break;
		case CMD_ID_3531:
			result = DATA_SIZE_3531;
			break;

	// RESPONSES
		case CMD_ID_3230:
			result = DATA_SIZE_3230;
			break;
		case CMD_ID_3232:
			result = DATA_SIZE_3232;
			break;
		case CMD_ID_3234:
			result = DATA_SIZE_3234;
			break;
		case CMD_ID_3332:
			result = DATA_SIZE_3332;
			break;
		case CMD_ID_3532:
			result = DATA_SIZE_3532;
			break;
		default:
			result = -1;
			break;
	}

	return result;
}
// ----------------------------------------------------------------------------
void ProtocolDataStructuresParse(uint8_t *data, uint16_t cmd_id)
{
	t_data_list_struct *first_list_item = ResponsesList;
	if(first_list_item == NULL) return;

	while(first_list_item != NULL) // пока не дошли до конца списка
	{
		if(cmd_id == first_list_item->cmd_id) //
		{
			memcpy(first_list_item->p_data, data, ProtocolDataStructuresGetDataSize(cmd_id));
		}
		first_list_item = first_list_item->p_next_item;
	}
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// записывает структуру данных вместе с CMD_ID, возвращает кол-во записаных байт
uint16_t ProtocolDataStructuresGetNextRequest(uint8_t *data, uint16_t size)
{
	uint16_t data_cnt;
	uint16_t cmd_id;
	uint16_t query_size;
	void * p_data;

	if(BleQueryNeedToSend)
	{
		cmd_id = QueryFromBle.cmd_id;
		query_size = QueryFromBle.data_size;
		p_data = QueryFromBle.p_data;
	}
	else
	{
		cmd_id = QueryesList->cmd_id;
		query_size = QueryesList->data_size;
		p_data = QueryesList->p_data;
	}


	data_cnt = 0;

	memcpy(&data[data_cnt], &cmd_id, sizeof(uint16_t));
	data_cnt += sizeof(uint16_t);

	memcpy(&data[data_cnt], p_data, query_size);
	data_cnt += query_size;

	if(!BleQueryNeedToSend)
		QueryesList = (t_data_list_struct*)QueryesList->p_next_item;
	else
		BleQueryNeedToSend = false;

	return data_cnt;
}
// ----------------------------------------------------------------------------
// помещает в очередь внешний запрос от BLE устройства
void ProtocolDataStructuresSendDataFromBle(uint16_t cmd_id, uint8_t *data_guery)
{
	int16_t data_size = ProtocolDataStructuresGetDataSize(cmd_id);

	if(data_size < 0)
		return;

	memcpy( QueryBleBuf, data_guery, data_size);
	QueryFromBle.cmd_id = cmd_id;
	QueryFromBle.data_size = data_size;

	BleQueryNeedToSend = true;
}
// ----------------------------------------------------------------------------
void ProtocolDataStructuresInit(void)
{
	QueryFromBle.p_data = QueryBleBuf;
	BleQueryNeedToSend = false;
	// ------------------------------------------------------------
	ResponseListItem_3231.cmd_id		= CMD_ID_3231;
	ResponseListItem_3231.p_data		= (void*)&ResponseData_3231;
	ResponseListItem_3231.data_size		= DATA_SIZE_3231;
	ResponseListItem_3231.p_next_item	= (void*)&ResponseListItem_3331;

	ResponseListItem_3331.cmd_id		= CMD_ID_3331;
	ResponseListItem_3331.p_data		= (void*)&ResponseData_3331;
	ResponseListItem_3331.data_size		= DATA_SIZE_3331;
	ResponseListItem_3331.p_next_item	= (void*)&ResponseListItem_3530;

	ResponseListItem_3530.cmd_id		= CMD_ID_3530;
	ResponseListItem_3530.p_data		= (void*)&ResponseData_3530;
	ResponseListItem_3530.data_size		= DATA_SIZE_3530;
	ResponseListItem_3530.p_next_item	= (void*)&ResponseListItem_3531;

	ResponseListItem_3531.cmd_id		= CMD_ID_3531;
	ResponseListItem_3531.p_data		= (void*)&ResponseData_3531;
	ResponseListItem_3531.data_size		= DATA_SIZE_3531;
	ResponseListItem_3531.p_next_item	= NULL;

	ResponsesList = &ResponseListItem_3231;

	// ------------------------------------------------------------

	QueryListItem_3230.cmd_id		= CMD_ID_3230;
	QueryListItem_3230.p_data		= (void*)&Query_3230;
	QueryListItem_3230.data_size	= DATA_SIZE_3230;
	QueryListItem_3230.p_next_item	= (void*)&QueryListItem_3232;

	QueryListItem_3232.cmd_id		= CMD_ID_3232;
	QueryListItem_3232.p_data		= (void*)&Query_3232;
	QueryListItem_3232.data_size	= DATA_SIZE_3232;
	QueryListItem_3232.p_next_item	= (void*)&QueryListItem_3234;

	QueryListItem_3234.cmd_id		= CMD_ID_3234;
	QueryListItem_3234.p_data		= (void*)&Query_3234;
	QueryListItem_3234.data_size	= DATA_SIZE_3234;
	QueryListItem_3234.p_next_item	= (void*)&QueryListItem_3332;

	QueryListItem_3332.cmd_id		= CMD_ID_3332;
	QueryListItem_3332.p_data		= (void*)&Query_3332;
	QueryListItem_3332.data_size	= DATA_SIZE_3332;
	QueryListItem_3332.p_next_item	= (void*)&QueryListItem_3532;

	QueryListItem_3532.cmd_id		= CMD_ID_3532;
	QueryListItem_3532.p_data		= (void*)&Query_3532;
	QueryListItem_3532.data_size	= DATA_SIZE_3532;
	QueryListItem_3532.p_next_item	= (void*)&QueryListItem_3230; // здесь ссылаемс€ на первый элемент, так как это циклический список

	QueryesList = &QueryListItem_3230;
}
// ----------------------------------------------------------------------------
