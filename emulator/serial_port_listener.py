import os
import time
import datetime
import serial
import serial.tools.list_ports
import random
from struct import *
from collections import namedtuple

LOG_FILE_NAME = 'log_'
LOG_FILE_EXT = '.txt'

JLINK_PORT_DESCRIBE = 'JLink'

HEAD = 0x23 # заголовок UART пакета

## CMD_ID запросов
CMD_ID_QUERY_DIC = {
    0x3230 : 't_3230_set_parameters_query',
    0x3232 : 't_3232_request_parameters_query',
    0x3234 : 't_3234_set_parameters_nonvolatile_mem_query',
    0x3332 : 't_3332_request_information_on_device_query',
    0x3532 : 't_3532_request_indication_and_auto_settings_query'
    }
## CMD_ID ответов
CMD_ID_REQ_DIC = {
    0x3132 : 't_3231_accept_parameters_response',
    0x3331 : 't_3331_receive_information_on_device_response',
    0x3530 : 't_3530_set_indication_and_auto_settings_response',
    0x3531 : 't_3531_request_indication_and_auto_settings_response'
    }

## длины структур данных ответов
CMD_LEN_REQ_DIC = {
    0x3132 : 33,
    0x3331 : 26,
    0x3530 : 45,
    0x3531 : 45
    }

## соответствия запросов и ответов
CMD_REQ_FOR_QUERY = {
    0x3230 : 0x3132,
    0x3232 : 0x3132,
    0x3234 : 0x3530,
    0x3332 : 0x3331,
    0x3532 : 0x3531
    }

## -----------------------------------------------------------------------------
def LogToFile(data):
    dt = datetime.datetime.now()
    f_name = LOG_FILE_NAME + dt.strftime("%d.%m.%Y_%H") + LOG_FILE_EXT
    log_string = dt.strftime("[%d.%m.%Y %H.%M.%S.%f] - ") + str(data) + '\r'
    print(log_string)
    f = open(f_name, 'a')
    f.write(log_string)
    f.close()
    pass
## -----------------------------------------------------------------------------
def PrintBytes(raw_bytes_arr):
    b_str = str()
    for b in raw_bytes_arr:
        b_str += ("{:02x}".format(b))
        b_str += ":"
    LogToFile('bytes len:' + str(len(raw_bytes_arr)))
    LogToFile(b_str)
## -----------------------------------------------------------------------------
def CrcXorCalk(raw_bytes_arr):
    usStartValue = 0xFF;
    for b in raw_bytes_arr:
        usStartValue = usStartValue ^ b
    return usStartValue;
## -----------------------------------------------------------------------------
def Make_3231():
    packed_format = '<HBBBHHIIIIHIBB' # 32
    print('packed_format size: ', calcsize(packed_format), 'bytes')

    t_3231_status_bit_field = 0x7FFF
    
    t_3231_MaximumNumbersOfSpeeds = 6
    t_3231_FanSpeed = 5
    t_3231_SleepTimerCurrentValue = 11
    t_3231_PM2_5_CurrentValue = 0x2222
    t_3231_VOC_CurrentValues = 0x3333
    t_3231_HardwareRunningTimeCounter = 0x44444444
    t_3231_FanRunningTimeCounter = 0x55555555
    t_3231_FilterRunningTimeCounter = 0x66666666
    t_3231_AntibacterialLayerTimeCounter = 0x77777777
    t_3231_FilterCCM = 0x8888
    t_3231_ErrorBitField = 0x99999999
    u8_service_bit_fld = 0x01 # pairing
    crc = 0

    pack_data = pack(packed_format,
                     t_3231_status_bit_field,
                     t_3231_MaximumNumbersOfSpeeds,
                     t_3231_FanSpeed,
                     t_3231_SleepTimerCurrentValue,
                     t_3231_PM2_5_CurrentValue,
                     t_3231_VOC_CurrentValues,
                     t_3231_HardwareRunningTimeCounter,
                     t_3231_FanRunningTimeCounter,
                     t_3231_FilterRunningTimeCounter,
                     t_3231_AntibacterialLayerTimeCounter,
                     t_3231_FilterCCM,
                     t_3231_ErrorBitField,
                     u8_service_bit_fld,
                     crc
                     )
    return pack_data
## -----------------------------------------------------------------------------
def RequestMake(cmd_id_query):
    data_list = list()
    LogToFile("cmd_id_query: {:02x}".format(cmd_id_query))
    cmd_id_req = CMD_REQ_FOR_QUERY[cmd_id_query]
    LogToFile("cmd_id_req: {:02x}".format(cmd_id_req))
    req_len = CMD_LEN_REQ_DIC[cmd_id_req] + 3
    LogToFile("req_len: " + str(req_len))

    data_list = []
    data_list.append(HEAD)
    data_list.append(cmd_id_req & 0xFF)
    data_list.append((cmd_id_req>>8) & 0xFF)

    if cmd_id_req == 0x3132:
##    if cmd_id_req == -1:
        data = Make_3231()
        for d in data:
            data_list.append(d)
    else:
        for i in range(req_len-3):
            b = random.randint(0,255)
            data_list.append(b)

##    if cmd_id_req == 0x3331:
##    if cmd_id_req == 0x3132: # pairing
####        data_list[2+3] = 0x6
##        data_list[len(data_list)-2] = 0x0;
    
    bs = bytes(data_list)

    crc = CrcXorCalk(bs[0 : len(bs)-1])
    data_list[len(bs)-1] = crc

    bs = bytes(data_list)

##    PrintBytes(bs)
    
    return bs
## -----------------------------------------------------------------------------
def CmdIdCheck(raw_bytes_arr):
    cmd_id = int.from_bytes(raw_bytes_arr[1:3], byteorder = 'little')
##    LogToFile('cmd_id = {:02X} -'.format(cmd_id) + str(CMD_ID_QUERY_DIC[cmd_id]))
    crc = raw_bytes_arr[len(raw_bytes_arr)-1]
    if crc == CrcXorCalk(raw_bytes_arr[0:len(raw_bytes_arr)-1]):
        LogToFile('crc - ok')
        return RequestMake(cmd_id)
## -----------------------------------------------------------------------------
def RawDataParser(raw_bytes_arr):
    head = raw_bytes_arr[0]
##    print()
    PrintBytes(raw_bytes_arr)
    if HEAD == head:
        return CmdIdCheck(raw_bytes_arr)
    else:
        LogToFile('unknown data!!!')
## -----------------------------------------------------------------------------

def main():
    LogToFile(123)
    jLink_port = ''
    ports_dict = {}

    ports = serial.tools.list_ports.comports()
    for port, desc, hwid in sorted(ports):
        print("{}: {} [{}]".format(port, desc, hwid))

    com_number = input('Input COM port number:')
    for port, desc, hwid in sorted(ports):
        name = 'COM' + com_number
        if port.find(name) > -1:
            print("{}: {} [{}]".format(port, desc, hwid))
            jLink_port = name

    if len(jLink_port) == 0:
        print('invalid COM port number')
        return

    ser = serial.Serial(
        port = jLink_port,
        baudrate = 9600, #115200
        parity = serial.PARITY_NONE,
        stopbits = serial.STOPBITS_ONE,
        bytesize = serial.EIGHTBITS,
        timeout = 0)

    ret = 'ret\r'
    cnt = 0

    while True:
        rx_bytes = ser.readline()
        
        if len(rx_bytes):
            LogToFile('rx_bytes: ' + ':'.join('{:02x}'.format(b) for b in rx_bytes))
            tx_bytes = RawDataParser(rx_bytes)
            if tx_bytes:
                LogToFile('tx_bytes: ' + ':'.join('{:02x}'.format(b) for b in tx_bytes))
                ser.write(tx_bytes)

    pass

if __name__ == '__main__':
    main()
