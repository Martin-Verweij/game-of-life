
#include <iostream>
#include <string>
#include <fstream>


using namespace std;
//For a space that is 'populated':
//Each cell with one or no neighbors dies, as if by solitude.
//Each cell with four or more neighbors dies, as if by overpopulation.
//Each cell with two or three neighbors survives.
//For a space that is 'empty' or 'unpopulated'
//Each cell with three neighbors becomes populated.

//fstream
ifstream infile;
bool donereading = false;
char charin;
int test;

//Define gridSize
const int gridSize = 100;
bool world[gridSize][gridSize] = { false };

//Define a cell
char cell;
char alive;

//file reader
char read() {
	charin = infile.get();
	if (!donereading) {
		if (infile.eof()) {
			infile.close();
			donereading = true;
			return '0';
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
				char infilechar = read();
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
				//				cout << thijs[i][j];
			}
		}
	}
}

char displaychar(bool alive) {
	if (alive) {
		cell = 'X';
	}
	else {
		cell = '.';
	}
	return cell;
}

//Create the display in console
int main() {
	infile.open("infile.txt");
	int test;
	fillrandom();
	for (int x = 0; x < gridSize; x++) {
		for (int y = 0; y < gridSize; y++) {
			if (alive) {
			}
			cout << displaychar(world[x][y]);
		}
		cout << endl;
	}
	cin >> test;

	return 0;
}