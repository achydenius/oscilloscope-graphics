from osc import clip_line
from pyrr import Vector3, vector3


def calculate_normal(vertices, face):
    return vector3.generate_normals(vertices[face[0]],
                                    vertices[face[1]],
                                    vertices[face[2]])


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


def cull_faces(vertices, faces, side='back'):
    def cull_face(vertices, face, side):
        z = calculate_normal(vertices, face)[2]
        return z >= 0 if side == 'back' else z < 0

    return [face for face in faces if cull_face(vertices, face, side)]


def edges_to_lines(edges, vertices, line_brightness):
    return [(vertices[edge[0]], vertices[edge[1]], line_brightness) for edge in edges]


def generate_lines(mesh, matrix, viewport, front_brightness, back_brightness):
    # Transform vertices, remove hidden faces and find edges
    vertices = [matrix * vertex for vertex in mesh['vertices']]

    # TODO: Make the rendering pipeline more generic
    visible_faces = cull_faces(vertices, mesh['faces'], 'back')
    hidden_faces = cull_faces(vertices, mesh['faces'], 'front')

    visible_edges = find_edges(visible_faces)
    hidden_edges = find_edges(hidden_faces)

    # Clip lines
    lines = edges_to_lines(visible_edges, vertices, front_brightness) + \
        edges_to_lines(hidden_edges, vertices, back_brightness)
    return [clipped
            for clipped in [clip_line(line, viewport) for line in lines]
            if clipped is not None]
