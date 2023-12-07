#pragma once
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL.h>
#include <array>
#include <cstdint>
#include <iterator>
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
  Coordinate() : x(0), y(0) {}
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
  std::array<Node*, 8> neighbourhood;
  uint64_t distance;

public:
  Node(SDL_Rect const& rect) 
    : rect(rect), free(true),
    neighbourhood(),
    color({202, 202, 202, SDL_ALPHA_OPAQUE}),
    distance(UINT64_MAX) {}

  Node(SDL_Rect const& rect, Color const& color, bool free)
    : rect(rect), free(free),
    neighbourhood(),
    color(color),  distance(UINT64_MAX) {}

  Node(SDL_Rect const& rect, Color const& color,
       std::array<Node*, 8> const& neighbours, bool free)
    : rect(rect), free(free),
    neighbourhood(neighbours),
    color(color),  distance(UINT64_MAX) {} 

  Node(Node const& node) :
      rect(node.rect), free(node.free),
      neighbourhood(node.neighbourhood), color(node.color),
      distance(node.distance) {}

  ~Node();

  void render(SDL_Renderer *renderer);
  std::array<Node*, 8> neighbours();
  bool is_free();
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

  void render(SDL_Renderer *renderer);
  std::optional<Node> find_node(Coordinate cord);
  void set_start(Node node); 
};

/*Build a grid with only free rects*/
class GridBuilder
{
private:
  uint32_t grid_width;
  uint32_t grid_height;
  uint32_t node_width;
  uint32_t node_height;
  uint8_t border;
  Grid grid;

public:
  GridBuilder(uint32_t grid_width, uint32_t grid_height,
              uint8_t border, Color const& color,
              uint32_t node_width, uint32_t node_height) 
    : grid_width(grid_width), grid_height(grid_height),
      node_width(node_width), node_height(node_height),
      border(border) {}

  void build_grid(); 
  Grid export_grid();

};

/*Class for modeling a Grid Editor*/
class GridEditor
{



};
