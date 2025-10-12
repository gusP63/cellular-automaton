#include <SDL3/SDL.h>

#define width 400

#define rows 100
#define cols 40

SDL_Window* window;
SDL_Renderer* renderer;

bool quit = false;

int blocks[rows][cols] = {0};

SDL_FRect rects[rows][cols] = {};

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

int iteration = 1;

void handleLogic(void (*decidingRule)(int, int)) {
  if (iteration >= rows)
    return;

  for (int i = 0; i < cols; i++) {
    (*decidingRule)(i, iteration);
  }

  iteration++;
  SDL_Delay(500);
}

void handleDrawing() {
  if(iteration >= rows)
    return;

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);  // black
  SDL_RenderClear(renderer);

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      if (blocks[i][j] == 1) SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);  // white
      if (blocks[i][j] == 0) SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);        // black

      SDL_RenderFillRect(renderer, &rects[i][j]);
    }
  }

  SDL_RenderPresent(renderer);
}

enum style { checkered, centered, custom };

void setStartingRow(int row[cols], enum style style) {
  switch (style) {
    case checkered:
      for (int i = 0; i < cols; i++) {
        row[i] = i % 2;
      }
      break;

    case centered:
      row[(int)cols / 2] = 1;
      break;

    case custom:
      //
      break;
  }
}

void alternatingRule(int i, int currentRow) {
  int rowAbove = currentRow - 1;

  if (blocks[rowAbove][i] == 0) blocks[currentRow][i] = 1;
  if (blocks[rowAbove][i] == 1) blocks[currentRow][i] = 0;
}

void oneAndOnlyOneOfThreeRule(int i, int currentRow) {  // is filled if one and only of the three cells above is filled
  int rowAbove = currentRow - 1;

  int filledCount = 0;

  if (blocks[rowAbove][i] == 1)
    filledCount++;

  if (i != 0 && blocks[rowAbove][i - 1] == 1)
    filledCount++;

  if (i != cols - 1 && blocks[rowAbove][i + 1] == 1)
    filledCount++;

  if (filledCount == 1)
    blocks[currentRow][i] = 1;

}

int main(int argc, char** argv) {
  SDL_Init(SDL_INIT_VIDEO);

  const int blockWidth = width / cols;
  const int height = blockWidth * rows;

  window = SDL_CreateWindow("automaton", width, height, 0);
  renderer = SDL_CreateRenderer(window, NULL);

  int startingRow[cols] = {0};

  setStartingRow(startingRow, checkered);

  /* for(int i = 0; i < cols; i++) */
  /* { */

  /* } */

  for (int i = 0; i < cols; i++) blocks[0][i] = startingRow[i];

  for (int currentRow = 0; currentRow < rows; currentRow++) {
    for (int currentCol = 0; currentCol < cols; currentCol++) {
      rects[currentRow][currentCol].w = blockWidth;
      rects[currentRow][currentCol].h = blockWidth;

      rects[currentRow][currentCol].x = blockWidth * currentCol;
      rects[currentRow][currentCol].y = blockWidth * currentRow;
    }
  }

  void (*decidingFunction)(int, int);

  decidingFunction = &oneAndOnlyOneOfThreeRule;

  while (!quit) {
    handleInput();
    handleLogic(decidingFunction);
    handleDrawing();
  }

  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_Quit();

  return 0;
}
