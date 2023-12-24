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
  virtual void make_step(Grid&, GridEditor&,std::queue<Node>&) = 0;
};

class BFS : public PathFindingStrategy
{
private:

public:
  void make_step(Grid&, GridEditor&,std::queue<Node>&) override;
};

class PathFinder
{
private:
  
  bool validate(Grid const&);

public: 

  void find_path(Grid&);
};
