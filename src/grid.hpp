#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL.h>
#include <bits/types/cookie_io_functions_t.h>
#include <cstdint>
#include <memory>
#include <unordered_map>
#include <vector>


typedef struct Color 
{
  uint8_t red;
  uint8_t green;
  uint8_t blue;
  uint8_t alpha;
} Color;

typedef struct Coordinate
{
  int32_t x;
  int32_t y;
  Coordinate() : x(0), y(0) {}
  Coordinate(Coordinate const& cord): x(cord.x), y(cord.y) {}
} Coordinate;

template<>
struct std::hash<Coordinate>
{
  std::size_t operator()(Coordinate const& coord)
  {
    std::hash<int32_t> hash;
    return hash(coord.x) ^ (hash(coord.y) << 1);
  }
};

/*Class that encapsulates a Rect*/
class Node
{
private:
  SDL_Rect const rect;
  std::vector<Node> neighbours;
  Color color;

public:
  Node(SDL_Rect const& rect) 
      : rect(rect), neighbours() {}

  bool is_free();
  SDL_Rect get_rect();
  std::vector<Node> get_neighbours();
};

/*A special class representing a Obstacle Node*/
class ObstacleNode : public Node
{
private:
  Color color;

public:
  ObstacleNode(SDL_Rect const& rect) : Node(rect) {}
  bool is_free();
};

/* Class representing the Grid on
 * which algorithms will be executed */
class Grid
{
private:
  std::unordered_map<Coordinate, Node> rects;

public:
  Grid() : rects() {}
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
  GridBuilder(uint32_t width, uint32_t height,
              uint8_t border, Color const& color) 
    : grid_w(width), grid_h(height),
      border(border), grid_color(color) {}

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


