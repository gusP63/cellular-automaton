#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdlib.h>

#include "constants.h"
#include "rules.h"
#include "setup.h"

SDL_Window* window;
SDL_Renderer* renderer;
TTF_Font* font;

SDL_Texture* tutorialTextTexture;
SDL_FRect tutorialTextBounds;

SDL_Texture* resetTextTexture;
SDL_FRect resetTextBounds;

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
    if (e.type == SDL_EVENT_QUIT) quit = true;

    if (currentState == setup) {
      switch (e.type) {
        case SDL_EVENT_KEY_DOWN:
          if (e.key.key == SDLK_1) {
            currentDrawingColor = white;
          }

          if (e.key.key == SDLK_2) {
            currentDrawingColor = black;
          }

          if (e.key.key == SDLK_R) {
            clearBoard(blocks);
          }

          if (e.key.key == SDLK_SPACE) {
            currentState = running;
          }

          if (e.key.key == SDLK_Q) {
            quit = true;
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

    if (currentState == running) {
      switch (e.type) {
        case SDL_EVENT_KEY_DOWN:
          if(e.key.key == SDLK_R || e.key.key == SDLK_ESCAPE){
            clearBoard(blocks);
            iteration = 0;
            currentState = setup;
          }

          if (e.key.key == SDLK_Q) {
            quit = true;
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

      if(cellToPaint.row >= 0 && cellToPaint.row <= rows - 1 && cellToPaint.col >= 0 && cellToPaint.col <= cols - 1)
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

int didDraw = 0;

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

  if (currentState == setup) {
    SDL_RenderTexture(renderer, tutorialTextTexture, NULL, &tutorialTextBounds);
  }

  SDL_RenderPresent(renderer);


  if(didDraw) didDraw = 0;
}

void getAssetsFolder(char* path) {
  const char* suffix = "/build/";
  size_t len = strlen(path);
  size_t suffix_len = strlen(suffix);

  if (len >= suffix_len && strcmp(path + len - suffix_len, suffix) == 0) {
    path[len - suffix_len] = '\0';  // remove "/build/"
    strcat(path, "/res/");          // append "/res/"
  }
}

void initSDL();
void setupUI();

int main(int argc, char** argv) {
  initSDL();
  setupUI();

  setupRects(rects);

  decidingFunction = &rule30;  // set the rule for life
  runsForever = 0;

  // main loop
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

void initSDL() {
  SDL_Init(SDL_INIT_VIDEO);
  window = SDL_CreateWindow("automaton", width, height, 0);
  renderer = SDL_CreateRenderer(window, NULL);

  if (!window) {
    printf("Couldn't create window, %s", SDL_GetError());
    exit(1);
  }

  if (!renderer) {
    printf("Couldn't create renderer, %s", SDL_GetError());
    exit(1);
  }

  char path[256] = "";
  strcat(path, SDL_GetCurrentDirectory());
  getAssetsFolder(path);
  strcat(path, "Coda-Regular.ttf");

  TTF_Init();
  font = TTF_OpenFont(path, 16);

  if (!font) {
    printf("Couldn't open font, %s", SDL_GetError());
    exit(1);
  }
}

void setupUI() {
  SDL_Color white_color = {255, 255, 255, SDL_ALPHA_OPAQUE};
  /* SDL_Color red_color = {255, 25, 25, SDL_ALPHA_OPAQUE}; */

  // tutorial/manual 
  char* tutorialString =
      "Add points with LEFT MOUSE BUTTON --- Press SPACE to start, R to reset, Q to quit --- Change color: 1 - white (life), 2 - black (void)";

  SDL_Surface* textSurface = TTF_RenderText_Blended(font, tutorialString, strlen(tutorialString), white_color);

  if(!textSurface) {
    SDL_Log("Surface error: %s", SDL_GetError());
    exit(1);
  }

  tutorialTextTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

  float textureW, textureH;
  SDL_GetTextureSize(tutorialTextTexture, &textureW, &textureH);

  tutorialTextBounds.h = textureH;
  tutorialTextBounds.w = textureW;
  tutorialTextBounds.x = width/2.0f - textureW/2.0f;
  tutorialTextBounds.y = height - textureH;

  //TODO: refactor this, SDL_Texture* getTextTexture(string, color, bounds (destRect)) 
  // reset/quit
  char* resetString = "Press R to reset, Q to quit";

  textSurface = TTF_RenderText_Blended(font, resetString, strlen(resetString), white_color);

  resetTextTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

  SDL_GetTextureSize(resetTextTexture, &textureW, &textureH);

  resetTextBounds.h = textureH;
  resetTextBounds.w = textureW;
  resetTextBounds.x = 16;
  resetTextBounds.y = textureH;


  SDL_DestroySurface(textSurface);
}


