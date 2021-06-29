from struct import *
from collections import namedtuple

##typedef struct
##{
##	int8_t ModeOfOperation;		// 1 – main application operating;
##	int16_t DeviceType;			// 0x8012
##	int16_t SubDeviceType;		// 0x0000 (int 16) - "Tion IQ 200", 0x0001 (int 16) - "Tion IQ 400"
##	int16_t SoftwareVertion;	// 0x0000 .. 0xFFFF
##	int16_t BoardVersion;		// 0x0000 .. 0xFFFF
##	int8_t ReserveArray[16];	//
##} t_3331_receive_information_on_device_response; // Receive information on device

UINT8_SIZE = 'b'
UINT16_SIZE = 'h'
UINT32_SIZE = ''

##t_3331_ModeOfOperation = 'ModeOfOperation'
##t_3331_DeviceType      = 'DeviceType'
##t_3331_SubDeviceType   = 'SubDeviceType'
##t_3331_SoftwareVertion = 'SoftwareVertion'
##t_3331_BoardVersion    = 'BoardVersion'
##t_3331_ReserveArray    = 'ReserveArray'
##
##t_3331_keys = (
##        t_3331_ModeOfOperation,
##        t_3331_DeviceType,
##        t_3331_SubDeviceType,
##        t_3331_SoftwareVertion,
##        t_3331_BoardVersion,
##        t_3331_ReserveArray
##    )
##
##t_3331_data_format = {
##        t_3331_ModeOfOperation   : UINT8_SIZE,
##        t_3331_DeviceType        : UINT16_SIZE,
##        t_3331_SubDeviceType     : UINT16_SIZE,
##        t_3331_SoftwareVertion   : UINT16_SIZE,
##        t_3331_BoardVersion      : UINT16_SIZE,
##        t_3331_ReserveArray      : 16
##    }

packed_format = 'bhhhh16s'
print('packed_format size: ', calcsize(packed_format), 'bytes')

t_3331_ModeOfOperation = 1
t_3331_DeviceType      = 2
t_3331_SubDeviceType   = 3
t_3331_SoftwareVertion = 4
t_3331_BoardVersion    = 5
t_3331_ReserveArray    = b'1234567890123456'
pack_data = pack(packed_format,
                 t_3331_ModeOfOperation,
                 t_3331_DeviceType,
                 t_3331_SubDeviceType,
                 t_3331_SoftwareVertion,
                 t_3331_BoardVersion,
                 t_3331_ReserveArray)

print(type(pack_data))
print(pack_data)

packed_format = 'hhb'
print('packed_format size: ', calcsize(packed_format), 'bytes')

pack_data = pack(packed_format, 1, 2, 3)
print('pack_data type: ', type(pack_data))
print('pack_data: ', pack_data)

unpack_data = unpack(packed_format, b'\x01\x00\x02\x00\x03')

print('unpack_data type: ', type(unpack_data))
print('unpack_data: ', unpack_data)


print('')

record = b'ramond    \x32\x12\x08\x01\x08'


name, serialnum, school, gradelevel = unpack('<10sHHb', record)
print('type(unpack:', type(unpack('<10sHHb', record)))

print(name, serialnum, school, gradelevel)

student_keys = 'name'
student_keys += ' ' + 'serialnum'
student_keys += ' ' + 'school'
student_keys += ' ' + 'gradelevel'

Student = namedtuple('Student', student_keys)
##Student = namedtuple('Student', 'name serialnum school gradelevel')
student_tuple = Student._make(unpack('<10sHHb', record))

print('type(student_tuple): ', type(student_tuple))
print(student_tuple)

for st in student_tuple:
    print(type(st), st)
print('')
for k, v in zip(student_tuple._fields, student_tuple):
    print(k,'=', v)
    
##for n, v in student_tuple._asdict().iteritems():
##    print(n, v)

