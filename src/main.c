#include <SDL3/SDL.h>

#define width 1500

#define rows 300
#define cols 500

const int blockWidth = width / cols;
const int height = blockWidth * rows;

SDL_Window* window;
SDL_Renderer* renderer;

bool quit = false;

int blocks[rows][cols] = {0};
SDL_FRect rects[rows][cols] = {};
int iteration = 0;

enum State { setup, running, paused };

void (*decidingFunction)(int, int);

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
    (*decidingFunction)(i, iteration);
  }

}

void handleDrawing() {
  if (iteration >= rows - 1) return;

  if(iteration == 0)
  {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);  // black
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);  // white
  }

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      if (blocks[i][j] == 1)
        SDL_RenderFillRect(renderer, &rects[i][j]);
    }
  }

  SDL_RenderPresent(renderer);
}

enum style {
  checkered,
  center,
  center_three,
  tails,
  every_three,
  every_four,
  every_five,
  every_ten,
  custom,
};

void setStartingRow(enum style style) {
  int row[cols] = {0};
  int centerCol = (cols / 2);

  switch (style) {
    case checkered:
      for (int i = 0; i < cols; i++) {
        row[i] = i % 2;
      }
      break;

    case center:
      row[centerCol] = 1;
      break;

    case center_three:
      row[centerCol] = 1;
      row[centerCol + 1] = 1;
      row[centerCol - 1] = 1;
      break;

    case tails:
      row[0] = 1;
      row[cols - 1] = 1;
      break;

    case every_three:
      for (int i = 0; i < cols; i++) {
        row[i] = i % 3 == 0 ? 1 : 0;
      }
      break;
    case every_four:
      for (int i = 0; i < cols; i++) {
        row[i] = i % 4 == 0 ? 1 : 0;
      }
      break;
    case every_five:
      for (int i = 0; i < cols; i++) {
        row[i] = i % 5 == 0 ? 1 : 0;
      }

    case every_ten:
      for (int i = 0; i < cols; i++) {
        row[i] = i % 10 == 0 ? 1 : 0;
      }
      break;

    case custom:
      /* row[centerCol]     = 1; */
      row[centerCol + 2] = 1;
      row[centerCol - 1] = 1;

      /* row[1] = 1; */

      /* row[10] = 1; */

      break;
  }

  for (int i = 0; i < cols; i++) blocks[0][i] = row[i];
}

void alternatingRule(int i, int currentRow) {
  int rowAbove = currentRow - 1;

  if (blocks[rowAbove][i] == 0) blocks[currentRow][i] = 1;
  if (blocks[rowAbove][i] == 1) blocks[currentRow][i] = 0;
}

void rule22(int i, int currentRow) {  // is filled if one and only of the three cells above is filled
  int rowAbove = currentRow - 1;

  int filledCount = 0;

  if (blocks[rowAbove][i] == 1) filledCount++;

  if (i != 0 && blocks[rowAbove][i - 1] == 1) filledCount++;

  if (i != cols - 1 && blocks[rowAbove][i + 1] == 1) filledCount++;

  if (filledCount == 1) {
    blocks[currentRow][i] = 1;
  }
}

void rule30(int i, int currentRow) {
  int rowAbove = currentRow - 1;

  int x1 = i == 0 ? 0 : blocks[rowAbove][i - 1];
  int x2 = blocks[rowAbove][i];
  int x3 = i == (cols - 1) ? 0 : blocks[rowAbove][i + 1];

  if (x1 && !x2 && !x3) blocks[currentRow][i] = 1;

  if (!x1 && x2 && x3) blocks[currentRow][i] = 1;

  if (!x1 && x2 && !x3) blocks[currentRow][i] = 1;

  if (!x1 && !x2 && x3) blocks[currentRow][i] = 1;
}

void rule22WithATail(int i, int currentRow) {  // same as above but has a tail
  int rowAbove = currentRow - 1;

  int filledCount = 0;

  if (blocks[rowAbove][i] == 1) filledCount++;

  if (i != 0 && blocks[rowAbove][i - 1] == 1) filledCount++;

  if (i != cols - 1 && blocks[rowAbove][i + 1] == 1) filledCount++;

  if (filledCount == 1) {
    blocks[currentRow][i] = 1;
    if (currentRow + 1 < rows) blocks[currentRow + 1][i] = 1;  // tail
  }
}

void setupRects() {
  for (int currentRow = 0; currentRow < rows; currentRow++) {
    for (int currentCol = 0; currentCol < cols; currentCol++) {
      rects[currentRow][currentCol].w = blockWidth;
      rects[currentRow][currentCol].h = blockWidth;

      rects[currentRow][currentCol].x = blockWidth * currentCol;
      rects[currentRow][currentCol].y = blockWidth * currentRow;
    }
  }
}

int main(int argc, char** argv) {
  SDL_Init(SDL_INIT_VIDEO);
  window = SDL_CreateWindow("automaton", width, height, 0);
  renderer = SDL_CreateRenderer(window, NULL);

  setupRects();
  setStartingRow(center);

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
