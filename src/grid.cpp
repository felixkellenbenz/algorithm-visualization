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

Grid::~Grid() {}

std::optional<Node> Grid::find_node(Coordinate cord)
{
  auto iter = rects.find(cord);
  if (iter != rects.end())
    return iter->second;
  else
    return {}; 
}

void Grid::set_start(Node node)
{
  start.emplace(node);
}

void GridBuilder::build_grid()
{
  


}


Grid GridBuilder::export_grid()
{
  return Grid();
}

