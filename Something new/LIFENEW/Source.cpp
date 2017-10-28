
#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>


using namespace std;
//For a space that is 'populated':
//Each cell with one or no neighbors dies, as if by solitude.
//Each cell with four or more neighbors dies, as if by overpopulation.
//Each cell with two or three neighbors survives.
//For a space that is 'empty' or 'unpopulated'
//Each cell with three neighbors becomes populated.

//menuvariables
string menutext = "kies een optie uit: C, R, P, S";
bool gameover = false;
bool insubmenu = false;

//fstream
ifstream infile;
bool donereading = false;
char charin;
int test;
bool screenchange = true;

//coordinate NOT SURE HOW THIS WORKS 
struct coordinates {
	int i; int j;
};
//Define gridSize and screensize
const int w = 25;
const int h = 100;
const int gridSize = 1000;

//Define a cell and world variables
char cell;
char alive;
bool world[gridSize][gridSize] = { false };
bool worldnew[gridSize][gridSize] = { false };
////prototypes
void menu();
void submenu();
void draw();
void fillrandom();
void matrix();
int alivecounter(int i, int j);

//matrixnew filler THIS IS SUPPOSED TO CREATE THE NEW MATRIX BASED ON HOW MANY NEIGHBOURS 
void matrixfiller() {
	for (int i = 0; i < gridSize + 1; i++) {
		for (int j = 0; j < gridSize + 1; j++) {
			bool alive2;
			if (alivecounter(i, j) == 3) {
				alive2 = true;
			}
			else if ((alivecounter(i, j) == 2) && (world[i][j])) {
				alive2 = true;
			}
			else {
				alive2 = false;
			}
			worldnew[i][j] = alive2;
		}
	}
	matrix();
}
//replace frames
void matrix() {
	for (int i = 0; i < gridSize + 1; i++) {
		for (int j = 0; j < gridSize + 1; j++) {
			world[i][j] = worldnew[i][j];
		}
	}
	worldnew[gridSize][gridSize] = { false };
}

////current menu////
void currentmenu() {
	draw();
	if (!insubmenu) {
		menu();
	}
	else {
		submenu();
	}
}
////submenu-function////
void submenu() {
	insubmenu = true;
	char input = cin.get();
	do {
		switch (input) {
		case 'b':
			fillrandom();
		case 't':
		case 'T':
			insubmenu = false;
			return;
		}
		input = cin.get();
	} while (input != 't' || input != 'T');
}
////parameters function////
void parameters() {
	menutext = "dit is een submenu optie 't' om terug te gaan";
	draw();
	submenu();
}
////clear function////
void clear() {
	cout << 'text' << endl;
}
////rename function////
void rename() {
	cout << "text" << endl;
}

////main menu////
void menu() {
	menutext = "kies een optie uit: C, R, P, S";
	draw();
	char input = cin.get();
	do {
		switch (input) {
		case 'c':
		case 'C':
			clear();
			return;
		case 'p':
		case 'P':
			parameters();
			return;
		case 'r':
		case 'R':
			rename();
			return;
		case 's':
		case 'S':
			gameover = true;
			return;

		}
		input = cin.get();
	} while ((input != 's') || (input != 'S'));
}
////number parser////
int filternum() {
	char input = cin.get();
	int num = 0;
	int numbercounter = 0;
	while (numbercounter < 3) {
		if (isdigit(input)) {
			num << input;
			numbercounter++;
		}
	}
	return num;
}
//file reader
char readfile() {
	charin = infile.get();
	if (!donereading) {
		if (infile.eof()) {
			infile.close();
			donereading = true;
			return 'e';
		}
		else {
			return charin;
		}
	}
	return 'e';
}

//fill random
void fillrandom() {
	for (int c = 0; c < (gridSize * gridSize) + 1; c++) {
		for (int i = 0; i < gridSize; i++) {
			for (int j = 0; j < gridSize; j++) {
				char infilechar = readfile();
				if (infilechar != 'e') {
					if (infilechar == ' ') {
						alive = false;
					}
					else if (infilechar == '\n') {
						i++; j = -1;
						alive = false;
					}
					else {
						alive = true;
					}
					world[i][j] = alive;
				}
				else {
					return;
				}
			}
		}
	}
}

char displaychar(bool alive) {
	if (alive) {
		cell = 'X';
	}
	else {
		cell = ' ';
	}
	return cell;
}

//Create the display in console 
void draw() {
	for (int x = 0; x < w; x++) {
		for (int y = 0; y < h; y++) {
			cout << displaychar(world[x][y]); //IF YOU WANT TO SEE THE AMOUNT OF NEIGHBOURS CHANGE THE COUT TO alivecounter(x,y)
		} cout << endl;
	}cout << menutext << endl;
	screenchange = false;
}
//counts live neighbours
int alivecounter(int i, int j) {
	int neighbour_counter = 0;
	bool neighbour_state;
	for (int x = -1; x < 2; x++) {
		for (int y = -1; y < 2; y++) {
			if (i + x < 0 || j + y < 0 || i + x > gridSize || j + y > gridSize) {
				neighbour_state = false;
			}
			else {
				neighbour_state = world[i + x][j + y];
			}
			if (neighbour_state) {
				neighbour_counter++;
			}
		}
	}
	if (world[i][j]) {
		neighbour_counter--;
	}
	return neighbour_counter;
}
//main
int main() {
	infile.open("infile.txt");
	int test;
	fillrandom();
	draw();
	for (int a = 0; a < 100; a++) {
		matrixfiller();
		draw();
	}
	cin >> test;

	return 0;
}