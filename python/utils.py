import numpy as np
import cv2

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


def load_image(image_path):
    image = cv2.imread(image_path)
    return image


def load_segmentation(segmentation_path):
    segm = np.load(segmentation_path)
    return segm


def extract_labels(label_path):
    with open(label_path) as f:
        labels = f.read().splitlines()
    return labels


def get_max_text_size(textlines, fontface, scale, thickness):
    max_size, _ = cv2.getTextSize(textlines[0], fontface, scale, thickness)
    for s in textlines[1:]:
        size, _ = cv2.getTextSize(s, fontface, scale, thickness)
        if size[0] > max_size[0]:
            max_size = size
    return max_size


def add_text(image, x, y, textlines):
    fontface = cv2.FONT_HERSHEY_SIMPLEX
    scale = 0.4
    thickness = 1
    line_padding = 30
    padding_x = 5
    padding_y = 30
    n = len(textlines)
    h, w = image.shape[:2]

    text_width, text_height = get_max_text_size(textlines, fontface, scale, thickness)

    # Top left coordinate
    y_top = y - padding_y
    x_left = x - padding_x
    # Upper left and bottom right corners of the rectangle
    upper_left = (x_left, y_top)
    bottom_right = (x_left + text_width + 2 * padding_x, y_top + n * text_height + (n - 1) * line_padding)
    cv2.rectangle(image, upper_left, bottom_right, (0, 0, 0), cv2.FILLED)

    for line in textlines:
        cv2.putText(
            image, line, (x, y), fontFace=fontface, fontScale=scale, color=(255, 255, 255), thickness=thickness
        )
        y += line_padding


def get_color_map(polygons):
    color_map = {}
    for p in polygons:
        if p.label not in color_map:
            color_map[p.label] = COLORS[len(color_map) % len(COLORS)]
    return color_map


def draw_contours(image, polygons, color_map):
    for p in polygons:
        pixels = np.array(list(p.contour_points))
        image[pixels[:, 0], pixels[:, 1]] = color_map[p.label]


def visualize(image, polygons, labels):
    def mouse_callback(event, x, y, flags, param):

        if event == cv2.EVENT_LBUTTONUP:
            poly = [p for p in polygons if p.seen((y, x))][0]
            add_text(
                image,
                x,
                y,
                [
                    f"Point: ({x}, {y})",
                    f"Name: {labels[poly.label]}",
                    f"Label: {poly.label}",
                    f"Score: {poly.points[(y, x)].score}",
                    f"Segment Score: {poly.score}",
                    f"Segment ID: {poly.id}",
                ],
            )

    # Get label to color map
    color_map = get_color_map(polygons)
    draw_contours(image, polygons, color_map)

    cv2.namedWindow("image")
    cv2.setMouseCallback("image", mouse_callback)

    image_clone = image.copy()

    # Loop until 'q' is pressed
    while True:

        # Display the image and wait for a keypress
        cv2.imshow("image", image)
        key = cv2.waitKey(1) & 0xFF

        # Press 'r' to reset the cropping region
        if key == ord("r"):
            image = image_clone.copy()

        # Press 'c' to break from the loop
        elif key in [ord("q"), ord("c")]:
            break
