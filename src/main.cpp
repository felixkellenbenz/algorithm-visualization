#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <cstdint>
#include <iostream>
#include <SDL2/SDL.h>

#include "grid.hpp"

uint32_t const WIDTH = 620;
uint32_t const HEIGTH = 480;


int main (int argc, char *argv[])
{
  Grid grid;
  SDL_Window *window;
  SDL_Renderer *renderer;

  if (SDL_Init(SDL_INIT_EVERYTHING))
  {
    std::cout << "Error: Could not init sdl\n";
  }

  SDL_CreateWindowAndRenderer(WIDTH, HEIGTH, 0, &window, &renderer);

  if (!window || !renderer)
  {
    std::cout << "Error: Could not create window and renderer\n";
  }

  grid.render(renderer);

  SDL_RenderPresent(renderer);

  SDL_Delay(2000);

  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);

  SDL_Quit();
}
