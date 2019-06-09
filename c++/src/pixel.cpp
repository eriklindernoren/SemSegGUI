#include "pixel.hpp"

using namespace std;

/**
  Constructor of Pixel
*/
Pixel::Pixel() {}

/**
    Constructor of Pixel

    @param xVal : x coordinate of the pixel
    @param yVal : y coordinate of the pixel
    @param labelVal : label of the segment at pixel
    @param scoreVal : score of label at pixel
    @param keyVal : Unique identifier corresponding to point (x, y)
*/
Pixel::Pixel(int xVal, int yVal, int labelVal, float scoreVal, int keyVal) {
  x = xVal;
  y = yVal;
  label = labelVal;
  score = scoreVal;
  key = keyVal;
}


/**
  Destructor of Pixel
*/
Pixel::~Pixel(){}
