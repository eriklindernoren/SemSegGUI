#include "utils.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <fstream>


using namespace std;
using namespace cv;


/**
    Loads an image corresponding to specified filename

    @param filename : filename of image to load
    @return image
*/
Mat loadImage(char* filename) {
  Mat image = imread(filename, CV_LOAD_IMAGE_COLOR);
  return image;
}


/**
    Extract class names contained in file

    @param filename : filename pointing to text file with class names
    @return vector of class names
*/
vector<string> extractLabels(char* filename) {
  ifstream file(filename);
  vector<string> labels;
  string str;
  while(getline(file, str))
    labels.push_back(str);
  return labels;
}
