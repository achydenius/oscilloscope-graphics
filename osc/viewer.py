from osc import clip_line
from pyrr import vector3


def calculate_normal(vertices, face):
    return vector3.generate_normals(vertices[face[0]],
                                    vertices[face[1]],
                                    vertices[face[2]])


def cull_faces(vertices, faces, side='back'):
    def cull_face(vertices, face, side):
        z = calculate_normal(vertices, face['indices'])[2]
        return z >= 0 if side == 'back' else z < 0

    return [face for face in faces if cull_face(vertices, face, side)]


def find_edges(faces, edges):
    indices = set()
    for face in faces:
        for index in face['edges']:
            indices.add(index)

    return list([edges[index] for index in indices])


def edges_to_lines(edges, vertices, line_brightness):
    return [(vertices[edge[0]], vertices[edge[1]], line_brightness) for edge in edges]


def generate_lines(mesh, matrix, viewport, front_brightness, back_brightness):
    # Transform vertices, remove hidden faces and get edges for faces
    vertices = [matrix * vertex for vertex in mesh['vertices']]

    # TODO: Make the rendering pipeline more generic
    visible_faces = cull_faces(vertices, mesh['faces'], 'back')
    hidden_faces = cull_faces(vertices, mesh['faces'], 'front')

    visible_edges = find_edges(visible_faces, mesh['edges'])
    hidden_edges = find_edges(hidden_faces, mesh['edges'])

    # Clip lines
    lines = edges_to_lines(visible_edges, vertices, front_brightness) + \
        edges_to_lines(hidden_edges, vertices, back_brightness)
    return [clipped
            for clipped in [clip_line(line, viewport) for line in lines]
            if clipped is not None]
