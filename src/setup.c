#include "setup.h"

void clearBoard(int blocks[rows][cols]) {
  for(int i = 0; i < rows; i++)
    for(int j = 0; j < cols; j++)
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

/* void setStartingRow(enum style style, int blocks[rows][cols]) { */
/*   int row[cols] = {0}; */
/*   int centerCol = (cols / 2); */

/*   switch (style) { */
/*     case checkered: */
/*       for (int i = 0; i < cols; i++) { */
/*         row[i] = i % 2; */
/*       } */
/*       break; */

/*     case center: */
/*       row[centerCol] = 1; */
/*       break; */

/*     case center_three: */
/*       row[centerCol] = 1; */
/*       row[centerCol + 1] = 1; */
/*       row[centerCol - 1] = 1; */
/*       break; */

/*     case tails: */
/*       row[0] = 1; */
/*       row[cols - 1] = 1; */
/*       break; */

/*     case every_three: */
/*       for (int i = 0; i < cols; i++) { */
/*         row[i] = i % 3 == 0 ? 1 : 0; */
/*       } */
/*       break; */
/*     case every_four: */
/*       for (int i = 0; i < cols; i++) { */
/*         row[i] = i % 4 == 0 ? 1 : 0; */
/*       } */
/*       break; */
/*     case every_five: */
/*       for (int i = 0; i < cols; i++) { */
/*         row[i] = i % 5 == 0 ? 1 : 0; */
/*       } */

/*     case every_ten: */
/*       for (int i = 0; i < cols; i++) { */
/*         row[i] = i % 10 == 0 ? 1 : 0; */
/*       } */
/*       break; */

/*     case custom: */
/*       /1* row[centerCol]     = 1; *1/ */
/*       row[centerCol + 2] = 1; */
/*       row[centerCol - 1] = 1; */

/*       /1* row[1] = 1; *1/ */

/*       /1* row[10] = 1; *1/ */

/*       break; */
/*   } */

/*   for (int i = 0; i < cols; i++) blocks[0][i] = row[i]; */
/* } */
