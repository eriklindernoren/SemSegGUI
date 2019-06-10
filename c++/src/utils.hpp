#include <opencv2/imgproc.hpp>
#include <vector>
#include <string>

using namespace std;
using namespace cv;

// Loads image corresponding to filename
Mat loadImage(char*);
// Extracts class labels from file
vector<string> extractLabels(char*);
