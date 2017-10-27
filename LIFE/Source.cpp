#include <iostream>
#include <ctype.h>
#include <string>
#include <fstream>

using namespace std;

const int w = 30;
const int h = 120;
bool gameover = false;
string menutext = "kies een optie uit: C, R, P, S";

void menu();
void draw() {
	for (int i = 0; i < w; i++) {
		for (int j = 0; j < h; j++) {
			cout << "X";
		} cout << endl;
	}cout << menutext << endl;
}
void submenu() {
	char input = cin.get();
	switch (input) {
	case 't':
	case 'T':
		menu();
	}
}
void clear() {
	cout << 'text' << endl;
}
void parameters() {
	menutext = "dit is een submenu";
	int m = 0;
	while (m != 1) {
		submenu();
	}
}
void rename() {
	cout << "text" << endl;
}
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
	return;
}

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
	int option;
	while (!gameover) { draw(); }
	menu();
	cin >> option;
	return 0;
}