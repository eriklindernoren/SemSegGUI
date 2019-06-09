#include "processor.hpp"
#include <stack>


/**
  Returns all neighboring points of (x, y)

  @param x : x coordinate of point
  @param y : y coordinate of point
  @return a vector of neighboring points to (x, y)
*/
vector<vector<int>> getNeighbors(int x, int y) {
  vector<vector<int>> deltas = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {1, 1}, {-1, 1}, {1, -1}};
  vector<vector<int>> neighbors(deltas.size(), vector<int>(2, 0));
  // Add deltas to point to get neighbors
  for(int i = 0; i < deltas.size(); i++) {
    neighbors[i][0] = x + deltas[i][0];
    neighbors[i][1] = y + deltas[i][1];
  }
  return neighbors;
}


/**
  Explore points contained in segment through tree search (dfs). Adds pixels
  contained in the segment to a segment object (along with contour points), and finally
  returns the segment object when all pixels have been visited.

  @param semseg : semantic segmentation
  @param x : x coordinate of starting point
  @param y : y coordinate of starting point
  @param segmentId : id of the segment that is returned
  @return a new segment object containing all member pixels
*/
Segment* extractSegment(Semseg& semseg, int x, int y, int segmentId) {
  // Extract label and score at coordinate
  MaxActivation act = semseg.getLabelAndScore(x, y);
  int label = act.label;
  Segment* segment = new Segment(label, segmentId, semseg.getRows(), semseg.getCols());

  // Intialize stack with point (x, y)
  stack<vector<int>> s;
  s.push(vector<int>{x, y});

  // Expand for search and save member points as well as contour points
  while(!s.empty()) {
    vector<int> point = s.top();
    s.pop();
    int x = point[0], y = point[1];
    // Get label and score at (x, y)
    MaxActivation act = semseg.getLabelAndScore(x, y);
    // Add as member of the segment
    segment->addPixel(x, y, act.label, act.score);
    // Explore neighbors
    for(vector<int> neighbor : getNeighbors(x, y)) {
      int nx = neighbor[0], ny = neighbor[1];
      // Add as contour point if new point is out of bounds
      if(!semseg.boundaryCheck(nx, ny)) {
        segment->addContourPixel(x, y, act.label, act.score);
        continue;
      }
      // Extract label and score at neighboring point
      MaxActivation nAct = semseg.getLabelAndScore(nx, ny);
      // Label missmatch => contour point
      if(nAct.label != segment->getLabel())
        segment->addContourPixel(x, y, act.label, act.score);
      // Explore neighbor point if it hasn't been visited
      else if(!segment->seen(nx, ny)) {
        s.push(neighbor);
      }
    }
  }
  return segment;
}


/**
  Extracts all segments from semantic segmentation input

  @param semseg : semantic segmentation, shaped (H x W x NumClasses)
  @return a vector of pointers to image segments (class Segment)
*/
vector<Segment*> extractSegments(Semseg& semseg) {
  vector<Segment*> segments;
  printf("Segment\t\tLabel\t\tPixels\t\tScore\n");
  // Iterate through pixels and explore each new segment
  for(int y = 0; y < semseg.getRows(); y++) {
    for(int x = 0; x < semseg.getCols(); x++) {
      // If the point is not a member of a known segment => explore
      if(unexploredPoint(segments, x, y)) {
        int segmentId = segments.size();
        Segment* s = extractSegment(semseg, x, y, segmentId);
        segments.push_back(s);
        printf("%d\t\t%d\t\t%d\t\t%f\n", segmentId, s->getLabel(), (int)s->getSize(), s->getScore());
      }
    }
  }

  return segments;
}
