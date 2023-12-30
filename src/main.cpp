#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <type_traits>

#include "grid.hpp"
#include "utils.hpp"
#include "algorithms.hpp"

uint32_t const WIDTH = 920;
uint32_t const HEIGTH = 780;
uint8_t const BORDER = 1;
uint32_t const NODE_SIZE = 20;
bool running = true;
bool executed = false;
int mouse_x, mouse_y;


int main ()
{
  GridBuilder builder(WIDTH - BORDER, HEIGTH - BORDER, BORDER, NODE_SIZE);
  builder.build_grid();
  auto grid = builder.build_grid();
  GridEditor editor(grid);

  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Event event;
  BFS null_strategy;
  PathFindingStrategy* placeholder = &null_strategy;

  if (SDL_Init(SDL_INIT_EVERYTHING))
  {
    std::cout << "Error: Could not initialize SDL\n";
  }

  // we have to decrement the width and heigth by border because of 
  // the way we render nodes 
  SDL_CreateWindowAndRenderer(WIDTH - BORDER, HEIGTH - BORDER, 
                              0, &window, &renderer);
  GridRenderer grid_renderer(renderer); 
  EventHandler event_handler(editor, running, executed, placeholder); 
  PathFinder finder(placeholder);

  if (!window || !renderer)
  {
    std::cout << "Error: Could not create window and renderer\n";
  }

  while (running)
  {
    event_handler.handle_events(event); 

    grid_renderer.render(grid);
    
    if (finder.validate(grid) && executed)
    {
      finder.set_strategy(event_handler.get_pathfinding_strategy());
      finder.find_path(grid, grid_renderer);
      executed = false;
    } 
  }

  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);

  SDL_Quit();
}
