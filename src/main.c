#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdlib.h>

#include "constants.h"
#include "rules.h"
#include "setup.h"

SDL_Window* window;
SDL_Renderer* renderer;
TTF_Font* font; 

SDL_Texture *textTexture;
float textureW, textureH;
SDL_FRect destRect;

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

SDL_FPoint mousePosition = {0, 0};

typedef struct GridCell {
  int row;
  int col;
} GridCell;

GridCell cellToPaint = {-1, -1};

int isMouseDown = 0;

void handleInput() {
  SDL_Event e;

  while (SDL_PollEvent(&e)) {
    switch (e.type) {
      case SDL_EVENT_QUIT:
        quit = true;
        break;

        if (currentState == setup) {
          case SDL_EVENT_KEY_DOWN:
            if (e.key.key == SDLK_1) {
              currentDrawingColor = white;
            }

            if (e.key.key == SDLK_2) {
              currentDrawingColor = black;
            }

            if (e.key.key == SDLK_SPACE) {
              currentState = running;
            }
            break;

          case SDL_EVENT_MOUSE_BUTTON_DOWN:
            isMouseDown = 1;
            SDL_GetMouseState(&mousePosition.x, &mousePosition.y);
            break;
          case SDL_EVENT_MOUSE_BUTTON_UP:
            isMouseDown = 0;
            break;

          case SDL_EVENT_MOUSE_MOTION:
            if (isMouseDown) {
              SDL_GetMouseState(&mousePosition.x, &mousePosition.y);
            }
            break;
        }
    }
  }
}

void handleLogic() {
  if (currentState == setup) {
    if (isMouseDown) {
      cellToPaint.row = (int)((int)mousePosition.y / blockWidth);
      cellToPaint.col = (int)((int)mousePosition.x / blockWidth);

      blocks[cellToPaint.row][cellToPaint.col] = currentDrawingColor == white ? 1 : 0;
    }
  }

  if (currentState == running) {
    if (!runsForever && (iteration >= rows || iteration == 0)) return;

    for (int i = 0; i < cols; i++) {
      (*decidingFunction)(i, iteration, blocks);
    }
  }
}

void handleDrawing() {
  if (!runsForever && iteration >= rows - 1) return;

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);  // black
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);  // white

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      if (blocks[i][j] == 1) SDL_RenderFillRect(renderer, &rects[i][j]);
    }
  }

  if(currentState == setup)
  {
    SDL_RenderTexture(renderer, textTexture, NULL, &destRect);

  }

  SDL_RenderPresent(renderer);
}

void getAssetsFolder(char *path) {
  const char *suffix = "/build/";
  size_t len = strlen(path);
  size_t suffix_len = strlen(suffix);

  if (len >= suffix_len && strcmp(path + len - suffix_len, suffix) == 0) {
    path[len - suffix_len] = '\0';     // remove "/build/"
    strcat(path, "/res/");              // append "/res/"
  }

}

int main(int argc, char** argv) {

  SDL_Init(SDL_INIT_VIDEO);
  window = SDL_CreateWindow("automaton", width, height, 0);
  renderer = SDL_CreateRenderer(window, NULL);

  char path[256] = "";
  strcat(path, SDL_GetCurrentDirectory());
  getAssetsFolder(path);
  strcat(path, "Coda-Regular.ttf");
  SDL_Log("%s", path);

  font = TTF_OpenFont(path , 16);

  if(!font){
    printf("Couldn't open font");
  }

  char* tutorialString = "Add points with LEFT MOUSE BUTTON --- Press SPACE to start --- Change color: 1 - white (life) 2 - black (void)";

  SDL_Color textColor = {255, 255, 255, SDL_ALPHA_OPAQUE};
  SDL_Surface *textSurface = TTF_RenderText_Blended(font, tutorialString, strlen(tutorialString), textColor);

  SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

  SDL_DestroySurface(textSurface);

  SDL_GetTextureSize(textTexture, &textureW, &textureH);

  destRect.h = textureH;
  destRect.w = textureW;
  destRect.x = 16;
  destRect.y = height - textureH;


  setupRects(rects);

  decidingFunction = &rule22;  // set the rule for life
  runsForever = 0;

  const Uint64 startingTime = SDL_GetTicks();

  while (!quit) {
    handleInput();
    handleLogic();
    handleDrawing();

    if (currentState == running) {
      iteration++;
    }
  }

  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_Quit();

  return 0;
}
