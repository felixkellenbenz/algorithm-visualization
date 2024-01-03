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
  virtual bool valid() = 0;
};

class BFS : public PathFindingStrategy
{
private :
  std::array<std::array<int, 2>, 4> offsets;
  std::queue<Node> explore_queue;
  std::optional<Node> start;
  std::optional<Node> end;

  bool explore_neigbour(std::optional<Node> neigbour, Node& parent, 
                        Grid& grid, Color const& color);
  void build_offsets(Grid&);

public:
  
  void reset() override;
  bool explore(Grid&, Color const&) override;
  void set_start(Node) override;
  void set_end(Node) override;
  bool valid() override;
};

class Dijskstra : public PathFindingStrategy
{
private :
  std::array<std::array<int, 2>, 4> offsets;
  std::priority_queue<Node> explore_queue;
  std::optional<Node> start;
  std::optional<Node> end;

public:
  
  void reset() override;
  bool explore(Grid&, Color const&) override;
  void set_start(Node) override;
  void set_end(Node) override;
  bool valid() override;
};

class NullStrategy : public PathFindingStrategy
{
public:
  void reset() override;
  bool explore(Grid&, Color const&) override;
  void set_start(Node) override;
  void set_end(Node) override;
  bool valid() override;
};

class PathFinder
{
private:
  static Color const EXPLORE_COLOR;
  static Color const PATH_COLOR;

  std::optional<Node> start;
  std::optional<Node> end;
  PathFindingStrategy* strategy;
  std::vector<Node> path;
  
  void initialize(Grid&);
  bool check_backtracking_conditions(std::optional<Node>);
  bool backtrack(Grid& grid);

public:
  PathFinder()
  : strategy(0) {}

  void color_path(Grid& grid);
  bool validate(Grid&);
  bool find_path(Grid&, GridRenderer&); 
  
  void set_strategy(PathFindingStrategy*);

  static Color const& explore_color();
  static Color const& path_color();
};
