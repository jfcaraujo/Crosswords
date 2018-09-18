#include "Dictionary.h"

Dictionary::Dictionary() {}

void Dictionary::update(string filename) {
	string line, allsynonymous, synonymous, headlines;
	size_t pos = 0;
	ifstream file; //old file
	file.open(filename, fstream::in);
	if (file.fail()) {
		cout << "Dictionary file not found! " << endl;
		exit(0);
	}
	while (getline(file, line)) {
		headlines = line.substr(0, line.find(": "));
		headlines = convertToUpperCase(headlines);
		allsynonymous = line.erase(0, line.find(": ") + 2);

		while ((pos = allsynonymous.find(", ")) != string::npos) {
			synonymous = allsynonymous.substr(0, pos);
			synonymous = convertToUpperCase(synonymous);
			allsynonymous.erase(0, pos + 2);
			dictionarymap[headlines].push_back(synonymous);
		}
		allsynonymous = convertToUpperCase(allsynonymous);
		dictionarymap[headlines].push_back(allsynonymous);
		pos = 0;
	}
	return;
}

string Dictionary::convertToUpperCase(string word) {
	for (unsigned int i = 0; i < word.size(); i++) {
		word[i] = toupper(word[i]);
	}
	return word;
}

bool Dictionary::checkIfWordExists(string word) {
	word = convertToUpperCase(word);
	if (dictionarymap.end() != dictionarymap.find(word)) return true;
	else return false;
}

vector<string> Dictionary::SearchWords(string wild) {
	vector<string> words;
	for (auto& i : dictionarymap) {
		if (wildcardMatch(i.first.c_str(), wild.c_str())) {
			words.push_back(i.first);
		}
	}
	return words;
}

bool Dictionary::wildcardMatch(const char * str, const char * strWild)
{
	//////////////////////////////////////////////////////////////////////////
	// WildcardMatch
	// str - Input string to match
	// strWild - Match mask that may contain wildcards like ? and *
	//
	// A ? sign matches any character, except an empty string.
	// A * sign matches any string inclusive an empty string.
	// Characters are compared caseless.
	//
	// ADAPTED FROM:
	// https://www.codeproject.com/Articles/188256/A-Simple-Wildcard-Matching-Function
	//
	// We have a special case where string is empty ("") and the mask is "*".
	// We need to handle this too. So we can't test on !*str here.
	// The loop breaks when the match string is exhausted.
	while (*strWild)
	{
		// Single wildcard character
		if (*strWild == '?')
		{
			// Matches any character except empty string
			if (!*str)
				return false;
			// OK next
			++str;
			++strWild;
		}
		else if (*strWild == '*')
		{
			// Need to do some tricks.
			// 1. The wildcard * is ignored.
			// So just an empty string matches. This is done by recursion.
			// Because we eat one character from the match string,
			// the recursion will stop.
			if (wildcardMatch(str, strWild + 1))
				// we have a match and the * replaces no other character
				return true;
			// 2. Chance we eat the next character and try it again,
			// with a wildcard * match. This is done by recursion.
			// Because we eat one character from the string,
			// the recursion will stop.
			if (*str && wildcardMatch(str + 1, strWild))
				return true;
			// Nothing worked with this wildcard.
			return false;
		}
		else
		{
			// Standard compare of 2 chars. Note that *str might be 0 here,
			// but then we never get a match on *strWild
			// that has always a value while inside this loop.
			if (toupper(*str++) != toupper(*strWild++))
				return false;
		}
	}
	// Have a match? Only if both are at the end...
	return !*str && !*strWild;
}

string Dictionary::newhint(string word, string hint) {
	map <string,vector<string>>::iterator i = dictionarymap.find(word);
	if (i == dictionarymap.end()) {
		cout << "The word " << word << " doesn't belong to the dictionary! " << endl;
		system("pause");
		exit(0);
	}
	vector<string> hintsd = i->second;
	string newhint;
	int x;
	srand((unsigned int)time(NULL));
	while (true) {
		x = rand() % hintsd.size();
		newhint = hintsd[x];
		if (newhint != hint)
			break;
		else hintsd.erase(hintsd.begin() + x);
	}
	return newhint;
}