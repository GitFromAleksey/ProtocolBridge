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


CHAR               = 'c' # size = 1 (bytes of length 1)
SINED_CHAR         = 'b' # size = 1 (integer)
UNSIHNED_CHAR      = 'B' # size = 1 (integer)
_BOOL              = '?' # size = 1 (bool)
SHORT              = 'h' # size = 2 (integer)
UNSIGNED_SHORT     = 'H' # size = 2 (integer)
INT                = 'i' # size = 4 (integer)
UNSIGNED_INT       = 'I' # size = 4 (integer)
LONG               = 'l' # size = 4 (integer)
UNSIGNED_LONG      = 'L' # size = 4 (integer)
LONG_LONG          = 'q' # size = 8 (integer)
UNSIGNED_LONG_LONG = 'Q' # size = 8 (integer)
SSIZE_T            = 'n' # size =  (integer)
SIZE_T             = 'N' # size =  (integer)
FLOAT_16BIT        = 'e' # size = 2 (float)
FLOAT_32BIT        = 'f' # size = 4 (float)
DOUBLE             = 'd' # size = 8 (float)
CHAR_ARR_S         = 's' # size =  (bytes)
CHAR_ARR_P         = 'p' # size =  (bytes)
VOID_PTR           = 'p' # size =  (integer)


INT8_FORMAT  = SINED_CHAR
INT16_FORMAT = SHORT
INT32_FORMAT = INT

UINT8_FORMAT  = UNSIHNED_CHAR
UINT16_FORMAT = UNSIGNED_SHORT
UINT32_FORMAT = UNSIGNED_LONG


##packed_format = INT8_FORMAT + (4*INT16_FORMAT) + ('16'+CHAR_ARR_S)
##print('packed_format:', packed_format)

ModeOfOperationName = 'ModeOfOperation'
DeviceTypeName      = 'DeviceType'
SubDeviceTypeName   = 'SubDeviceType'
SoftwareVertionName = 'SoftwareVertion'
BoardVersionName    = 'BoardVersion'
ReserveArrayName    = 'ReserveArray[16]'

ModeOfOperation = {ModeOfOperationName  : INT8_FORMAT}
DeviceType      = {DeviceTypeName       : INT16_FORMAT}
SubDeviceType   = {SubDeviceTypeName    : INT16_FORMAT}
SoftwareVertion = {SoftwareVertionName  : INT16_FORMAT}
BoardVersion    = {BoardVersionName     : INT16_FORMAT}
ReserveArray    = {ReserveArrayName     : INT8_FORMAT}

t_3331_types = {
        ModeOfOperationName : INT8_FORMAT,
        DeviceTypeName      : INT16_FORMAT,
        SubDeviceTypeName   : INT16_FORMAT,
        SoftwareVertionName : INT16_FORMAT,
        BoardVersionName    : INT16_FORMAT,
        ReserveArrayName    : '16' + CHAR_ARR_S
    }

t_3331_values = {
        ModeOfOperationName : 0,
        DeviceTypeName      : 0,
        SubDeviceTypeName   : 0,
        SoftwareVertionName : 0,
        BoardVersionName    : 0,
        ReserveArrayName    : 0
        }

STRUCT_DATA_KEY = 'data_key'
STRUCT_TYPES_KEY = 'types_key'

t_3331_struct = {
        STRUCT_DATA_KEY  : t_3331_values,
        STRUCT_TYPES_KEY : t_3331_types
    }
##------------------------------------------------------------------------------
def GetPackedFormat(data_struct):
    pack_types = data_struct[STRUCT_TYPES_KEY]
    packed_format = '<'
    for key in pack_types:
        packed_format += pack_types[key]
    return packed_format
##------------------------------------------------------------------------------
def UnPack(data_struct, data_bytes):
    pack_data = data_struct[STRUCT_DATA_KEY]
    packed_format = GetPackedFormat(data_struct) #'<'
    unpack_data = unpack(packed_format, data_bytes)
    i = 0;
    for key in pack_data:
        pack_data[key] = unpack_data[i]
        i += 1
##------------------------------------------------------------------------------
def Pack(data_struct):
    pack_data = data_struct[STRUCT_DATA_KEY]
    pack_types = data_struct[STRUCT_TYPES_KEY]
    bytes_data = bytes()
    for key in pack_data:
        bytes_data += pack(pack_types[key], pack_data[key])
        print(pack(pack_types[key], pack_data[key]))
        print('pack_type:',pack_types[key],'data:',pack_data[key],pack(pack_types[key], pack_data[key]))
    return bytes_data
##------------------------------------------------------------------------------
def StructFill(struct_for_fill):
    values = struct_for_fill[STRUCT_DATA_KEY]
    i = 1
    for key in values.keys():
        values[key] = i
        i += 1
    values[ReserveArrayName] = b'1234567890123456'

StructFill(t_3331_struct)
data_bytes = Pack(t_3331_struct)
print(data_bytes)
UnPack(t_3331_struct, data_bytes)
print('unpacks:', t_3331_struct)
##------------------------------------------------------------------------------

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
print('student_tuple:',student_tuple)

for st in student_tuple:
    print(type(st), st)
print('')
for k, v in zip(student_tuple._fields, student_tuple):
    print(k,'=', v)
    
##for n, v in student_tuple._asdict().iteritems():
##    print(n, v)

