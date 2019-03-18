#include <stdio.h>
#include <streambuf>
#include <algorithm>  
#include <string>
#include <fstream>
#include <iostream>
#include "lexer.h"
#include "lexer.cpp"
#include "parser.h"
#include "parser.cpp"
#include "token.h"
#include "visitor.h"
#include "visitor.cpp"



using namespace std;


int main()
{

	
	ifstream t("//home/meta1/Dev/git/compiler/toy_compiler/progSamples/sampleSem"); 
	
	if(!t){

		cout<< "Cannot open input file!" << endl;
		return 1;
	}
	
	
    //LEXICAL ANALYSIS
	Lexer lexer(t);
	lexer.run_Lexer();
	//lexer.displayTokenList();
	vector<Token> wordList = lexer.returnWords();


    //SYNTAX ANALYSIS
	Parser parser(wordList);
	bool success = parser.run();
	Node * ast = parser.getASTroot();
	parser.traverse(ast);
	if (success){cout << "\nsuccessfuly parsed\n";}
	
	//SEMANTIC ANALYSIS
	Visitor * vis = new Visitor();
	cout << "\n\n1ST PASS\n\n";
	vis->traverse(ast);
	vis->printSymbolTables();
	vis->printErrors();
	
	cout << "\n\n2ND PASS\n\n";
	vis->traverse2(ast);
	vis->printErrors();
	
	return 0;
}
