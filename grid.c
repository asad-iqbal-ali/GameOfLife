#include<stdlib.h>
#include<stdio.h>
#include "grid.h"


void freegrid(grid *g){
	xval *thisx = g->xs;
	xval *nextx = NULL;

	yval *thisy = NULL;
	yval *nexty = NULL;

	while(thisx != NULL){
		nextx = thisx->next;
		thisy = thisx->ys;
		while(thisy != NULL){
			nexty = thisy->next;
			free(thisy);
			thisy = nexty;
		}
		free(thisx);
		thisx = nextx;
	}

	g->xs = NULL; 

	g->min_x = g->min_y = g->max_x = g->max_y = 0;
}

void ins_point(grid *g, int newx, int newy){

	xval *thisx = g->xs;
	xval *lastx = NULL;

	yval *thisy = NULL;
	yval *lasty = NULL;


	if(thisx == NULL){
		g->xs = malloc(sizeof(xval));
		(g->xs)->x = newx;
		(g->xs)->next = NULL;
		(g->xs)->ys = malloc(sizeof(yval));
		((g->xs)->ys)->y = newy;
		((g->xs)->ys)->next = NULL;
		g->max_x = g->min_x = newx;
		g->max_y = g->min_y = newy;
		return;
	}

	if(newy > g->max_y)
		g->max_y = newy;
	if(newy < g->min_y)
		g->min_y = newy;

	while(thisx != NULL && thisx->x < newx){
		lastx = thisx;
		thisx = thisx->next;
	}

	if(thisx == NULL){
		g->max_x = newx;
		lastx->next = malloc(sizeof(xval));
		thisx = lastx->next;
		thisx->ys = NULL;
		thisx->next = NULL;
		thisx->x = newx;
	}

	else if(thisx->x > newx){
		if(lastx == NULL){
			g->min_x = newx;
			g->xs = malloc(sizeof(xval));
			(g->xs)->next = thisx;
			thisx = g->xs;
		}
		else{
			lastx->next = malloc(sizeof(xval));
			(lastx->next)->next = thisx;
			thisx = lastx->next;
		}
		thisx->x = newx;
		thisx->ys = NULL;
	}

	thisy = thisx->ys;
	
	if(thisy == NULL){
		thisx->ys = malloc(sizeof(yval));
		thisy = thisx->ys;
		thisy->next = NULL;
		thisy->y = newy;
		return;
	}

	while(thisy != NULL && thisy->y < newy){
		lasty = thisy;
		thisy = thisy->next;
	}

	if(thisy == NULL){
		lasty->next = malloc(sizeof(yval));
		thisy = lasty->next;
		thisy->y = newy;
		thisy->next = NULL;
	}

	else if(thisy->y > newy){
		if(lasty == NULL){
			thisx->ys = malloc(sizeof(yval));
			(thisx->ys)->next = thisy;
			thisy = thisx->ys;
		}
		else{
			lasty->next = malloc(sizeof(yval));
			(lasty->next)->next = thisy;
			thisy = lasty->next;
		}
		thisy->y = newy;
	}

	return;
	

}



grid *makegrid(){
	grid *g = malloc(sizeof(grid));
	g->xs = NULL;
	g->min_x = g->min_y = g->max_x = g->max_y = 0;

	return g;
}


void print_grid(grid *g){

	if(g->xs == NULL){
		printf("Grid empty\n");
		return;
	}

	int range = g->max_x - g->min_x;

	xval **list = malloc(sizeof(xval *)*(range+1));

	int x, y, i=0;
	xval *thisx = g->xs;
	yval *thisy;
	
	printf("Min x: %d, Max x: %d, Min y: %d, Max y: %d\n", g->min_y, g->max_y, g->min_x, g->max_x);

	for(x = g->min_x; x <= g->max_x; ++x){
	//	printf("%d\t", x);
		if(x > thisx->x)
			thisx = thisx->next;
		if(x < thisx->x){
			list[i++] = NULL;
	/*		for(y = g->min_y; y <= g->max_y; ++y){
				if(y == 0){
					if(x == 0)
						printf("+");
					else printf("|");
				}
				else if (x == 0)
					printf("-");
				else printf("+");
			}
	*/	}
		else{
			list[i++] = thisx;
	/*
			thisy = thisx->ys;
			for(y = g->min_y; y <= g->max_y; ++y){
				if(thisy != NULL && y > thisy->y)
					thisy = thisy->next;
				if(thisy == NULL || y < thisy->y){
					if(y == 0){
						if(x == 0)
							printf("+");
						else printf("|");
					}	
					else if (x == 0)
						printf("-");
					else printf("+");
					
				}
				else printf("o");
			}
	*/	}
	
//		printf("\n");
	}

	x = g->max_x;

	for(i = range; i > -1; --i){
	printf("%d\t", x);
		if(list[i] == NULL){
			for(y = g->min_y; y <= g->max_y; ++y){
				if(y == 0){
					if(x == 0)
						printf("+");
					else printf("|");
				}
				else if (x == 0)
					printf("-");
				else printf("+");
			}
		}
		else{

			thisy = list[i]->ys;
			for(y = g->min_y; y <= g->max_y; ++y){
				if(thisy != NULL && y > thisy->y)
					thisy = thisy->next;
				if(thisy == NULL || y < thisy->y){
					if(y == 0){
						if(x == 0)
							printf("+");
						else printf("|");
					}	
					else if (x == 0)
						printf("-");
					else printf("+");
					
				}
				else printf("o");
			}

		}
	

	--x;
	printf("\n");
	}

	free(list);
}



void read_grid(grid *g, FILE *fp){
	char *line = NULL;
	size_t size = 0;
	int x, y;
	while(getline(&line, &size, fp)!= -1){
		if(line[0] == '#')
			continue;
		else{
			sscanf(line, "%d %d", &y, &x);
			ins_point(g, x, y);
		}

	}

	free(line);


}

void print_grid_to_file(grid *g, FILE *fp){
	xval *thisx = g->xs;

	yval *thisy = NULL;

	while(thisx != NULL){
		thisy = thisx->ys;
		while(thisy != NULL){
			fprintf(fp, "%d\t%d\n", thisy->y, thisx->x);
			thisy = thisy->next;
		}
		thisx = thisx->next;

	}


}
