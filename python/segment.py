from pixel import Pixel


class Segment:
    """Class representing image segment. Contains corresponding pixels and
    contour pixels.

    Parameters:
    -----------
    label: int
        segment label
    id: float
        segment id
    """

    def __init__(self, label, id):
        self.label = label
        self.contour_pixels = set()
        self.pixels = {}
        self.id = id
        self.score = 0

    def seen(self, point):
        """Checks if the point is a member of the segment"""
        return point in self.pixels

    def add_contour_pixel(self, point):
        """Add as contour pixel"""
        self.contour_pixels.add(point)

    def add_pixel(self, point, label, score):
        """Add pixel as segment member and update global segment score"""
        y, x = point
        n = len(self.pixels)
        self.score = (n * self.score + score) / (n + 1)
        self.pixels[point] = Pixel(y, x, label, score)
