

#include "algorithms.hpp"
#include "grid.hpp"
#include "utils.hpp"
#include <SDL2/SDL_timer.h>
#include <cstdint>
#include <iterator>


Color const PathFinder::EXPLORE_COLOR = {100, 150, 235};

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
  end->set_free(true);
  if (explore_queue.empty()) return true;

  auto next = explore_queue.front();
  auto next_coords = next.coordinates();
  explore_queue.pop();

  for (std::size_t i = 0; i < 4; i++)
  {
    uint32_t neigbour_y = next_coords.y + offsets[i][1];
    uint32_t neigbour_x = next_coords.x + offsets[i][0];
    auto neigbour = grid.find_node(neigbour_x, neigbour_y);

    if (!(neigbour.has_value() && neigbour.value().is_free()))
      continue;

    explore_queue.push(neigbour.value());

    grid.link_nodes(next, neigbour.value());
    grid.recolor_node(neigbour.value(), false, color);

    if (neigbour.value() == end.value())
      return true;
    
  }

  return false; 
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
  if (!validate(grid) || !strategy) return false;
  
  strategy->set_start(start.value());
  strategy->set_end(end.value());
  
  // This should be in a while loop, but it doesn't work
  strategy->explore(grid, EXPLORE_COLOR);
  renderer.render(grid);
  
  SDL_Delay(2);

  return false;
}

Path PathFinder::backtrack()
{

  return {};
}
