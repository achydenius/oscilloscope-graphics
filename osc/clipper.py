from dataclasses import dataclass, astuple
from math import sqrt
from typing import NamedTuple, Optional


class Point(NamedTuple):
    x: float
    y: float


class Line(NamedTuple):
    a: Point
    b: Point
    z: float


def sub(a: Point, b: Point) -> Point:
    return Point(a.x - b.x, a.y - b.y)


def dot(a: Point, b: Point) -> float:
    return a.x * b.x + a.y * b.y


def normalize(p: Point) -> Point:
    length = sqrt((p.x ** 2) + (p.y ** 2))
    return Point(p.x / length, p.y / length)


def calculate_normal(a: Point, b: Point) -> Point:
    x, y = sub(a, b)
    return normalize(Point(-y, x))


class Polygon:
    def __init__(self, points=[Point(-1.0, 0.75), Point(1.0, 0.75), Point(1.0, -0.75), Point(-1.0, -0.75)]):
        self.points = points
        self.normals = self._calculate_normals(self.points)

    def _calculate_normals(self, points):
        normals = []
        for i in range(len(points)):
            x, y = (sub(points[(i + 1) % len(points)], points[i]))
            normals.append(normalize(Point(-y, x)))

        return normals


#
# Clip line against a convex polygon
#
# Cyrus-Beck line clipping algorithm implementation:
# https://en.wikipedia.org/wiki/Cyrus%E2%80%93Beck_algorithm
#
def clip_line(line: Line, polygon: Polygon) -> Optional[Line]:
    a, b, z = line
    [te, tl] = [0.0, 1.0]

    p1p0 = sub(b, a)

    for i in range(len(polygon.points)):
        pei = sub(a, polygon.points[i])
        num = dot(polygon.normals[i], pei)
        den = dot(polygon.normals[i], p1p0)

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

    return Line(
        Point(a.x + p1p0.x * te, a.y + p1p0.y * te),
        Point(a.x + p1p0.x * tl, a.y + p1p0.y * tl),
        z
    )
