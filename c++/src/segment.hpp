#ifndef SEGMENT_H
#define SEGMENT_H

#include "pixel.hpp"
#include <set>
#include <map>
#include <vector>

using namespace std;

// Class representing image segment
class Segment {
  int label;
  set<Pixel> contourPixels;
  map<int, Pixel> pixels;
  int id;
  int imgHeight;
  int imgWidth;
  float score;

  private:
    int getKey(int, int);

  public:
    Segment(int, int, int, int);
    ~Segment();
    bool seen(int, int);
    void addPixel(int, int, int, float);
    void addContourPixel(int, int, int, float);
    int getLabel();
    int getSize();
    float getScore();
    set<Pixel> getCountour();
    int getId();
    Pixel getPixel(int, int);
};

#endif // SEGMENT_H

// Returns segment of which (x, y) is a member
Segment* segmentCorrespondingToPoint(vector<Segment*>, int, int);
// Returns true if (x, y) is not a member of any segment
bool unexploredPoint(vector<Segment*>, int, int);
// Frees memory allocated to segments
void free(vector<Segment*>);
