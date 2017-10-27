#include <iostream>
#include <ctype.h>
#include <string>
#include <fstream>

using namespace std;

const int w = 10;
const int h = 10;
bool gameover = false;
string menutext = "kies een optie uit: C, R, P, S";
char alive = '@';
char dead = ' ';
bool screenchange = true;

class LIFE {
public:
};

int matrix[w][h]{
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,1,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
};

////menu prototype
void menu();
////render-function/////
void draw() {
	for (int i = 0; i < w; i++) {
		for (int j = 0; j < h; j++) {
			if (matrix[i][j] == 1) {
				cout << alive;
			}
			else {
				cout << dead;
			}
		} cout << endl;
	}cout << menutext << endl;
	screenchange = false;
}
void clearscreen() {

}
////submenu-function////
void submenu() {
	char input = cin.get();
	switch (input) {
	case 't':
	case 'T':
		menu();
	}
}
////clear function////
void clear() {
	cout << 'text' << endl;
}
////parameters function////
void parameters() {
	menutext = "dit is een submenu";
	screenchange = true;
	int m = 0;
	while (m != 1) {
		submenu();
	}

}
////rename function////
void rename() {
	cout << "text" << endl;
}
////main menu////
void menu() {
	char input;
	menutext = "kies een optie uit: C, R, P, S";
	input = cin.get();
	do {
		switch (input) {
		case 'c':
		case 'C':
			clear();
			break;
		case 'p':
		case 'P':
			parameters();
			break;
		case 'r':
		case 'R':
			rename();
			break;
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

int main() {
	do {
		if (screenchange == true) {
			draw();
		}
		menu();
	} while (!gameover);
	return 0;
}