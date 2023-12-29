#pragma once
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL.h>
#include <cstdint>
#include <optional>
#include <unordered_map>

/*
 * A structure that holds information about
 * the rgba values of a colour.
 */
typedef struct Color 
{
  uint8_t red;
  uint8_t green;
  uint8_t blue;
  uint8_t alpha;

  Color(uint8_t _red, uint8_t _green, uint8_t _blue)
  : red(_red), green(_green), blue(_blue), 
    alpha(SDL_ALPHA_OPAQUE) {}

  Color(uint8_t _red, uint8_t _green,
        uint8_t _blue, uint8_t _alpha)
  : red(_red), green(_green), blue(_blue),
    alpha(_alpha) {}

  bool operator==(Color const&) const;

} Color;

/*
 * A struct reprsenting a point in 2D space
 */
typedef struct Coordinate
{
  int32_t x;
  int32_t y;

  Coordinate(uint32_t _x = 0, uint32_t _y = 0) : x(_x), y(_y){}
  Coordinate(Coordinate const& cord): x(cord.x), y(cord.y) {}

  bool operator==(Coordinate const&) const;

} Coordinate;

template<>
struct std::hash<Coordinate>
{
  std::size_t operator()(Coordinate const& coord) const
  {
    std::hash<int32_t> int_hash;
    return (int_hash(coord.x)) ^ (int_hash(coord.y) << 1);
  }
};

/*
 * A class representing a node that can be visited
 * or explored by an algorithm 
 */
class Node
{
private:
  SDL_Rect rect;
  Color color;
  std::optional<Node*> parent;
  bool free;
  uint64_t distance;

public:
  Node(SDL_Rect const& _rect, Color const& _color)
    : rect(_rect), color(_color),
      free(true), distance(UINT64_MAX) {} 

  Node(Node const& node) :
      rect(node.rect), color(node.color), parent(node.parent),
      free(node.free), distance(node.distance) {}

  ~Node();

  Node& operator=(Node node);
  bool operator==(Node const&) const;

  Coordinate coordinates() const;
  bool is_free() const;
  SDL_Rect get_rect() const;
  Color get_color() const;
  std::optional<Node*> get_parent() const;
  void set_free(bool);
  void set_color(Color);
  void set_parent(Node*);
};

/* 
 * A class representing a 2D grid
 */
class Grid
{
private: 
  std::unordered_map<Coordinate, Node> nodes;
  uint32_t node_size;
  uint32_t width;
  uint32_t heigth;

public:

  Grid(uint32_t _width, uint32_t _heigth, uint32_t _node_size) 
    : nodes(), node_size(_node_size), width(_width), heigth(_heigth) {}

  Grid(const Grid& grid) : nodes(grid.nodes) {}

  ~Grid();

  std::optional<Node> find_node(uint32_t x, uint32_t y) const;
  void recolor_node(Node const&, bool, Color const&);
  void link_nodes(Node const&, Node const&);
  void add_node(Node);

  std::unordered_map<Coordinate, Node> const& get_nodes() const;
  uint32_t get_heigth() const;
  uint32_t get_width() const;
  uint32_t get_node_size() const;
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
  GridBuilder(uint32_t const _grid_width, uint32_t const _grid_height,
              uint8_t const _border, uint32_t const _node_size) 
    : grid_width(_grid_width), grid_height(_grid_height),
    node_size(_node_size), border(_border) {}

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

  void render_node(Node const& node);

public:
  GridRenderer(SDL_Renderer *ren)
    : renderer(ren) {}

  void render(Grid const&);
};

class GridEditor
{
private:
  static Color const BASIC_NODE_COLOR;
  Grid& grid;

  Coordinate parse_coordinate(uint32_t, uint32_t);

public:
  GridEditor(Grid& _grid) : grid(_grid) {}

  ~GridEditor();

  void clean_grid();
  void color_node(uint32_t, uint32_t, Color const&);
  void color_unique(uint32_t, uint32_t, Color const&);
};
