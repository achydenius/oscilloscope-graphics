from pyrr import Vector3


class Polygon:
    def __init__(self):
        self.points = [
            Vector3([-0.75, 0.75, 0]),
            Vector3([0.75, 0.75, 0]),
            Vector3([0.75, -0.75, 0]),
            Vector3([-0.75, -0.75, 0])
        ]
        self.normals = self._calculate_normals(self.points)

    def _calculate_normals(self, points):
        normals = []
        for i in range(len(points)):
            dir = points[(i + 1) % len(points)] - points[i]
            normal = dir ^ Vector3([0, 0, -1.0])
            normal.normalize()
            normals.append(normal)

        return normals


class Clipper:
    def __init__(self):
        self.polygon = Polygon()

    #
    # Clip line against a convex polygon
    #
    # Cyrus-Beck line clipping algorithm implementation:
    # https://en.wikipedia.org/wiki/Cyrus%E2%80%93Beck_algorithm
    #
    def clip_line(self, line):
        a, b = line
        te = 0
        tl = 1

        p1p0 = b - a

        for i in range(len(self.polygon.points)):
            pei = a - self.polygon.points[i]
            num = self.polygon.normals[i] | pei
            den = self.polygon.normals[i] | p1p0

            if den != 0:
                t = num / -den
                if (den < 0):
                    te = max(te, t)  # Entering
                else:
                    tl = min(tl, t)  # Exiting
            elif num > 0:  # Parallel and outside
                return None

        if te > tl:  # Outside
            return None

        return (
            (a.x + p1p0.x * te, a.y + p1p0.y * te, a.z + p1p0.z * te),
            (a.x + p1p0.x * tl, a.y + p1p0.y * tl, a.z + p1p0.z * tl)
        )
