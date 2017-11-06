#include<iostream>
#include<fstream>
#include<string>

using namespace std;
//ifstream
ifstream infile;
bool donereading;
//world
const int grid = 1000;
bool world[grid][grid] = { false };
bool world_[grid][grid] = { false };

//prototypes
void overwrite_world();

struct position {
	int i;
	int j;
};
struct cell {
	position pos;
	bool alive;
	bool stays_alive;
	int buren = 0;
};
class LIFE {
	int screen_w = 100;
	int screen_h = 25;
	int max_buren = 3;
	int min_buren = 2;
public:
	int screen_w_0 = 0;
	int screen_h_0 = 0; 
	position cursor_ = { 5,5 };
	char alive = '@';
	char dead = ' ';
	bool game_over = false;
	string menu_text = "kies een optie uit: C, R, P, S of G voor generaties";
	
	void clear();
	void draw(LIFE &L);
	void parameters(LIFE &L);
	void menu(LIFE &L);
	void cell_initializer(int a, cell &c);
	void set_position(int a, cell &c);
	void is_alive(cell &c);
	void tel_buren(cell &c);
	void stays_alive(cell &c);
};
void LIFE::cell_initializer(int a, cell &c) {
	set_position(a, c); is_alive(c); tel_buren(c); stays_alive(c);
}
void LIFE::set_position(int a, cell &c) {
	c.pos.i = a / grid;
	c.pos.j = a % grid;
}
void LIFE::is_alive(cell &c) {
	c.alive = world[c.pos.i][c.pos.j];
//	if (c.alive) { c.buren--; }
}
void LIFE::tel_buren(cell &c) {
	c.buren += world[c.pos.i + 1][c.pos.j + 1];
	c.buren += world[c.pos.i + 1][c.pos.j];
	c.buren += world[c.pos.i][c.pos.j + 1];
	c.buren += world[c.pos.i - 1][c.pos.j + 1];
	c.buren += world[c.pos.i + 1][c.pos.j - 1];
	c.buren += world[c.pos.i][c.pos.j - 1];
	c.buren += world[c.pos.i - 1][c.pos.j];
	c.buren += world[c.pos.i - 1][c.pos.j - 1];
}
void LIFE::stays_alive(cell &c) {
	if ((c.buren == max_buren) || (c.buren == min_buren && c.alive)) {
		c.stays_alive = true;
	}
	else {
		c.stays_alive = false;
	}
}
//file reader
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
void fill_world() {
	for (int c = 0; c < (grid * grid) + 1; c++) {
		for (int i = 0; i < grid; i++) {
			for (int j = 0; j < grid; j++) {
				char infilechar = readfile();
				if (infilechar != 'e') {
					if (infilechar == ' ') {
						world[i][j] = false;
					}
					else if (infilechar == '\n') {
						world[i][j] = false;
						i++; j = -1;
					}
					else {
						world[i][j] = true;
					}
				}
				else return;
			}
		}
	}
}
void LIFE::draw(LIFE &L) {
	cout << string(50, '\n');
//	static char buffer[25 * 100];
//	char * p_next_write = &buffer[0];
	int c = 0;
	for (int x = 0; x < 25; x++) {
		for (int y = 0; y < 100; y++) {
//			*p_next_write++ = displaychar(world[x][y]);
			if (x == cursor_.i && y == cursor_.j) { cout << '+'; }
			else if (world[x+L.screen_h_0][y+L.screen_w_0]) { cout << L.alive; }
			else cout << L.dead;
		} cout << '\n';
//		*p_next_write++ = '\n';
	}//cout.write(&buffer[0], 25*100);
	cout << L.menu_text;
}
void single_gen() {
	LIFE G;
	for (int g = 0; g < grid * grid; g++) {
		cell cell;
		G.cell_initializer(g, cell);
		world[cell.pos.i][cell.pos.j] = cell.alive;
		world_[cell.pos.i][cell.pos.j] = cell.stays_alive;
	}
	overwrite_world();
}
void overwrite_world() {
	for (int c = 0; c < grid*grid; c++) {
		position p = { c / grid,c % grid };
		world[p.i][p.j] = world_[p.i][p.j];
	}
}
void LIFE::parameters(LIFE &L) {
	menu_text = "Druk op l om de cell ascii waarde te veranderen";
	draw(L);
	char input = cin.get();
	switch (input){
		case 'l':
			cout << "change state 1 char to:" << endl;
			cin >> L.alive;
			cout << "change state 0 char to:" << endl;
			cin >> L.dead;
		case 't':
			return;
		default:
			parameters(L);
	}
}
int filter_num(int a) {
	char input = cin.get();
	int number_length = 0;
	int to_return = 0;
	while (number_length < a) {
		if (isdigit(input)) {
			number_length++;
			to_return << input;
		}
		input = cin.get();
	} return a;
}
void LIFE::clear() {
	for (int a = 0; a < screen_w * screen_h + 1; a++) {
		for (int i = 0; i < screen_h + 1; i++) {
			for (int j = 0; j < screen_w + 1; j++) {
				world[i][j] = false;
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
void LIFE::menu(LIFE &L) {
	char input;
	L.menu_text = "kies een optie uit: C, R, P, S of G voor generaties";
	input = cin.get();
	switch (input) {
		case '2':
			L.screen_h_0--;
			break;
		case 'w':
			L.screen_h_0++;
			break;
		case 'q':
			L.screen_w_0++;
			break;
		case 'e':
			L.screen_w_0--;
			break;
		case 'i':
			L.cursor_.i--;
			break;
		case 'k':
			L.cursor_.i++;
			break;
		case 'j': 
			L.cursor_.j--;
			break;
		case 'l':
			L.cursor_.j++;
			break;
		case 'u':
			world[L.cursor_.i][L.cursor_.j] = true;
			break;
		case 'g':
			for (int i = 0; i < 80; i++) { single_gen(); draw(L); }
		case 'c':
			clear();
			break;
		case 'p':
			parameters(L);
			break;
		case 'r':
			reset();
			break;
		case 's':
			L.game_over = true;
	} draw(L);
}
int main() {
	LIFE M;
	infile.open("infile.txt");
	fill_world();
	M.draw(M);
	cell c;
	while (!M.game_over) {
		M.menu(M);
	}
	cin.get();
}