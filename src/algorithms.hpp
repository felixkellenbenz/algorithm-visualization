#pragma once
#include <cstdint>
#include <optional>
#include <queue>
#include <tuple>
#include <vector>
#include <array>


#include "grid.hpp"

class PathFindingStrategy
{
public:
  virtual void reset() = 0;
  virtual bool explore(Grid&, Color const&) = 0;
  virtual void set_start(Node) = 0;
  virtual void set_end(Node) = 0;
};

class BFS : public PathFindingStrategy
{
private :
  std::array<std::array<int, 2>, 4> const offsets =  {{{-20, 0}, {20, 0}, {0, -20}, {0, 20}}};
  std::queue<Node> explore_queue;
  std::optional<Node> start;
  std::optional<Node> end;

public:
  
  void reset() override;
  bool explore(Grid&, Color const&) override;
  void set_start(Node) override;
  void set_end(Node) override;
};

class NullStrategy : public PathFindingStrategy
{
public:
  void reset() override;
  bool explore(Grid&, Color const&) override;
  void set_start(Node) override;
  void set_end(Node) override;
};

class PathFinder
{
private:
  std::optional<Node> start;
  std::optional<Node> end;
  PathFindingStrategy* strategy;
  std::vector<Node> path;

  

public:
  static Color const EXPLORE_COLOR;
  static Color const PATH_COLOR;

  PathFinder(PathFindingStrategy* _strategy)
    : strategy(_strategy) {}

  void color_path(Grid& grid);

  bool validate(Grid&);

  bool find_path(Grid&, GridRenderer&); 

  bool backtrack(Grid& grid);

  void set_strategy(PathFindingStrategy*);

};
