#pragma once
#include "Image.h"

class Sprite: public Image
{
  public:
  int transparency;
  unsigned char pointCount;
  const signed short (*points)[2];
  
  void init(int xres, int yres, const unsigned char *pixels, unsigned char pointCount, const signed short points[][2], int transparency)
  {
    static const signed short zeroPoint[][2] = {0, 0};
    Image::init(xres, yres, pixels);
    this->transparency = transparency;
    if(pointCount)
    {
      this->pointCount = pointCount;
      this->points = points;
    }
    else
    {
      this->pointCount = 1;
      this->points = zeroPoint;      
    }
  }

  void draw(Graphics &g, int x, int y)
  {
    Image::draw(g, x - points[0][0], y - points[0][1], transparency);
  }
};

class Sprites
{
  public:
  int count;
  Sprite *sprites;
  
  Sprites(int count, const unsigned char pixels[], const int offsets[], const unsigned short resolutions[][2], const signed short points[][2], const short pointOffsets[], int transparency)
  {
    this->count = count;
    this->sprites = new Sprite[count];
    for(int i = 0; i < count; i++)
      this->sprites[i].init(resolutions[i][0], resolutions[i][1], &pixels[offsets[i]], pointOffsets[i + 1] - pointOffsets[i], &points[pointOffsets[i]], transparency);
  }

  void draw(Graphics &g, int sprite, int x, int y)
  {
    sprites[sprite].draw(g, x, y);
  }

  int xres(int sprite)
  {
    return sprites[sprite].xres;
  }
  
  int yres(int sprite)
  {
    return sprites[sprite].yres;    
  }

  const short (*points(int sprite))[2]
  {
    return sprites[sprite].points;
  }
  
  const short *point(int sprite, int point)
  {
    return sprites[sprite].points[point];
  }
};

