#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <cstdint>
#include <SDL2/SDL.h>
#include <vector>

class Color 
{
private:
  uint8_t red;
  uint8_t green;
  uint8_t blue;
  uint8_t alpha;

public:
  Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) :
    red(r), green(g), blue(b), alpha(a) {}

  Color(uint8_t r, uint8_t g, uint8_t b) :
    red(r), green(g), blue(b), alpha(SDL_ALPHA_OPAQUE) {}

};

/*Abstraction for something that should be visible on the Screen*/
class ScreenElement
{
public:
  virtual void render(SDL_Renderer*) = 0;

};


/*Class that encapsulates a Rect*/
class Node
{
private:
  SDL_Rect rect;
  std::vector<Node> neighbours;

public:
  void render();

};

/*A special class representing a Obstacle Node*/
class ObstacleNode
{
private:

public:
  void render();

};

/* Class representing the Grid on
 * which algorithms will be executed */
class Grid
{
private:

  std::vector<Node> free_rects;
  std::vector<ObstacleNode> obstacle_rects;
  ScreenElement* start_rect;

public:
  Grid();
  ~Grid();
  Grid(const Grid&);
  Node& find_node(int x, int y);
  void render();

};

/*Build a grid with only free rects*/
class GridBuilder
{
private:
  uint32_t grid_w;
  uint32_t grid_h;
  uint8_t border;
  Color grid_color;
  Grid grid;

public:
  void build_grid(); 
  Grid export_grid();

};

/*Class for modeling a Grid Editor*/
class GridEditor
{



};


