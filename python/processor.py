from pixel import Pixel
from segment import Segment
import numpy as np


def get_neighbors(point):
    """Return neighboring points"""
    r, c = point
    deltas = [(-1, 0), (1, 0), (0, -1), (0, 1), (-1, -1), (1, 1), (-1, 1), (1, -1)]
    neighbors = [(r + delta_r, c + delta_c) for delta_r, delta_c in deltas]
    return neighbors


def extract_segment(semseg, point, segment_id):
    """Uses tree search (dfs) to explore points within the segment.
    Adds all points within the segment as members of the segment and stores points at the border
    of the segment as contour points.

    Parameters:
    -----------
    semseg: ndarray
        semantic segmentation of shape (H x W x NumClasses)
    point: tuple(x, y)
        start point of dfs
    id: float
        segment id

    returns: Segment
        image segment
    """
    r, c = point
    h, w = semseg.shape[:2]
    label = semseg[r, c].argmax()
    segment = Segment(label, segment_id)
    stack = [point]
    while stack:
        point = stack.pop()
        r, c = point
        label, score = semseg[r, c].argmax(), semseg[r, c].max()
        segment.add_pixel(point, label, score)
        for neighbor in get_neighbors((r, c)):
            nr, nc = neighbor
            if not all([nr >= 0, nr < h, nc >= 0, nc < w]) or semseg[nr, nc].argmax() != segment.label:
                # Set pixel as contour pixel if neighbor is out of frame or of other label
                segment.add_contour_pixel(point)
            elif not segment.seen(neighbor):
                # Explore neighbor
                stack.append(neighbor)

    return segment


def extract_segments(semseg):
    """Extracts image segments from semantic segmentation.

    Parameters:
    -----------
    semseg: ndarray
        semantic segmentation of shape (H x W x NumClasses)

    returns: list(Segment)
        list of image segments
    """
    segments = []
    pid = 0
    print("Segment\t\tLabel\t\tPixels\t\tScore")
    for r in range(semseg.shape[0]):
        for c in range(semseg.shape[1]):
            # If the pixel does not belong to a segment we create a new one
            if not any([p.seen((r, c)) for p in segments]):
                s = extract_segment(semseg, (r, c), pid)
                segments.append(s)
                print("%d\t\t%d\t\t%d\t\t%f" % (pid, s.label, len(s.pixels), s.score))
                pid += 1

    return segments
