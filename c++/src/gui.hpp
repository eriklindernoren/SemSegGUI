#include <opencv2/imgproc.hpp>
#include "segment.hpp"

using namespace cv;
using namespace std;

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

struct CallbackParams {
  Mat image;
  vector<Segment*> segments;
  vector<string> labels;
};

template <typename T>
string toStr( const T & t );
Mat loadImage(char*);
vector<string> extractLabels(char*);
void addTextLines(Mat, int, int, vector<string>);
void mouseCallback(int, int, int, int, void*);
void drawContours(Mat&, vector<Segment*>);
void visualize(Mat image, vector<Segment*>, vector<string>);
