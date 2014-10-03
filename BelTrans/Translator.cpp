#include "Translator.h"

using std::fstream;
using std::cout;
using std::endl;
using std::string;

Translator::Translator(void)
{

}

Translator::Translator(string input, string k, string alph, TRANSL_MODE mode)
{
	string::iterator it;

	// Decide whether file or string input
	switch(mode)
	{
	case TRANSL_MODE::FROM_FILE:
		if(!loadTextFromFile(input))
		{
			cout << "Error loading text from file." << endl;
		}
		break;

	case TRANSL_MODE::FROM_STRING:
		for(it = input.begin(); it != input.end(); *it++ = toupper(*it));
		original_text = input;
		break;

	default:
		break;
	}

	// Initialize
	for(it = k.begin(); it != k.end(); *it++ = toupper(*it));
	for(it = alph.begin(); it != alph.end(); *it++ = toupper(*it));

	key = k;
	base_alphabet = alph;
	loadAlphabets(base_alphabet);
	translate();
}

Translator::~Translator(void)
{
}

void Translator::translate()
{
	string::iterator key_iter;
	string::iterator text_iter;

	for(key_iter = key.begin(), text_iter = original_text.begin();
		text_iter != original_text.end();
		++text_iter)
	{
		// Wrap the key around if we've reached the end
		if(key_iter == key.end())
		{
			key_iter = key.begin();
		}

		translated_text += matchCharacter(*text_iter, *key_iter);
		++key_iter;
	}
}

bool Translator::saveResults(string filename)
{
	fstream file_stream = fstream(filename, fstream::out);
	string tmp;
	if(!file_stream.is_open())
	{
		cout << "Failed to open " << filename << endl;
		cout << "Results were not saved to disk" << endl;
		return false;
	}

	file_stream << translated_text;
	file_stream.close();
	return true;
}

void Translator::printOriginal()
{
	cout << original_text << endl;
}

void Translator::printTranslation()
{
	cout << translated_text << endl;
}
bool Translator::loadKey(string k)
{
	if(k == "") return false;
	key = k;
	return true;
}

bool Translator::loadAlphabets(string in_alphabet)
{
	string bottom_template;
	string bottom;
	string id;

	// WARNING: Don't mix iterators from different strings. They crash :P
	string::iterator it;
	string::iterator marker;
	string::iterator code_it;
	string::iterator middle;
	char tmp = 0;

	// Return if it's not even
	if(in_alphabet.length() % 2 != 0) return false;

	base_alphabet = in_alphabet;
	it = base_alphabet.begin();
	code_it = base_alphabet.begin();

	// Fill in the top row
	for(unsigned int i = 0; i < base_alphabet.length() / 2; ++i)
	{
		tmp = *it;
		top_row += tmp;
		++it;
	}
	middle = it;
	marker = base_alphabet.end();

	// Load the bottom row of the alphabet
	// Iterate while we have to rotate and append
	for(unsigned int i = 0; i < base_alphabet.length() / 2; ++i)
	{
		bottom = "";
		id = "";
		it = marker;
		// Iterate over the lower half to create the bottom row
		for(unsigned int j = 0; j < base_alphabet.length() / 2; ++j, ++it)
		{
			base_alphabet.end();
			// Wrap around every time we reach the end
			if(it == base_alphabet.end())
			{
				it = middle;
			}

			tmp = *it;
			bottom += tmp;	
		}

		// Store the row
		bottom_row.push_back(bottom);

		// Add the corresponding ID
		for(int i = 0; i < 2; ++i, ++code_it)
		{
			tmp = *code_it;
			id += tmp;
		}
		row_id.push_back(id);

		// Update the start point
		--marker;
	}
	
	return true;
}

bool Translator::appendAlphabet(string bottom)
{
	if(bottom.length() != top_row.length()) return false;
	bottom_row.push_back(bottom);
	return true;
}

bool Translator::loadTextFromFile(string const filename)
{
	fstream file_stream = fstream(filename, fstream::in);
	string tmp;
	if(!file_stream.is_open())
	{
		cout << "Failed to open " << filename << endl;
		return false;
	}

	while(file_stream.good())
	{
		file_stream >> tmp;
		original_text += tmp;
	}

	file_stream.close();

	return true;
}

void Translator::printAlphabets()
{
	if(row_id.size() != bottom_row.size())
	{
		cout << "Integrity problem. ID-row count mismatch" << endl;
		return;
	}

	std::vector<string>::iterator it_id;
	std::vector<string>::iterator it_row;

	it_id = row_id.begin();
	it_row = bottom_row.begin();

	cout << "  \t" << top_row << endl;
	for(it_row; it_row != bottom_row.end(); ++it_row, ++it_id)
	{
		cout << *it_id << "\t" << *it_row << endl;
	}
}

std::string Translator::pickAlphabet(char ch)
{
	int counter;
	string::iterator it;

	// Find the corresponding alphabet to a given character code
	for(it = base_alphabet.begin(), counter = 0; it != base_alphabet.end(); it += 2, ++counter)
	{
		if(ch == *it || ch == *(it+1))
			return bottom_row[counter];
	}

	return "";
}

char Translator::matchCharacter(char ch, char k)
{
	string::iterator it;
	string bottom = pickAlphabet(k);
	int counter = 0;
	unsigned int size = base_alphabet.size();
	unsigned int halfSize = size / 2;
	char match = 0;

	// Find if it's either upper or lower.
	// WARNING: Characters must be in order!!
	if(ch >= top_row.front() && ch <= top_row.back())
	{
		// Try to match it to a letter
		for(counter = 0, it = top_row.begin(); ch != *it && it != top_row.end(); ++counter, ++it);
		match = *(bottom.begin() + counter);
	}
	else if(ch >= base_alphabet[halfSize - 1] &&
		ch <= base_alphabet.back())
	{
		for(counter = 0, it = bottom.begin(); ch != *it && it != bottom.end(); ++counter, ++it);
		match = *(top_row.begin() + counter);
	}

	return match;
}