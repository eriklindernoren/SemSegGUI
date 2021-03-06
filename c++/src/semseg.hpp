#include <map>
#include <vector>

using namespace std;

#ifndef MAXACTIVATION_H
#define MAXACTIVATION_H

// Class with label corresponding to the largest output at a point (x, y)
// in the semantic segmentation. Also contains size of output (score).
typedef struct MaxActivation {
  int label;
  int score;
  MaxActivation();
  MaxActivation(int, int);
} MaxActivation;

#endif // MAXACTIVATION_H

#ifndef SEMSEG_H
#define SEMSEG_H


// Class representing the semantic segmentation input
class Semseg {
    vector<vector<vector<double>>> semseg;
    map<int, MaxActivation> activations;
    int rows;
    int cols;
    int cls;

  private:
    int getKey(int, int);
  public:
    Semseg(char *filename);
    MaxActivation getLabelAndScore(int, int);
    int getRows();
    int getCols();
    int getClasses();
    bool boundaryCheck(int, int);
};

#endif // SEMSEG_H
