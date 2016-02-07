#include<stdlib.h>
#include<stdio.h>
#include "sim.h"

int main(int argc, char **argv){

 grid* g[2]; 
 g[0] = makegrid();
 g[1] = makegrid();

 FILE *fp;


 int i, ticks;

 if(argc < 2 || argc > 3){
	fprintf(stderr, "Usage: $./simulator [(input)] (ticks)\n");
	return -1;
 }

 if(argc == 3){
	fp = fopen(argv[1], "r");
	if(fp == NULL){
		fprintf(stderr, "Could not open file: %s\n", argv[1]);
		return -1;
	}
	read_grid(g[0], fp);
	fclose(fp);
	ticks = atoi(argv[2]);
 } 

 else{
  ticks = atoi(argv[1]);
  ins_point(g[0], -1, 1);
  ins_point(g[0], -1, 0);
  ins_point(g[0], 0, 1);
  ins_point(g[0], 0, -1);
  ins_point(g[0], 1, 0);

  ins_point(g[0], -2, -1);
  ins_point(g[0], 2, 1);
 }

 if(ticks < 1){
	fprintf(stderr, "Invalid # of ticks\n");
	return -1;
 }


 printf("Starting grid:\n");
 print_grid(g[0]);
 printf("\n");

 for(i = 0; i < ticks; ++i){
	if(g[i%2]->xs == NULL){
		printf("Grid empty at tick %d\n", i);
		
 		freegrid(g[1-(i%2)]);
 		free(g[0]);
		free(g[1]);
		return 0;
	}
	tick(g[i%2], g[1 - (i%2)]);
 }

 
 printf("Final grid:\n");
 print_grid(g[i%2]);

 fp = fopen("output", "w");

 print_grid_to_file(g[i%2], fp); 

 fclose(fp);
 freegrid(g[0]);
 freegrid(g[1]);
 free(g[0]);
 free(g[1]);
 return 0;
}

