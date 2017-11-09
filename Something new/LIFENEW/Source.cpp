#include<iostream>
#include<fstream>
#include<string>
#include <stdio.h>
#include <wchar.h>
#include <windows.h>

#define ESC "\x1b["
using namespace std;
//ifstream
ifstream infile; //declaring ifstream input file;
bool donereading; 
//world
const int grid = 1024; //world length
bool world[grid][grid] = { false }; //matrix current world 
bool world_[grid][grid] = { false }; //matrix current world 1 gen further
static long getal = 29; 
string buffer;
//prototypes
void overwrite_world(); void single_gen(); void reset(); void fill_random(int a); 
int random(); int filter_num(int a);
//struct for position in matrix
struct position { 
	int i;//position in column
	int j;//position in row
};
//struct for cell
struct cell {
	position pos;//position in matrix
	int buren = 0;//number of live neighbours
	bool alive;//current state
	bool stays_alive;//state in next generation
};
bool drawn = false;
//class life
class LIFE {
	//rules of the game
	int max_buren = 3;
	int min_buren = 2;
public:	
	int gen_counter = 0;
	bool in_submenu = false; //to decide if in submenu
	bool game_over = false; //to decide if the user has exited the game
	int screen_w = 100; //screenwidth in characters
	int screen_h = 20; //screenheigth in characters
	position screen = { 100, 100 }; //screen starting position screen
	position cursor_ = { screen.i + 10, screen.j + 10 }; //starting position cursor
	char alive = '%'; //character to display for live cells
	char dead = ' '; //character to display for dead cells
	string infilename;//the file to load the world
	//member functions
	void clear(LIFE L);//clears the view
	void draw(LIFE L);//displays one frame
	void parameters(LIFE &L);//submenu 
	void menu(LIFE &L);//main menu
	void cell_initializer(int a, cell &c);//sets values for each cell in world
	void set_position(int a, cell &c);//sets the position of each cell
	void is_alive(cell &c);//sets alive-state of each cell
	void tel_buren(cell &c);//counts neighbours and sets the value for each cell
	void stays_alive(cell &c);//decides wether the cell stays alive
};
void display_menu(LIFE L) {
	for (int c = 0; c < 83; c++) { cout << '_'; } 
	cout << endl;
	if (L.in_submenu) {
		cout << "[s]creen dimmensions" << " w = " << L.screen_w << ", h =" << L.screen_h;
		cout << " | display [c]haracter " << "1= '" << L.alive << "' 0 = '" << L.dead << "'";
		cout << endl;
		for (int c = 0; c < 83; c++) { cout << '_'; }
		cout << endl;
		cout << "to go back press [t]" << endl;
	}
	else {
		cout << "screen position (" << L.screen.i << ',' << L.screen.j << ")";
		cout << " | alive = '" << L.alive << "' dead = '" << L.dead;
		cout << "' | cursor [+] position (" << L.cursor_.i << ',' << L.cursor_.j << ") gen [" << L.gen_counter;
		cout <<"]"<< endl;
		for (int c = 0; c < 83; c++) { cout << '_'; }
		cout << endl;
		cout << "   [W][A][S][D] to move the view  |  [I][J][K][L] to move the cursor \n" ;
		cout << "   [p]arameters] | [f]ill(random) | [t]oggle | single g[e]n | [g]enerations \n";
		cout << "   [c]lear | [r]eset | [r]eset | e[x]it \n";
		for (int c = 0; c < 83; c++) { cout << '_'; }
		cout << endl;
	}
}
void LIFE::cell_initializer(int a, cell &c) {
	set_position(a, c); is_alive(c); tel_buren(c); stays_alive(c);
}
void LIFE::set_position(int a, cell &c) {
	c.pos = { a / grid, a % grid };
}
void LIFE::is_alive(cell &c) {
	c.alive = world[c.pos.i][c.pos.j];
	if (c.alive){c.buren--;}
}
void LIFE::tel_buren(cell &c) {
	for (int a = 0; a < 9; a++) {
		position p = { a / 3,a % 3 };
		if (((c.pos.i == 0 && p.i == 0) || (c.pos.j == 0 && p.j == 0)) ||
		   ((c.pos.i == grid && p.i == 2) || (c.pos.j == grid && p.j == 2))) {	}
		else {
			position b = { c.pos.i + p.i - 1,c.pos.j + p.j - 1 };
			c.buren += world[b.i][b.j];
		}
	}
}
void LIFE::stays_alive(cell &c) {
	if ((c.buren == max_buren) || (c.buren == min_buren && c.alive)) {
		c.stays_alive = true;
	}
	else {
		c.stays_alive = false;
	}
}
void wasd(char c, LIFE &L) {
	switch (c) {
	case 'w':
		if (L.screen.i != 0) { L.screen.i--; }
		break;
	case 's':
		if (L.screen.i + L.screen_h != grid) { L.screen.i++; }
		break;
	case 'a':
		if (L.screen.j != 0) { L.screen.j--; }
		break;
	case 'd':
		if (L.screen.j + L.screen_w != grid) { L.screen.j++; }
	}
}
void cursor(char c, LIFE &L) {
	switch (c) {
	case 'i':
		if (L.cursor_.i != 0) { L.cursor_.i--; }
		break;
	case 'k':
		if (L.cursor_.i != grid) { L.cursor_.i++; }
		break;
	case 'j':
		if (L.cursor_.j != 0) { L.cursor_.j--; }
		break;
	case 'l':
		if (L.cursor_.j != grid) { L.cursor_.j++; }
		break;
	case 't':
		if (world[L.cursor_.i][L.cursor_.j]) {
			world[L.cursor_.i][L.cursor_.j] = false;
		}
		else world[L.cursor_.i][L.cursor_.j] = true;
	}
}
void fetch_percentage() {
	cout << "Approximately what percentage of cells do you want to be alive" << endl;
	char input = cin.get();
	int a = filter_num(3);
	if (a > 100) { a = 100; }
	fill_random(a);
}
void many_gen(LIFE &L) {
	cout << "how many generations do you want to run?" << endl;
	char input = cin.get();
	int z = filter_num(4);
	for (L.gen_counter; L.gen_counter < z; L.gen_counter++) { single_gen(); L.draw(L); };
	L.gen_counter = 0;
}
void open_file(LIFE &L) {
	cout << "do you wish to load the world from a file?(y/n)" << endl;
	char ans;
	cin >> ans;
	switch (ans) {
	case 'y':
		cout << "what file do you want to open" << endl;
		cin >> L.infilename;
		infile.open(L.infilename);
		return;
	case 'n':
		return;
	default:
		open_file(L);
	}
}
char readfile() {
	char charin = infile.get();
	if (!donereading) {
		if (infile.eof()) {
			donereading = true;
			infile.close();
			return 'e';
		}
		else {
			return charin;
		}
	}
	return 'e';
}
void fill_world(LIFE L) {
	for (int i = 0; i < grid; i++) {
		for (int j = 0; j < grid; j++) {
			char infilechar = readfile();
			if (infilechar != 'e') {
				if (infilechar == ' ') {
					world[i + L.screen.i][j + L.screen.j] = false;
				}
				else if (infilechar == '\n') {
					world[i + L.screen.i][j + L.screen.j] = false;
					i++; j = -1;
				}
				else {
					world[i + L.screen.i][j + L.screen.j] = true;
				}
			}
			else return;
		}
	}
}
void LIFE::menu(LIFE &L) {
	char input;
	input = cin.get();
	if (input == 'w' || input == 's' || input =='a'|| input==  'd') {
		wasd(input, L);
	}
	else if (input == 'i' || input == 'j' || input == 'k' || input == 'l' || input == 't') {
		cursor(input, L);
	}
	else {
		switch (input) {
		case 'e':
			single_gen();
			break;
		case 'o':
			open_file(L);
			fill_world(L);
			break;
		case 'f':
			fetch_percentage();
			break;
		case 'g':
			many_gen(L);
			break;
		case 'c':
			clear(L);
			break;
		case 'p':
			L.in_submenu = true;
			parameters(L);
			break;
		case 'r':
			reset();
			break;
		case 'x':
			L.game_over = true;
		}
	}
	draw(L);
}
int filter_num(int a) {
	char input = cin.get();
	int number_length = 0;
	int to_return = 0;
	if (input == '\n') { filter_num(a); }
	while (input != '\n') {
		if (number_length < a) {
			if (isdigit(input)) {
				number_length++;
				to_return = to_return * 10 + (input - '0');
			}
		}
		input = cin.get();
	}
	return int(to_return);
}
char get_char() {
	char kar = cin.get();
	char to_return = kar;
	if (kar == '\n') { get_char(); }
	while(kar != '\n'){
		kar = cin.get();
	} return to_return;
}
void LIFE::parameters(LIFE &L) {
	draw(L);
	char input = cin.get();
	switch (input) {
	case 's':
	{
		cout << "input the desired heigth of the display" << endl;
		char a = cin.get();
		L.screen_h = filter_num(3);
		cout << "input the desired width of the display" << endl;
		char b = cin.get();
		L.screen_w = filter_num(3);
//		system("cls");
		break;
	}
	case 'c':
	{
		char kar = cin.get();
		cout << "change state 1 char to:" << endl;
		L.alive = get_char();
		cout << "change state 0 char to:" << endl;
		L.dead = get_char();
		break;
	}
	case 't':
		L.in_submenu = false;
		return;
	default:
		parameters(L);
	} L.in_submenu = false;
}
void LIFE::draw(LIFE L) {
//	system("cls");
	printf(ESC "H");
	int c = 0;
	for (int x = 0; x < L.screen_h; x++) {
		for (int y = 0; y < L.screen_w; y++) {
			if (x == cursor_.i - L.screen.i && y == cursor_.j - L.screen.j) { cout << '+'; }
			else if (world[x + L.screen.i][y + L.screen.j]) { cout << L.alive; }
			else cout << L.dead;
		} cout << endl;
	}
	printf(ESC "J");
	display_menu(L);
}
void single_gen() {
	LIFE G;
	for (int g = 0; g < grid * grid; g++) {
		cell cell;
		G.cell_initializer(g, cell);
		world[cell.pos.i][cell.pos.j] = cell.alive;
		world_[cell.pos.i][cell.pos.j] = cell.stays_alive;
	} overwrite_world();
}
void overwrite_world() {
	for (int c = 0; c < grid*grid; c++) {
		position p = { c / grid,c % grid };
		world[p.i][p.j] = world_[p.i][p.j];
	}
}
void LIFE::clear(LIFE L) {
	for (int a = 0; a < screen_w * screen_h + 1; a++) {
		for (int i = 0; i < screen_h + 1; i++) {
			for (int j = 0; j < screen_w + 1; j++) {
				world[i + L.screen.i][j + L.screen.j] = false;
			}
		}
	}
}
void reset() {
	for (int c = 0; c < grid*grid; c++) {
		position p = { c / grid,c % grid };
		world[p.i][p.j] = false;
	}
}
int random() {
	getal = (19051 * getal + 1);
	return abs(getal);
}
void fill_random(int a) {
	for (int c = 0; c < grid*grid; c++) {
		position p = { c / grid, c % grid };
		if (a > random() % 100) {
			world[p.i][p.j] = true;
		}
		else { world[p.i][p.j] = false; }
	}
}
int vt()
{
	// Set output mode to handle virtual terminal sequences
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hOut == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
	if (hIn == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	DWORD dwOriginalOutMode = 0;
	DWORD dwOriginalInMode = 0;
	if (!GetConsoleMode(hOut, &dwOriginalOutMode))
	{
		return false;
	}
	if (!GetConsoleMode(hIn, &dwOriginalInMode))
	{
		return false;
	}

	DWORD dwRequestedOutModes = ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN;
	DWORD dwRequestedInModes = ENABLE_VIRTUAL_TERMINAL_INPUT;

	DWORD dwOutMode = dwOriginalOutMode | dwRequestedOutModes;
	if (!SetConsoleMode(hOut, dwOutMode))
	{
		// we failed to set both modes, try to step down mode gracefully.
		dwRequestedOutModes = ENABLE_VIRTUAL_TERMINAL_PROCESSING;
		dwOutMode = dwOriginalOutMode | dwRequestedOutModes;
		if (!SetConsoleMode(hOut, dwOutMode))
		{
			// Failed to set any VT mode, can't do anything here.
			return -1;
		}
	}

	DWORD dwInMode = dwOriginalInMode | ENABLE_VIRTUAL_TERMINAL_INPUT;
	if (!SetConsoleMode(hIn, dwInMode))
	{
		// Failed to set VT input mode, can't do anything here.
		return -1;
	}

	return 0;
}
int main() {
	vt();
	printf(ESC "25l");
	LIFE M;
	open_file(M);
	fill_world(M);
	M.draw(M);
	drawn = true;
	while (!M.game_over) {
		M.menu(M);
	}
}