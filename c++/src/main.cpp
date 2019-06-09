#include "gui.hpp"
#include "processor.hpp"
#include "semseg.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <string>
#include "cnpy.h"

using namespace cv;
using namespace std;

int main(int argc, char** argv ) {
  if( argc != 4)
  {
   cout <<" Usage: app <image-path> <semseg-path> <label-path>" << endl;
   return -1;
  }

  // Extract frame
  char* image_path = argv[1];
  Mat image = loadImage(image_path);

  // Extract segmentation semseg
  char* semseg_path = argv[2];
  Semseg semseg(semseg_path);

  printf("Segmentation shape: %d x %d x %d\n", semseg.getRows(), semseg.getCols(), semseg.getClasses());

  // Extract class labels
  char* label_path = argv[3];
  vector<string> labels = extractLabels(label_path);

  printf("Number of classes: %d\n", (int)labels.size());

  // Extract segments from semseg
  vector<Segment*> segments = extractSegments(semseg);
  printf("Number of Segments %d\n", (int)segments.size());

  // Visualize segments
  visualize(image, segments, labels);

  // Free allocated memory
  free(segments);

  return 0;
}
