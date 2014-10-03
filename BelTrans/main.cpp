#include <fstream>
#include <iostream>
#include <string>
#include "Translator.h"

using namespace std;

int main(int argc, char* argv[])
{
	Translator bellaso;
	string action;
	string input;
	string key;
	string alph = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	while(true)
	{
		cout << "Do you wish to load a text from (F)ile, from a (S)tring or (Q)uit?" << endl;
		cin >> action;
		if("F" == action || "f" == action)
		{
			cout << "Filename: ";
			cin >> input;
			cout << "Key: ";
			cin >> key;
			bellaso = Translator(input, key, alph, TRANSL_MODE::FROM_FILE);
			bellaso.printOriginal();
			cout << "---------------------------------------" << endl;
			bellaso.printTranslation();
			cout << endl;
		}
		else if("S" == action || "s" == action)
		{
			cout << "String: ";
			cin >> input;
			cout << "Key: ";
			cin >> key;
			bellaso = Translator(input, key, alph, TRANSL_MODE::FROM_STRING);
			bellaso.printOriginal();
			cout << "---------------------------------------" << endl;
			bellaso.printTranslation();
		}
		else if("Q" == action || "q" == action)
		{
			cout << "Bye bye!" << endl;
			return 0;
		}
		else
		{
			cout << "Unknown input." << endl;
			continue;		
		}

		while(true)
		{
			string answer;
			cout << "Do you wish to save your results? Y/N" << endl;
			cin >> answer;
			if("Y" == answer || "y" == answer)
			{
				string saveName;
				cout << "What file to save to? ";
				cin >> saveName;
				bellaso.saveResults(saveName);
				break;
			}
			else if("N" == answer || "n" == answer)
			{
				break;
			}
			else cout << "Unknown input" << endl;
		}

	}

	return 0;
}