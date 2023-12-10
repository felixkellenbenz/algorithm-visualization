#include <SDL2/SDL_events.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <cstdint>
#include <iostream>
#include <SDL2/SDL.h>

#include "grid.hpp"

uint32_t const WIDTH = 620;
uint32_t const HEIGTH = 480;
uint8_t const BORDER = 1;
uint32_t const NODE_SIZE = 20;
bool running = true;
int mouse_x, mouse_y;


int main (int argc, char *argv[])
{

  GridBuilder builder(WIDTH, HEIGTH, BORDER, NODE_SIZE);
  builder.build_grid();
  auto grid = builder.export_grid();
  GridEditor editor(grid, NODE_SIZE, BORDER);

  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Event ev;

  if (SDL_Init(SDL_INIT_EVERYTHING))
  {
    std::cout << "Error: Could not init sdl\n";
  }

  SDL_CreateWindowAndRenderer(WIDTH, HEIGTH, 0, &window, &renderer);
  GridRenderer grid_renderer(renderer, grid); 

  if (!window || !renderer)
  {
    std::cout << "Error: Could not create window and renderer\n";
  }

  while (running)
  {
    while (SDL_PollEvent(&ev))
    {
      if (ev.type == SDL_QUIT)
        running = false;
      if (ev.type == SDL_MOUSEBUTTONDOWN)
      {
        SDL_GetMouseState(&mouse_x, &mouse_y);
        editor.make_obstacle(mouse_x, mouse_y);
      }
    }

    grid_renderer.render(); 

    SDL_RenderPresent(renderer); 
  }

  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);

  SDL_Quit();
}
