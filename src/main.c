#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "constants.h"
#include "rules.h"
#include "setup.h"

SDL_Window *window;
SDL_Renderer *renderer;
TTF_Font *font;

SDL_Texture *tutorialTextTexture;
SDL_FRect tutorialTextBounds;

void createTexture(SDL_Texture **texture, const char *text, SDL_FRect *bounds, int x, int y, int selected);

struct LifeRule {
  char *name;
  void (*function)(int, int, int[rows][cols], int[rows][cols]);
  SDL_Texture *texture;
  SDL_FRect bounds;
  bool runsForever;
} typedef LifeRule;

const int n_rules = 10;

LifeRule rules[10] = {{"alternatingRule", &alternatingRule, NULL, {}, false},
                      {"rule22", &rule22, NULL, {}, false},
                      {"rule30", &rule30, NULL, {}, false},
                      {"rule22WithATail", &rule22WithATail, NULL, {}, false},
                      {"rule90", &rule90, NULL, {}, false},
                      {"rule184", &rule184, NULL, {}, false},
                      {"ruleTnt", &ruleTnt, NULL, {}, true},
                      {"ruleRain", &ruleRain, NULL, {}, true},
                      {"ruleSmoke", &ruleSmoke, NULL, {}, true},
                      {"conwayGameOfLife", &conwayGameOfLife, NULL, {}, true}};

int currentRule = 0;

SDL_Texture *resetTextTexture;
SDL_FRect resetTextBounds;

bool quit = false;

int blocks[rows][cols] = {0};
int blocksBuffer[rows][cols] = {0};

SDL_FRect rects[rows][cols] = {};

int iteration = 1;

/* int runsForever = 0; */

enum State { setup, running, paused, chooseRule };
enum Color { black, white };

enum State currentState = setup;
enum Color currentDrawingColor = white;

void (*decidingFunction)(int, int, int[rows][cols], int[rows][cols]);

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

        if (e.key.key == SDLK_S || e.key.key == SDLK_ESCAPE) {
          currentState = chooseRule;
        }

        if (e.key.key == SDLK_X) {
          for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
              int chance = 25;
              if (rand() % 100 <= chance) blocks[i][j] = 1;
            }
          }
        }

        if (e.key.key == SDLK_SPACE) {
          for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
              blocksBuffer[i][j] = blocks[i][j];
            }
          }

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

    if (currentState == chooseRule) {
      switch (e.type) {

      case SDL_EVENT_KEY_DOWN:
        if (e.key.key == SDLK_ESCAPE || e.key.key == SDLK_RETURN) {
          currentState = setup;
          break;
        }
        if (e.key.key == SDLK_UP || e.key.key == SDLK_K) {
          if (currentRule == 0)
            currentRule = n_rules - 1;
          else
            currentRule--;
        }
        if (e.key.key == SDLK_DOWN || e.key.key == SDLK_J) {
          if (currentRule == (n_rules - 1))
            currentRule = 0;
          else
            currentRule++;
        }
        break;
      }
    }

    if (currentState == running) {
      switch (e.type) {
      case SDL_EVENT_KEY_DOWN:
        if (e.key.key == SDLK_R || e.key.key == SDLK_ESCAPE) {
          clearBoard(blocks);
          iteration = 1;
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

      if (cellToPaint.row >= 0 && cellToPaint.row <= rows - 1 && cellToPaint.col >= 0 && cellToPaint.col <= cols - 1)
        blocks[cellToPaint.row][cellToPaint.col] = currentDrawingColor == white ? 1 : 0;
    }
  }

  if (currentState == running) {
    if (!rules[currentRule].runsForever && (iteration >= rows || iteration == 0)) return;

    // top down rules
    if (!rules[currentRule].runsForever) {
      for (int i = 0; i < cols; i++) {
        (*rules[currentRule].function)(i, iteration, blocks, blocksBuffer);
      }
      iteration++;
      return;
    }

    // make a buffer array (apply rules to every cell simultaneously)
    // whole board rules
    if (rules[currentRule].runsForever) {
      for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
          (*rules[currentRule].function)(j, i, blocks, blocksBuffer);
        }
      }

      for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
          blocks[i][j] = blocksBuffer[i][j];
        }
      }
    }
    iteration++;
  }

  /* if (currentState == chooseRule) { */
  /* } */
}

void handleDrawing(int delay) { // milliseconds
  if (!rules[currentRule].runsForever && iteration >= rows - 1) return;

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE); // black
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE); // white

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      if (blocks[i][j] == 1) SDL_RenderFillRect(renderer, &rects[i][j]);
    }
  }

  if (currentState == setup) {
    SDL_RenderTexture(renderer, tutorialTextTexture, NULL, &tutorialTextBounds);
  }

  if (currentState == chooseRule) {
    for (int i = 0; i < n_rules; i++) {
      if (i == currentRule) {
        createTexture(&rules[i].texture, rules[i].name, &rules[i].bounds, width, height / 4.0f + (i * 48), 1);
      } else
        createTexture(&rules[i].texture, rules[i].name, &rules[i].bounds, width, height / 4.0f + (i * 48), 0);

      SDL_RenderTexture(renderer, rules[i].texture, NULL, &rules[i].bounds);
    }
  }

  SDL_RenderPresent(renderer);
  SDL_Delay(delay);
}

void getAssetsFolder(char *path) {
  const char *suffix = "/build/";
  size_t len = strlen(path);
  size_t suffix_len = strlen(suffix);

  if (len >= suffix_len && strcmp(path + len - suffix_len, suffix) == 0) {
    path[len - suffix_len] = '\0'; // remove "/build/"
    strcat(path, "/res/");         // append "/res/"
  }
}

void initSDL();
void setupUI();

int main(int argc, char **argv) {
  initSDL();
  setupUI();

  setupRects(rects);

  srand(time(0));

  // main loop
  while (!quit) {
    handleInput();
    handleLogic();
    handleDrawing(10);
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

void createTexture(SDL_Texture **texture, const char *text, SDL_FRect *bounds, int x, int y, int selected) {
  SDL_Color white_color = {255, 255, 255, SDL_ALPHA_OPAQUE};
  SDL_Color green_color = {0, 255, 0, SDL_ALPHA_OPAQUE};

  SDL_Color color = selected ? green_color : white_color;

  float w, h;

  SDL_Surface *textSurface = TTF_RenderText_Blended(font, text, strlen(text), color);

  *texture = SDL_CreateTextureFromSurface(renderer, textSurface);

  SDL_GetTextureSize(*texture, &w, &h);

  bounds->h = h;
  bounds->w = w;
  bounds->x = x / 2.0f - w / 2.0f;
  bounds->y = y + h;

  SDL_DestroySurface(textSurface);
}

void setupUI() {
  char *tutorialString =
      "Add points with LEFT MOUSE BUTTON --- Press S to select rule, SPACE to start, R to reset, X to randomize, Q to "
      "quit --- Change color: 1 - white "
      "(life), 2 - black (void)";

  createTexture(&tutorialTextTexture, tutorialString, &tutorialTextBounds, width, height - 48, 0);

  char *resetString = "Press R to reset, Q to quit";

  createTexture(&resetTextTexture, resetString, &resetTextBounds, 16, 0, 0);

  /* for (int i = 0; i < n_rules; i++) { */
  /*   createTexture(&rules[i].texture, rules[i].name, &rules[i].bounds, width, height / 4.0f + (i * 48), 0); */
  /* } */
}

