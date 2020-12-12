from osc import Api, Clipper
from osc.mesh import create_icosahedron
from math import pi, sin, sqrt
from time import sleep
from pyrr import Vector3, Matrix44

SERIAL_PORT_NAME = '/dev/cu.usbmodem1432201'
SERIAL_PORT_BAUDRATE = 115200


def calculate_normal(vertices, face):
    a = vertices[face[0]] - vertices[face[1]]
    b = vertices[face[0]] - vertices[face[2]]
    normal = a ^ b
    normal.normalize()
    return normal


def find_edges(faces):
    edges = []
    existing = set()

    for face in faces:
        for i in range(len(face)):
            edge = (face[i], face[(i + 1) % len(face)])
            s = frozenset(edge)
            if s not in existing:
                existing.add(s)
                edges.append(edge)

    return edges


def edges_to_lines(edges, vertices):
    return [(vertices[edge[0]], vertices[edge[1]]) for edge in edges]


if __name__ == '__main__':
    Api.print_ports()
    api = Api(SERIAL_PORT_NAME, SERIAL_PORT_BAUDRATE)
    clipper = Clipper()

    mesh = create_icosahedron()
    phase = 0
    while True:
        # Create transformation matrix
        translation = Matrix44.from_translation([0, 0, 5.0])
        rotation = Matrix44.from_eulers([phase * 0.4, phase * 0.2, 0])
        projection = Matrix44.perspective_projection(45.0, 1.0, 0.01, 100.0)
        matrix = projection * translation * rotation

        # Transform vertices, remove hidden faces and find edges
        vertices = [matrix * vertex for vertex in mesh['vertices']]
        faces = [face for face in mesh['faces']
                 if calculate_normal(vertices, face).z > 0]
        edges = find_edges(faces)

        # Clip lines
        lines = edges_to_lines(edges, vertices)
        clipped_lines = [clipped
                         for clipped in [clipper.clip_line(line) for line in lines]
                         if clipped is not None]

        api.send(clipped_lines)
        phase += 0.01
