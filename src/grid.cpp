#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL.h>
#include <optional>

#include "grid.hpp"


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
  auto found = rects.find(cords);
  if (found == rects.end())
  {
    rects.insert({cords, node});
  } else {
    found->second = node;
  }

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
  // coordinates of the top-left most node is (0,0)
  int next_x = 0, next_y = 0;
  int actual_size = node_size - border;

  while (next_x <= grid_width)
  {
    Node node({next_x, next_y, actual_size, actual_size});
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

void GridRenderer::render()
{
  SDL_SetRenderDrawColor(renderer, background.red, 
                         background.green, background.blue,
                         background.alpha);
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

Coordinate GridEditor::parse_coordinate(uint32_t x, uint32_t y)
{
  uint32_t cord_x = x - (x % node_size);
  uint32_t cord_y = y - (y % node_size);
  return {cord_x, cord_y};
}

void GridEditor::update_node(Color color, bool free, std::optional<Node> node)
{
  if (!node.has_value()) return;

  node->set_free(free);
  node->set_color(color);
  grid.add_node(node.value()); 
}


void GridEditor::reset_grid()
{

}

void GridEditor::make_obstacle(uint32_t x, uint32_t y)
{
  Color obstacle_color = {0, 123, 184};
  auto cord = parse_coordinate(x, y);
  auto node = grid.find_node(cord);
  update_node(obstacle_color, false, node);
}


void GridEditor::make_start(uint32_t x, uint32_t y)
{
  // make so that a grid can only have one start
  Color start_color = {255, 0, 0};
  auto cord = parse_coordinate(x, y);
  auto node = grid.find_node(cord);
  update_node(start_color, false, node); 
}
