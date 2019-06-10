import numpy as np
import cv2

# Cityscapes class colors
# Ref: https://github.com/mcordts/cityscapesScripts/blob/master/cityscapesscripts/helpers/labels.py
COLORS = [
    (0, 0, 0),
    (0, 0, 0),
    (0, 0, 0),
    (0, 0, 0),
    (0, 0, 0),
    (111, 74, 0),
    (81, 0, 81),
    (128, 64, 128),
    (244, 35, 232),
    (250, 170, 160),
    (230, 150, 140),
    (70, 70, 70),
    (102, 102, 156),
    (190, 153, 153),
    (180, 165, 180),
    (150, 100, 100),
    (150, 120, 90),
    (153, 153, 153),
    (153, 153, 153),
    (250, 170, 30),
    (220, 220, 0),
    (107, 142, 35),
    (152, 251, 152),
    (70, 130, 180),
    (220, 20, 60),
    (255, 0, 0),
    (0, 0, 142),
    (0, 0, 70),
    (0, 60, 100),
    (0, 0, 90),
    (0, 0, 110),
    (0, 80, 100),
    (0, 0, 230),
    (119, 11, 32),
    (0, 0, 142),
]


def get_max_text_size(textlines, fontface, scale, thickness):
    """Extracts the size of the widest text line"""
    max_size, _ = cv2.getTextSize(textlines[0], fontface, scale, thickness)
    for s in textlines[1:]:
        size, _ = cv2.getTextSize(s, fontface, scale, thickness)
        if size[0] > max_size[0]:
            max_size = size
    return max_size


def add_text(image, x, y, textlines):
    """Draws a popup with metadata contained in textlines"""

    # Style parameters
    fontface = cv2.FONT_HERSHEY_SIMPLEX
    scale = 0.4
    thickness = 1
    line_padding = 30
    padding_x = 5
    padding_y = 30

    n = len(textlines)
    h, w = image.shape[:2]

    text_width, text_height = get_max_text_size(textlines, fontface, scale, thickness)

    # Rectangle height and width
    rect_height = n * text_height + (n - 1) * line_padding
    rect_width = text_width + 2 * padding_x
    # Top-left coordinate of rect (ensure we don't draw out of frame)
    y_top = min(h - rect_height, max(0, y - padding_y))
    x_left = min(w - rect_width, max(0, x - padding_x))
    # Upper left and bottom right corners of the rectangle
    upper_left = (x_left, y_top)
    bottom_right = (x_left + rect_width, y_top + rect_height)
    cv2.rectangle(image, upper_left, bottom_right, (0, 0, 0), cv2.FILLED)
    # Set text coordinate based on upper left corner of rect
    x = x_left + padding_x
    y = y_top + padding_y
    for line in textlines:
        cv2.putText(
            image, line, (x, y), fontFace=fontface, fontScale=scale, color=(255, 255, 255), thickness=thickness
        )
        y += line_padding


def draw_contours(image, segments):
    """Draws segment contours"""
    for p in segments:
        pixels = np.array(list(p.contour_pixels))
        image[pixels[:, 0], pixels[:, 1]] = COLORS[p.label % len(COLORS)]


def visualize(image, segments, labels):
    """Main GUI method. Opens a frame and waits for commands"""

    def mouse_callback(event, x, y, flags, param):
        """Callback method for mouse clicks. Triggers a popup in case of left clicks."""
        if event == cv2.EVENT_LBUTTONUP:
            # Find segment of which the point is a member
            segment = [s for s in segments if s.seen((y, x))][0]
            add_text(
                image,
                x,
                y,
                [
                    f"Point: ({x}, {y})",
                    f"Name: {labels[segment.label]}",
                    f"Label: {segment.label}",
                    f"Score: {segment.pixels[(y, x)].score}",
                    f"Segment Score: {segment.score}",
                    f"Segment ID: {segment.id}",
                ],
            )

    draw_contours(image, segments)

    cv2.namedWindow("image")
    cv2.setMouseCallback("image", mouse_callback)

    # Save image clone to enable resets
    image_clone = image.copy()

    # Loop until 'q' or 'c' is pressed
    while True:

        cv2.imshow("image", image)

        # Wait for keypress
        key = cv2.waitKey(1) & 0xFF

        # Press 'r' to reset the frame
        if key == ord("r"):
            image = image_clone.copy()

        # Press 'q' or 'c' to break from the loop
        elif key in [ord("q"), ord("c")]:
            break
