#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "gui.hpp"
#include <iostream>
#include <iomanip>
#include <string>

using namespace std;
using namespace cv;


/**
    Helper method which converts input to string format

    @param t : input
    @return string representation of t
*/
template <typename T>
string toStr(const T& t) {
   ostringstream os;
   os << setprecision(2) << t;
   return os.str();
}


/**
    Returns size of the widest text line in textlines
    @return size of widest text lines
*/
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


/**
    Draws a black rectangle with metadata (specified by 'lines') at coordinate (x, y)

    @param image
    @param x : x coordinate of point
    @param y : y coordinate of point
    @param lines : vector of text lines (metadata)
*/
void addTextLines(Mat image, int x, int y, vector<string> lines) {

  // Style parameters
  int fontface = FONT_HERSHEY_SIMPLEX;
  double scale = 0.4;
  int thickness = 1;
  int baseline = 0;
  int linePadding = 30;
  int paddingX = 5;
  int paddingY = 30;

  int imgHeight = image.rows;
  int imgWidth = image.cols;
  int n = lines.size();

  // Find the widest text line
  Size text = getMaxTextSize(lines, fontface, scale, thickness, baseline);
  // Height and width of the background rectangle
  int rectHeight = n * text.height + (n - 1) * linePadding;
  int rectWidth = text.width + 2 * paddingX;
  // Ensure we don't draw rect out of frame
  int yTop = min(imgHeight - rectHeight, max(0, y - paddingY));
  int xLeft = min(imgWidth - rectWidth, max(0, x - paddingX));
  // Upper left and bottom right corners of the rectangle
  Point upperLeft = Point(xLeft, yTop);
  Point bottomRight = Point(
    xLeft + rectWidth,
    yTop + rectHeight
  );
  // Draw background rectangle
  rectangle(image, upperLeft, bottomRight, Scalar(0, 0, 0), CV_FILLED);

  // Set text coordinates relative to upper left coordinate of rectangle
  x = xLeft + paddingX;
  y = yTop + paddingY;
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


/**
    Callback function that is triggered for left mouse clicks. Extracts relevant metadata
    for the coordinate of the click and calls addTextLines to display the data.

    @param event : event type
    @param x : x coordinate of click
    @param y : y coordinate of click
    @param flags : ignored
    @param userData : CallbackParams object containing image, segments and label names
*/
void mouseCallback(int event, int x, int y, int flags, void* userdata)
{
     if  ( event == EVENT_LBUTTONUP )
     {
        // Extract image and segments
        CallbackParams* params = (CallbackParams*) userdata;
        Mat image = params->image;
        vector<Segment*> segments = params->segments;
        vector<string> labels = params->labels;

        // Find the segment that was clicked
        Segment* s = segmentCorrespondingToPoint(segments, x, y);

        cout  << "Left button of the mouse is clicked - position (" << x
              << ", " << y << ") - label " << s->getLabel()
              << " - pid " << s->getId() << endl;

        // Define metadata to be visualized
        vector<string> lines;
        lines.push_back(toStr("Point (") + toStr(x) + toStr(", ") + toStr(y) + toStr(")"));
        lines.push_back(toStr("Name ") + toStr(labels[s->getLabel()]));
        lines.push_back(toStr("Label ") + toStr(s->getLabel()));
        lines.push_back(toStr("Score ") + toStr(s->getPixel(x, y).score));
        lines.push_back(toStr("Segment Score ") + toStr(s->getScore()));
        lines.push_back(toStr("Segment ID ") + toStr(s->getId()));

        // Draw metadata
        addTextLines(image, x, y, lines);
     }
}


/**
    Draws each segment contour to the image

    @param image
    @param segments : image segments
*/
void drawContours(Mat& image, vector<Segment*> segments) {
  for(Segment* s : segments)
    for(Pixel p : s->getCountour())
      for(int i = 0; i < 3; i++)
        image.at<Vec3b>(Point(p.x, p.y))[i] = COLORS[s->getLabel() % COLORS.size()][i];
}


/**
    Main GUI method. Calls other methods to draw image (with contours)
    and sets up callback for mouse click events. Then waits for key press to reset the frame
    or to the close the GUI.

    @param image
    @param segments : image segments
    @param labels : class names
*/
void visualize(Mat image, vector<Segment*> segments, vector<string> labels) {

  // Draw contours of segments
  drawContours(image, segments);

  // Define parameters for mouse click events
  CallbackParams params;
  params.image = image;
  params.segments = segments;
  params.labels = labels;

  // Create a clone to enable frame resets
  Mat imageCopy = image.clone();

  namedWindow("image", 1);
  setMouseCallback("image", mouseCallback, &params);

  while(true){

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
