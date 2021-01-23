from osc import Api, Polygon, clip_line
from math import sin
from time import sleep
from pyrr import Vector3, Matrix44

SERIAL_PORT_NAME = '/dev/cu.usbmodem69352701'
SERIAL_PORT_BAUDRATE = 115200


def calculate_normal(vertices, face):
    a = vertices[face[0]] - vertices[face[1]]
    b = vertices[face[0]] - vertices[face[2]]
    normal = a ^ b
    normal.normalize()
    return normal


def create_icosahedron():
    x = 0.525731112119133606
    z = 0.850650808352039932

    return {
        'vertices': [
            Vector3([-x, 0, z]), Vector3([x, 0, z]), Vector3([-x, 0, -z]), Vector3([x, 0, -z]),
            Vector3([0, z, x]), Vector3([0, z, -x]), Vector3([0, -z, x]), Vector3([0, -z, -x]),
            Vector3([z, x, 0]), Vector3([-z, x, 0]), Vector3([z, -x, 0]), Vector3([-z, -x, 0])
        ],
        'faces': [
            [0, 4, 1], [0, 9, 4], [9, 5, 4], [4, 5, 8], [4, 8, 1],
            [8, 10, 1], [8, 3, 10], [5, 3, 8], [5, 2, 3], [2, 7, 3],
            [7, 10, 3], [7, 6, 10], [7, 11, 6], [11, 0, 6], [0, 1, 6],
            [6, 1, 10], [9, 0, 11], [9, 11, 2], [9, 2, 5], [7, 2, 11]
        ]
    }


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


def edges_to_lines(edges, vertices, line_brightness):
    return [(vertices[edge[0]], vertices[edge[1]], line_brightness) for edge in edges]


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

        # Transform vertices, remove hidden faces and find edges
        vertices = [matrix * vertex for vertex in mesh['vertices']]

        # TODO: Clean up this stuff
        visible_faces = []
        hidden_faces = []
        for face in mesh['faces']:
            if calculate_normal(vertices, face).z > 0:
                visible_faces.append(face)
            else:
                hidden_faces.append(face)

        visible_edges = find_edges(visible_faces)
        hidden_edges = find_edges(hidden_faces)

        # Clip lines
        lines = edges_to_lines(visible_edges, vertices, 0.001) + edges_to_lines(hidden_edges, vertices, 0.0225)
        clipped_lines = [clipped
                         for clipped in [clip_line(line, viewport) for line in lines]
                         if clipped is not None]

        api.send(clipped_lines)
        phase += 0.01

        sleep(0.001)
