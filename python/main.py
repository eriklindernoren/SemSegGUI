import argparse
import os
import sys
import random
import numpy as np
from utils import load_image
from utils import load_segmentation
from utils import extract_labels
from gui import visualize
from processor import extract_segments

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Parser")
    parser.add_argument("--image_path", type=str, required=True, help="path to image")
    parser.add_argument("--segmentation_path", type=str, required=True, help="path to semantic segmentation")
    parser.add_argument("--label_path", type=str, required=True, help="path to text file with label names")
    args = parser.parse_args()

    image = load_image(args.image_path)
    semseg = load_segmentation(args.segmentation_path)
    labels = extract_labels(args.label_path)

    print(f"+ Segmentation shape: {semseg.shape}")

    # Extract image segments from semantic segmentation
    segments = extract_segments(semseg)

    print(f"+ Number of segments: {len(segments)}")

    # Trigger GUI and visualize segments
    visualize(image, segments, labels)
