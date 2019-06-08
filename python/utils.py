import matplotlib.pyplot as plt
import numpy as np
import cv2


def load_image(image_path):
    image = cv2.imread(image_path)
    return image


def load_annotation(annotation_path):
    ann = cv2.imread(annotation_path)
    ann = ann[..., :1]
    return ann


def add_text(image, x, y, textlines):
    for line in textlines:
        cv2.putText(
            image, line, (x, y), fontFace=cv2.FONT_HERSHEY_SIMPLEX, fontScale=0.8, color=(255, 255, 255), thickness=1
        )
        y += 30


def visualize(image, polygons=None):
    def visualize_meta(event, x, y, flags, param):

        if event == cv2.EVENT_LBUTTONUP:
            poly = [p for p in polygons if p.seen((y, x))][0]
            add_text(
                image,
                x,
                y,
                [
                    f"Point: ({x}, {y})",
                    f"PID: {poly.id}",
                    f"Label: {poly.label}",
                    f"Score: {poly.points[(y, x)].score}",
                ],
            )

    cv2.namedWindow("image")
    cv2.setMouseCallback("image", visualize_meta)

    clone = image.copy()

    # Loop until 'q' is pressed
    while True:

        for p in polygons:
            pixels = np.array(list(p.contour_points))
            image[pixels[:, 0], pixels[:, 1]] = 255

        # Display the image and wait for a keypress
        cv2.imshow("image", image)
        key = cv2.waitKey(1) & 0xFF

        # Press 'r' to reset the cropping region
        if key == ord("r"):
            image = clone.copy()

        # Press 'c' to break from the loop
        elif key == ord("c"):
            break
