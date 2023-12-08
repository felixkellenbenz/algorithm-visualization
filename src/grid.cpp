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

void Node::render(SDL_Renderer *renderer)
{
  SDL_SetRenderDrawColor(renderer, color.red, color.green
                           ,color.blue, color.alpha);
  SDL_RenderFillRect(renderer, &rect);
}

bool Node::is_free()
{
  return free;
}

std::array<Node*, 8> Node::neighbours()
{
  return neighbourhood; 
}

void Grid::render(SDL_Renderer *renderer)
{
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);
  for (auto& it : rects)
  {
    it.second.render(renderer);
  }
}

Coordinate Node::coordinate()
{

}

Grid::~Grid() {}

std::optional<Node> Grid::find_node(Coordinate cord)
{
  auto iter = rects.find(cord);
  if (iter != rects.end())
    return iter->second;
  else
    return {}; 
}

void Grid::add_node(Node node)
{

}

void Grid::set_start(Node node)
{
  start.emplace(node);
}

void GridBuilder::build_grid()
{
   
  uint32_t next_x, next_y;
  uint32_t actual_size = node_size - 2 * border;

  


}


Grid GridBuilder::export_grid()
{
  return Grid();
}

