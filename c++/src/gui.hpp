#include <opencv2/imgproc.hpp>
#include "segment.hpp"

using namespace cv;
using namespace std;

// Cityscapes class colors
// Ref: https://github.com/mcordts/cityscapesScripts/blob/master/cityscapesscripts/helpers/labels.py
const vector<vector<int>> COLORS = {
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {111, 74, 0},
  {81, 0, 81},
  {128, 64, 128},
  {244, 35, 232},
  {250, 170, 160},
  {230, 150, 140},
  {70, 70, 70},
  {102, 102, 156},
  {190, 153, 153},
  {180, 165, 180},
  {150, 100, 100},
  {150, 120, 90},
  {153, 153, 153},
  {153, 153, 153},
  {250, 170, 30},
  {220, 220, 0},
  {107, 142, 35},
  {152, 251,152},
  {70, 130, 180},
  {220, 20, 60},
  {255, 0, 0},
  {0, 0, 142},
  {0, 0, 70},
  {0, 60, 100},
  {0, 0, 90},
  {0, 0, 110},
  {0, 80, 100},
  {0, 0, 230},
  {119, 11, 32},
  {0,  0, 142},
};

// Stores data to be extracted in callback function
struct CallbackParams {
  Mat image;
  vector<Segment*> segments;
  vector<string> labels;
};

// Converts input to string repr.
template <typename T>
string toStr(const T& t);
// Draws metadata to image
void addTextLines(Mat, int, int, vector<string>);
// Callback for left mouse clicks. Triggers drawing of metadata
void mouseCallback(int, int, int, int, void*);
// Draws contours of image segments
void drawContours(Mat&, vector<Segment*>);
// Main GUI method. Adds event listeners and draws image.
void visualize(Mat image, vector<Segment*>, vector<string>);
