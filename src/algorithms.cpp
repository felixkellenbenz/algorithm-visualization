
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <cstdint>
#include <iterator>
#include <optional>
#include <sys/types.h>
#include <vector>

#include "algorithms.hpp"
#include "grid.hpp"
#include "utils.hpp"

Color const PathFinder::EXPLORE_COLOR = {0, 206, 209};
Color const PathFinder::PATH_COLOR = {255, 195, 11};

void BFS::set_start(Node node)
{
  explore_queue.push(node);
  start = node;
}

void BFS::set_end(Node node)
{
  end = node;
}

bool BFS::explore_neigbour(std::optional<Node> neigbour, Node& parent, 
                           Grid& grid, Color const& color)
{
  if (!(neigbour.has_value())) 
    return false;

  if (neigbour.value() == end.value())
  {
    grid.link_nodes(parent, end.value());
    return true;
  }

  if (!neigbour.value().is_free() || neigbour.value() == parent)
    return false;;

  explore_queue.push(neigbour.value());

  grid.link_nodes(parent, neigbour.value());
  grid.recolor_node(neigbour.value(), false, color); 

  return false;
}

void BFS::build_offsets(Grid& grid)
{
  int node_size = grid.get_node_size();
  int neg_node_size = -1 * node_size;
  offsets[0] = {neg_node_size, 0};
  offsets[1] = {node_size, 0};
  offsets[2] = {0, neg_node_size};
  offsets[3] = {0, node_size};
}

bool BFS::explore(Grid& grid, Color const& color)
{
  if (explore_queue.empty()) return true;

  build_offsets(grid);
  auto next = explore_queue.front();
  auto next_coords = next.coordinates();
  explore_queue.pop();

  for (uint8_t i  = 0; i < 4; i++)
  {
    uint32_t neigbour_y = next_coords.y + offsets[i][1];
    uint32_t neigbour_x = next_coords.x + offsets[i][0];

    if (neigbour_y > grid.get_heigth() || neigbour_x > grid.get_width())
      continue;

    auto neigbour = grid.find_node(neigbour_x, neigbour_y);

    if(explore_neigbour(neigbour, next, grid, color))
      return true;
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
  return color.red;
}

void NullStrategy::reset()
{

}

bool PathFinder::validate(Grid& grid)
{
  auto nodes = grid.get_nodes();

  for (auto& node : nodes)
  {
    if (node.second.get_color() == EventHandler::end_color())
      end = node.second;
    if (node.second.get_color() == EventHandler::start_color())
      start = node.second;
  }

  return end.has_value() && start.has_value() && strategy;
}

bool PathFinder::find_path(Grid& grid, GridRenderer& renderer)
{ 
  initialize(grid);

  bool found = false;

  while(!found)
  {
    found = strategy->explore(grid, EXPLORE_COLOR);
    renderer.render(grid);
    SDL_Delay(5);
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

  if (!check_backtracking_conditions(end_opt)) return false;

  Node next = *(end_opt->get_parent().value());

  while (next.get_parent().has_value() 
    && !(next == start.value()))
  {
    path.push_back(next);
    Node* parent = next.get_parent().value();
    next = *parent;
  }

  return next == start.value();
}

void PathFinder::initialize(Grid & grid)
{
  path.clear();
  grid.reset();
  strategy->reset();

  strategy->set_start(start.value());
  strategy->set_end(end.value()); 
}

bool PathFinder::check_backtracking_conditions(std::optional<Node> end_in_grid)
{
  if (!end_in_grid.has_value()) return false;

  if (!(end_in_grid.value().get_parent().has_value())) return false;

  if (!(end_in_grid.value().get_parent().value())) return false;

  return true;
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

Color const& PathFinder::explore_color()
{
  return EXPLORE_COLOR;
}

Color const& PathFinder::path_color()
{
  return PATH_COLOR;
}
