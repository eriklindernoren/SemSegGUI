#include "segment.hpp"

/**
  Comparison operator for Pixel objects. Utilizes unique key derived from
  each pixel's x and y coordinates.
*/
inline bool operator<(const Pixel& lhs, const Pixel& rhs) {
  return lhs.key < rhs.key;
}

//-------------------
// Segment functions
//-------------------

/**
  Constructor of the Segment class

  @param labelVal : segment label
  @param idVal : segment ID
  @param height : height of the image (used for boundary checks)
  @param width : width of the image (used for boundary checks)
*/
Segment::Segment(int labelVal, int idVal, int height, int width) {
  label = labelVal;
  id = idVal;
  imgHeight = height;
  imgWidth = width;
  score = 0;
};


/**
  Segment destructor
*/
Segment::~Segment() {}


/**
  Returns a unique key derived from the coordinate (x, y)

  @param x : x coordinate of point
  @param y : y coordinate of point
  @return key : unique key
*/
int Segment::getKey(int x, int y) {
  int key = y * imgWidth + x;
  return key;
}


/**
  Returns boolean of whether the point is contained within the segment

  @param x : x coordinate of point
  @param y : y coordinate of point
  @return bool
*/
bool Segment::seen(int x, int y) {
  int key = getKey(x, y);
  return pixels.find(key) != pixels.end();
}


/**
  Adds new pixel member to segment, and updates the global score of the segment

  @param x : x coordinate of point
  @param y : y coordinate of point
  @param label : label of the pixel
  @param pixelScore : score of the pixel
*/
void Segment::addPixel(int x, int y, int label, float pixelScore) {
  int key = getKey(x, y);
  Pixel pixel(x, y, label, pixelScore, key);
  pixels[key] = pixel;
  int size = getSize();
  score = (score * size + pixelScore) / (size + 1);
}


/**
  Adds pixel as contour pixel of the segment

  @param x : x coordinate of point
  @param y : y coordinate of point
  @param label : label of the pixel
  @param pixelScore : score of the pixel
*/
void Segment::addContourPixel(int x, int y, int label, float score) {
  int key = getKey(x, y);
  Pixel pixel(x, y, label, score, key);
  contourPixels.insert(pixel);
}


/**
  Returns the label of the segment
*/
int Segment::getLabel() {return label;}


/**
  Returns the size of the segment
*/
int Segment::getSize() {return pixels.size();}


/**
  Returns the ID of the segment
*/
int Segment::getId() {return id;}


/**
  Returns the global score of the segment
*/
float Segment::getScore() {return score;}


/**
  Returns the contour pixels of the segment
*/
set<Pixel> Segment::getCountour() {return contourPixels;}


/**
  Returns the pixel object corresponding to (x, y)
*/
Pixel Segment::getPixel(int x, int y) {
  int key = getKey(x, y);
  if(seen(x, y)) return pixels[key];
}

//-------------------
// Generic functions
//-------------------


/**
  Returns the segment of which the point is a member.

  @param segments : Known segments
  @param x : x coordinate of point
  @param y : y coordinate of point
*/
Segment* segmentCorrespondingToPoint(vector<Segment*> segments, int x, int y) {
  for(Segment* s: segments)
    if(s->seen(x, y))
      return s;
  return NULL;
}


/**
  Checks whether the point (x, y) is a member of any known segment

  @param segments : vector of pointers to known segments
  @param x : x coordinate of point
  @param y : y coordinate of point
  @param bool : true if the point is not a member of any segment, else false
*/
bool unexploredPoint(vector<Segment*> segments, int x , int y) {
  for(Segment* s: segments)
    if(s->seen(x, y))
      return false;
  return true;
}


/**
  Frees memory allocated for each segment
*/
void free(vector<Segment*> segments) {
  for(Segment* s: segments)
    delete s;
}
