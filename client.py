import serial.tools.list_ports
import struct
from time import sleep
from math import pi, sin, cos

SERIAL_PORT_NAME = '/dev/cu.usbmodem1432201'
SERIAL_PORT_BAUD_RATE = 115200


def print_ports():
    for port in serial.tools.list_ports.comports():
        print(port)


def generate_circle(vertex_count, radius, offset=0):
    vertices = []
    for i in range(0, vertex_count):
        rad = 2 * pi / vertex_count * i
        vertices.append((cos(rad + offset) * radius,
                         sin(rad + offset) * radius))

    lines = []
    for i in range(0, vertex_count):
        lines.append(
            (vertices[i], vertices[(i + 1) % vertex_count])
        )

    return lines


def coords_to_lines(coords):
    lines = []
    for i in range(len(coords)):
        lines.append(
            (coords[i], coords[(i + 1) % len(coords)])
        )
    return lines


def lines_to_serial(lines):
    # Flatten line coordinates to a one-dimensional list
    coords = [coord
              for line in lines
              for coords in line
              for coord in coords]

    # Format coordinates as binary floats
    bytes = [struct.pack('<f', coord) for coord in coords]

    # First four bytes define the length of the list
    bytes.insert(0, len(coords).to_bytes(4, byteorder='little', signed=True))

    return bytearray(b''.join(bytes))


if __name__ == '__main__':
    print_ports()
    port = serial.Serial(SERIAL_PORT_NAME, SERIAL_PORT_BAUD_RATE)

    phase = 0
    while True:
        lines = generate_circle(8, 0.75, phase)
        port.write(lines_to_serial(lines))
        phase += 0.01

        sleep(0.01)
