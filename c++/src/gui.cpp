#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "gui.hpp"
#include <iostream>
#include <iomanip>
#include <string>


template <typename T>
string toStr( const T & t ) {
   ostringstream os;
   os << setprecision(2) << t;
   return os.str();
}


Mat loadImage(char* filename) {
  Mat image = imread(filename, CV_LOAD_IMAGE_COLOR);
  return image;
}


Size getMaxTextSize(vector<string> lines, int fontface, double scale, int thickness, int baseline) {
  Size maxSz;
  maxSz.width = -1;
  for(string s : lines) {
    Size sz = getTextSize(s, fontface, scale, thickness, &baseline);
    if(sz.width > maxSz.width)
      maxSz = sz;
  }
  return maxSz;
}


void addTextLines(Mat image, int x, int y, vector<string> lines) {

  // Style parameters
  int fontface = FONT_HERSHEY_SIMPLEX;
  double scale = 0.4;
  int thickness = 1;
  int baseline = 0;
  int linePadding = 30;
  int paddingX = 5;
  int paddingY = 15;

  int n = lines.size();

  // Find the widest text line
  Size text = getMaxTextSize(lines, fontface, scale, thickness, baseline);

  // Top left corner of rectangle
  int y_top = y - text.height - paddingY;
  int x_left = x - paddingX;
  // Upper left and bottom right corners of the rectangle
  Point upperLeft = Point(x_left, y_top);
  Point bottomRight = Point(
    x_left + text.width + 2 * paddingX,
    y_top + n * text.height + (n - 1) * linePadding
  );
  // Draw background rectangle
  rectangle(image, upperLeft, bottomRight, Scalar(0, 0, 0), CV_FILLED);

  // Draw text lines on top of the rectangle
  for(int i = 0; i < lines.size(); i++) {
    putText(
        image, lines[i], Point(x, y), fontface, scale, Scalar(255, 255, 255), thickness, 8
    );
    y += linePadding;
  }
  // Display image
  imshow("image", image);
}


void mouseCallback(int event, int x, int y, int flags, void* userdata)
{
     if  ( event == EVENT_LBUTTONUP )
     {
        // Extract image and segments
        CallbackParams* params = (CallbackParams*) userdata;
        Mat image = params->image;
        vector<Segment*> segments = params->segments;

        // Find the segment that was clicked
        Segment* s = segmentCorrespondingToPoint(segments, x, y);

        cout  << "Left button of the mouse is clicked - position (" << x
              << ", " << y << ") - label " << s->getLabel()
              << " - pid " << s->getId() << endl;

        // Define metadata to be visualized
        vector<string> lines;
        lines.push_back(toStr("Point (") + toStr(x) + toStr(", ") + toStr(y) + toStr(")"));
        lines.push_back(toStr("Label ") + toStr(s->getLabel()));
        lines.push_back(toStr("Score ") + toStr(s->getPixel(x, y).score));
        lines.push_back(toStr("Segment Score ") + toStr(s->getScore()));
        lines.push_back(toStr("PID ") + toStr(s->getId()));

        // Draw metadata
        addTextLines(image, x, y, lines);
     }
}


map<int, vector<int>> getColorMap(vector<Segment*> segments) {
  map<int, vector<int>> colorMap;
  for(Segment* s : segments) {
    if(colorMap.find(s->getLabel()) == colorMap.end())
      colorMap[s->getLabel()] = COLORS[colorMap.size() % COLORS.size()];
  }
  return colorMap;
}


void drawContours(Mat& image, vector<Segment*> segments, map<int, vector<int>> colorMap) {
  for(Segment* s : segments)
    for(Pixel p : s->getCountour())
      for(int i = 0; i < 3; i++)
        image.at<Vec3b>(Point(p.x, p.y))[i] = colorMap[s->getLabel()][i];
}


void visualize(Mat image, vector<Segment*> segments) {

  // Get label to color map and draw contours
  map<int, vector<int>> colorMap = getColorMap(segments);
  drawContours(image, segments, colorMap);

  // Define parameters for mouse click events
  CallbackParams params;
  params.image = image;
  params.segments = segments;

  // Create a clone to enable frame resets
  Mat imageCopy = image.clone();

  namedWindow("image", 1);
  setMouseCallback("image", mouseCallback, &params);

  while(true){

     // Display the image and wait for a keypress
     imshow("image", params.image);

     // Wait for key command
     char key = waitKey(1) & 0xFF;

     // Press 'r' to reset the frame
     if(key == 'r')
         params.image = imageCopy.clone();
     // Press 'c' or 'q' to break from the loop
     else if(key == 'c' || key == 'q')
         break;
  }
}