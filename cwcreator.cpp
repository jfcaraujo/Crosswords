#include "Board.h"

string beginning() {
	string choice;
	cout << "*****************************************************************" << endl;
	cout << "*                                                               *" << endl;
	cout << "*    Name       : Crosswords maker                              *" << endl;
	cout << "*    Date       : 20-May-2018                                   *" << endl;
	cout << "*    Authors    : Carlos Nunes  Joao Filipe Araujo              *" << endl;
	cout << "*    Class      : 3                                             *" << endl;
	cout << "*    Team       : 10                                            *" << endl;
	cout << "*    EIC0012 | PROGRAMMING | 2017/2018                          *" << endl;
	cout << "*                                                               *" << endl;
	cout << "*****************************************************************" << endl;
	cout << "*****************************************************************" << endl;
	cout << "*    INSTRUCTIONS                                               *" << endl;
	cout << "*   Position ( LCD / CTRL-Z = stop)                             *" << endl;
	cout << "*   LDC stands for Line Column and Direction                    *" << endl;
	cout << "*   Line can go from A to Z or 1 to 26                          *" << endl;
	cout << "*   Column can go from a to z or 1 to 26                        *" << endl;
	cout << "*   Direction is H/h(Horizontal) or V/v(Vertical)               *" << endl;
	cout << "*****************************************************************" << endl;
	cout << "*   CTRL-Z = stop, the user can quit the program                *" << endl;
	cout << "*****************************************************************" << endl;
	cout << "*   Do you wish to continue the board later?                    *" << endl;
	cout << "*   Answer must be Yes/y or No/n (lower or upper case)          *" << endl;
	cout << "*****************************************************************" << endl;
	cout << "* Create a puzzle-> Lets player start creating a puzzle         *" << endl;
	cout << "* Resume puzzle-> Lets player resume a previously stored puzzle *" << endl;
	cout << "* Exit-> Exit program                                           *" << endl;
	cout << "*****************************************************************" << endl;

	cout << endl << "OPTIONS:" << endl;
	cout << "1 - Create a puzzle" << endl;
	cout << "2 - Resume puzzle" << endl;
	cout << "0 - Exit" << endl;
	while (true) { //asks for the option again in case it isn't valid
		cout << endl << "Option ? ", cin >> choice;
		if (cin.fail() || not(isdigit(choice[0])) || choice.size() != 1) {
			cin.clear();
			choice = "10";
		}
		if (stoi(choice) < 0 || stoi(choice) > 2)
			cout << "That is not a valid option. ";
		else return choice;
	}
}
void program1() {
	string filename, position, word;
	ifstream file;
	string lines, columns;
	cout << endl << "-----------------------------------------------------------------" << endl;
	cout << "CREAT PUZZLE" << endl;
	cout << "-----------------------------------------------------------------" << endl;
	while (true) {//keeps asking for a dictionary until it can open one
		cout << "Dictionary file name ? ";
		cin >> filename; 
		if (cin.fail()) {
			cin.clear();
		}
		else {
			file.open(filename, fstream::in);
			if (not(file.fail())) {
				file.close();
				break;
			}
		}
		cout << "Dictionary file not found! " << endl;
	}
	cout << "Board size (lines columns) ? ";
	cin >> lines >> columns;
	while (true) {//keeps asking for a valid number of lines and columns
		if (cin.fail() || not(isdigit(lines[0])) || lines.size() > 2 || not(isdigit(columns[0])) || columns.size() > 2 || not(isdigit(lines[1])) || not(isdigit(columns[1])))
			cin.clear();
		else if (stoi(lines) > 0 || stoi(columns) > 0 || stoi(lines) < 27 || stoi(columns) < 27)
			break;
		cout << "Board size (lines columns) ? ";
		cin >> lines >> columns;
	} 
	Board b1(lines, columns, filename);
	b1.receiving();
	b1.endofprogram(filename, "1");
	return;
}
void program2() {
	string filename;
	ifstream file;
	cout << "Enter board name: ";
	cin >> filename;
	while (true) {
		if (cin.fail())
			cin.clear();
		else {
			file.open(filename, fstream::in);
			if (file.fail())
				cout << "File not found! " << endl;
			else {
				file.close();
				break;
			}
		}
		cout << "Enter board name: ";
		cin >> filename;
	}
	Board b1(filename);
	b1.receiving();
	string line;
	file.open(filename, fstream::in);
	getline(file, line);
	b1.endofprogram(line, filename);
	return;
}
int main()
{
	string x = beginning();
	if (x == "1")
		program1();
	else if (x == "2")
		program2();
	else {
		cout << "End of program." << endl;
	}
	system("pause");
	return 0;
}

