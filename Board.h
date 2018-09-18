#ifndef board_h
#define board_h
#include <cstdlib>
#include <windows.h>
#include <iomanip>
#include <cctype>
#include <algorithm>
#include "Dictionary.h"
using namespace std;

class Board {
public:
	Board(string lines, string columns, string filename);//builds a board from scratch (cwcreator)
	Board(string filename);//rebuilds a board from a text file (both)
	void show();//displays the contents of the board for cwcreator
	void show2();//displays the contents of the board for cwplayer
	void hintshow();//displays the list of hints (cwplayer)
	void receiving();//general funtion for cwcreator, keeps asking for positions and words
	void setcolor(unsigned int color);//changes the color of the text (both)
	void setcolor(unsigned int color, unsigned int background_color);//changes the color of the text and the background (both)
	bool isvalidp(string position);//checks if the position is valid (both)
	bool exists(string word);//checks if the word has already been placed on the board (both)
	bool isfull();//checks if the board is full (both)
	bool isvalid(string word);//checks if the word is valid, including if it belongs to the dictionary (both)
	string convpos(string position);//converts a position (example: Aav->11V) (both)
	string reverseconvpos(string position);//transforms the position from numbers to letters (example: 11V->AaV) (both)
	void fill();//all "." turn into "#" (cwcreator)
	void fillblank();//deletes all the words from the grid, orders them, copies them to vector words and then deletes the last vector (cwplayer)
	void assign(string input);//receives position+" "+word and sends to horizontal or vertical function (cwcreator)
	void assign2(string input);//receives position+" "+word and places the word (cwplayer)
	void remove(string position);//removes a word from the grid in cwcreator
	void remove2(string position);//removes a word from the grid in cwplayer
	void helph(string position);//gives the sugestions for words in the horizontal position (cwcreator)
	void helpv(string position);//gives the sugestions for words in the vertical position (cwcreator)
	void help2(string position);//gives a new synonym for the word in that position (cwplayer)
	void horizontal(string position, string word);//places a word in the horizontal position (cwcreator)
	void vertical(string position, string word);//places a word in the vertical position (cwcreator)
	void endofprogram(string dictionary, string choice);//saves the board (cwcreator)
	void finalcheck();//checks if the words were placed correctly (cwplayer)
	
	
private:
	int l, c, h, v; //number of lines, columns, horizontal words and vertical words (the last 2 are only used in cwplayer)
	bool full;//indicates wether the board is full or not 
	string dictionary;//name of the dictionary
	vector<string> words;//words that were used building the board and their position
	vector<string> hints;//synonyms for the words that were used building the board, with the same positions of words
	vector<vector<string>> grid;//group of vectors that represent each line on the board, and the last vector represents the positions and words already placed on the board
	Dictionary d1;//dictionary
};

#endif