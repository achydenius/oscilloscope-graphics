import serial.tools.list_ports
import struct
from time import sleep

SERIAL_PORT_NAME = '/dev/cu.usbmodem1432201'
SERIAL_PORT_BAUD_RATE = 115200


def print_ports():
    for port in serial.tools.list_ports.comports():
        print(port)


if __name__ == '__main__':
    print_ports()
    port = serial.Serial(SERIAL_PORT_NAME, SERIAL_PORT_BAUD_RATE)

    while True:
        coords = [-0.5, 0.5, 0.5, 0.5, 0.5, -0.5, -0.5, -0.5]
        bytes = [struct.pack('f', coord) for coord in coords]
        bytes.insert(0, len(coords).to_bytes(
            4, byteorder='little', signed=True))

        port.write(bytearray(b''.join(bytes)))

        sleep(0.01)
