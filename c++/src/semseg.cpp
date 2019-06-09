#include "semseg.hpp"
#include <vector>
#include "cnpy.h"

using namespace cnpy;

//---------------------------
// MaxActivation definitions
//---------------------------

MaxActivation::MaxActivation() {};
MaxActivation::MaxActivation(int idx, int val) : label(idx), score(val) {};

//--------------------
// Semseg definitions
//--------------------

Semseg::Semseg(char *filename) {
  NpyArray arr = npy_load(filename);
  rows = arr.shape[0], cols = arr.shape[1], cls = arr.shape[2];
  double* loaded_data = arr.data<double>();
  semseg.assign(rows, vector<vector<double>>(cols, vector<double>(cls, 0.0)));
  double sum = 0;
  for(int r = 0; r < rows; r++)
    for(int c = 0; c < cols; c++)
      for(int d = 0; d < cls; d++) {
        int idx = r * cols * cls + c * cls + d;
        semseg[r][c][d] = (double)loaded_data[idx];
        sum += semseg[r][c][c];
      }

};

int Semseg::getKey(int x, int y) {
  return y * cols + x;
}

int Semseg::getRows() {return rows;}
int Semseg::getCols() {return cols;}
int Semseg::getClasses() {return cls;}

bool Semseg::boundaryCheck(int x, int y) {
  return x >= 0 && x < cols && y >= 0 && y < rows;
}

MaxActivation Semseg::getLabelAndScore(int x, int y) {
  int key = getKey(x, y);

  // Check for cached values
  if(activations.find(key) != activations.end())
    return activations[key];

  // Find label with highest score
  double maxVal = -1;
  int maxIdx = -1;
  for(int i = 0; i < cls; i++) {

    double currVal = semseg[y][x][i];
    if(currVal > maxVal) {
      maxVal = currVal;
      maxIdx = i;
    }
  }
  MaxActivation act(maxIdx, maxVal);
  activations[key] = act; // cache
  return act;
}
