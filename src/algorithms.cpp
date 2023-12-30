
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <iterator>
#include <vector>

#include "algorithms.hpp"
#include "grid.hpp"
#include "utils.hpp"

Color const PathFinder::EXPLORE_COLOR = {67, 85, 133};
Color const PathFinder::PATH_COLOR = {188, 163, 127};

void BFS::set_start(Node node)
{
  explore_queue.push(node);
  start = node;
}

void BFS::set_end(Node node)
{
  end = node;
}

bool BFS::explore(Grid& grid, Color const& color)
{
  if (explore_queue.empty()) return true;

  auto next = explore_queue.front();
  auto next_coords = next.coordinates();
  explore_queue.pop();

  for (uint8_t i  = 0; i < 4; i++)
  {
    uint32_t neigbour_y = next_coords.y + offsets[i][1];
    uint32_t neigbour_x = next_coords.x + offsets[i][0];
    auto neigbour = grid.find_node(neigbour_x, neigbour_y);

    if (!(neigbour.has_value()) || 
        neigbour_y > grid.get_heigth() || neigbour_x > grid.get_width()) continue;

    if (neigbour.value() == end.value())
    {
      grid.link_nodes(next, end.value());
      return true;
    }

    if (!neigbour.value().is_free() || neigbour.value() == next) continue;

    explore_queue.push(neigbour.value());

    grid.link_nodes(next, neigbour.value());
    grid.recolor_node(neigbour.value(), false, color);  
  }

  return false; 
}

void BFS::reset()
{
  start = {};
  end = {};
  
  while (!explore_queue.empty())
  {
    explore_queue.pop();
  }
}

void NullStrategy::set_start(Node start) 
{
  start.get_parent();
}

void NullStrategy::set_end(Node end) 
{
  end.get_parent();
}

bool NullStrategy::explore(Grid& grid, Color const& color) 
{
  auto nodes = grid.get_nodes();
  Color ref_color = color;
  return ref_color.red;
}

void NullStrategy::reset()
{

}

bool PathFinder::validate(Grid& grid)
{
  auto nodes = grid.get_nodes();

  for (auto& node : nodes)
  {
    if (node.second.get_color() == EventHandler::END_COLOR)
      end = node.second;
    if (node.second.get_color() == EventHandler::START_COLOR)
      start = node.second;
  }

  return end.has_value() && start.has_value();
}

bool PathFinder::find_path(Grid& grid, GridRenderer& renderer)
{ 
  path.clear();
  grid.reset();
  strategy->reset();

  strategy->set_start(start.value());
  strategy->set_end(end.value());
  
  bool found = false;

  while(!found)
  {
    found = strategy->explore(grid, EXPLORE_COLOR);
    renderer.render(grid);
    SDL_Delay(2);
  }

  if (backtrack(grid))
  {
    color_path(grid);
    return true;
  }
  else
    return false;
}

bool PathFinder::backtrack(Grid& grid)
{
  auto end_coords = end->coordinates();
  auto end_opt = grid.find_node(end_coords.x, end_coords.y);

  if (!end_opt.has_value()) return false;

  if (!(end_opt.value().get_parent().has_value())) return false;

  if (!(end_opt.value().get_parent().value())) return false;

  Node next = *(end_opt->get_parent().value());

  while (next.get_parent().has_value() 
    && !(next == start.value()))
  {
    path.push_back(next);
    Node* parent = next.get_parent().value();
    next = *parent;
  }

  if (next == start.value())
    return true;
  else 
    return false; 

}

void PathFinder::color_path(Grid& grid)
{
  for (auto& node : path)
  {
    grid.recolor_node(node, false, PATH_COLOR);
  }
}

void PathFinder::set_strategy(PathFindingStrategy* _strategy)
{
  strategy = _strategy;
}
