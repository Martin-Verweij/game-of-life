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
	void cell_initializer(int a, cell &c);
	void set_position(int a, cell &c);
	void is_alive(cell &c);
	void tel_buren(cell &c);
	void stays_alive(cell &c);
};
void LIFE::set_position(int a, cell &c) {
	c.pos.i = a / grid;
	c.pos.j = a % grid;
}
void LIFE::is_alive(cell &c) {
	c.alive = world[c.pos.i][c.pos.j];
}
void LIFE::cell_initializer(int a, cell &c) {
	set_position(a, c); is_alive(c); tel_buren(c); stays_alive(c);
}
void LIFE::tel_buren(cell &c) {
	for (int i = -1; i < 2; i++) {
		for (int j = -1; j < 2; j++) {
			cell buur;
			buur.pos = { c.pos.i+i, c.pos.j+j }; is_alive(buur);
			if (buur.pos.i < 0) { i++; }
			else if (buur.pos.j < 0) { j++; }
			else if (buur.alive) {
				c.buren++;
			}
		}
	} if (c.alive) { c.buren--; }
}
void LIFE::stays_alive(cell &c) {
	if (c.buren == max_buren) {
		c.stays_alive = true;
	}
	else if (c.buren == min_buren && c.alive) {
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
char displaychar(bool b) {
	char c;
	if (b) {
		c = '@';
	}
	else {
		c = '_';
	}
	return c;
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
void draw() {
	cout << string(100, '\n');
	static char buffer[25 * 100];
	char * p_next_write = &buffer[0];
	int c = 0;
	for (int x = 0; x < 25; x++) {
		for (int y = 0; y < 100; y++) {
			*p_next_write++ = displaychar(world[x][y]);
		} 
		*p_next_write++ = '\n';
	}cout.write(&buffer[0], 25*100);
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
int main() {
	int test;
	LIFE M;
	infile.open("infile.txt");
	fill_world();
	cin >> test;
	for (int i = 0; i < test; i++) {
		
		single_gen();draw();
	}
	cin >> test;
	return 0;
}