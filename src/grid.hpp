#pragma once
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL.h>
#include <array>
#include <optional>
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
  Coordinate(uint32_t x = 0, uint32_t y = 0) : x(x), y(y){}
  Coordinate(Coordinate const& cord): x(cord.x), y(cord.y) {}
  bool operator==(Coordinate const&) const;
} Coordinate;

template<>
struct std::hash<Coordinate>
{
  std::size_t operator()(Coordinate const& coord) const
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
  bool free;
  Color color;
  uint64_t distance;

public:
  Node(SDL_Rect const& rect) 
    : rect(rect), free(true),
    color({202, 202, 202, SDL_ALPHA_OPAQUE}),
    distance(UINT64_MAX) {}

  Node(SDL_Rect const& rect, Color const& color, bool free)
    : rect(rect), free(free),
    color(color),  distance(UINT64_MAX) {} 

  Node(Node const& node) :
      rect(node.rect), free(node.free),
      color(node.color),
      distance(node.distance) {}

  ~Node();

  Coordinate coordinates() const;
  bool is_free() const;
  SDL_Rect get_rect() const;
  Color get_color() const;
  void set_free(bool);
  void set_color(Color);
};

/* Class representing the Grid on
 * which algorithms will be executed */
class Grid
{
private:
  std::unordered_map<Coordinate, Node> rects;
  std::optional<Node> start;

public:

  Grid() : rects(), start() {}
  ~Grid();
  Grid(const Grid& grid) : rects(grid.rects), start(grid.start) {}

  std::optional<Node> find_node(Coordinate) const;
  void add_node(Node);
  std::unordered_map<Coordinate, Node> const& get_nodes() const;
  void set_start(Node); 
};

/*Build a grid with only free rects*/
class GridBuilder
{
private:
  uint32_t grid_width;
  uint32_t grid_height;
  uint32_t node_size;
  uint8_t border;
  Grid grid;

public:
  GridBuilder(uint32_t const grid_width, uint32_t const grid_height,
              uint8_t const border, uint32_t const node_size) 
    : grid_width(grid_width), grid_height(grid_height),
    node_size(node_size), border(border), grid() {}

  void build_grid(); 
  Grid export_grid();

};

/*Class for modeling a Grid Editor*/
class GridEditor
{
  


};

/*A class for rendering the Grid*/
class GridRenderer
{
private:
  SDL_Renderer *renderer;
  Grid const& grid;

public:
  GridRenderer(SDL_Renderer *ren,Grid const& grid)
    : renderer(ren), grid(grid) {}
  void render();
};
