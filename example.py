from client import Client
from math import pi, sin, cos
from time import sleep

SERIAL_PORT_NAME = '/dev/cu.usbmodem1432201'
SERIAL_PORT_BAUDRATE = 115200


def generate_circle_lines(vertex_count, radius, offset=0):
    # Create vertices
    vertices = []
    for i in range(0, vertex_count):
        rad = 2 * pi / vertex_count * i
        vertices.append((cos(rad + offset) * radius,
                         sin(rad + offset) * radius))

    # Create lines from vertices
    lines = []
    for i in range(0, vertex_count):
        lines.append(
            (vertices[i], vertices[(i + 1) % vertex_count])
        )

    return lines


if __name__ == '__main__':
    Client.print_ports()
    client = Client(SERIAL_PORT_NAME, SERIAL_PORT_BAUDRATE)

    phase = 0
    while True:
        lines = generate_circle_lines(8, 0.75, phase)
        client.send(lines)
        phase += 0.01

        sleep(0.01)
