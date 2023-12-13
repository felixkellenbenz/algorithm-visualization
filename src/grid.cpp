#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL.h>
#include <cstdint>
#include <memory>
#include <optional>

#include "grid.hpp"

Color const GridEditor::BASIC = {229, 229, 229};
Color const GridEditor::START = {51, 184, 100};
Color const GridEditor::END = {197, 30, 58};
Color const GridRenderer::BACKGROUND = {36, 36, 36};
Color const GridBuilder::NODE_COLOR = {220, 220, 220};

bool Coordinate::operator==(Coordinate const& cord) const
{
  return cord.x == x && cord.y == y;
}

Node::~Node() {}

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

std::optional<Node> Grid::find_node(Coordinate const& cord) const
{
  auto iter = rects.find(cord);
  if (iter != rects.end())
    return iter->second;
  else
    return {}; 
}

void Grid::add_node(Node node)
{
  Coordinate cords = node.coordinates();
  auto found = rects.find(cords);
  if (found == rects.end())
  {
    rects.insert({cords, node});
  } else {
    found->second = node;
  }

}

std::optional<Node const*> Grid::find_node(Node const& node) const
{
  for (auto const& it : rects)
    if (it.second == node)
      return &(it.second);
  return {};
}

std::unordered_map<Coordinate, Node> const& Grid::get_nodes() const
{
  return rects;
}

std::optional<Node> Grid::get_start() const
{
  return start;
}

uint32_t Grid::get_width() const
{
  return width;
}

uint32_t Grid::get_heigth() const
{
  return heigth;
}

std::optional<Node> Grid::get_end() const
{
  return end;
}

void Grid::set_start(std::optional<Node> node)
{
  if (node.has_value())
    start.emplace(node.value());
  else
    start = node;
}

void Grid::set_end(std::optional<Node> node)
{
  if (node.has_value())
    end.emplace(node.value());
  else
    end = node;
}

void GridBuilder::build_grid()
{
  // coordinates of the top-left most node is (0,0)
  int next_x = 0, next_y = 0;
  int actual_size = node_size - border;

  while (next_x <= grid_width)
  {
    Node node({next_x, next_y, actual_size, actual_size}, NODE_COLOR);
    grid.add_node(node);
    next_x += node_size;

    if (next_x >= grid_width
        && next_y <= grid_height)
    {
      next_x = 0;
      next_y += node_size;
    }
  }
}

Grid GridBuilder::export_grid()
{
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

  if (grid.get_start().has_value())
    render_node(grid.get_start().value()); 

}

GridEditor::~GridEditor() {}

Coordinate GridEditor::parse_coordinate(uint32_t x, uint32_t y)
{
  uint32_t cord_x = x - (x % node_size);
  uint32_t cord_y = y - (y % node_size);
  return {cord_x, cord_y};
}

void GridEditor::update_grid(Color const& color, bool free, std::optional<Node> node)
{ 
  if (!node.has_value()) return;

  node->set_free(free);
  node->set_color(color);
  grid.add_node(node.value()); 
}


void GridEditor::reset_grid()
{
  auto nodes = grid.get_nodes();

  for (auto const& node : nodes)
  {
    update_grid(BASIC, true, node.second);
  }

  grid.set_start({});
}

void GridEditor::color_node(uint32_t x, uint32_t y, Color const& color)
{
  auto cord = parse_coordinate(x, y);
  auto node = grid.find_node(cord);

  if (!node.has_value() || !node.value().is_free()) return;

  update_grid(color, false, node);
}

void GridEditor::color_node(Node const& node, Color const& color)
{
  auto cord = node.coordinates();
  color_node(cord.x, cord.y, color); 
}

void GridEditor::color_start(uint32_t x, uint32_t y)
{
  auto cord = parse_coordinate(x, y);
  auto node = grid.find_node(cord);

  if (!node.has_value() || !node->is_free()) return;

  node->set_color(START);
  node->set_free(false);

  if (grid.get_start().has_value())
  {
    update_grid(BASIC, true, grid.get_start());
  }

  grid.add_node(node.value());
  grid.set_start(node); 
}

void GridEditor::color_end(uint32_t x, uint32_t y)
{
  auto cord = parse_coordinate(x, y);
  auto node = grid.find_node(cord);

  if (!node.has_value() || !node->is_free()) return;

  node->set_color(END);
  node->set_free(false);

  if (grid.get_end().has_value())
  {
    update_grid(BASIC, true, grid.get_end());
  }

  grid.add_node(node.value());
  grid.set_end(node); 
}
