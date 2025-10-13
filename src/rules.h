#include "constants.h"

#ifndef rules_h
#define rules_h

void alternatingRule(int i, int currentRow, int blocks[rows][cols], int blocksNext[rows][cols]);

void rule22(int i, int currentRow, int blocks[rows][cols], int blocksNext[rows][cols]);

void rule30(int i, int currentRow, int blocks[rows][cols], int blocksNext[rows][cols]);

void rule22WithATail(int i, int currentRow, int blocks[rows][cols], int blocksNext[rows][cols]);

void rule90(int i, int currentRow, int blocks[rows][cols], int blocksNext[rows][cols]);

void rule184(int i, int currentRow, int blocks[rows][cols], int blocksNext[rows][cols]);

void ruleTnt(int i, int currentRow, int blocks[rows][cols], int blocksNext[rows][cols]);

void ruleRain(int i, int currentRow, int blocks[rows][cols], int blocksNext[rows][cols]);

void ruleSmoke(int i, int currentRow, int blocks[rows][cols], int blocksNext[rows][cols]);

void conwayGameOfLife(int i, int currentRow, int blocks[rows][cols], int blocksNext[rows][cols]);

#endif
