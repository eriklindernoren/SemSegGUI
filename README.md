# Apple Home Assignment

## Preprocessing
To convert annotated Kitti segmentation data (shaped `H x W`), run:

```
python3 kitti_to_numpy.py --segmentation_path data/kitti/segmentation/000010_10.png
```

This will create a file `data/000010_10.npy` with the shape `H x W x NumClasses`, which will be used
as input to the application, along with the corresponding image `data/kitti/image_2/000010_10.png`.

## GUI
Left click to show meta data for a given pixel. To reset the popup-windows click the 'r' key.
To close the window click 'c' or 'q'.

## Implementations

### C++
#### Setup
The application uses [cnpy](https://github.com/rogersce/cnpy) to read .npy files containing the semantic segmentation.
When installing this package I got the same error as [here](https://github.com/rogersce/cnpy/issues/34), but solved it using
the steps in [this answer](https://github.com/rogersce/cnpy/issues/34#issuecomment-401502398).

#### Run
```
cd c++/
./app ../data/kitti/image_2/000010_10.png ../data/000010_10.npy
```

### Python
#### Setup
Dependencies:
```
numpy
cv2
argparse
```

#### Run
```
cd python/
python3 main.py --image_path ../data/kitti/image_2/000010_10.png --segmentation_path ../data/000010_10.npy
```
