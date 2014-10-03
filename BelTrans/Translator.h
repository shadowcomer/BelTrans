#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <iostream>

using std::string;

enum TRANSL_MODE { FROM_FILE, FROM_STRING };

class Translator
{
public:
	Translator(string pText, string k, string alph, TRANSL_MODE mode);
	Translator(void);
	~Translator(void);

	void printAlphabets();
	void printTranslation();
	void printOriginal();
	bool saveResults(string filename);

private:
	std::string translated_text;
	std::string original_text;
	std::string base_alphabet;
	std::string key;

	// Alphabets are divided into two rows: top and bottom. 
	// Top is fixed and bottom rotates to the right.
	std::string top_row;
	std::vector<string> bottom_row;
	std::vector<string> row_id;

	bool loadTextFromFile(std::string const filename);
	bool loadAlphabets(std::string in_alphabet);
	bool loadKey(std::string k);

	// Appends a bottom row encoding
	bool appendAlphabet(std::string bottom);
	std::string pickAlphabet(char ch);
	char matchCharacter(char ch, char k);
	void translate();

};

