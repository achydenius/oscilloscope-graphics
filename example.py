from osc import Api, Polygon
from osc.viewer import create_icosahedron, generate_lines
from math import sin
from time import sleep
from pyrr import Matrix44

SERIAL_PORT_NAME = '/dev/cu.usbmodem69352701'
SERIAL_PORT_BAUDRATE = 115200


if __name__ == '__main__':
    Api.print_ports()
    api = Api(SERIAL_PORT_NAME, SERIAL_PORT_BAUDRATE)
    viewport = Polygon()

    mesh = create_icosahedron()
    phase = .0
    while True:
        # Create transformation matrix
        translation = Matrix44.from_translation([sin(phase), 0, 4.0])
        rotation = Matrix44.from_eulers([0, phase, phase * 0.5])
        projection = Matrix44.perspective_projection(45.0, 1.0, 0.01, 100.0)
        matrix = projection * translation * rotation

        clipped_lines = generate_lines(mesh, matrix, viewport, 0.001, 0.0225)
        api.send(clipped_lines)
        phase += 0.01

        sleep(0.001)
