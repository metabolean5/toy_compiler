
#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include <algorithm>  
#include <stdio.h>
#include "lexer.h"
#include "token.cpp"
#include "table.cpp"
#include "table.h"


using namespace std;

Lexer::Lexer() {
	
}

Lexer::Lexer(ifstream &progFile){

	string str((std::istreambuf_iterator<char>(progFile)), std::istreambuf_iterator<char>());
	progString = str;
	progString.erase(std::remove(progString.begin(), progString.end(), '\t'), progString.end());
}


void Lexer::addToken(Token* toke){
	
	tokenList.push_back(*toke);
}

void  Lexer::displayTokenList(){
	
	tokenList.pop_back();
	cout << "TOKENS LIST:" << endl;
	for (size_t i = 0; i < tokenList.size(); ++i) {
		 tokenList.at(i).printToken(false);
		}
}


void Lexer::printString(){
	
	for(int i = 0; i < progString.size(); ++i) {
		cout << progString[i];
		}
}

char Lexer::nextChar(){
	
	charPosition++;
	nbCol++;
	return progString[charPosition];
}


Token Lexer::nextToken(){
	
	nbOfTokens++;
	Token token;
	//cout << "Looking for token number " << nbOfTokens << endl;
	currState = 1;
	
	cout << " --> ROOT " ;
	
	bool tokenFound = false;
	while(tokenFound == false){
		
		char next = nextChar();
	
		if (next == '\0' || next == '^') {tokenFound = true ; eof = true; break;}
		if (next == '\n' || next == '\r'  ) {nbCol = 0; lineNb++;}
		
		cout << "-->";
		currState = table.transit(next,currState);
		cout << currState ;
		
		if (table.getState(currState).getFinal()){
			
			tokenFound = true;
			token.setTopos(nbCol, lineNb);
			token.assignType(currState);
			
			if (table.getState(currState).getBacktrack()){charPosition--;}
			
			if(token.getLexeme() == ""){
				token.concatLex(next);
				token.assignType(currState); // in case of 0 ' s type is not 
				}
				
		}else{
			
			if(next != '\n' && next != ' ' ) {
				token.concatLex(next);
			}
		}
	}
	
	cout << ' ' << token.getLexeme() << endl;
	
	return token;
	
}

void Lexer::run_Lexer() {
	
	ifstream in("//home/meta1/Dev/git/compiler/toy_compiler/tools/FSMtable"); 
	table = Table(in);
	
    Token* tokenPtr = new Token;
	
	while (eof == false){
		
		tokenPtr = new Token;
	    *tokenPtr = nextToken();
		tokenPtr->symbolCheck();
	    addToken(tokenPtr);
	}
}

vector<Token> Lexer::returnWords(){
	

	return tokenList;
	
	
}