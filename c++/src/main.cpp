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
  if( argc != 3)
  {
   cout <<" Usage: app <image-path> <semseg-path>" << endl;
   return -1;
  }

  // Extract frame
  char* image_path = argv[1];
  Mat image = loadImage(image_path);

  // Extract segmentation semseg
  char* semseg_path = argv[2];
  Semseg semseg(semseg_path);

  cout << "Segmentation shape: " << semseg.getRows() << " x " << semseg.getCols() << " x " << semseg.getClasses() << endl;

  // Extract segments from semseg
  vector<Segment*> segments = extractSegments(semseg);
  printf("Number of Segments %d\n", (int)segments.size());

  // Visualize segments
  visualize(image, segments);

  // Free allocated memory
  free(segments);

  return 0;
}
