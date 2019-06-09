class Pixel:
    """Class representing image segmentation pixel. Contains label and score.

    Parameters:
    -----------
    y: int
        y coordinate
    x: int
        x coordinate
    label: int
        segment label
    score: float
        label confidence
    """

    def __init__(self, y, x, label, score):
        self.y = y
        self.x = x
        self.label = label
        self.score = score
