#include <cstdint>
#include <optional>
#include <queue>
#include <tuple>
#include <vector>
#include <array>


#include "grid.hpp"

class Path 
{

};

class PathFindingStrategy
{
public:
  virtual bool make_step(Grid&, Color&) = 0;
};

class BFS : public PathFindingStrategy
{
private:
  std::queue<Node> explore_queue;

public:
  
  bool make_step(Grid&, Color&) override;



};

class PathFinder
{
private:
  static Color const EXPLORE_COLOR;
  static Color const PATH_COLOR;
  std::optional<Node> start;
  std::optional<Node> end;
  PathFindingStrategy* strategy;

  bool validate();

public:  
  void find_path(Grid&, GridRenderer&); 

  Path backtrack();

};
