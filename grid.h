#include<stdio.h>
//A linked list of columns containing live cells within a row
typedef struct yval_ {
	int y;
	struct yval_ *next;
} yval;

//A linked list of rows containing live cells within a grid
typedef struct xval_ {
	int x;
	yval *ys;
	struct xval_ *next;
} xval;

//A data structure for a grid. This includes the current boundary
//values for rows and columns, and a linked list of rows with
//live cells in them ("active rows")
typedef struct grid_ {
	int max_x;
	int min_x;
	int max_y;
	int min_y;
	xval *xs;
} grid;

//free up all the memory used in a grid struct's linked lists
void freegrid(grid *g);

//insert a point into a grid
void ins_point(grid *g, int x, int y);

//Initialize a grid
grid *makegrid();

//Print a visual representation of the grid.
//Unfortunately, this is rotated clockwise 90 degrees
//For ease of coding.
void print_grid(grid *g);

//Read a grid from an input file
void read_grid(grid *g, FILE *fp);

//Print the grid to a file in the format from the prompt
void print_grid_to_file(grid *g, FILE *fp);
