#include "semseg.hpp"
#include <vector>
#include "cnpy.h"

using namespace cnpy;

//---------------------------
// MaxActivation definitions
//---------------------------


/**
  Constructors of MaxActivation
*/
MaxActivation::MaxActivation() {};
MaxActivation::MaxActivation(int idx, int val) : label(idx), score(val) {};

//--------------------
// Semseg definitions
//--------------------


/**
  Semseg constructor. Reads semantic segmentation data in the .npy file corresponding to the passed
  filepath and stores the data in a 3D vector.

  @param filename : filepath of .npy file containing semseg data
*/
Semseg::Semseg(char *filename) {
  // Read semantic segmentation
  NpyArray arr = npy_load(filename);
  // Semantic segmentation shape
  rows = arr.shape[0], cols = arr.shape[1], cls = arr.shape[2];
  double* loaded_data = arr.data<double>();
  semseg.assign(rows, vector<vector<double>>(cols, vector<double>(cls, 0.0)));
  // Assign values to target vector
  for(int r = 0; r < rows; r++)
    for(int c = 0; c < cols; c++)
      for(int d = 0; d < cls; d++) {
        int idx = r * cols * cls + c * cls + d;
        semseg[r][c][d] = (double)loaded_data[idx];
      }

};


/**
  Extracts unique key corresponding to point (x, y)

  @param x : x coordinate of point
  @param y : y coordinate of point
  @return key : unique key
*/
int Semseg::getKey(int x, int y) {
  return y * cols + x;
}


/**
  Returns number of rows in the segmentation data
*/
int Semseg::getRows() {return rows;}


/**
  Returns number of rows in the segmentation data
*/
int Semseg::getCols() {return cols;}


/**
  Returns number of classes in (depth of) the segmentation data
*/
int Semseg::getClasses() {return cls;}


/**
  Checks wether the point (x, y) is within the bounds of the data
*/
bool Semseg::boundaryCheck(int x, int y) {
  return x >= 0 && x < cols && y >= 0 && y < rows;
}


/**
  Extracts the label corresponding to the maximum output at coordinate (x, y),
  as well as the size of the output (score)

  @param x : x coordinate of point
  @param y : y coordinate of point
  @return act : MaxActivation object containing label and score
*/
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
  // Create object with label and score
  MaxActivation act(maxIdx, maxVal);
  activations[key] = act; // cache
  return act;
}
