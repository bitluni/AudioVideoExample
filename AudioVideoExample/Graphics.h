#pragma once
class TriangleTree;
class Font;

class Graphics
{ 
  public:
  int xres;
  int yres;
  char **frame;
  char **backbuffer;
  char **zbuffer;
  int cursorX, cursorY, cursorBaseX;
  int frontColor, backColor;
  Font *font;
  
  TriangleTree *triangleBuffer;
  TriangleTree *triangleRoot;
  int trinagleBufferSize;
  int triangleCount;

  Graphics(int w, int h, int initialTrinagleBufferSize = 0);
  void setTextColor(int front, int back = -1);
  void init();
  
  void setFont(Font &font);
  void setCursor(int x, int y);
  void print(const char *str);
  void print(int number, int base = 10, int minCharacters = 1);
  
  void begin(int clear = -1);
  void flush();
  void end();

  inline void dotFast(int x, int y, char color)
  {
    backbuffer[y][x] = color;
  }
  
  inline void dot(int x, int y, char color)
  {
    if((unsigned int)x < xres && (unsigned int)y < yres)
      backbuffer[y][x] = color;
  }
  
  inline void dotAdd(int x, int y, char color)
  {
    if((unsigned int)x < xres && (unsigned int)y < yres)
      backbuffer[y][x] = (color + backbuffer[y][x]) > 54 ? 54 : color + backbuffer[y][x];
  }
  
  inline char get(int x, int y)
  {
    if((unsigned int)x < xres && (unsigned int)y < yres)
      return backbuffer[y][x];
    return 0;
  }

  inline void xLine(int x0, int x1, int y, char color)
  {
    if(x0 > x1)
    {
      int xb = x0;
      x0 = x1;
      x1 = xb;
    }
    if(x0 < 0) x0 = 0;
    if(x1 > xres) x1 = xres;
    for(int x = x0; x < x1; x++)
      dotFast(x, y, color);
  }
    
  void enqueueTriangle(short *v0, short *v1, short *v2, char color);
  void triangle(short *v0, short *v1, short *v2, char color); 
  void line(int x1, int y1, int x2, int y2, char color);
  void fillRect(int x, int y, int w, int h, int color);
  void rect(int x, int y, int w, int h, int color);
};

