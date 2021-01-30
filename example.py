from osc import Api, Polygon
from osc.viewer import generate_lines, Object, Camera
from osc.mesh import create_icosahedron
from math import sin
from time import sleep
from pyrr import Matrix44

SERIAL_PORT_NAME = '/dev/cu.usbmodem69352701'
SERIAL_PORT_BAUDRATE = 115200


if __name__ == '__main__':
    Api.print_ports()
    api = Api(SERIAL_PORT_NAME, SERIAL_PORT_BAUDRATE)

    obj = Object(create_icosahedron())
    camera = Camera(45.0, 1.0, 0.01, 100.0)
    viewport = Polygon()

    phase = .0
    while True:
        obj.set_translation(sin(phase), 0, 0)
        obj.set_rotation(0, phase, phase * 0.5)
        camera.set_eye(0, 0, 4.0)
        camera.set_target(0, 0, 0)

        clipped_lines = generate_lines(obj, camera, viewport, 0.001, 0.0225)
        api.send(clipped_lines)
        phase += 0.01

        sleep(0.001)
