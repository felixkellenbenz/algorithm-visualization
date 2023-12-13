#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <functional>
#include <optional>

#include "grid.hpp"



class EventHandler
{
private:
  GridEditor& editor;
  int mouse_x, mouse_y;
  bool& running;
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
  EventHandler(GridEditor& editor, bool& running) 
    : editor(editor), mouse_x(0), mouse_y(0),
      edit_flag(0), running(running) {}
  void handle_events(SDL_Event&);
};
