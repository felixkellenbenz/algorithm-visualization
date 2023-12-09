#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <array>
#include <cstdint>
#include <SDL2/SDL.h>
#include <optional>

#include "grid.hpp"


bool Coordinate::operator==(Coordinate const& cord) const
{
  return cord.x == x && cord.y == y;
}

Node::~Node() {}

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

void Node::set_free(bool new_val)
{
  free = new_val;
}

void Node::set_color(Color new_color)
{
  color = new_color;
}

Grid::~Grid() {}

std::optional<Node> Grid::find_node(Coordinate cord) const
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
  rects.insert({cords, node});
}

std::unordered_map<Coordinate, Node> const& Grid::get_nodes() const
{
  return rects;
}

void Grid::set_start(Node node)
{
  start.emplace(node);
}

void GridBuilder::build_grid()
{ 
  int next_x = border, next_y = border;
  int actual_size = node_size - 2 * border;

  while (next_x <= grid_width)
  {
    Node node({next_x, next_y, actual_size, actual_size});
    grid.add_node(node);
    next_x += node_size;
    if (next_x >= grid_width
      && next_y <= grid_height)
    {
      next_x = border;
      next_y += node_size;
    }
  }
}


Grid GridBuilder::export_grid()
{
  return grid;
}

void GridRenderer::render()
{
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  auto nodes = grid.get_nodes();
  for (auto const& it : nodes)
  {
    auto next_rect = it.second.get_rect();
    auto next_color = it.second.get_color();

    SDL_SetRenderDrawColor(renderer, next_color.red, next_color.green, 
                           next_color.blue, next_color.alpha);

    SDL_RenderFillRect(renderer, &next_rect);
    SDL_RenderDrawRect(renderer, &next_rect);
  }

}

GridEditor::~GridEditor() {}

void GridEditor::make_obstacle(Coordinate cord)
{

}


void GridEditor::make_start(Coordinate cord)
{

}
