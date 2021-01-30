from typing import List, Tuple
from dataclasses import dataclass
from pyrr import Vector3, vector3, Matrix44, matrix44
from osc import clip_line, Polygon, Line

Edge = Tuple[int, int]


@dataclass
class Face:
    vertices: List[int]
    edges: List[int]


@dataclass
class Mesh:
    vertices: List[Vector3]
    faces: List[Face]
    edges: List[Edge]


@dataclass
class Object:
    mesh: Mesh
    rotation: Tuple[float, float, float] = (0, 0, 0)
    translation: Tuple[float, float, float] = (0, 0, 0)


@dataclass
class Camera:
    fovy: float
    aspect: float
    near: float
    far: float
    eye: Tuple[float, float, float] = (0, 0, 0)
    target: Tuple[float, float, float] = (0, 0, 0)


def cull_face(vertices: List[Vector3], face: Face, side: str = 'back') -> bool:
    z = vector3.generate_normals(vertices[face.vertices[0]],
                                 vertices[face.vertices[1]],
                                 vertices[face.vertices[2]])[2]
    return z >= 0 if side == 'back' else z < 0


def find_edges(faces: List[Face], edges: List[Edge]) -> List[Edge]:
    indices = set()
    for face in faces:
        for index in face.edges:
            indices.add(index)

    return list([edges[index] for index in indices])


def edges_to_lines(edges: List[Edge], vertices: List[Vector3], line_brightness: float) -> List[Line]:
    return [Line(vertices[edge[0]], vertices[edge[1]], line_brightness) for edge in edges]


def generate_lines(obj: Object, camera: Camera, viewport: Polygon, front_brightness: float, back_brightness: float) -> List[Line]:
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
