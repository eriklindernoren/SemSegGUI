import numpy as np
import cv2
import argparse

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Parser")
    parser.add_argument("--segmentation_path", type=str, required=True, help="path to segmentation mask")
    args = parser.parse_args()

    image = cv2.imread(args.segmentation_path)
    image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)

    h, w = image.shape[:2]
    x = np.zeros((h, w, image[..., 0].max() + 1))

    for r in range(h):
        for c in range(w):
            x[r, c, image[r, c, 0]] = 1

    filename = args.segmentation_path.split("/")[-1].split(".")[0]
    filepath = f"data/{filename}.npy"
    np.save(filepath, x)
    print(f"Saved file to '{filepath}'")
