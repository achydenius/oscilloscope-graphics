from pyrr import Vector3
from dataclasses import dataclass
from typing import List, Tuple


@dataclass
class Face:
    vertices: List[int]
    edges: List[int]


@dataclass
class Mesh:
    vertices: List[Vector3]
    faces: List[Face]
    edges: List[Tuple[int, int]]


def find_edge_index(edge, edges):
    (a, b) = edge
    if (a, b) in edges:
        return edges.index((a, b))
    elif (b, a) in edges:
        return edges.index((b, a))
    else:
        return None


def create_mesh(vertices, indices):
    all_edges = []
    faces = []
    for inds in indices:
        edges = []
        for i in range(len(inds)):
            edge = (inds[i], inds[(i + 1) % len(inds)])
            edge_index = find_edge_index(edge, all_edges)
            if edge_index is None:
                all_edges.append(edge)
                edge_index = len(all_edges) - 1
            edges.append(edge_index)

        faces.append(Face(inds, edges))

    return Mesh(vertices, faces, all_edges)


def create_icosahedron():
    x = 0.525731112119133606
    z = 0.850650808352039932

    return create_mesh([
        Vector3([-x, 0, z]), Vector3([x, 0, z]), Vector3([-x, 0, -z]), Vector3([x, 0, -z]),
        Vector3([0, z, x]), Vector3([0, z, -x]), Vector3([0, -z, x]), Vector3([0, -z, -x]),
        Vector3([z, x, 0]), Vector3([-z, x, 0]), Vector3([z, -x, 0]), Vector3([-z, -x, 0])
    ], [
        [0, 4, 1], [0, 9, 4], [9, 5, 4], [4, 5, 8], [4, 8, 1],
        [8, 10, 1], [8, 3, 10], [5, 3, 8], [5, 2, 3], [2, 7, 3],
        [7, 10, 3], [7, 6, 10], [7, 11, 6], [11, 0, 6], [0, 1, 6],
        [6, 1, 10], [9, 0, 11], [9, 11, 2], [9, 2, 5], [7, 2, 11]
    ])
