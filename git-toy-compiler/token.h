#pragma once
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <fstream>
#include <iterator>
#include <algorithm> 
 

using namespace std;

 
class Token
{
public:
	
	//CONSTRUCTORS
	Token();
	Token(int aline, int acolumn){ line = aline ; column = acolumn;} 
	Token(int aline, int acolumn, string alexeme, string atype){ line = aline ; column = acolumn ; type = atype; lexeme = alexeme;} 
	
	//METHODS
	void printToken(bool error);
	void assignType(int finalState);
	string getLexeme(){return lexeme;}
	string getWord(){return word;}
	string getType(){return type;}
	int getLine(){return line;};
	void concatLex(char c);
	void symbolCheck();
	void setTopos(int acol, int aline) {column = acol ; line = aline;}

	
private:
    
    int line;
	int column;
	string type;
	string lexeme = "";
	string word;

	
};