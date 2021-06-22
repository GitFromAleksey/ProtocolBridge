import os
import time
import serial
import serial.tools.list_ports


JLINK_PORT_DESCRIBE = 'JLink'

def CountFilesInSubdirs(path):
    counter = int()
    for root, dirs, files in os.walk(path):
##    print('root:', root)
##    print('dirs:', dirs)
##    print('files:', files)
        counter += 1;
    print('counter =', counter)


def main():
##    CountFilesInSubdirs(os.getcwd())
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
        baudrate = 115200,
        parity = serial.PARITY_NONE,
        stopbits = serial.STOPBITS_ONE,
        bytesize = serial.EIGHTBITS,
        timeout = 0)

    ret = 'ret\r'
    cnt = 0

    while True:
        s = ser.readline()
        if len(s):
            print(s)
            print( ":".join("{:02x}".format(c) for c in s) )
##            time.sleep(1)
##            ret = 'TX from python!' + str(cnt) + '\n'
##            cnt += 1
##            ser.write(ret.encode())

    pass

if __name__ == '__main__':
    main()
