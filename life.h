// life.h

#ifndef LIFE_H
#define LIFE_H

// global settings that control behavior (screen size, speed, etc)
static int maxX=-1;
static int maxY=-1;
int starting_density=50;	// percent of cells initialized to alive

// global constants and variables that should not be modified.
static const int dead=0;
static const int live=1;

static WINDOW * mainwin;

// FUNCTION DECLARATIONS

int main(int, char**);
void start(void);
void quit(int);

// CLASS DECLARATIONS

// Class to hold the matrix of cells for a single generation
class Earth {
private:
	int *cell;
public:
	Earth() ;
	~Earth() ;
	void randomize() ;
	int get(int x, int y) ;
	int set(int x, int y, int state) ;
	int neighbors(int x, int y) ;
	void draw() ;
};

// Class to impliment the game.  Contains two generations, today and tomorrow and impliments the rules
class Life {
private:
	Earth *today;
	Earth *tomorrow;
public: 
	Life() ;
	~Life() ;
	void run() ;
private:
	void generate() ;
};	

#endif
