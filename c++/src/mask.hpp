#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <map>

using namespace std;
using namespace cv;

#ifndef MAXACTIVATION_H
#define MAXACTIVATION_H

typedef struct MaxActivation {
  int label;
  int score;
  MaxActivation();
  MaxActivation(int, int);
} MaxActivation;

#endif // MAXACTIVATION_H


#ifndef MASK_H
#define MASK_H

class Mask {
    Mat mask;
    map<int, MaxActivation> activations;

  private:
    int getKey(int, int);
  public:
    Mask(Mat&);
    MaxActivation getLabelAndScore(int, int);
    int getRows();
    int getCols();
    bool boundaryCheck(int, int);
};


#endif // MASK_H
