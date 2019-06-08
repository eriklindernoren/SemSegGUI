#include "segment.hpp"
#include "mask.hpp"

vector<vector<int>> getNeighbors(int, int);
Segment* extractSegment(Mask&, int, int, int);
vector<Segment*> extractSegments(Mask&);
