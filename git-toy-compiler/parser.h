 
#pragma once
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <fstream>
#include <iterator>
#include <algorithm> 
#include <stack>
#include "token.h"
#include "node.h"

using namespace std;


class Parser 
{
	
public:
	
	//CONSTRUCTORS
	Parser();
	Parser(vector<Token> wordL);
	
	
	//METHODS
	void initialize(vector<Token> wordL);
	Token getWordAt(int p){ return wordList.at(p);}
	bool run();
	Token nextWord();
	Token prevWord(int n);
	Token goBackNwords(int n);
	bool backTack(Token &a, string &x);
	vector<string> syntaxTable(string production , string word);
	bool isTerminal(string word);
	int getProdIndex(string prod);
	int getWordIndex(string w);
	void showstack();
	void pushRHS(vector<string> rhs);
	void push(string r){ parseStack.push(r);}
	void writeDerivation(string x , vector<string> rhs );
	void skipErrors(Token a);
	
	//METHODS FOR AST SEMANTIC TREE
	void showSEMstack();
	void semanticRecord(string x, Token a);
	void makeNode(Token a, string namerec);
	void makeFamilyN2(string op, string namerec);
	void makeFamilyN(string op, string namerec, string namrec2 , int limit);
	void attach(Token a, string namerec);
	void injectSign(Token a, string namerec);
	void traverse(Node * root);
	void setASTroot(Node * root){ASTroot = root;};
	Node * getASTroot(){return ASTroot;};


private:
    
	//member vars
	vector<Token> wordList;
	vector<string> terminalSet;
	vector<vector<string>> rules;
	int wordPosition = -1;
    vector<vector<int>> syntaxMatrix;
	stack <string> parseStack;
	stack <Node*>  semanticStack;
	stack <string> opStack;
	bool error = false;
	Node * ASTroot;
	
	
	//code utilities
	
	
};
 

