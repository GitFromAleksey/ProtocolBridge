from struct import *
from collections import namedtuple

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
