#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <ncurses.h>

// debugging flags
#define DEBUG 0
static const int gDrawNeighbors=0;

// global settings that control behavior (screen size, speed, etc)
static int maxX=-1;
static int maxY=-1;
static const int starting_density=30;	// percent of cells initialized to alive
static const int generations=80;
static const long delay=100000; 		// amount of time in microseconds to sleep between generations

// global constants and variables that should not be modified.
static const int dead=0;
static const int live=1;
int earths=0;
int gen=0;

static WINDOW * mainwin;

// function declarations
int main(int, char**);
void start(void);
void quit(int);

/*******************************************************************************
 * Earth
 * 		Class that defines a single 'Earth' ie., a single generation at a time
 * 		there are generally two objects of this class, the 'today' object and
 * 		the 'tomorrow' object, representing the current and next generation 
 */
class Earth {
private:
	int *cell; 
	int n;

public:
	// constructor for Earth
	Earth() {
#ifdef DEBUG
		if ((maxX < 1 || maxX > 300) || (maxY < 1 || maxY > 300)) {
			fprintf(stderr,"Screen parameters out of bounds: maxX = %d maxY = %d\n", maxX, maxY);
			quit(1);
		}
#endif
		cell = new int[maxX * maxY];
		n = ++earths;
		randomize();
	}

	~Earth() {
		delete[] cell;
		printf("Earth %d destroyed\n", n);
	}

	// initialize cells.   Set all to dead except for a section in the middle of the screen which 
	// are randomly either dead or live
	void randomize() {
		for (int y=0 ; y<maxY ; y++) 
			for (int x=0 ; x<maxX ; x++) {
				if ((x>(maxX/2)-3) && 
					(x<(maxX/2)+3) && 
					(y>(maxY/2)-3) && 
					(y<(maxY/2)+3) && 
				    (rand()*100 < starting_density)) {
					set(x,y,live);		
				} else {
					set(x,y,dead);		
				}
			}
	}

	int get(int x, int y) {
#ifdef DEBUG
		if ((x<0 || x>=maxX) || (y<0 || y>=maxY)) {
			fprintf(stderr, "Value out of bounds in Earth::get(%d, %d)\n", x, y);
			quit(1);
		}
#endif
		return cell[x + y*maxX];
	}

	int set(int x, int y, int state) {
#ifdef DEBUG
		if ((x<0 || x>=maxX) || (y<0 || y>=maxY)) {
			fprintf(stderr, "Value out of bounds in Earth::set(%d, %d, %d)\n", x, y, state);
			quit(1);
		}
#endif
		cell[x + y*maxX] = state;
	}

	// count how many neighbors a cell has, including edge wrap
	int neighbors(int x, int y) {
#ifdef DEBUG
		if ((x<0 || x>=maxX) || (y<0 || y>=maxY)) {
			fprintf(stderr, "Value out of bounds in Earth::neighbors(%d, %d)\n", x, y);
			quit(1);
		}
#endif
		int u = y==maxY-1 ? 0 : y+1;
		int d = y ? y-1 : maxY-1;
		int l = x ? x-1 : maxX-1;
		int r = x==maxX-1 ? 0 : x+1;
		int count = get(l,u) + get(x,u) + get(r,u) +
		            get(l,y) +            get(r,y) +
		            get(l,d) + get(x,d) + get(r,d);

#ifdef DEBUG
		if (count < 0 || count > 8) {
			fprintf(stderr,"cell(%d,%d) has invalid number of neighbors: %d\n",x,y,count);
			fprintf(stderr,"u=%d\n",u);
			fprintf(stderr,"d=%d\n",d);
			fprintf(stderr,"l=%d\n",l);
			fprintf(stderr,"r=%d\n",r);
			quit(1);
		}

#endif
		return count;
	}

	// draw the grid of the cells
	void draw() {
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


};

/**************************************************************************************
  * Life
  *		Class that is the main workhorse of the program.   It creates two 'Earth' objects
  *		which define the state of the current and next generation of cells.
  *		Each generation consists of drawing itself to the display, then calculating the
  *		next generation based on the state of the current generation.
  */
class Life {
private:
	Earth *today;
	Earth *tomorrow;

public: 
	Life() {
		today = new Earth();
		tomorrow = new Earth();
	}

	~Life() {
		delete today;
		delete tomorrow;
		printf("Stopping after %d generations\n", gen);
	}

public: 
	void run() {
		//while(gen++ < generations) {
		bool done = false;
		while(!done) {
			gen++;
			today->draw();
			generate();

			Earth *tmp = tomorrow;
			tomorrow = today;
			today = tmp;

			char c=getch();
			switch (c) {
				case 'q': 
					done=true;
					break;
				case 'r':
					today->randomize();
					break;
			}
		}

	}

private:
	// determine next generation
	void generate() {
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
			}
		}
	}

};	

/*******************************************************************************
 *	main()
 *		very little is done in the main itself, just initialize, 
 *		then create the 'Life' object and run it, and then finally
 *		shut down clean.
 */
int main (int argc, char **argv) {
	start();

	srand(time(NULL)); 	// seed random number generator with current time 
	Life l;				// create the life object, which holds both worlds
	l.run();			// run the world

	quit(0);
}

/*******************************************************************************
 * start()
 * 		do the basic initialization for ncurses and etc
 */
void start(void) {
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
