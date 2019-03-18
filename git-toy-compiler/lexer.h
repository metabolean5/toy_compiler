#pragma once
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <fstream>
#include <iterator>
#include <algorithm> 
#include "token.h"
#include "table.h"



using namespace std;


class Lexer 
{
	
public:
	
	//CONSTRUCTORS
	Lexer();
	Lexer(ifstream &progFile);
	
	//METHODS
	void run_Lexer();
	Token nextToken();
	char nextChar();
	void addToken(Token* toke);
	void displayTokenList();
	void printString();
	vector<Token> returnWords();

private:
    
	//member vars
	string progString;
	int currState = 0;
	int charPosition = -1;
	int lineNb = 1;
	vector<Token> tokenList;
	int nbOfTokens = 0;
	Table table;
	
	
	//code utilities
	bool eof = false;
	int nbCol = -1;
	
	
};
 

