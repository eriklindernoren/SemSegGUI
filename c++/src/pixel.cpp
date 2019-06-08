#include "pixel.hpp"

using namespace std;

Pixel::Pixel() {}

Pixel::Pixel(int xVal, int yVal, int labelVal, float scoreVal, int keyVal) {
  x = xVal;
  y = yVal;
  label = labelVal;
  score = scoreVal;
  key = keyVal;
}

Pixel::~Pixel(){}
