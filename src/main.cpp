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

uint32_t const WIDTH = 720;
uint32_t const HEIGTH = 540;
uint8_t const BORDER = 1;
uint32_t const NODE_SIZE = 20;
bool running = true;
bool algo_running = false;
int mouse_x, mouse_y;


int main (int argc, char *argv[])
{
  GridBuilder builder(WIDTH - BORDER, HEIGTH - BORDER, BORDER, NODE_SIZE);
  builder.build_grid();
  auto grid = builder.build_grid();
  GridEditor editor(grid);


  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Event event;

  if (SDL_Init(SDL_INIT_EVERYTHING))
  {
    std::cout << "Error: Could not init sdl\n";
  }

  // we have to decrement the width and heigth by border because of 
  // the way we render nodes 
  SDL_CreateWindowAndRenderer(WIDTH - BORDER, HEIGTH - BORDER, 
                              0, &window, &renderer);
  GridRenderer grid_renderer(renderer, grid); 
  EventHandler event_handler(editor, running);



  if (!window || !renderer)
  {
    std::cout << "Error: Could not create window and renderer\n";
  }

  while (running)
  {
    event_handler.handle_events(event); 

    grid_renderer.render();

    SDL_RenderPresent(renderer); 
  }

  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);

  SDL_Quit();
}
