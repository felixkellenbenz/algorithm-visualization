#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_mouse.h>
#include <functional>
#include <array>

#include "utils.hpp"

using option = void(EventHandler::*)(SDL_Event&);


void EventHandler::handle_events(SDL_Event& event)
{
  while (SDL_PollEvent(&event))
  {
    handle_event(event);
  }
}

void EventHandler::handle_event(SDL_Event& event)
{
  switch (event.type) 
  {
    case SDL_MOUSEBUTTONDOWN:
      consider_edit_flag(event, &EventHandler::on_mousebutton_edit, 
                         &EventHandler::on_mousebutton);
      break;
    case SDL_KEYDOWN:
      consider_edit_flag(event, &EventHandler::on_key_edit,
                         &EventHandler::on_key);
      break;
    case SDL_MOUSEMOTION:
      consider_edit_flag(event,&EventHandler::on_mousemotion_edit,
                         &EventHandler::on_mousemotion);
      break;
    case SDL_QUIT:
      running = false;
      break;
  } 
}

void EventHandler::consider_edit_flag(SDL_Event& event, 
                                      option on_edit, option on_event)
{
  if (edit_flag)
    (this->*on_edit)(event);
  else
    (this->*on_event)(event);
}

void EventHandler::on_mousebutton(SDL_Event& event)
{
  
}

void EventHandler::on_mousebutton_edit(SDL_Event& event)
{
  SDL_GetMouseState(&mouse_x, &mouse_y);

  if (event.button.button == SDL_BUTTON_LEFT)
    editor.make_obstacle(mouse_x, mouse_y);
}

void EventHandler::on_key(SDL_Event& event)
{
  if (event.key.keysym.sym == 105)
    edit_flag = true;
  else if (event.key.keysym.sym == 114)
    editor.reset_grid();
}

void EventHandler::on_key_edit(SDL_Event& event)
{
  SDL_GetMouseState(&mouse_x, &mouse_y);

  if (event.key.keysym.sym == 101)
    editor.make_end(mouse_x, mouse_y);
  else if (event.key.keysym.sym == 115)
    editor.make_start(mouse_x, mouse_y);
  else if(event.key.keysym.sym == 105)
    edit_flag = false;
  else if (event.key.keysym.sym == 114)
    editor.reset_grid();
}

void EventHandler::on_mousemotion_edit(SDL_Event& event)
{
  if (event.button.button == (SDL_BUTTON_LEFT))
      editor.make_obstacle(event.motion.x, event.motion.y);
}

void EventHandler::on_mousemotion(SDL_Event& event)
{
  return;
}
