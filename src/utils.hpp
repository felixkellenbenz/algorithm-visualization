#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <functional>
#include <optional>

#include "grid.hpp"
#include "algorithms.hpp"


class EventHandler
{
private:
  static Color const END_COLOR;
  static Color const START_COLOR;
  static Color const OBSTACLE;
  GridEditor& editor;
  int mouse_x, mouse_y;
  PathFindingStrategy* strategy;
  bool& running;
  bool& executed;
  bool edit_flag;
  
  // this could be cleaner with the c++ way of function pointers
  void consider_edit_flag(SDL_Event&, void(EventHandler::*)(SDL_Event&),
                          void(EventHandler::*)(SDL_Event&));
  void on_mousebutton(SDL_Event&);
  void on_key(SDL_Event&);
  void on_mousebutton_edit(SDL_Event&);
  void on_key_edit(SDL_Event&);
  void handle_event(SDL_Event&);
  void on_mousemotion_edit(SDL_Event&);
  void on_mousemotion(SDL_Event&);

public:
  EventHandler(GridEditor& _editor, bool& _running,
              bool& _executed) 
  : editor(_editor),
    mouse_x(0),
    mouse_y(0),
    strategy(0),
    running(_running),
    executed(_executed),
    edit_flag(0){}

  void handle_events(SDL_Event&);

  PathFindingStrategy* get_pathfinding_strategy();

  static Color const& obstacle_color();
  static Color const& end_color();
  static Color const& start_color();
};
