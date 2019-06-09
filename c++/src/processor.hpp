#include "segment.hpp"
#include "semseg.hpp"

// Returns neighbor points of (x, y)
vector<vector<int>> getNeighbors(int, int);
// Returns full segment of which (x, y) is a member
Segment* extractSegment(Semseg&, int, int, int);
// Returns all segments in semenatic segmentation
vector<Segment*> extractSegments(Semseg&);
