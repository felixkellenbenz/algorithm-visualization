#include <SDL2/SDL_rect.h>
#include <cstdint>
#include <unordered_map>
#include <SDL2/SDL.h>
#include <tuple>
#include <vector>


class ScreenElement
{
public:

  virtual void render() = 0;

};


/* Class representing the Grid on
 * which algorithms will be executed */
class Grid
{

private:	


public:


};

/*Build a grid with only free rects*/
class GridBuilder
{


};

/*Edit grid*/
class GridEditor
{



};

/*Render the grid with an SDL_Renderer*/
class GridRenderer
{



};

/*Class that encapsulates a Rect*/
class Node
{
private:
  // uderlying rect
  SDL_Rect rect;

public:
};

/*A special class representing a Obstacle Node*/
class ObstacleNode
{
private:
};
