#include <SDL2/SDL.h>

int main(int argc, char** argv) {
  SDL_Window *window;

  SDL_Init(SDL_INIT_VIDEO);

  window = SDL_CreateWindow(
    "emulator",
    SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
    640,
    640,
    SDL_WINDOW_OPENGL
  );

  int running = 1;
  while (running) {
    SDL_Event event;

    while (SDL_PollEvent(&event) > 0) {
      if (event.type == SDL_QUIT || event.type == SDL_KEYDOWN) {
        running = 0;
      }
    }
  }

  SDL_DestroyWindow(window);

  SDL_Quit();
  return 0;
}
