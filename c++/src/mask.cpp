#include "mask.hpp"

//---------------------------
// MaxActivation definitions
//---------------------------

MaxActivation::MaxActivation() {};
MaxActivation::MaxActivation(int idx, int val) : label(val), score(val) {};

//------------------
// Mask definitions
//------------------

int Mask::getKey(int x, int y) {
  return y * mask.cols + x;
}

int Mask::getRows() {return mask.rows;}
int Mask::getCols() {return mask.cols;}

bool Mask::boundaryCheck(int x, int y) {
  return x >= 0 && x < mask.cols && y >= 0 && y < mask.rows;
}

Mask::Mask(Mat& m) {
  mask = m;
};

MaxActivation Mask::getLabelAndScore(int x, int y) {
  int key = getKey(x, y);

  // Check for cached values
  if(activations.find(key) != activations.end())
    return activations[key];

  // Find label with highest score
  int val = -1, idx;
  for(int i = 0; i < mask.channels(); i++) {
    int currVal = (int) mask.at<Vec3b>(y, x).val[i];
    if(currVal > val) {
      val = currVal;
      idx = i;
    }
  }
  MaxActivation act(idx, val);
  activations[key] = act; // cache
  return act;
}
