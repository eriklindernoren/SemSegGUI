#include "mask.hpp"
#include <vector>

//---------------------------
// MaxActivation definitions
//---------------------------

MaxActivation::MaxActivation() {};
MaxActivation::MaxActivation(int idx, int val) : label(idx), score(val) {};

//------------------
// Mask definitions
//------------------

Mask::Mask(char *filename) {
  NpyArray arr = npy_load(filename);
  rows = arr.shape[0], cols = arr.shape[1], cls = arr.shape[2];
  double* loaded_data = arr.data<double>();
  mask.assign(rows, vector<vector<double>>(cols, vector<double>(cls, 0.0)));
  double sum = 0;
  for(int r = 0; r < rows; r++)
    for(int c = 0; c < cols; c++)
      for(int d = 0; d < cls; d++) {
        int idx = r * cols * cls + c * cls + d;
        mask[r][c][d] = (double)loaded_data[idx];
        sum += mask[r][c][c];
      }

};

int Mask::getKey(int x, int y) {
  return y * cols + x;
}

int Mask::getRows() {return rows;}
int Mask::getCols() {return cols;}
int Mask::getClasses() {return cls;}

bool Mask::boundaryCheck(int x, int y) {
  return x >= 0 && x < cols && y >= 0 && y < rows;
}

MaxActivation Mask::getLabelAndScore(int x, int y) {
  int key = getKey(x, y);

  // Check for cached values
  if(activations.find(key) != activations.end())
    return activations[key];

  // Find label with highest score
  double maxVal = -1;
  int maxIdx = -1;
  for(int i = 0; i < cls; i++) {

    double currVal = mask[y][x][i];
    if(currVal > maxVal) {
      maxVal = currVal;
      maxIdx = i;
    }
  }
  MaxActivation act(maxIdx, maxVal);
  activations[key] = act; // cache
  return act;
}
