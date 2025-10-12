#include <SDL3/SDL.h>

#define width 800

#define rows 250
#define cols 200

const int blockWidth = width / cols;
const int height = blockWidth * rows;

SDL_Window* window;
SDL_Renderer* renderer;

bool quit = false;

int blocks[rows][cols] = {0};
SDL_FRect rects[rows][cols] = {};

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

int iteration = 1;

void handleLogic() {
  if (iteration >= rows)
    return;

  for (int i = 0; i < cols; i++) {
    (*decidingFunction)(i, iteration);
  }

  iteration++;
  SDL_Delay(10);
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

enum style { checkered, centered, center_three, custom };

void setStartingRow(enum style style) {
  int row[cols] = {0};

  int centerCol = (cols / 2);

  switch (style) {
    case checkered:
      for (int i = 0; i < cols; i++) {
        row[i] = i % 2;
      }
      break;

    case centered:
      row[centerCol] = 1;
      break;

    case center_three:
      row[centerCol]     = 1;
      row[centerCol + 1] = 1;
      row[centerCol - 1] = 1;
      break;

    case custom:
      row[centerCol]     = 1;
      row[centerCol + 1] = 1;
      row[centerCol - 1] = 1;

      row[0] = 1;
      row[cols - 1] = 1;

      break;
  }

  for (int i = 0; i < cols; i++)
    blocks[0][i] = row[i];
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
  setStartingRow(custom);

  decidingFunction = &oneAndOnlyOneOfThreeRule; // set the rule for reproduction

  while (!quit) {
    handleInput();
    handleLogic();
    handleDrawing();
  }

  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_Quit();

  return 0;
}
