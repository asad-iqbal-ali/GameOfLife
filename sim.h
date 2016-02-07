#include "grid.h"

//Take the input grid and calculate the output grid after one tick
void tick(grid *input, grid *output);

//Given a row value in the grid, and a pointer to the row itself, 
//the row above it, and the row below it (or NULL if any of those rows
//are not active rows) calculate that row's live cells and insert them 
//into output.
void solverow(grid *output, int row, int lowval, xval *hi, xval *mid, xval *low);

//Given three lists of y-values, find the next lowest y-value and put it in out.
//If no further values are available, return 0
int findnexty(yval **hi, yval **mid, yval **low, int *out);

//Check spot (row, cell) as located within the three provided row lists to see if
//it will be alive in the next iteration.. If it is, insert it into the output grid
void checkspot(grid *out, int cell, int row, xval *hi, xval *mid, xval *low);
