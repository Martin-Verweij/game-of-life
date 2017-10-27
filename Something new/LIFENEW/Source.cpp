
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



//Define gridSize
const int gridSize = 50;

//Define a cell
char cell;
char alive;
bool thijs[gridSize][gridSize];
//file reader
bool read() {
	infile.open("infile.txt");
	char charin = infile.get();
	cout << charin;
	if (!infile.eof()) {
		if (charin = ' ') {
			alive = false;
		}
		else {
			alive = true;
		}
		charin = infile.get();
	}
	return alive;
}


//fill random
void fillrandom() {
	for (int c = 0; c < (gridSize * gridSize) + 1; c++) {
		for (int i = 0; i < gridSize; i++) {
			for (int j = 0; j < gridSize; j++) {
				thijs[i][j] = read();
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
int main() {
	int test;
	fillrandom();
	for (int x = 0; x < gridSize; x++) {
		for (int y = 0; y < gridSize; y++) {
			if (alive) {

			}
			cout << displaychar(thijs[x][y]);

		}
		cout << endl;
	}
	cin >> test;

	return 0;
}