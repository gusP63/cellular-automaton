#include <SDL3/SDL.h>
#include "constants.h"

#ifndef setup_h
#define setup_h

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

void setStartingRow(enum style style, int blocks[rows][cols]); 
void setupRects(SDL_FRect  rects[rows][cols]); 

#endif
