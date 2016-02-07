#include "sim.h"
#include<stdlib.h>
#include<stdio.h>

void tick(grid *input, grid *output){

	//"row" is the row we're looking to solve.
	//The first row we check is just above the lowest active row
	int row = input->min_x - 1;

	int lowy = input->min_y - 1;

	//We keep track of the current active row, as well as the previous active row
	//and the next one
	xval *current_row = input->xs;
	xval *prev_row = NULL;
	xval *next_row = NULL;
	

	//Start by clearing out any data that may have been in the output grid
	freegrid(output);

	if(current_row!= NULL)
		next_row = current_row->next;

	//Scan through each active row, solving the row directly above it, the row itself, and the one directly
	//below it. There are a few additional checks to ensure that we're not solving any rows multiple times.
	while(current_row != NULL){
		//Start by solving the row directly above the current active row
		//unless we already did in the previous iteration
		if(row < current_row->x){
			row = current_row->x - 1;
			//The row we're solving cannot be active, and neither can the row immediately above it.
			//If either of those rows were active, we would have already solved this row in the previous
			//iteration
			solverow(output,row,lowy,NULL,NULL,current_row);
			++row;
		}
		//Solve the current active row. 
		if(row == current_row->x){
			//If the previous active row or the next active row are immediately adjacent, 
			//include them in the calucations.
			solverow(output,row,lowy,
				(prev_row != NULL && prev_row->x == row - 1? prev_row : NULL),
				current_row,
				(next_row != NULL && next_row->x == row + 1? next_row : NULL));
			++row;
		}
		//Solve the row directly below the current active row
		//unless that row is also an active row, in which case
		//just jump to the next iteration.
		if(row == current_row->x + 1 && (next_row == NULL || next_row->x != row)){
			//if the next active row is directly below the one we're solving, include
			//it in the calculation.
			solverow(output,row,lowy,current_row,NULL,
				(next_row != NULL && next_row->x == row + 1? next_row : NULL));
			++row;
		}

		prev_row = current_row;
		current_row = next_row;
		if(current_row!= NULL)
			next_row = current_row->next;
	}
}

void solverow(grid *output, int row, int firsty, xval *hirow, xval *middlerow, xval *lowrow){
	//The three y-values to track. Min is set by default to the lowest possible y-value.
	int min = firsty;
	int mid;
	int max;

	//Used for swapping values around when min jumps to the next available y-value
	int tmp;

	//Tracks the y-value up to which we've already checked any cells necessary.
	//Initially, set it to 2 behind the lowest y-value, to ensure that if the lowest y is the min of the first check, we check all relevant cells anyway.
	int solved_to = firsty-2;


	//Trackers which run along the three rows
	yval *hi_tracker = NULL;
	yval *mid_tracker = NULL;
	yval *low_tracker = NULL;

	if(hirow != NULL)
		hi_tracker = hirow->ys;
	
	if(middlerow != NULL)
		mid_tracker = middlerow->ys;

	if(lowrow != NULL)
		low_tracker = lowrow->ys;

	//Assign the values of min, mid, and max to the first three y-values in the
	//rows being considered. If there are not three live cells in the three rows,
	//there can't be any live cells in the middle row in the next iteration,
	//so we are done.
	if(!findnexty(&hi_tracker, &mid_tracker, &low_tracker, &min))
		return;
	if(!findnexty(&hi_tracker, &mid_tracker, &low_tracker, &mid))
		return;
	if(!findnexty(&hi_tracker, &mid_tracker, &low_tracker, &max))
		return;
	
	do{
		//Swapping around values whenever min has jumped to the next available y-value
		//as necessary to ensure that min, mid, and max are in increasing order.
		if(min > mid){
			tmp = mid;
			mid = min;
			min = tmp;
		}
		if(mid > max){
			tmp = max;
			max = mid;
			mid = tmp;
		}
		//If the range of values we have is small enough, check each of the necessary cells in turn.
		//However, if a given cell has already been checked, skip over it.
		if((max - min)<3){
			if(solved_to < min+1)
				checkspot(output, min+1, row,hirow,middlerow,lowrow);
		
			if((max-min)<2){
				if(solved_to < min)
					checkspot(output, min, row,hirow,middlerow,lowrow);
			
				if((max-min)<1){
					if(solved_to < min-1)
						checkspot(output, min-1, row,hirow,middlerow,lowrow);
				}
			}
		}
		solved_to = min + 1;
	}while(findnexty(&hi_tracker, &mid_tracker, &low_tracker, &min) != 0);
	

	return;
}

int findnexty(yval **hi, yval **mid, yval **low, int *out){
	//return value to state the row in which the next y-value was found.
	int found = 0;

	if(*hi != NULL ){
		*out = (*hi)->y;
		found = 1;
	}

	if(*mid != NULL && (!found || ((*mid)->y < *out))){
		*out = (*mid)->y;
		found = 2;
	}	
	
	if(*low != NULL && (!found || ((*low)->y < *out))){
		*out = (*low)->y;
		found = 3;
	}	

	//If the next lowest y-value was found in a given row, move that row's tracker
	//to its next value.
	switch(found){
		case 1:
			*hi = (*hi)->next;
			break;
		case 2:
			*mid = (*mid)->next;
			break;
		case 3:
			*low = (*low)->next;
			break;
		default :
			break;
	}

	return found;
}

void checkspot(grid *out, int cell, int row, xval *hi, xval *mid, xval *low){

	int neighbors = 0;
	int alive = 0;

	yval *hitrack = NULL;
	if(hi != NULL)
		hitrack = hi->ys;
	yval *midtrack = NULL;
	if(mid != NULL)
		midtrack = mid->ys;
	yval *lowtrack = NULL;
	if(low != NULL)
		lowtrack = low->ys;

	
	//Count neighbors that exist in a given row.
	while(hitrack != NULL && hitrack->y < cell + 2){
		if(hitrack->y > cell - 2)
			++neighbors;
		hitrack = hitrack->next;
	}
	
	while(lowtrack != NULL && lowtrack->y < cell + 2){
		if(lowtrack->y > cell - 2)
			++neighbors;
		lowtrack = lowtrack->next;
	}
	//For the middle row, also be sure to check whether the cell itself
	//is alive.
	while(midtrack != NULL && midtrack->y < cell + 2){
		if((midtrack->y == (cell - 1))|| (midtrack->y == (cell + 1)))
			++neighbors;
		else if(midtrack->y == cell)
			alive = 1;
		midtrack = midtrack->next;
	}

	//The actual rules of the game!
	if((neighbors == 3) || (alive && (neighbors == 2 )))
		ins_point(out, row, cell); 
}
