#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL.h>
#include <cstdint>
#include <functional>
#include <memory>
#include <optional>

#include "grid.hpp"

Color const GridEditor::BASIC_NODE_COLOR = {229, 229, 229};
Color const GridRenderer::BACKGROUND = {36, 36, 36};
Color const GridBuilder::BASIC_NODE_COLOR = {229, 229, 229};

bool Color::operator==(Color const& color) const
{
  return red == color.red && green == color.green 
         && blue == color.blue && alpha == color.alpha; 
}

bool Coordinate::operator==(Coordinate const& cord) const
{
  return cord.x == x && cord.y == y;
}

Node::~Node() { }

Node& Node::operator=(Node node)
{
  free = node.free;
  color = node.color;
  return *this;
}

bool Node::operator==(Node const& node) const
{
  return rect.x == node.rect.x && rect.y == node.rect.y 
          && node.distance == distance && free == node.free;
}

Coordinate Node::coordinates() const
{
  uint32_t x = rect.x;
  uint32_t y = rect.y;
  return {x, y};
}

bool Node::is_free() const
{
  return free;
}

SDL_Rect Node::get_rect() const
{
  return rect;
}

Color Node::get_color() const
{
  return color;
}

std::optional<Node const*> Node::get_parent() const
{
  return parent;
}

void Node::set_free(bool new_val)
{
  free = new_val;
}

void Node::set_color(Color new_color)
{
  color = new_color;
}

void Node::set_parent(Node const* node)
{
  parent.emplace(node);
}

Grid::~Grid() {}

std::optional<Node> Grid::find_node(uint32_t x, uint32_t y) const
{
  auto iter = nodes.find({x, y});
  if (iter != nodes.end())
    return iter->second;
  else
    return {}; 
}

void Grid::recolor_node(Node const& to_recolor, bool free, Color const& color)
{
  auto iter = nodes.find(to_recolor.coordinates());
  if (iter != nodes.end())
  {
    iter->second.set_color(color);
    iter->second.set_free(free);
  } else 
    return;
}

void Grid::link_nodes(Node const& parent, Node const& child)
{
  auto iter_child = nodes.find(child.coordinates());
  auto iter_parent = nodes.find(parent.coordinates()); 
  if (iter_child != nodes.end() && iter_parent != nodes.end())
    iter_child->second.set_parent(&iter_parent->second);
}

void Grid::add_node(Node node)
{
  Coordinate cords = node.coordinates();
  auto found = nodes.find(cords);
  if (found == nodes.end())
  {
    nodes.insert({cords, node});
  } else {
    found->second = node;
  }

}

std::unordered_map<Coordinate, Node> const& Grid::get_nodes() const
{
  return nodes;
}

uint32_t Grid::get_width() const
{
  return width;
}

uint32_t Grid::get_heigth() const
{
  return heigth;
}

uint32_t Grid::get_node_size() const
{
  return node_size;
}

Grid GridBuilder::build_grid()
{
  
  Grid grid(grid_width, grid_height, node_size);

  uint32_t next_x = 0, next_y = 0;
  int rect_x = 0, rect_y = 0;
  int actual_size = node_size - border;

  while (next_x <= grid_width)
  {
    Node node({rect_x, rect_y, actual_size, actual_size}, BASIC_NODE_COLOR);
    grid.add_node(node);
    next_x += node_size;
    rect_x += node_size;

    if (next_x >= grid_width
        && next_y <= grid_height)
    {
      next_x = 0;
      rect_x = 0;
      next_y += node_size;
      rect_y += node_size;
    }
  }

  return grid;
}

void GridRenderer::render_node(Node const& node)
{
  auto next_rect = node.get_rect();
  auto next_color = node.get_color();

  SDL_SetRenderDrawColor(renderer, next_color.red, next_color.green, 
                           next_color.blue, next_color.alpha);

  SDL_RenderFillRect(renderer, &next_rect);
  SDL_RenderDrawRect(renderer, &next_rect); 
}

void GridRenderer::render()
{
  SDL_SetRenderDrawColor(renderer, BACKGROUND.red, 
                         BACKGROUND.green, BACKGROUND.blue,
                         BACKGROUND.alpha);
  SDL_RenderClear(renderer);

  auto nodes = grid.get_nodes();
  for (auto const& it : nodes)
  {
    render_node(it.second);
  } 
}

GridEditor::~GridEditor() {}

Coordinate GridEditor::parse_coordinate(uint32_t x, uint32_t y)
{
  auto node_size = grid.get_node_size();
  uint32_t coord_x = x - (x % node_size);
  uint32_t coord_y = y - (y % node_size);
  return {coord_x, coord_y};
}

void GridEditor::clean_grid()
{
  auto nodes = grid.get_nodes();

  for (auto const& it : nodes)
  {
    grid.recolor_node(it.second, true, BASIC_NODE_COLOR);
  }
}

void GridEditor::color_node(uint32_t x, uint32_t y, 
                            Color const& color)
{
  auto cord = parse_coordinate(x, y);
  auto node = grid.find_node(cord.x, cord.y);
  std::optional<Node> to_recolor = {};

  if (!(node.has_value() && node.value().is_free())) return;

  grid.recolor_node(node.value(), false, color);
}

void GridEditor::color_unique(uint32_t x, uint32_t y,
                              Color const& color)
{
  auto& nodes = grid.get_nodes();
  auto cord = parse_coordinate(x, y);
  auto node = grid.find_node(cord.x, cord.y); 

  if (!node.has_value()) return;

  for (auto const& current_node : nodes)
    if (current_node.second.get_color() == color)
      grid.recolor_node(current_node.second, 
                        true, BASIC_NODE_COLOR);
  
  grid.recolor_node(node.value(), false, color);
}
