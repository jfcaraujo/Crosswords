#ifndef player_h
#define player_h 
#include <iostream> // cout, cin
#include <string> // string
#include <ctime> // clock, CLOCKS_PER_SEC
#include <fstream> //
using namespace std; // std::

class Player {
public:
	void PlayerName(); // asks for the player name
	string ShowName(); // shows the player name
	void StartTime(); // starts the timer
	double TotalTime(); // shows how much time has passed
	void newClue(); // adds a clue to the current number
	int showclues();//shows the number of clues used
	
private:
	int clues;//number of clues used
	string name;//name of the player
	clock_t start;//time that the game began
};

#endif