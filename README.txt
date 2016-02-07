Asad Ali
Game of Life Simulator

Files:
	grid.h/grid.c:	The data structures used in the simulation and functions for manipulating those structures. Described further below.
	sim.h/sim.c:	The functions for calculating a new grid from an input grid.
	makefile:	Makefile. ">make" will create the executable file "simulator", and ">make clean" will remove it.
	sample:		A sample input file, as described in the prompt.

Executing:
	>make
	>./simulator [(input file)] (# of ticks)

	If an input file is not specified, the simulator runs a simple hard-coded simulation.

	The output is first a visual representation of the starting grid, followed by a visual representation of the final grid, output to the command line.

	The way my data structures are set up, x values are row numbers increasing from top to bottom, and y values are column numbers increasing from left to right.
	This was for ease of programming and printing, but unfortunately it does mean that the intended visualization of an input graph is most likely rotated 90 degrees.

	Grid visualization:
		left column: x values
		+	dead cell
		|	dead cell on x axis
		-	dead cell on y axis
		o	live cell

	The simulator also outputs to a tab-delineated list in the file "output", as described by the prompt.

Algorithm:
	I refer to rows or columns containing live cells as "active".

	The brute force method for solving this problem would be to scan through every cell within the ranges of active rows and columns and count live neighbors to determine if the cell is alive in the next iteration. That would be an O(mn) solution, where m and n represent the ranges of active rows and columns, respectively. While my solution does boil down to a brute force solution, it involves a few methods for skipping over rows and columns and as such is O(un), where u represents the number of active rows. Since u <= m by definition, this would at least help for large, sparse grids. The rules of the simulation, which seem generally biased toward producing dead cells, seem to incline most simulations toward becoming larger and sparser over time, so I thought optimizing for large, sparse grids was the best way to go.

	To start with, I figured that since a cell with no neighbors will not be alive in the next simulation, it's only necessary to examine rows which are or are immediately adjacent to active rows. Thus, the simulator walks down the grid from (min(x) - 1) to (max(x) + 1) and then runs calculations only on those rows. A row is "solved" using a function that tracks along the row itself and its two neighbors.

	A given cell will only be alive in the next iteration if its 3x3 neighborhood contains at least 3 live cells (itself and 2 neighbors, itself and 3 neighbors, or exactly 3 neighbors). As such, it is only necessary to check whether cells whose neighborhoods have at least 3 live cells will be alive in the following iteration, and the rest can be skipped over. The simulation does this by tracking along the row and its neighbors simulatneously from lowest y-value to highest, keeping track of 3 y-values at a time. If the range of those three values is greater than 2, it drops the smallest of the three and scans for the next lowest y-value of a live cell that is not one of the other 2.

	If the range of the 3 y-values is 0, then they are on a straight vertical line, and the minimum y-value (in this case, identical to the other 2 y values) must be checked along with both of its neighbors.

	If the range of the 3 y-values 1, then the y-value just below the minimum value does not need to be checked, as it is not possible for it to have 3 neighbors, since at least one of the y-values is at the min value +1, which is outside of that cell's neighborhood. If there are live cells with lower y-values than the 3 being considered, they were already considered in the previous iteration.

	If the range of the 3 y-values is 2, then only the middle value needs to be checked, as it is the only one that can contain all three values in its neighborhood. As above, if there are additional cells which would make the min value or the one before it necessary to check, those have already been considered in the previous iteration.

	Once no more new y-values can be checked, the row is complete.

Data Structures:
	Since this method involves traversing lists of numbers with an unknown range and distribution, I chose to structure my data as a linked list of row values for active rows, which in turn each contain linked lists of column values for active cells. The struct for a grid therefore contains a pointer to a list of x's, as well as the min and max values for x and y.
