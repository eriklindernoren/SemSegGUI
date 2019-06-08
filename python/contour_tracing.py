import numpy as np


class Point:
    def __init__(self, y, x, label, score):
        self.y = y
        self.x = x
        self.label = label
        self.score = score


class Segment:
    def __init__(self, label, id):
        self.label = label
        self.contour_points = set()
        self.points = {}
        self.id = id

    def seen(self, point):
        return point in self.points

    def add_contour_point(self, point):
        self.contour_points.add(point)

    def add_point(self, point, label, score):
        y, x = point
        self.points[point] = Point(y, x, label, score)


class MooreNeighborTracer:
    def __init__(self):
        self.segments = []

    def get_neighbors(self, point):
        r, c = point
        deltas = [(-1, 0), (1, 0), (0, -1), (0, 1), (-1, -1), (1, 1), (-1, 1), (1, -1)]
        neighbors = [(r + delta_r, c + delta_c) for delta_r, delta_c in deltas]
        return neighbors

    def extract_segment(self, mask, point, segment_id):
        r, c = point
        label = mask[r, c, 0]
        segment = Segment(label, segment_id)

        stack = [point]
        while stack:
            point = stack.pop()
            r, c = point
            label, score = mask[r, c].argmax(-1), mask[r, c].max(-1)
            segment.add_point(point, label, score)
            for neighbor in self.get_neighbors((r, c)):
                nr, nc = neighbor
                h, w = mask.shape[:2]
                if not all([nr >= 0, nr < h, nc >= 0, nc < w]) or mask[nr, nc, 0] != segment.label:
                    # Set point as contour point if neighbor is out of frame or of other label
                    segment.add_contour_point(point)
                elif not segment.seen(neighbor):
                    # Explore point
                    stack.append(neighbor)

        return segment

    def extract_segments(self, mask):
        segments = []
        pid = 0
        for r in range(mask.shape[0]):
            for c in range(mask.shape[1]):
                # If the point does not belong to a segment we create a new one
                if not any([p.seen((r, c)) for p in segments]):
                    segments.append(self.extract_segment(mask, (r, c), pid))
                    pid += 1
                    print(pid)

        print(len(segments))
        for p in segments:
            print(len(p.contour_points))

        return segments
