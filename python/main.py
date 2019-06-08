import argparse
import os
import sys
import random
from PIL import Image
import numpy as np
from utils import visualize, load_image, load_annotation
from contour_tracing import MooreNeighborTracer

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Parser")
    parser.add_argument("--image_path", type=str, required=False, help="path to image")
    parser.add_argument("--annotation_path", type=str, required=False, help="path to semantic mask")
    args = parser.parse_args()

    image = load_image(args.image_path)
    mask = load_annotation(args.annotation_path)

    tracer = MooreNeighborTracer()
    segments = tracer.extract_segments(mask)

    visualize(image, segments)
