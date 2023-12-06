#include <SDL2/SDL_render.h>
#include <array>
#include <cstdint>
#include <SDL2/SDL.h>
#include <optional>

#include "grid.hpp"


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
  for (auto& it : rects)
  {
    it.second.render(renderer);
  }
}

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


}

