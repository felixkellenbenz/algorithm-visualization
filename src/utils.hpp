#include <SDL2/SDL.h>

#include "grid.hpp"

class EventHandler
{
private:
  GridEditor& editor;
  int mouse_x, mouse_y;
  
  void on_mouse_down();
  void on_button_down();

public:
  EventHandler(GridEditor& editor)
    : editor(editor), mouse_x(0), mouse_y(0) {}
  void handle_event(SDL_Event&);


};
