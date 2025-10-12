#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "constants.h"
#include "rules.h"
#include "setup.h"

SDL_Window* window;
SDL_Renderer* renderer;

bool quit = false;

int blocks[rows][cols] = {0};
SDL_FRect rects[rows][cols] = {};

int iteration = 0;

int runsForever = 0;

enum State { setup, running, paused };
enum Color { black, white };

enum State currentState = setup;
enum Color currentDrawingColor = white;

void (*decidingFunction)(int, int, int[rows][cols]);

void handleInput() {
  SDL_Event e;

  while (SDL_PollEvent(&e)) {
    switch (e.type) {
      case SDL_EVENT_QUIT:
        quit = true;
        break;
      case SDL_EVENT_KEY_DOWN:
        if(currentState == setup)
        {

        }
        break;
    }
  }
}

void handleLogic() {
  if (!runsForever && (iteration >= rows || iteration == 0)) return;

  for (int i = 0; i < cols; i++) {
    (*decidingFunction)(i, iteration, blocks);
  }

}

void handleDrawing() {
  if (!runsForever && iteration >= rows - 1) return;

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

  TTF_Font* font = TTF_OpenFont("../res/Coda-Regular.ttf", 16);

  if(!font){
    printf("Couldn't open font");
  }

  setupRects(rects);
  setStartingRow(center_three, blocks);

  decidingFunction = &rule30;  // set the rule for life
  runsForever = 0;


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
