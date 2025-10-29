#include "rules.h"
#include "constants.h"
#include <stdio.h>
#include <stdlib.h>

void alternatingRule(int i, int currentRow, int blocks[rows][cols], int blocksNext[rows][cols]) {
  int rowAbove = currentRow - 1;

  if (blocks[rowAbove][i] == 0) blocks[currentRow][i] = 1;
  if (blocks[rowAbove][i] == 1) blocks[currentRow][i] = 0;
}

void rule22(int i,
            int currentRow,
            int(blocks)[rows][cols],
            int blocksNext[rows][cols]) { // is filled if one and only of the three cells above is filled
  int rowAbove = currentRow - 1;

  int filledCount = 0;

  if (blocks[rowAbove][i] == 1) filledCount++;

  if (i != 0 && blocks[rowAbove][i - 1] == 1) filledCount++;

  if (i != cols - 1 && blocks[rowAbove][i + 1] == 1) filledCount++;

  if (filledCount == 1) {
    blocks[currentRow][i] = 1;
  }
}

void rule30(int i, int currentRow, int blocks[rows][cols], int blocksNext[rows][cols]) {
  int rowAbove = currentRow - 1;

  int x1 = i == 0 ? 0 : blocks[rowAbove][i - 1];
  int x2 = blocks[rowAbove][i];
  int x3 = i == (cols - 1) ? 0 : blocks[rowAbove][i + 1];

  if (x1 && !x2 && !x3) blocks[currentRow][i] = 1;

  if (!x1 && x2 && x3) blocks[currentRow][i] = 1;

  if (!x1 && x2 && !x3) blocks[currentRow][i] = 1;

  if (!x1 && !x2 && x3) blocks[currentRow][i] = 1;
}

void rule22WithATail(int i,
                     int currentRow,
                     int blocks[rows][cols],
                     int blocksNext[rows][cols]) { // same as above but has a tail
  int rowAbove = currentRow - 1;

  int filledCount = 0;

  if (blocks[rowAbove][i] == 1) filledCount++;

  if (i != 0 && blocks[rowAbove][i - 1] == 1) filledCount++;

  if (i != cols - 1 && blocks[rowAbove][i + 1] == 1) filledCount++;

  if (filledCount == 1) {
    blocks[currentRow][i] = 1;
    if (currentRow + 1 < rows) blocks[currentRow + 1][i] = 1; // tail
  }
}

void rule90(int i, int currentRow, int blocks[rows][cols], int blocksNext[rows][cols]) {

  int rowAbove = currentRow - 1;

  int x1 = i == 0 ? 0 : blocks[rowAbove][i - 1];
  int x2 = blocks[rowAbove][i];
  int x3 = i == (cols - 1) ? 0 : blocks[rowAbove][i + 1];

  if (x1 && x2 && !x3) blocks[currentRow][i] = 1;

  if (x1 && !x2 && !x3) blocks[currentRow][i] = 1;

  if (!x1 && x2 && x3) blocks[currentRow][i] = 1;

  if (!x1 && !x2 && x3) blocks[currentRow][i] = 1;
}

void rule184(int i, int currentRow, int blocks[rows][cols], int blocksNext[rows][cols]) {
  int rowAbove = currentRow - 1;

  int x1 = i == 0 ? 0 : blocks[rowAbove][i - 1];
  int x2 = blocks[rowAbove][i];
  int x3 = i == (cols - 1) ? 0 : blocks[rowAbove][i + 1];

  if (x1 && x2 && x3) blocks[currentRow][i] = 1;

  if (x1 && !x2 && x3) blocks[currentRow][i] = 1;

  if (x1 && !x2 && !x3) blocks[currentRow][i] = 1;

  if (!x1 && x2 && x3) blocks[currentRow][i] = 1;
}

void ruleTnt(int i, int currentRow, int blocks[rows][cols], int blocksNext[rows][cols]) { // i == currentCol;
  int rowAbove = currentRow - 1;
  int rowBelow = currentRow + 1;

  // TODO: a func that returns all the surrounding blocks
  int x1 = (i == 0 || currentRow == 0) ? 0 : blocks[rowAbove][i - 1];
  int x2 = (currentRow == 0) ? 0 : blocks[rowAbove][i];
  int x3 = (i == (cols - 1) || currentRow == 0) ? 0 : blocks[rowAbove][i + 1];
  int x4 = i == (cols - 1) ? 0 : blocks[currentRow][i + 1];
  int x5 = (i == (cols - 1) || currentRow == (rows - 1)) ? 0 : blocks[rowBelow][i + 1];
  int x6 = currentRow == (rows - 1) ? 0 : blocks[rowBelow][i];
  int x7 = (i == 0 || currentRow == rows - 1) ? 0 : blocks[rowBelow][i - 1];
  int x8 = i == 0 ? 0 : blocks[currentRow][i - 1];

  if (x1 && x2 && x3 && x4 && x5 && x6 && x7) {
    blocksNext[currentRow][i] = 0;
    return;
  }

  if (x1 || x2 || x3 || x4 || x5 || x6 || x7) {
    blocksNext[currentRow][i] = 1;
  }
}

void ruleRain(int i, int currentRow, int blocks[rows][cols], int blocksNext[rows][cols]) { // i == currentCol;
  int rowAbove = currentRow - 1;
  int rowBelow = currentRow + 1;

  // TODO: a func that returns all the surrounding blocks
  int x1 = (i == 0 || currentRow == 0) ? 0 : blocks[rowAbove][i - 1];
  int x2 = (currentRow == 0) ? 0 : blocks[rowAbove][i];
  int x3 = (i == (cols - 1) || currentRow == 0) ? 0 : blocks[rowAbove][i + 1];
  int x4 = i == (cols - 1) ? 0 : blocks[currentRow][i + 1];
  int x5 = (i == (cols - 1) || currentRow == (rows - 1)) ? 0 : blocks[rowBelow][i + 1];
  int x6 = currentRow == (rows - 1) ? 0 : blocks[rowBelow][i];
  int x7 = (i == 0 || currentRow == rows - 1) ? 0 : blocks[rowBelow][i - 1];
  int x8 = i == 0 ? 0 : blocks[currentRow][i - 1];

  if (currentRow == 0) {
    if (rand() % (100) <= 5) {
      blocksNext[currentRow][i] = 1;
      return;
    }
  }

  if (x2) {
    blocksNext[currentRow - 1][i] = 0;
    blocksNext[currentRow][i] = 1;

    if (rand() % (100) <= 1) {
      if (i >= 1) blocksNext[currentRow][i - 1] = 1;
    }

    if (rand() % (100) <= 1) {
      if (i < cols - 1) blocksNext[currentRow][i + 1] = 1;
    }
  }

  if (x5 || x6 || x7) {
    blocksNext[currentRow][i] = 0;
  }
}

void ruleSmoke(int i, int currentRow, int blocks[rows][cols], int blocksNext[rows][cols]) { // i == currentCol;
  int rowAbove = currentRow - 1;
  int rowBelow = currentRow + 1;

  // TODO: a func that returns all the surrounding blocks
  int x1 = (i == 0 || currentRow == 0) ? 0 : blocks[rowAbove][i - 1];
  int x2 = (currentRow == 0) ? 0 : blocks[rowAbove][i];
  int x3 = (i == (cols - 1) || currentRow == 0) ? 0 : blocks[rowAbove][i + 1];
  int x4 = i == (cols - 1) ? 0 : blocks[currentRow][i + 1];
  int x5 = (i == (cols - 1) || currentRow == (rows - 1)) ? 0 : blocks[rowBelow][i + 1];
  int x6 = currentRow == (rows - 1) ? 0 : blocks[rowBelow][i];
  int x7 = (i == 0 || currentRow == rows - 1) ? 0 : blocks[rowBelow][i - 1];
  int x8 = i == 0 ? 0 : blocks[currentRow][i - 1];

  if (x1 && x2 && x3 && x4 && x5 && x6 && x7) {
    blocksNext[currentRow][i] = 0;
    return;
  }

  if (x1 || x2 || x3 || x4 || x5 || x6 || x7) {
    blocksNext[currentRow][i] = 1;
  }
}

void conwayGameOfLife(int col, int row, int blocks[rows][cols], int blocksNext[rows][cols]) {
  /* printf("Conway1\n"); */
  if (col < 0 || row < 0 || col >= cols || row >= rows) return;

  int rowAbove = row - 1;
  int rowBelow = row + 1;

  // TODO: a func that returns all the surrounding blocks
  int x1 = (col == 0 || row == 0) ? 0 : blocks[rowAbove][col - 1];
  int x2 = (row == 0) ? 0 : blocks[rowAbove][col];
  int x3 = (col == (cols - 1) || row == 0) ? 0 : blocks[rowAbove][col + 1];
  int x4 = col == (cols - 1) ? 0 : blocks[row][col + 1];
  int x5 = (col == (cols - 1) || row == (rows - 1)) ? 0 : blocks[rowBelow][col + 1];
  int x6 = row == (rows - 1) ? 0 : blocks[rowBelow][col];
  int x7 = (col == 0 || row == rows - 1) ? 0 : blocks[rowBelow][col - 1];
  int x8 = col == 0 ? 0 : blocks[row][col - 1];

  int neighborsAlive = 0;

  const int radius = 8;
  int neighbors[] = {x1, x2, x3, x4, x5, x6, x7, x8};

  for (int i = 0; i < radius; i++) {
    neighborsAlive += neighbors[i];
  }

  if (blocks[row][col] == 1) {
    if (neighborsAlive < 2) // underpopulation
    {
      blocksNext[row][col] = 0;
      return;
    }

    if (neighborsAlive > 3) // overpopulation
    {
      blocksNext[row][col] = 0;
    }
  }

  if (blocks[row][col] == 0) {
    if (neighborsAlive == 3) // reproduction
      blocksNext[row][col] = 1;
  }
  /* printf("Conway2\n"); */
}
