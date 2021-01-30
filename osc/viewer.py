from osc import clip_line
from pyrr import vector3, Matrix44, matrix44


class Object:
    def __init__(self, mesh):
        self.mesh = mesh
        self.matrix = Matrix44()
        self.rotation = [0, 0, 0]
        self.translation = [0, 0, 0]

    def set_rotation(self, pitch, roll, yaw):
        self.rotation = [pitch, roll, yaw]

    def set_translation(self, x, y, z):
        self.translation = [x, y, z]


class Camera:
    def __init__(self, fovy, aspect, near, far):
        self.fovy = fovy
        self.aspect = aspect
        self.near = near
        self.far = far
        self.eye = [0, 0, 0]
        self.target = [0, 0, 0]

    def set_eye(self, x, y, z):
        self.eye = [x, y, z]

    def set_target(self, x, y, z):
        self.target = [x, y, z]


def cull_face(vertices, face, side='back'):
    z = vector3.generate_normals(vertices[face.vertices[0]],
                                 vertices[face.vertices[1]],
                                 vertices[face.vertices[2]])[2]
    return z >= 0 if side == 'back' else z < 0


def edges_to_lines(edges, vertices, line_brightness):
    return [(vertices[edge[0]], vertices[edge[1]], line_brightness) for edge in edges]


def find_edges(faces, edges):
    indices = set()
    for face in faces:
        for index in face.edges:
            indices.add(index)

    return list([edges[index] for index in indices])


def generate_lines(obj, camera, viewport, front_brightness, back_brightness):
    # Create transformation matrix
    translation = Matrix44.from_translation(obj.translation)
    rotation = Matrix44.from_eulers(obj.rotation)
    projection = Matrix44.perspective_projection(
        camera.fovy, camera.aspect, camera.near, camera.far)
    cam = matrix44.create_look_at(camera.eye, camera.target, [0, 1.0, 0])
    matrix = projection * cam * translation * rotation

    # Transform vertices, remove hidden faces and get edges for faces
    # TODO: Make the rendering pipeline more generic
    vertices = [matrix * vertex for vertex in obj.mesh.vertices]
    front_faces = [face for face in obj.mesh.faces
                   if cull_face(vertices, face, 'front')]
    back_faces = [face for face in obj.mesh.faces
                  if cull_face(vertices, face, 'back')]
    front_edges = find_edges(front_faces, obj.mesh.edges)
    back_edges = find_edges(back_faces, obj.mesh.edges)

    # Clip lines
    lines = edges_to_lines(front_edges, vertices, front_brightness) + \
        edges_to_lines(back_edges, vertices, back_brightness)
    return [clipped
            for clipped in [clip_line(line, viewport) for line in lines]
            if clipped is not None]
