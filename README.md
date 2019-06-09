# Apple Home Assignment

## Preprocessing
To convert annotated Kitti segmentation data (shaped `H x W`), run:

```
python3 kitti_to_numpy.py --segmentation_path data/kitti/segmentation/000010_10.png
```

This will create a file `data/000010_10.npy` with the shape `H x W x NumClasses`, which will be used
as input to the application, along with the corresponding image `data/kitti/image_2/000010_10.png`.

## GUI
Actions:
```
+ Left mouse click  : Triggers popup with metadata
+ 'r'               : Resets the frame
+ 'c'               : Closes the application
+ 'q'               : Closes the application
```

## C++
### Setup
The application uses [cnpy](https://github.com/rogersce/cnpy) to read .npy files containing the semantic segmentation.
When installing this package I got the same error as [here](https://github.com/rogersce/cnpy/issues/34), but solved it using
the steps in [this answer](https://github.com/rogersce/cnpy/issues/34#issuecomment-401502398).

### Files
```
+ gui.{hpp/cpp}         : Methods pertaining to gui operations
+ main.cpp              : Main file
+ pixel.{hpp/cpp}       : Class representing a pixel (contains score and label)
+ processor.{hpp/cpp}   : Extracts segments from the semantic segmentation
+ segment.{hpp/cpp}     : Class representing each image segment. Contains segment pixels and contour pixels.
+ semseg.{hpp/cpp}      : Class representing semantic segmentation
```

### Run
```
cd c++/
./app ../data/kitti/image_2/000010_10.png ../data/000010_10.npy
```

## Python
### Setup
Dependencies:
```
numpy
cv2
argparse
```

### Files
```
+ main.py               : Main file
+ processor.py          : File containing classes representing segments and pixels. Extracts segments
                          from semantic segmentation.
+ utils.py              : Contains gui operations as well as helper methods for loading image and semantic segmentation.
```

### Run
```
cd python/
python3 main.py --image_path ../data/kitti/image_2/000010_10.png --segmentation_path ../data/000010_10.npy
```
