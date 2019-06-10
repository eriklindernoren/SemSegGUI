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

    def within_bounds(p):
        """Checks whether point is within frame"""
        r, c = p
        h, w = semseg.shape[:2]
        return all([r >= 0, r < h, c >= 0, c < w])

    # Get label of segment
    label = semseg[point].argmax()
    # Define new segment object
    segment = Segment(label, segment_id)
    # Stack for dfs
    stack = [point]
    while stack:
        point = stack.pop()
        label, score = semseg[point].argmax(), semseg[point].max()
        # Add point as member of segment
        segment.add_pixel(point, label, score)
        for neighbor in get_neighbors(point):
            # If neighbor is out of frame or of other class than segment => contour point
            if not within_bounds(neighbor) or semseg[neighbor].argmax() != segment.label:
                segment.add_contour_pixel(point)
            # Add to stack if neighbor has not been visited
            elif not segment.seen(neighbor):
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
    print("Segment\t\tLabel\t\tPixels\t\tScore")
    for r in range(semseg.shape[0]):
        for c in range(semseg.shape[1]):
            # If the pixel does not belong to a known segment => extract new segment
            if not any([p.seen((r, c)) for p in segments]):
                s = extract_segment(semseg, (r, c), len(segments))
                print("%d\t\t%d\t\t%d\t\t%f" % (len(segments), s.label, len(s.pixels), s.score))
                segments.append(s)

    return segments
