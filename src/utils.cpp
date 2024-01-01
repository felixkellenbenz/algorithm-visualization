#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_mouse.h>
#include <algorithm>
#include <functional>
#include <array>
#include <iostream>

#include "algorithms.hpp"
#include "utils.hpp"
#include "grid.hpp"

Color const EventHandler::START_COLOR = {51, 184, 100};
Color const EventHandler::END_COLOR = {197, 30, 58};
Color const EventHandler::OBSTACLE = {36, 36, 36};

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
  // dummy to avoid compiler warning
  if (event.type == SDL_QUIT)
    return;
}

void EventHandler::on_mousebutton_edit(SDL_Event& event)
{
  SDL_GetMouseState(&mouse_x, &mouse_y);

  if (event.button.button == SDL_BUTTON_LEFT)
    editor.color_node(mouse_x, mouse_y, OBSTACLE);
}

void EventHandler::on_key(SDL_Event& event)
{
  if (event.key.keysym.sym == 105)
    edit_flag = true;
  else if (event.key.keysym.sym == 114)
    editor.clean_grid();
  else if (event.key.keysym.sym == 98)
   executed = true;
  else if (event.key.keysym.sym == 99)
  {
    editor.clean_color(PathFinder::explore_color());
    editor.clean_color(PathFinder::path_color()); 
  }
  else if (event.key.keysym.sym == 49) 
  {
    static NullStrategy null_strat;
    strategy = &null_strat;
  } else if (event.key.keysym.sym == 50) 
  {
    static BFS bfs;
    strategy = &bfs;
  }
}

void EventHandler::on_key_edit(SDL_Event& event)
{
  SDL_GetMouseState(&mouse_x, &mouse_y);

  if (event.key.keysym.sym == 101)
    editor.color_unique(mouse_x, mouse_y, END_COLOR);
  else if (event.key.keysym.sym == 115)
    editor.color_unique(mouse_x, mouse_y, START_COLOR);
  else if(event.key.keysym.sym == 105)
    edit_flag = false;
  else if (event.key.keysym.sym == 114)
    editor.clean_grid();
  else if (event.key.keysym.sym == 99)
  {
    editor.clean_color(PathFinder::explore_color());
    editor.clean_color(PathFinder::path_color()); 
  }
}

void EventHandler::on_mousemotion_edit(SDL_Event& event)
{
  if (event.button.button == SDL_BUTTON(SDL_BUTTON_LEFT))
    editor.color_node(event.motion.x, event.motion.y, OBSTACLE);
}

void EventHandler::on_mousemotion(SDL_Event& event)
{
  // dummy to avoid compiler warning
  if (event.type == SDL_QUIT)
    return;
}

PathFindingStrategy* EventHandler::get_pathfinding_strategy()
{
  return strategy;
}

Color const& EventHandler::obstacle_color()
{
  return OBSTACLE;
}

Color const& EventHandler::end_color()
{
  return END_COLOR;
}

Color const& EventHandler::start_color()
{
  return START_COLOR;
}
