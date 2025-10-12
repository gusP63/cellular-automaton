#include <SDL3/SDL.h>
#include "constants.h"
#include "rules.h"
#include "setup.h"

SDL_Window* window;
SDL_Renderer* renderer;

bool quit = false;

int blocks[rows][cols] = {0};
SDL_FRect rects[rows][cols] = {};
int iteration = 0;

enum State { setup, running, paused };

void (*decidingFunction)(int, int, int[rows][cols]);

void handleInput() {
  SDL_Event e;

  while (SDL_PollEvent(&e)) {
    switch (e.type) {
      case SDL_EVENT_QUIT:
        quit = true;
        break;
      case SDL_EVENT_KEY_DOWN:
        //
        break;
    }
  }
}

void handleLogic() {
  if (iteration >= rows || iteration == 0) return;

  for (int i = 0; i < cols; i++) {
    (*decidingFunction)(i, iteration, blocks);
  }

}

void handleDrawing() {
  if (iteration >= rows - 1) return;

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);  // black
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);  // white

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      if (blocks[i][j] == 1)
        SDL_RenderFillRect(renderer, &rects[i][j]);
    }
  }

  SDL_RenderPresent(renderer);
}

int main(int argc, char** argv) {
  SDL_Init(SDL_INIT_VIDEO);
  window = SDL_CreateWindow("automaton", width, height, 0);
  renderer = SDL_CreateRenderer(window, NULL);

  setupRects(rects);
  setStartingRow(center_three, blocks);

  decidingFunction = &rule30;  // set the rule for life

  const Uint64 startingTime = SDL_GetTicks();


  while (!quit) {
    handleInput();
    handleLogic();
    handleDrawing();
    iteration++;
  }

  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_Quit();

  return 0;
}
