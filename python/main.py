import argparse
import os
import sys
import random
import numpy as np
from utils import visualize, load_image, load_segmentation, extract_labels
from processor import extract_segments

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Parser")
    parser.add_argument("--image_path", type=str, required=False, help="path to image")
    parser.add_argument("--segmentation_path", type=str, required=False, help="path to semantic mask")
    parser.add_argument("--label_path", type=str, required=False, help="path to semantic mask")
    args = parser.parse_args()

    image = load_image(args.image_path)
    mask = load_segmentation(args.segmentation_path)
    labels = extract_labels(args.label_path)

    segments = extract_segments(mask)

    visualize(image, segments, labels)
