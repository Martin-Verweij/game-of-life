#include<iostream>
#include<fstream>
#include<string>

////THIS PROGRAM WAS CREATED AND COMPILED IN MICROSOFT VISUAL STUDIO ////

using namespace std;
//ifstream
ifstream infile;                         //declaring ifstream input file;
bool donereading;
//world
const int grid = 1024;                                     //world length
bool world[grid][grid] = { false };                //matrix current world 
bool world_[grid][grid] = { false }; //matrix current world 1 gen further
static long getal = 29;
string buffer;
//prototypes
void overwrite_world(); void single_gen();
void reset(); void fill_random(int a);
int random(); int filter_num(int a);
//struct for position in matrix
struct position {
	int i;                                           //position in column
	int j;                                              //position in row
};
//struct for cell
struct cell {
	position pos;                                    //position in matrix
	int buren = 0;                            //number of live neighbours
	bool alive;                                           //current state
	bool stays_alive;                          //state in next generation
};
//class life
class LIFE {
	//rules of the game
	int max_buren = 3;
	int min_buren = 2;
public:
	int gen_counter = 0;
	bool in_submenu = false;                    //to decide if in submenu
	bool game_over = false;   //to decide if the user has exited the game
	int screen_w = 100;                       //screenwidth in characters
	int screen_h = 20;                       //screenheigth in characters
	position screen = { 300, 300 };     //screen starting position screen
	position cursor_ = { screen.i + 10, screen.j + 10 };//position cursor
	char alive = '%';               //character to display for live cells
	char dead = ' ';                //character to display for dead cells
	string infilename;                       //the file to load the world
											 //member functions
	void clear(LIFE L);                                 //clears the view
	void draw(LIFE L);                               //displays one frame
	void parameters(LIFE &L);                        //parameters submenu 
	void menu(LIFE &L);                                       //main menu
	void cell_initializer(int a, cell &c);    //sets values for each cell
	void set_position(int a, cell &c);   //sets the position of each cell
	void is_alive(cell &c);               //sets alive-state of each cell
	void tel_buren(cell &c);  //counts sets neighbour value for each cell
	void stays_alive(cell &c);      //decides wether the cell stays alive
};
//this functions decides which menu to display and then displays it
void display_menu(LIFE L) {
	for (int c = 0; c < 83; c++) { cout << '_'; }
	cout << endl;
	if (L.in_submenu) {
		cout << "[s]creen dimmensions w = " << L.screen_w << ", h =";
		cout << L.screen_h << " | display [c]haracter 1= '" << L.alive;
		cout << "' 0 = '" << L.dead << "'" << endl;
		for (int c = 0; c < 83; c++) { cout << '_'; }
		cout << endl;
		cout << "to go back press [t]" << endl;
	}
	else {
		cout << "screen position (" << L.screen.i << ',' << L.screen.j << ")";
		cout << " | alive = '" << L.alive << "' dead = '" << L.dead;
		cout << "' | cursor [+] position (" << L.cursor_.i << ',';
		cout << L.cursor_.j << ") gen [" << L.gen_counter << "]" << endl;
		for (int c = 0; c < 83; c++) { cout << '_'; }
		cout << endl;
		cout << "  [W][A][S][D] to move the view  | ";
		cout << "[I][J][K][L] to move the cursor \n";
		cout << "  [p]arameters] | [f]ill(random) | [t]oggle";
		cout << "| single g[e]n | [g]enerations \n";
		cout << "   [c]lear | [r]eset | [r]eset | e[x]it \n";
		for (int c = 0; c < 83; c++) { cout << '_'; }
		cout << endl;
	}
}
//initializes all the values for a cell
void LIFE::cell_initializer(int a, cell &c) {
	set_position(a, c); is_alive(c); tel_buren(c); stays_alive(c);
}
//sets the position of a cell corresponding to a certain int
void LIFE::set_position(int a, cell &c) {
	c.pos = { a / grid, a % grid };
}
//sets the boolean state of a cell(alive or dead)
void LIFE::is_alive(cell &c) {
	c.alive = world[c.pos.i][c.pos.j];
	c.buren -= world[c.pos.i][c.pos.j]; //corrects number of neighbours
}
//counts the live neighbours around each cell
void LIFE::tel_buren(cell &c) {
	for (int a = 0; a < 9; a++) {  //creates coordinate values for each
		position p = { a / 3,a % 3 };                       //neighbour             
															//prevents memory outside the matrix from being accessed
		if (((c.pos.i == 0 && p.i == 0) || (c.pos.j == 0 && p.j == 0)) ||
			((c.pos.i == grid && p.i == 2) || (c.pos.j == grid && p.j == 2))) {
		}
		else {
			//makes the cell the center of the 3*3 square
			position b = { c.pos.i + p.i - 1,c.pos.j + p.j - 1 };
			c.buren += world[b.i][b.j];     //counts live cells in 3*3 
		}
	}
}
void LIFE::stays_alive(cell &c) {
	if ((c.buren == max_buren) || (c.buren == min_buren && c.alive)) {
		c.stays_alive = true;         //if 3 neigbours cell stays alive
									  //if 2 neighbours and cell alive also stays alive
	}
	else { //otherwise cell dies
		c.stays_alive = false;
	}
}
//movement function for the view
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
//movement and toggling of the cursor
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
	case 't':                //toggles the world at cursor location
		if (world[L.cursor_.i][L.cursor_.j]) {
			world[L.cursor_.i][L.cursor_.j] = false;
		}
		else world[L.cursor_.i][L.cursor_.j] = true;
	}
}

void fetch_percentage() {
	cout << "What percentage of cells do you want to be alive" << endl;
	char input = cin.get();
	int a = filter_num(3);
	if (a > 100) { a = 100; }
	fill_random(a);
}
//menu function to run a series of generations
void many_gen(LIFE &L) {
	cout << "how many generations do you want to run?" << endl;
	char input = cin.get();
	int z = filter_num(4);
	for (L.gen_counter; L.gen_counter < z; L.gen_counter++) {
		single_gen(); L.draw(L);
	}
	L.gen_counter = 0;
}
//asks the user which file to open and opens it
void open_file(LIFE &L) {
	cout << "do you wish to load the world from a file?(y/n)" << endl;
	char ans;
	cin >> ans;
	switch (ans) {
	case 'y':
		cout << "what file do you want to open" << endl;
		cin >> L.infilename;
		infile.open(L.infilename);
		if (infile.fail()) { open_file(L); }
		return;
	case 'n':
		return;
	default:
		open_file(L);
	}
}
//returns one character from the input file at a time
char readfile() {
	char charin = infile.get();
	if (!donereading) {
		if (infile.eof()) {
			donereading = true;
			infile.close();
			return 'e';
			//to communicate that the file has been read through 
		}
		else {
			return charin;
		}
	}
	return 'e';
}
//fills the world for the intitial values from the input file
void fill_world(LIFE L) {
	for (int i = 0; i < grid; i++) {
		for (int j = 0; j < grid; j++) {
			char infilechar = readfile();
			//runs if the file is still open
			if (infilechar != 'e') {
				if (infilechar == ' ') {     //creates dead cells
					world[i + L.screen.i][j + L.screen.j] = false;
				}
				else if (infilechar == '\n') { //goes to next row
					world[i + L.screen.i][j + L.screen.j] = false;
					i++; j = -1;
				}
				else {        //characters that create live cells
					world[i + L.screen.i][j + L.screen.j] = true;
				}
			}
			else return;           //when done reading input file
		}
	}
}
//main menu function
void LIFE::menu(LIFE &L) {
	char input = cin.get();                    //first input char
	if (input == 'w' || input == 's'
		|| input == 'a' || input == 'd') {
		wasd(input, L);     //calls in the view movement function
	}
	else if (input == 'i' || input == 'j' || input == 'k'
		|| input == 'l' || input == 't') {
		cursor(input, L);          //calls in the cursor function
	}
	else {
		switch (input) {     //decides what to do with user input
		case 'e':                       //runs one generation
			single_gen();
			break;
		case 'f':   //fills the world randomly (see function)
			fetch_percentage();
			break;
		case 'g':      //runs many generations (see function)
			many_gen(L);
			break;
		case 'c':       //clears view (more info at function)
			clear(L);
			break;
		case 'p':              //opens the parameters submenu
			L.in_submenu = true;    //to display correct menu
			parameters(L);
			break;
		case 'r':                   //clears the entire world
			reset();
			break;
		case 'x':               //exits the menu and the game
			L.game_over = true;
		}
	}
	draw(L);
}
//this function returns an int from the (mixed) input
int filter_num(int a) {            //int a specifies number length
	char input = cin.get();
	int number_length = 0;
	int to_return = 0;                            //to create int
	if (input == '\n') { filter_num(a); } //ignores empty [enters]
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
//gets first character from user input
char get_char() {
	char kar = cin.get();
	char to_return = kar;               //to return the first char
	if (kar == '\n') { get_char(); }        //ignores empty enters
	while (kar != '\n') {
		kar = cin.get();
	} return to_return;
}
// parameters submenu allows user to change some values
void LIFE::parameters(LIFE &L) {
	draw(L);                        //draws new frame with submenu
	char input = cin.get();
	switch (input) {
	case 's':
	{                //allows user to change dimensions of display
		cout << "input the desired heigth of the display" << endl;
		char a = cin.get();
		L.screen_h = filter_num(3);
		cout << "input the desired width of the display" << endl;
		L.screen_w = filter_num(3);
		break;
	}
	case 'c':
	{    //changes the display characters for alive and dead cells
		char kar = cin.get();
		cout << "change state 1 char to:" << endl;
		L.alive = get_char();
		cout << "change state 0 char to:" << endl;
		L.dead = get_char();
		break;
	}
	case 't':
		// returns to main menu
		L.in_submenu = false;
		return;
	default:                  //if invalid input stays in submenu
		parameters(L);
	} L.in_submenu = false;
}
//this function displays the world 
void LIFE::draw(LIFE L) {
	system("cls"); //clears the screen
	for (int x = 0; x < L.screen_h; x++) {
		for (int y = 0; y < L.screen_w; y++) {
			//displays border
			if ((L.screen.i == x && x == 0) ||
				(L.screen.j == y && y == 0)) {
				cout << '#';
			}
			else if ((L.screen.i == grid - L.screen_h
				&& x == L.screen_h - 1) ||
				(L.screen.j == grid - L.screen_w&&y == L.screen_w - 1)) {
				cout << '#';
			}
			//displays cursor
			else if (x == cursor_.i - L.screen.i &&
				y == cursor_.j - L.screen.j) {
				cout << '+';
			}
			//displays live cells
			else if (world[x + L.screen.i][y + L.screen.j]) {
				cout << L.alive;
			}
			//displays dead cells
			else cout << L.dead;
		} cout << endl;
	}
	//calls to display current menu
	display_menu(L);
}
//runs a single generation of life 
void single_gen() {
	LIFE G;
	//for loop that goes through every cell in matrix 
	for (int g = 0; g < grid * grid; g++) {
		cell cell;
		G.cell_initializer(g, cell);    //gives values to each cell
		world[cell.pos.i][cell.pos.j] = cell.alive;
		world_[cell.pos.i][cell.pos.j] = cell.stays_alive;
	} overwrite_world();    //calls function to overwrite the world
}
//overwrites the current world with values from the next generation
void overwrite_world() {
	for (int c = 0; c < grid*grid; c++) {
		position p = { c / grid,c % grid };
		world[p.i][p.j] = world_[p.i][p.j];
	}
}
//clears all cells in the view
void LIFE::clear(LIFE L) {
	for (int i = 0; i < screen_h; i++) {
		for (int j = 0; j < screen_w; j++) {
			world[i + L.screen.i][j + L.screen.j] = false;
		}
	}
}
// clears entire world
void reset() {
	for (int c = 0; c < grid*grid; c++) {
		position p = { c / grid,c % grid };
		world[p.i][p.j] = false;
	}
}
//generates random number
int random() {
	//possible overflow but it is not detrimental
	getal = (19051 * getal + 1);
	return abs(getal);                          //corrects overflow
}
//fills the world with a user defined percentage of live cells
void fill_random(int a) {
	for (int c = 0; c < grid*grid; c++) {
		position p = { c / grid, c % grid };
		if (a >= random() % 101) {        //sets %(a) live cells
			world[p.i][p.j] = true;
		}
		else { world[p.i][p.j] = false; }
	}
}
void infoblokje() {
	cout << "Dit programma is gemaakt door Martin Verweij ";
	cout << " (9 november 2017)" << endl;
	cout << "gemaakt en gecompiled in MICROSOFT VISUAL STUDIO";
	cout << endl << "Studentennummber: s2028719 ";
	cout << " Studierichting: Natuur/Wiskunde ";
	cout << " Jaar van aankomst: 2017" << endl;
	cout << "Programmeer Opgave: LIFE" << endl;
	cout << "Dit is een console based game of life" << endl;
	cout << "De gebruiker word gepresenteerd met";
	cout << "een reeks opties en controls voor het bedienen";
	cout << "van het programma" << endl << endl << endl;
}
int main() {
	infoblokje();
	LIFE M;                             //declares instance of life
	open_file(M);                       //asks and opens input file
	fill_world(M);                          //fills with input file
	M.draw(M);                              //draws the first frame	                      
	while (!M.game_over) {        //everything is run from the menu 
		M.menu(M);
	} //so as long as the user hasnt exited the menu gets called in
	return 0;
}