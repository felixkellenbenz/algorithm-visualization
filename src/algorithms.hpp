#pragma once
#include <cstdint>
#include <iterator>
#include <optional>
#include <queue>
#include <tuple>
#include <unordered_map>
#include <vector>
#include <array>


#include "grid.hpp"

class PathFindingStrategy
{
protected:
  std::array<std::array<int, 2>, 4> offsets;
  std::optional<Node> start;
  std::optional<Node> end;

public:

  virtual ~PathFindingStrategy();

  virtual void reset() = 0;
  virtual bool explore(Grid&, Color const&) = 0; 
  virtual bool valid() = 0;
  virtual void set_start(Node&) = 0;
  void build_offsets(Grid&);
  void set_end(Node&);
};

class BFS : public PathFindingStrategy
{
private:
  std::queue<Node> explore_queue;

  bool explore_neigbour(std::optional<Node> neigbour, Node& parent, 
                        Grid& grid, Color const& color);
public:
  
  void reset() override;
  bool explore(Grid&, Color const&) override;
  bool valid() override;
  void set_start(Node&) override;
};

class Dijskstra : public PathFindingStrategy
{
private:
  std::queue<Node> explore_queue;
  std::unordered_map<Coordinate, uint32_t> distances;

public:
  
  void reset() override;
  bool explore(Grid&, Color const&) override;
  bool valid() override;
  void set_start(Node&) override;
};

class NullStrategy : public PathFindingStrategy
{
public:
  void reset() override;
  bool explore(Grid&, Color const&) override;
  bool valid() override;
  void set_start(Node&) override;
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
