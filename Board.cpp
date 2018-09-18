#include "Board.h"
using namespace std;
#define BLACK 0
#define BLUE 1
#define GREEN 2
#define CYAN 3
#define RED 4
#define MAGENTA 5
#define BROWN 6
#define LIGHTGRAY 7
#define DARKGRAY 8
#define LIGHTBLUE 9
#define LIGHTGREEN 10
#define LIGHTCYAN 11
#define LIGHTRED 12
#define LIGHTMAGENTA 13
#define YELLOW 14
#define WHITE 15

Board::Board(string lines, string columns, string filename) {
	l = stoi(lines);
	c = stoi(columns);
	dictionary = filename;
	d1.update(filename);
	grid.resize(l + 1);//last position will be the words already placed on the board
	for (int i = 0; i < l; i++)
		grid[i].resize(c, ".");
}

Board::Board(string filename) {
	ifstream file;
	string line;
	vector<string> puzzle;
	l = 0;
	file.open(filename, fstream::in);
	getline(file, line);
	dictionary = line;
	d1.update(dictionary);//places all the words and synonyms on a map
	getline(file, line);
	while (true) {//until it reads all of the board
		getline(file, line);
		if (line == "")
			break;
		puzzle.push_back(line);
		l++;
	}
	c = (puzzle[0].size() + 1) / 2;
	grid.resize(l + 1);//last position will be the words already placed on the board
	for (int i = 0; i < l; i++)
		grid[i].resize(c, ".");
	for (int i = 0; i < l; i++)
		for (int x = 0; x < c; x++)
			grid[i][x] = puzzle[i][x * 2];//*2 because of the spaces
	while (getline(file, line)) {
		if (line == "")
			break;
		grid[l].push_back(convpos(line.substr(0, 3)) + line.substr(3));//placing the words already placed on the board on the vector with the list of those words
	}
	file.close();
}

void Board::show() {
	cout << endl << "  ";
	setcolor(4);//changes color of letters to red
	for (int i = 0; i < c; i++)//for the first line
		cout << " " << (char)((int)'a' + i);
	for (int i = 0; i < l; i++) {
		setcolor(4);
		cout << endl << (char)((int)'A' + i) << " ";//the first 2 caracters of each line
		setcolor(0, 7);//changes color to lightgrey background and black letters
		for (int x = 0; x < c; x++) {//for each letters on the line, a " " and the letter
			cout << " ";
			if (grid[i][x] == "#") {//if its a "#", reverses the color patern first 
				setcolor(7, 0);
				cout << "#";
				setcolor(0, 7);
			}
			else cout << grid[i][x];
		}
		cout << " ";//the last of each line
	}
	cout << endl << endl;
	setcolor(7, 0);//returns color to black background with lightgray letters
	return;
}

void Board::show2() {
	cout << endl << "  ";
	setcolor(4);//changes color of letters to red
	for (int i = 0; i < c; i++)//for the first line
		cout << " " << (char)((int)'a' + i);
	for (int i = 0; i < l; i++) {
		setcolor(4);
		cout << endl << (char)((int)'A' + i) << " ";//the first 2 caracters of each line
		setcolor(0, 7);//changes color to lightgrey background and black letters
		for (int x = 0; x < c; x++) {
			if (x == 0) {//if it's the first of the line, the space will be black
				setcolor(7, 0);
				cout << " ";
				setcolor(0, 7);
			}
			else if (grid[i][x] == "#" || grid[i][x - 1] == "#") {//if the previous or next are a "#", the space will be black
				setcolor(7, 0);
				cout << " ";
				setcolor(0, 7);
			}
			else cout << " ";
			if (grid[i][x] == "#") {//if its a "#", reverses the color patern first 
				setcolor(7, 0);
				cout << grid[i][x];
			}
			else cout << grid[i][x];
		}
		setcolor(7, 0);
		cout << " "; // the last of each line, always black background
	}
	cout << endl << endl;
	setcolor(7, 0);//returns color to black background with lightgray letters
	hintshow();
	return;
}

void Board::hintshow() {
	setcolor(4);//changes text color to red
	cout << "List of hints: " << endl;
	cout << "Horizontal:" << endl;
	setcolor(7);//changes text color to normal
	for (int i = 0; i < h; i++)//h=number of horizontal words (in total, not only the ones placed on the board)
		cout << reverseconvpos(words[i].substr(0, 3)) << ": " << hints[i] << endl;//words is a list of the words used to build the board, first the horizontal and then the vertical
	setcolor(4);
	cout << endl << "Vertical:" << endl;
	setcolor(7);
	for (int i = 0; i < v; i++)
		cout << reverseconvpos(words[h + i].substr(0, 3)) << ": " << hints[h + i] << endl;
	cout << endl;
	return;
}

void Board::receiving() {
	string position, word;
	full = false;
	while (true) {
		show();
		if (isfull()) {
			cout << "The board is full!" << endl;
			full = true;
			break;
		}
		cout << "Position ( LCD / CTRL-Z = stop ) ? ";
		cin >> position;
		if (cin.fail()) {
			if (cin.eof()) {//if ctrl-z
				cin.clear();
				break;
			}
			else {
				cin.clear();
				cin.ignore(100, '\n');
				break;
			}
		}
		else if (not (isvalidp(position))) {//checks if the position is valid
			cout << "That is not a valid position!" << endl;
		}
		else {
			position = convpos(position);//converts position (example: Aav->11V)
			cout << "Word ( - = remove / ? = help ) .. ? ";
			cin >> word;
			if (cin.fail()) {
				cin.clear();
				cout << "That is not a valid input!" << endl;
			}
			else if (word == "-")
				remove(position);
			else if (word == "?") {
				if (position[2] == 'H')
					helph(position);
				else helpv(position);
			}
			else if (isvalid(word)) {//if it belongs to the dictionary
				word = d1.convertToUpperCase(word);//converts the word to uppercase
				if (exists(word))//if the word as already been placed on the board
					cout << "The word has already been placed on the crosswords!" << endl;
				else {
					assign(position + " " + word);
				}
			}
		}
	}
	return;
}

bool Board::isvalidp(string position) { //can be letters or numbers
	if (position.length() != 3)//if the size isn't 3
		return false;
	if (position.compare(2, 1, "H") != 0 && position.compare(2, 1, "V") != 0 && position.compare(2, 1, "h") != 0 && position.compare(2, 1, "v") != 0)//if the 3rd character isn't a v, V, h or H
		return false;
	char t = position[0];
	if (isdigit(t)) {
		if ((int)t - (int)'0' > l || t == '0')//if its a number bigger than the number of lines or it's 0
			return false;
	}
	else if ((int)t - (int)'A' >= l || (int)t - (int)'A' < 0)//if it's not within the range of the lines
		return false;
	t = position[1];
	if (isdigit(t)) {
		if ((int)t - (int)'0' > c || t == '0')//if its a number bigger than the number of columns or it's 0
			return false;
	}
	else if ((int)t - (int)'a' >= c || (int)t - (int)'a' < 0)//if it's not within the range of the columns
		return false;
	return true;
}

bool Board::exists(string word) {
	for (unsigned int i = 0; i < grid[l].size(); i++)//for every string on that vector
		if (word == grid[l][i].substr(4))//substr(4) because grid[l][i]=position+" "+word
			return true;
	return false;
}

bool Board::isfull() {
	for (int i = 0; i < l; i++)
		for (int x = 0; x < c; x++)//for every position on the grid
			if (grid[i][x] == "." || grid[i][x] == " ")
				return false;
	return true;
}

bool Board::isvalid(string word) {
	for (unsigned int i = 0; i < word.size(); i++) {//for every letter of the word
		int w = (int)word[i];
		if (w < (int)'A' || ((int)'Z' < w && w < (int)'a') || (int)'z' < w) {
			cout << "The word isn't valid!" << endl;
			return false;
		}
	}
	if (d1.checkIfWordExists(word))//if it belongs to the dictionary
		return true;
	else {
		cout << "The word doesn't exist in the dictionary!" << endl;
		return false;
	}
}

string Board::convpos(string position) {//turns Aav to 11V
	char t = position[0];
	if (not(isdigit(t)))
		position[0] = (char)((int)t - (int)'A' + 1 + (int)'0');
	t = position[1];
	if (not(isdigit(t)))
		position[1] = (char)((int)t - (int)'a' + 1 + (int)'0');
	position[2] = toupper(position[2]);
	return position;
}

string Board::reverseconvpos(string position) {//turns 11V to AaV
	position[0] = (char)(stoi(string(1, position[0])) + (int)'A' - 1);
	position[1] = (char)(stoi(string(1, position[1])) + (int)'a' - 1);
	return position;
}

void Board::fill() {//turns all "." to "#"
	for (int i = 0; i < l; i++)
		for (int x = 0; x < c; x++)
			if (grid[i][x] == ".")
				grid[i][x] = "#";
	return;
}

void Board::fillblank() {
	vector<string> horizontal, vertical;
	for (int i = 0; i < l; i++)//turns all letters into " "
		for (int x = 0; x < c; x++)
			if (grid[i][x] != "#")
				grid[i][x] = " ";
	words = grid[l];	
	for (unsigned int i = 0; i < words.size(); i++) {//orders all the words into horizontal and vertical
		if (words[i][2] == 'H')
			horizontal.push_back(words[i]);
		else vertical.push_back(words[i]);
	}
	v = vertical.size();
	h = horizontal.size();
	sort(horizontal.begin(), horizontal.end());//orders the words by line
	sort(vertical.begin(), vertical.end());
	words = horizontal;
	words.insert(words.end(), vertical.begin(), vertical.end());
	for (unsigned int i = 0; i < words.size(); i++) {
		hints.push_back(d1.newhint(words[i].substr(4), " "));//creates the hints
	}
	grid[l].clear();

	return;
}

void Board::assign(string input) {
	string position = input.substr(0, 3);
	string word = input.substr(4);
	if (position[2] == 'H')
		horizontal(position, word);
	else vertical(position, word);
	return;
}

void Board::assign2(string input) {
	string position = input.substr(0, 3);
	string word = input.substr(4);
	int l2 = (int)position[0] - (int)'0' - 1;
	int c2 = (int)position[1] - (int)'0' - 1;
	int x = -1;
	for (unsigned int i = 0; i < words.size(); i++) {//finds in which position of the vector the position is
		if (words[i].substr(0, 3) == position) {
			x = i;
			break;
		}
	}
	if (x == -1) {//if it doesn't find that position in the list of words
		cout << "The word can't be put there!" << endl;
		return;
	}
	if (word.length() != words[x].substr(4).length()) {//if the size of the word is different from the size of the original word
		cout << "The word doesn't fit!" << endl;
		return;
	}
	if (position[2] == 'H') {
		for (unsigned int i = 0; i < word.size(); i++) {
			if (string(1, word[i]) != grid[l2][c2 + i] && grid[l2][c2 + i] != " ") {//if it doesn't match the letters already placed
				cout << "The word can't be put there!" << endl;
				return;
			}
		}
		for (unsigned int i = 0; i < word.size(); i++)
			grid[l2][c2 + i] = string(1, word[i]);//places the word
	}
	else for (unsigned int i = 0; i < word.size(); i++) {
		if (string(1, word[i]) != grid[l2 + i][c2] && grid[l2 + i][c2] != " ") {//if it doesn't match the letters already placed
			cout << "The word can't be put there!" << endl;
			return;
		}
		for (unsigned int i = 0; i < word.size(); i++)
			grid[l2 + i][c2] = string(1, word[i]);//places the word
	}
	grid[l].push_back(position + " " + word);//adds to the list of words placed on the board
	return;
}

void Board::remove(string position) {
	int x = -1;
	for (unsigned int i = 0; i < grid[l].size(); i++) {//finds in which position of the vector the position is
		if (grid[l][i].substr(0, 3) == position) {
			x = i;
			break;
		}
		else x = -1;
	}
	if (x == -1) {//if it doesn't find that position in the list of words
		cout << "There is no word there!" << endl;
		return;
	}
	for (int i = 0; i < l; i++)//empties the board
		grid[i].assign(c, ".");
	cout << "The word " << grid[l][x].substr(4) << " was removed!" << endl;
	grid[l].erase(grid[l].begin() + x);
	for (unsigned int i = 0; i < grid[l].size(); i++)//places again all the words on the board
		assign(grid[l][i]);
	return;
}

void Board::remove2(string position) {
	int x = -1;
	for (unsigned int i = 0; i < grid[l].size(); i++)//finds in which position of the vector the position is
		if (grid[l][i].substr(0, 3) == position) {
			x = i;
			break;
		}
	if (x == -1) {//if it doesn't find that position in the list of words
		cout << "There is no word there!" << endl;
		return;
	}
	cout << "The word " << grid[l][x].substr(4) << " was removed!" << endl;
	for (unsigned int i = 0; i < grid[l][x].substr(4).length(); i++) {//removes from the grid the word
		if (position[2] == 'H')
			grid[position[0] - '0' - 1][position[1] - '0' + i - 1] = " ";
		else grid[position[0] - '0' - 1 + i][position[1] - '0' - 1] = " ";
	}
	grid[l].erase(grid[l].begin() + x);
	return;
}

void Board::helph(string position) {
	int count = 0;
	int l2 = (int)position[0] - (int)'0' - 1;
	int c2 = (int)position[1] - (int)'0' - 1;
	vector<string>words;
	string letter = grid[stoi(string(1, position[0])) - 1][stoi(string(1, position[1])) - 1];//letter of the initial position
	if (letter == ".")
		words = d1.SearchWords("*");
	else words = d1.SearchWords(letter + "*");
	for (unsigned int x = 0; x < words.size(); x++) {//checks if the word can be placed
		bool fits = true;
		string word = words[x];
		if (size_t(c - c2) < word.length())//checks if the word fits on the board
			fits = false;
		else {
			for (unsigned int i = 0; i < word.size(); i++)//checks if the word matches the letters already placed
				if (string(1, word[i]) != grid[l2][c2 + i] && grid[l2][c2 + i] != ".")
					fits = false;
			if ((c2 != 0 && grid[l2][c2 - 1] != "#" && grid[l2][c2 - 1] != ".") || (c2 + word.length() != c && grid[l2][c2 + word.length()] != "." && grid[l2][c2 + word.length()] != "#"))
				fits = false;
			if (exists(word))
				fits = false;
			if (fits) {
				cout << word << endl;
				count++;
			}
		}
	}
	if (count == 0)
		cout << "There are no words!" << endl;
	return;
}

void Board::helpv(string position) {
	int count = 0;
	vector<string> words;
	string letter = grid[stoi(string(1, position[0])) - 1][stoi(string(1, position[1])) - 1];//letter of the initial position
	if (letter == ".")
		words = d1.SearchWords("*");
	else words = d1.SearchWords(letter + "*");
	int l2 = (int)position[0] - (int)'0' - 1;
	int c2 = (int)position[1] - (int)'0' - 1;
	for (unsigned int x = 0; x < words.size(); x++) {//checks if the word can be placed
		bool fits = true;
		string word = words[x];
		if (size_t(l - l2) < word.length())//checks if the word fits on the board
			fits = false;
		else {
			for (unsigned int i = 0; i < word.size(); i++)//checks if the word matches the letters already placed
				if (string(1, word[i]) != grid[l2 + i][c2] && grid[l2 + i][c2] != ".")
					fits = false;
			if ((l2 != 0 && grid[l2 - 1][c2] != "#" && grid[l2 - 1][c2] != ".") || (l2 + word.length() != l && grid[l2 + word.length()][c2] != "." && grid[l2 + word.length()][c2] != "#"))
				fits = false;
			if (exists(word))
				fits = false;
			if (fits) {
				cout << word << endl;
				count++;
			}
		}
	}
	if (count == 0)
		cout << "There are no words!" << endl;
	return;
}

void Board::help2(string position) {//gives a new hint
	string word, hint;
	unsigned int i;
	for (i = 0; i < words.size(); i++)
		if (position == words[i].substr(0, 3)) {
			word = words[i].substr(4);
			hint = hints[i];
			break;
		}
	hints[i] = d1.newhint(word, hint);
	return;
}

void Board::horizontal(string position, string word) {
	int l2 = (int)position[0] - (int)'0' - 1;
	int c2 = (int)position[1] - (int)'0' - 1;
	if (size_t(c - c2) < word.length()) {
		cout << "The word doesn't fit!" << endl;
		return;
	}
	for (unsigned int i = 0; i < word.size(); i++) {
		if (string(1, word[i]) != grid[l2][c2 + i] && grid[l2][c2 + i] != ".") {
			cout << "The word can't be put there!" << endl;
			return;
		}
	}
	if ((c2 != 0 && grid[l2][c2 - 1] != "#" && grid[l2][c2 - 1] != ".") || (c2 + word.length() != c && grid[l2][c2 + word.length()] != "." && grid[l2][c2 + word.length()] != "#")) {//
		cout << "The word can't be put there!" << endl;
		return;
	}
	for (unsigned int i = 0; i < word.size(); i++) {
		grid[l2][c2 + i] = string(1, word[i]);
	}
	if (c2 != 0)
		grid[l2][c2 - 1] = "#";
	if (c2 + word.length() != c)
		grid[l2][c2 + word.length()] = "#";
	if (not(exists(word)))
		grid[l].push_back(position + " " + word);
	return;
}

void Board::vertical(string position, string word) {
	int l2 = (int)position[0] - (int)'0' - 1;
	int c2 = (int)position[1] - (int)'0' - 1;
	if (size_t(l - l2) < word.length()) {
		cout << "The word doesn't fit!" << endl;
		return;
	}
	for (unsigned int i = 0; i < word.size(); i++) {
		if (string(1, word[i]) != grid[l2 + i][c2] && grid[l2 + i][c2] != ".") {
			cout << "The word can't be put there!" << endl;
			return;
		}
	}
	if ((l2 != 0 && grid[l2 - 1][c2] != "#" && grid[l2 - 1][c2] != ".") || (l2 + word.length() != l && grid[l2 + word.length()][c2] != "." && grid[l2 + word.length()][c2] != "#")) {//
		cout << "The word can't be put there!" << endl;
		return;
	}
	for (unsigned int i = 0; i < word.size(); i++) {
		grid[l2 + i][c2] = string(1, word[i]);
	}
	if (l2 != 0)
		grid[l2 - 1][c2] = "#";
	if (l2 + word.length() != l)
		grid[l2 + word.length()][c2] = "#";
	if (not(exists(word)))
		grid[l].push_back(position + " " + word);
	return;
}

void Board::endofprogram(string dictionary, string choice) {
	ofstream newfile;
	ifstream file;
	string filename = "b001.txt", answer;
	for (unsigned int i = 0; i < grid[l].size(); i++)//final check for the words
		if (not(d1.checkIfWordExists(grid[l][i].substr(4))))
			cout << "The word " << grid[l][i].substr(4) << " doesn't belong to the dictionary!";
	if (not(full))
		while (true) {
			cout << "Do you whish to continue the board later? ", cin >> answer;
			if (cin.fail())
				cin.clear();
			answer = d1.convertToUpperCase(answer);
			if (answer == "Y" || answer == "YES")
				break;
			else if (answer == "N" || answer == "NO") {
				cout << "Filling blanks..." << endl;
				fill();
				break;
			}
			else cout << "That is not a valid answer! " << endl;
		}
	if (choice == "1")
		while (true) {
			file.open(filename, fstream::in);
			if (not(file.fail())) {
				int a = stoi(filename.substr(1, filename.size() - 5)) + 1;
				string b = to_string(a);
				if (b.length() == 1)
					b = "00" + b;
				else if (b.length() == 2)
					b = "0" + b;
				filename = "b" + b + ".txt";
				file.close();
			}
			else break;
		} //verificação caso o ficheiro não exista
	else filename = choice;
	cout << "Saving program in file " << filename << " ..." << endl;
	newfile.open(filename, fstream::out);
	newfile << dictionary << endl << endl;
	for (int i = 0; i < l; i++)
		for (int x = 0; x < c; x++) {
			if (x + 1 == c)
				newfile << grid[i][x] << endl;
			else newfile << grid[i][x] << " ";
		}
	for (unsigned int i = 0; i < grid[l].size(); i++) {
		grid[l][i] = reverseconvpos(grid[l][i].substr(0, 3)) + grid[l][i].substr(3);
		newfile << endl << grid[l][i];
	}
	newfile.close();
	return;
}

void Board::finalcheck() {
	bool wrong = false;
	vector<string> horizontal, vertical;
	for (unsigned int i = 0; i < grid[l].size(); i++) {
		if (grid[l][i][2] == 'H')
			horizontal.push_back(grid[l][i]);
		else vertical.push_back(grid[l][i]);
	}
	v = vertical.size();
	h = horizontal.size();
	sort(horizontal.begin(), horizontal.end());
	sort(vertical.begin(), vertical.end());
	grid[l] = horizontal;
	grid[l].insert(grid[l].end(), vertical.begin(), vertical.end());

	for (unsigned int i = 0; i < grid[l].size(); i++) {
		if (not(d1.checkIfWordExists(grid[l][i].substr(4)))) {
			cout << "The word " << grid[l][i].substr(4) << " doesn't belong to the dictionary!" << endl;
			wrong = true;
		}
		else if (grid[l][i] != words[i]) {
			cout << "The word " << grid[l][i].substr(4) << " is wrong!" << endl;
			wrong = true;
		}
	}
	if (wrong) {
		system("pause");
		exit(0);
	}
	else return;

}

void Board::setcolor(unsigned int color)
{
	HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hcon, color);
}
void Board::setcolor(unsigned int color, unsigned int background_color)
{
	HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	if (background_color == BLACK)
		SetConsoleTextAttribute(hCon, color);
	else
		SetConsoleTextAttribute(hCon, color | BACKGROUND_BLUE | BACKGROUND_GREEN |
			BACKGROUND_RED);
}
