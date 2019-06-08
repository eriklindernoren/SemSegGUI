#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "segment.hpp"

using namespace cv;
using namespace std;

struct CallbackParams {
  Mat image;
  vector<Segment*> segments;
};

template <typename T>
string toStr( const T & t );

Mat loadImage(char*);
void addTextLines(Mat, int, int, vector<string>);
void mouseCallback(int, int, int, int, void*);
void drawContours(Mat&, vector<Segment*>);
void visualize(Mat& image, vector<Segment*>);
