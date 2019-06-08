#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "cnpy.h"
#include <map>
#include <vector>

using namespace std;
using namespace cv;
using namespace cnpy;

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
    vector<vector<vector<double>>> mask;
    map<int, MaxActivation> activations;
    int rows;
    int cols;
    int cls;

  private:
    int getKey(int, int);
  public:
    Mask(char *filename);
    MaxActivation getLabelAndScore(int, int);
    int getRows();
    int getCols();
    int getClasses();
    bool boundaryCheck(int, int);
};


#endif // MASK_H
