#include "segment.hpp"

using namespace std;

inline bool operator<(const Pixel& lhs, const Pixel& rhs) {
  return lhs.key < rhs.key;
}

//-------------------
// Segment functions
//-------------------

Segment::Segment(int labelVal, int idVal, int height, int width) {
  label = labelVal;
  id = idVal;
  imgHeight = height;
  imgWidth = width;
  score = 0;
};

Segment::~Segment() {}

int Segment::getKey(int x, int y) {
  int key = y * imgWidth + x;
  return key;
}

bool Segment::seen(int x, int y) {
  int key = getKey(x, y);
  return pixels.find(key) != pixels.end();
}

void Segment::addPixel(int x, int y, int label, float pixelScore) {
  int key = getKey(x, y);
  Pixel pixel(x, y, label, pixelScore, key);
  pixels[key] = pixel;
  int size = getSize();
  score = (score * size + pixelScore) / (size + 1);
}

void Segment::addContourPixel(int x, int y, int label, float score) {
  int key = getKey(x, y);
  Pixel pixel(x, y, label, score, key);
  contourPixels.insert(pixel);
}

int Segment::getLabel() {return label;}
int Segment::getSize() {return pixels.size();}
int Segment::getId() {return id;}
float Segment::getScore() {return score;}

set<Pixel> Segment::getCountour() {return contourPixels;}

Pixel Segment::getPixel(int x, int y) {
  int key = getKey(x, y);
  if(seen(x, y)) return pixels[key];
}

//-------------------
// Generic functions
//-------------------

Segment* segmentCorrespondingToPoint(vector<Segment*> segments, int x, int y) {
  for(Segment* s: segments)
    if(s->seen(x, y))
      return s;
  return NULL;
}

bool unexploredPoint(vector<Segment*> segments, int x , int y) {
  for(Segment* s: segments)
    if(s->seen(x, y))
      return false;
  return true;
}

void free(vector<Segment*> segments) {
  for(Segment* s: segments)
    delete s;
}
