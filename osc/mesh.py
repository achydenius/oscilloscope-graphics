from pyrr import Vector3


def create_square():
    return {
        'vertices': [
            Vector3([-1.0, 1, 0]),
            Vector3([-1.0, -1.0, 0]),
            Vector3([1.0, -1.0, 0]),
            Vector3([1.0, 1.0, 0]),
        ],
        'faces': [
            [0, 1, 2, 3]
        ]
    }


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