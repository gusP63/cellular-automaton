#include "setup.h"

void clearBoard(int blocks[rows][cols]) {
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < cols; j++)
      blocks[i][j] = 0;
}

void setupRects(SDL_FRect rects[rows][cols]) {
  for (int currentRow = 0; currentRow < rows; currentRow++) {
    for (int currentCol = 0; currentCol < cols; currentCol++) {
      rects[currentRow][currentCol].w = blockWidth;
      rects[currentRow][currentCol].h = blockWidth;

      rects[currentRow][currentCol].x = blockWidth * currentCol;
      rects[currentRow][currentCol].y = blockWidth * currentRow;
    }
  }
}
