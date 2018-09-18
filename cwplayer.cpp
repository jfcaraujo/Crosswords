#include "Board.h"
#include "Dictionary.h"
#include "Player.h"

int main() {
	string filename;
	ifstream file;
	cout << "*****************************************************************" << endl;
	cout << "*                                                               *" << endl;
	cout << "*    Name       : Crosswords player                             *" << endl;
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
	cout << "*   Word ( - = remove / ? = help ) .. ?                         *" << endl;
	cout << "*   Removes word from the board                                 *" << endl;
	cout << "*   To use remove the user must input '-'                       *" << endl;
	cout << "*   Help shows to the user clues(synonyms) on the board         *" << endl;
	cout << "*   To use help the user must input '?'                         *" << endl;
	cout << "*****************************************************************" << endl << endl;
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
	Player p1;
	Dictionary d;
	b1.fillblank();
	p1.PlayerName();
	p1.StartTime();
	string position, word;
	while (true) {
		b1.show2();
		if (b1.isfull()) {
			cout << "The board is full!" << endl;
			break;
		}
		cout << "Position ( LCD / CTRL-Z = stop ) ? ";
		cin >> position;
		if (cin.fail()) {
			if (cin.eof()) {
				cin.clear();
				cout << "The game was stopped midway." << endl;
				system("pause");
				exit(0);
			}
			else {
				cin.clear();
				cin.ignore(100, '\n');
				break;
			}
		}
		else if (not (b1.isvalidp(position))) {//verificação se a posição introduzida é válida
			cout << "That is not a valid position!" << endl;
		}
		else {
			position = b1.convpos(position);
			cout << "Word ( - = remove / ? = help ) .. ? ";
			cin >> word;
			if (cin.fail()) {
				cin.clear();
				cout << "That is not a valid input!" << endl;
			}
			else if (word == "-")
				b1.remove2(position);
			else if (word == "?") {
				b1.help2(position);
				p1.newClue();
			}
			else if (b1.isvalid(word)) {
				word = d.convertToUpperCase(word);
				if (b1.exists(word))
					cout << "The word has already been placed on the crosswords!" << endl;
				else {
					b1.assign2(position + " " + word);

				}
			}
		}
	}
	b1.finalcheck();
	ofstream newfile;
	string line;
	filename = filename.substr(0, 4) + "_p" + ".txt";
	cout << "Saving stats in file " << filename << " ..." << endl;
	file.open(filename, fstream::in);
	if (file.fail()) {
		newfile.open(filename, fstream::out);
		newfile << "Player: " << p1.ShowName() << " Time: " << p1.TotalTime() << " Clues: " << p1.showclues();
		newfile.close();
	}
	else {
		vector<string> temp;
		while (getline(file, line))
			temp.push_back(line);
		file.close();
		newfile.open(filename, fstream::out);
		for (unsigned int i = 0; i < temp.size(); i++)
			newfile << temp[i] << endl;
		newfile << "Player: " << p1.ShowName() << "  Clues: " << p1.showclues() << "  Time: " << p1.TotalTime();
		newfile.close();
	}
	system("pause");
	return 0;
}