/*
 * LIFE
 *
 * This is an OO implimentation of Conway's LIFE 
 *
 * Curses is used for terminal IO and the system was developed on a Raspberry pi 2
 * with Raspian OS
 *
 * R. Sweet	2015
 */
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <ncurses.h>

// debugging flags
#define NDEBUG			// comment this out to enable assertions
#include <assert.h>

#include "life.h"


/*******************************************************************************
 *	main()
 *		very little is done in the main itself, just initialize, 
 *		then create the 'Life' object and run it, and then finally
 *		shut down clean.
 */
int main (int argc, char **argv) {
	initialize();			// do all the ncurses setup

	starting_density=75;	// set initial population density of the starting section

	srand(time(NULL)); 	// seed random number generator with current time 
	Life l;					// create the life object, which holds both worlds
	l.run();				// run the world

	quit(0);
}

/*******************************************************************************
 * start()
 * 		do the basic initialization for ncurses and etc
 */
void initialize(void) {
    /*  Initialize ncurses  */
	if ( (mainwin = initscr()) == NULL ) {
		fprintf(stderr, "Error initialising ncurses.\n");
		exit(EXIT_FAILURE);
	}

	// get screen dimensions
	getmaxyx(mainwin, maxY, maxX);

	cbreak();
	noecho();
	nodelay(mainwin,true);
}

/*******************************************************************************
 * quit()
 * 		final cleanup
 */
void quit(int code) {
	delwin(mainwin);
	endwin();
	refresh();

	exit(code);
}

/*******************************************************************************
 * Earth
 * 		Class that defines a single 'Earth' ie., a single generation at a time
 * 		there are generally two objects of this class, the 'today' object and
 * 		the 'tomorrow' object, representing the current and next generation 
 */

// constructor for Earth
Earth::Earth() {
	assert((maxX >= 1 && maxX <= 300) && (maxY >= 1 && maxY < 300));
	cell = new int[maxX * maxY];
	randomize();
}

Earth::~Earth() {
	delete[] cell;
}

// initialize cells.   Set all to dead except for a section in the middle of the screen which 
// are randomly either dead or live
void Earth::randomize() {
	for (int y=0 ; y<maxY ; y++) 
		for (int x=0 ; x<maxX ; x++) {
			if ((x>(maxX/2)-3) && 
				(x<(maxX/2)+3) && 
				(y>(maxY/2)-3) && 
				(y<(maxY/2)+3) && 
				(int(rand() % 100) <= starting_density)) {
					set(x,y,live);		
			} else {
				set(x,y,dead);		
			}
		}
}

int Earth::get(int x, int y) {
	assert ((x>=0 && x<maxX) && (y>=0 && y<maxY));
	return cell[x + y*maxX];
}

int Earth::set(int x, int y, int state) {
	assert ((x>=0 && x<maxX) && (y>=0 && y<maxY));
	cell[x + y*maxX] = state;
    return state;
}

// count how many neighbors a cell has, including edge wrap
int Earth::neighbors(int x, int y) {
	assert ((x>=0 && x<maxX) && (y>=0 && y<maxY));
	int u = y==maxY-1 ? 0 : y+1;
	int d = y ? y-1 : maxY-1;
	int l = x ? x-1 : maxX-1;
	int r = x==maxX-1 ? 0 : x+1;
	int count = get(l,u) + get(x,u) + get(r,u) +
				get(l,y) +            get(r,y) +
				get(l,d) + get(x,d) + get(r,d);

	assert(count >= 0 && count <= 8);

	return count;
}

// draw the grid of the cells
void Earth::draw() {
	// draw the cells
	for (int y=0; y<maxY ; y++) {
		move(y,0);
		for (int x=0; x<maxX ; x++) {
			char c = get(x,y) ? 'O' : ' ';
			addch(c);
		}
	}
	refresh();
}

/**************************************************************************************
  * Life
  *		Class that is the main workhorse of the program.   It creates two 'Earth' objects
  *		which define the state of the current and next generation of cells.
  *		Each generation consists of drawing itself to the display, then calculating the
  *		next generation based on the state of the current generation.
  */
Life::Life() {
	today = new Earth();
	tomorrow = new Earth();
}

Life::~Life() {
	delete today;
	delete tomorrow;
}

// Main loop of the game
// 1. draw the 'today' matrix of cells
// 2. generate the 'tomorrow' matrix by calling generate()
// 3. check for user input
// 4. repeat back to 1
void Life::run() {
	bool done = false;
	while(!done) {
		today->draw();
		generate();

		// swap today and tomorrow.
		// really what's happening is tomorrow becomes today, and then we reuse 
		// the no longer needed old today as a new tomorrow
		Earth *tmp = tomorrow;
		tomorrow = today;
		today = tmp;

		// check to see if the user hit a key
		char c=getch();
		switch (c) {
			case 'q': 		// quit
				done=true;
				break;
			case 'r':		// restart & randomize
				today->randomize();
				break;
		}
	}
}

// iterate through the today Earth, updating the corresponding cell in tomorrow
void Life::generate() {
	int count=0;	// total count of all living cells in the current generation
	for (int y=0; y<maxY ; y++) {
		for (int x=0; x<maxX ; x++) {
			int state = dead;
			int n = today->neighbors(x,y);
			if (n == 2) {
				state = today->get(x,y);
			} else if (n == 3) {
				state = live;
			} 
			tomorrow->set(x,y,state);
			count += state;
		}
	}
	// if there are no living cells then start over
	if (! count) {
		tomorrow->randomize();
	}
}

