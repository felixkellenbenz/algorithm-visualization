#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <cstdint>
#include <SDL2/SDL.h>
#include <iterator>
#include <memory>
#include <unordered_map>
#include <vector>

struct Color 
{
  uint8_t red;
  uint8_t green;
  uint8_t blue;
  uint8_t alpha;
};

struct Coordinate
{
  int32_t x;
  int32_t y;
};

/*Class that encapsulates a Rect*/
class Node
{
private:
  SDL_Rect rect;
  std::vector<std::shared_ptr<Node>> neighbours;
  Color drawing_color;

public: 
  bool is_free();
  

};

/*A special class representing a Obstacle Node*/
class ObstacleNode : public Node
{
private:
  Color drawing_color;

public:
  ObstacleNode() : {}
  bool is_free();

};

/* Class representing the Grid on
 * which algorithms will be executed */
class Grid
{
private:

  std::unordered_map<Coordinate, Node> rects;
  Node start;

public:
  Grid() : {}
  ~Grid();
  Grid(const Grid&);
  Node& find_node(int32_t x, int32_t y);
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
  GridBuilder() : {}
  void build_grid(); 
  Grid export_grid();

};

/*Class for modeling a Grid Editor*/
class GridEditor
{



};

/*Visitor style grid renderer*/
class GridRenderer
{
private:
    SDL_Renderer *renderer;

public:
  GridRenderer(SDL_Renderer* renderer) : renderer(renderer) {}
  void render(ObstacleNode);
  void render(Node);
};


