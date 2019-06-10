#include "utils.hpp"
#include "gui.hpp"
#include "processor.hpp"
#include "semseg.hpp"
#include <iostream>
#include "cnpy.h"

using namespace cv;
using namespace std;

int main(int argc, char** argv ) {
  if( argc != 4)
  {
   printf("Usage: ./app <image-path> <semseg-path> <label-path>\n");
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

  // Extract each segments from the semantic segmentation
  vector<Segment*> segments = extractSegments(semseg);
  printf("Number of Segments %d\n", (int)segments.size());

  // Trigger GUI
  visualize(image, segments, labels);

  // Free allocated memory
  free(segments);

  return 0;
}
