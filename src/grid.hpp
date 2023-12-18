#pragma once
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL.h>
#include <cstdint>
#include <optional>
#include <unordered_map>


typedef struct Color 
{
  uint8_t red;
  uint8_t green;
  uint8_t blue;
  uint8_t alpha;

  Color(uint8_t red, uint8_t green, uint8_t blue)
  : red(red), green(green), blue(blue), 
    alpha(SDL_ALPHA_OPAQUE) {}

  Color(uint8_t red, uint8_t green,
        uint8_t blue, uint8_t alpha)
  : red(red), green(green), blue(blue),
    alpha(alpha) {} 

} Color;


Color const OBSTACLE = {36, 36, 36};
Color const PATH = {0 ,0, 0};
Color const EXPLORE = {255, 0, 0};


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

/*
 * A class representing a node that can be visited
 * or explored by an algorithm 
 */
class Node
{
private:
  SDL_Rect const rect;
  Color color;
  std::optional<Node const*> parent;
  bool free;
  uint64_t distance;

public:
  Node(SDL_Rect const& rect, Color const& color)
    : rect(rect), free(true),
    color(color),  distance(UINT64_MAX) {} 

  Node(Node const& node) :
      rect(node.rect), free(node.free),
      color(node.color),
      distance(node.distance) {}

  ~Node();

  Node& operator=(Node node);
  bool operator==(Node const&) const;

  Coordinate coordinates() const;
  bool is_free() const;
  SDL_Rect get_rect() const;
  Color get_color() const;
  std::optional<Node const*> get_parent() const;
  void set_free(bool);
  void set_color(Color);
  void set_parent(Node const*);
};

/* 
 * A class representing a 2D grid
 */
class Grid
{
private: 
  std::unordered_map<Coordinate, Node> nodes;
  std::optional<Node> start;
  std::optional<Node> end;
  uint32_t width;
  uint32_t heigth;

public:

  Grid(uint32_t width, uint32_t heigth) 
    : nodes(), start(), width(width), heigth(heigth) {}

  Grid(const Grid& grid) : nodes(grid.nodes), start(grid.start) {}

  ~Grid();

  std::optional<Node> find_node(uint32_t x, uint32_t y) const;
  void recolor_node(Node const&, bool, Color const&);
  void link_nodes(Node const&, Node const&);
  void add_node(Node);

  std::unordered_map<Coordinate, Node> const& get_nodes() const;
  uint32_t get_heigth() const;
  uint32_t get_width() const;
  std::optional<Node> get_start() const;
  std::optional<Node> get_end() const;
  void set_start(std::optional<Node>); 
  void set_end(std::optional<Node>);
};

/*Build a new Grid with only free rects*/
class GridBuilder
{
private:
  static Color const BASIC_NODE_COLOR;
  uint32_t const grid_width;
  uint32_t const grid_height;
  uint32_t const node_size;
  uint8_t const border;

public:
  GridBuilder(uint32_t const grid_width, uint32_t const grid_height,
              uint8_t const border, uint32_t const node_size) 
    : grid_width(grid_width), grid_height(grid_height),
    node_size(node_size), border(border) {}

  Grid build_grid();
};

/*
 * A class for rendering the Grid
 * using a SDL_Renderer
 */
class GridRenderer
{
private:
  static Color const BACKGROUND;
  SDL_Renderer *renderer;
  Grid const& grid;

  void render_node(Node const& node);

public:
  GridRenderer(SDL_Renderer *ren,Grid const& grid)
    : renderer(ren), grid(grid) {}

  void render();
};


class GridEditor
{
private:




  void handle_start();
  void handle_end();

public:


  void clean_grid();
  void color_node(uint32_t x, uint32_t y, Color color);
};
