import numpy as np
import cv2

# Segment colors
COLORS = [
    (0, 0, 0),
    (255, 255, 255),
    (255, 0, 0),
    (0, 255, 0),
    (0, 0, 255),
    (255, 255, 0),
    (0, 255, 255),
    (255, 0, 255),
    (192, 192, 192),
    (128, 128, 128),
    (128, 0, 0),
    (128, 128, 0),
    (0, 128, 0),
    (128, 0, 128),
    (0, 128, 128),
    (0, 0, 128),
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
    y_top = min(h - rect_height, y - padding_y)
    x_left = min(w - rect_width, x - padding_x)
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


def get_color_map(segments):
    """Returns a mapping from segmentation labels to a corresponding color"""
    color_map = {}
    for p in segments:
        if p.label not in color_map:
            color_map[p.label] = COLORS[len(color_map) % len(COLORS)]
    return color_map


def draw_contours(image, segments, color_map):
    """Draws segment contours"""
    for p in segments:
        pixels = np.array(list(p.contour_pixels))
        image[pixels[:, 0], pixels[:, 1]] = color_map[p.label]


def visualize(image, segments, labels):
    """Main GUI method. Opens a frame and waits for commands"""

    def mouse_callback(event, x, y, flags, param):
        """Callback method for mouse clicks. Triggers a popup in case of left clicks."""
        if event == cv2.EVENT_LBUTTONUP:
            poly = [p for p in segments if p.seen((y, x))][0]
            add_text(
                image,
                x,
                y,
                [
                    f"Point: ({x}, {y})",
                    f"Name: {labels[poly.label]}",
                    f"Label: {poly.label}",
                    f"Score: {poly.pixels[(y, x)].score}",
                    f"Segment Score: {poly.score}",
                    f"Segment ID: {poly.id}",
                ],
            )

    # Get label to color map
    color_map = get_color_map(segments)
    draw_contours(image, segments, color_map)

    cv2.namedWindow("image")
    cv2.setMouseCallback("image", mouse_callback)

    # Save image clone to enable resets
    image_clone = image.copy()

    # Loop until 'q' is pressed
    while True:

        # Display the image and wait for a keypress
        cv2.imshow("image", image)
        key = cv2.waitKey(1) & 0xFF

        # Press 'r' to reset the frame
        if key == ord("r"):
            image = image_clone.copy()

        # Press 'c' to break from the loop
        elif key in [ord("q"), ord("c")]:
            break
