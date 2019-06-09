#include <opencv2/imgproc.hpp>
#include "segment.hpp"

using namespace cv;
using namespace std;

// Segment colors
const vector<vector<int>> COLORS = {
    {0, 0, 0},
    {255, 255, 255},
    {255, 0, 0},
    {0, 255, 0},
    {0, 0, 255},
    {255, 255, 0},
    {0, 255, 255},
    {255, 0, 255},
    {192, 192, 192},
    {128, 128, 128},
    {128, 0, 0},
    {128, 128, 0},
    {0, 128, 0},
    {128, 0, 128},
    {0, 128, 128},
    {0, 0, 128},
};

// Stores data to be extracted in callback function
struct CallbackParams {
  Mat image;
  vector<Segment*> segments;
  vector<string> labels;
};

// Converts input to string repr.
template <typename T>
string toStr( const T & t );
// Loads image corresponding to filename
Mat loadImage(char*);
// Extracts class labels from file
vector<string> extractLabels(char*);
// Draws metadata to image
void addTextLines(Mat, int, int, vector<string>);
// Callback for left mouse clicks. Triggers drawing of metadata
void mouseCallback(int, int, int, int, void*);
// Draws contours of image segments
void drawContours(Mat&, vector<Segment*>);
// Main GUI method. Adds event listeners and draws image.
void visualize(Mat image, vector<Segment*>, vector<string>);
