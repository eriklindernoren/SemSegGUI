import numpy as np
import cv2


def load_image(image_path):
    """Loads input image"""
    image = cv2.imread(image_path)
    return image


def load_segmentation(segmentation_path):
    """Loads semantic segmentation"""
    segm = np.load(segmentation_path)
    return segm


def extract_labels(label_path):
    """Extracts segmentation labels from text file"""
    with open(label_path) as f:
        labels = f.read().splitlines()
    return labels
