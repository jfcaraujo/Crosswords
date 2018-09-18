#ifndef dictionary_h
#define dictionary_h
#include <fstream> // ifstream, ofstream
#include <iostream> // cout, cin , cerr
#include <string> // string, getline, .size
#include <vector> // vector
#include <map> // multimap
#include <ctime>
using namespace std; // std::

class Dictionary {
public:
	Dictionary();//starts the class
	void update(string filename);//adds the synonyms to the dictionary
	string convertToUpperCase(string word);//converts the word to uppercase
	bool checkIfWordExists(string word);//checks if the word belongs to the dictionary
	vector<string> SearchWords(string wild);//searches with a wild string in the dictionary
	bool wildcardMatch(const char *str, const char *strWild);
	string newhint(string word, string hint);//gives a new hint
private:
	map <string, vector<string>> dictionarymap;//first->all the words second->the synonyms
};

#endif