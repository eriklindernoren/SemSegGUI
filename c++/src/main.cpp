#include "image_utils.hpp"
#include "processor.hpp"
#include "mask.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <string>

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

  // Extract segmentation mask
  char* mask_path = argv[2];
  Mat semseg = loadImage(mask_path);

  printf("(%d x %d x %d)\n", semseg.rows, semseg.cols, semseg.channels());
  Mask mask = Mask(semseg);

  // Extract segments from mask
  vector<Segment*> segments = extractSegments(mask);

  printf("Number of Segments %d\n", (int)segments.size());

  // Visualize segments
  visualize(image, segments);

  // Free allocated memory
  free(segments);

  return 0;
}
