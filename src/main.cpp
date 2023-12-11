#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <type_traits>

#include "grid.hpp"

uint32_t const WIDTH = 720;
uint32_t const HEIGTH = 540;
uint8_t const BORDER = 1;
uint32_t const NODE_SIZE = 20;
bool running = true;
int mouse_x, mouse_y;


int main (int argc, char *argv[])
{
  GridBuilder builder(WIDTH - BORDER, HEIGTH - BORDER, BORDER, NODE_SIZE);
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

  // we have to decrement the width and heigth by border because of 
  // the way we render nodes 
  SDL_CreateWindowAndRenderer(WIDTH - BORDER, HEIGTH - BORDER, 
                              0, &window, &renderer);
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

      if (ev.type == SDL_KEYDOWN)
      {
        if (ev.key.keysym.sym == 114)
        {
          editor.reset_grid();
        }
      }
    }

    grid_renderer.render(); 

    SDL_RenderPresent(renderer); 
  }

  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);

  SDL_Quit();
}
