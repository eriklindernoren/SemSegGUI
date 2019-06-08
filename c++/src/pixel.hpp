#ifndef PIXEL_H
#define PIXEL_H

struct Pixel {
  int x;
  int y;
  int label;
  float score;
  int key;
  Pixel();
  Pixel(int, int, int, float, int);
  ~Pixel();
};

#endif // PIXEL_H
