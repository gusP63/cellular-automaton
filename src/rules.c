#include "rules.h"
#include "constants.h"

void alternatingRule(int i, int currentRow, int blocks[rows][cols]) {
  int rowAbove = currentRow - 1;

  if (blocks[rowAbove][i] == 0) blocks[currentRow][i] = 1;
  if (blocks[rowAbove][i] == 1) blocks[currentRow][i] = 0;
}

void rule22(int i, int currentRow, int (blocks)[rows][cols]) {  // is filled if one and only of the three cells above is filled
  int rowAbove = currentRow - 1;

  int filledCount = 0;

  if (blocks[rowAbove][i] == 1) filledCount++;

  if (i != 0 && blocks[rowAbove][i - 1] == 1) filledCount++;

  if (i != cols - 1 && blocks[rowAbove][i + 1] == 1) filledCount++;

  if (filledCount == 1) {
    blocks[currentRow][i] = 1;
  }
}

void rule30(int i, int currentRow, int blocks[rows][cols]) {
  int rowAbove = currentRow - 1;

  int x1 = i == 0 ? 0 : blocks[rowAbove][i - 1];
  int x2 = blocks[rowAbove][i];
  int x3 = i == (cols - 1) ? 0 : blocks[rowAbove][i + 1];

  if (x1 && !x2 && !x3) blocks[currentRow][i] = 1;

  if (!x1 && x2 && x3) blocks[currentRow][i] = 1;

  if (!x1 && x2 && !x3) blocks[currentRow][i] = 1;

  if (!x1 && !x2 && x3) blocks[currentRow][i] = 1;
}

void rule22WithATail(int i, int currentRow, int blocks[rows][cols]) {  // same as above but has a tail
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

void rule90(int i, int currentRow, int blocks[rows][cols]) {

  int rowAbove = currentRow - 1;


  int x1 = i == 0 ? 0 : blocks[rowAbove][i - 1];
  int x2 = blocks[rowAbove][i];
  int x3 = i == (cols - 1) ? 0 : blocks[rowAbove][i + 1];

  if (x1 && x2 && !x3) blocks[currentRow][i] = 1;

  if (x1 && !x2 && !x3) blocks[currentRow][i] = 1;

  if (!x1 && x2 && x3) blocks[currentRow][i] = 1;

  if (!x1 && !x2 && x3) blocks[currentRow][i] = 1;


}

void rule184(int i, int currentRow, int blocks[rows][cols]) {

  /* / current pattern   111   110   101   100   011   010   001   000 / */
  /* /   new state for    1     0     1     1     1     0     0     0 / */

  int rowAbove = currentRow - 1;


  int x1 = i == 0 ? 0 : blocks[rowAbove][i - 1];
  int x2 = blocks[rowAbove][i];
  int x3 = i == (cols - 1) ? 0 : blocks[rowAbove][i + 1];

  if (x1 && x2 && x3) blocks[currentRow][i] = 1;

  if (x1 && !x2 && x3) blocks[currentRow][i] = 1;

  if (x1 && !x2 && !x3) blocks[currentRow][i] = 1;

  if (!x1 && x2 && x3) blocks[currentRow][i] = 1;


}
