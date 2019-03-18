#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include <algorithm>  
#include <stdio.h>
#include <stack>
#include "parser.h"
#include "token.h"
#include "node.h"
#include "node.cpp"
#include "symboltable.h"


using namespace std;



void Parser::initialize(vector<Token> wordL){
	
	wordList  = wordL;
	
	terminalSet = { "main",";","class","id","{","}",":",",","(",")","sr","for",
					"if","then","else","return","write","read","+","-",".",
					"floatNum","intNum","not","[","]","integer","float","=",
					"eq","neq","lt","gt","leq","geq","or","*","/","and","$"};
	

	
	rules = { { "" },
		      { "REP1", "REP2" , "main" , "~Kmain", "funcBody", "~FbodyN" , "~Attach" , "~Fprog" , ";" },
	          { "classDec" , "REP1" },
			  { "EPSILON" },
			  { "funcDef", "REP2" },
			  { "EPSILON" },
			  { "class", "id" , "~K" , "~inherList", "OP1",  "~Ksblock", "{" , "REP3" , "REP4" , "}",  "~Fsblock"  , "~Fclass" , ";" },
			  { ":" , "id" , "~K" , "~Attach", "REP6" },
			  { "EPSILON" },
			  { ",", "id" , "~K" , "~Attach","REP6" },
			  { "EPSILON" },                       //10
			  { "varDec" , "REP3"},
			  { "EPSILON",},
			  { "funcDec" , "REP4"},
			  { "EPSILON" },
			  { "type"  , "~Ktype", "id", "~K" ,  "~Params", "(" , "fParams" , ")" , "~FfuncN3", ";" },
			  { "type", "~Ktype",  "OP2", "id", "~K", "~Params", "(", "fParams", ")"},
				{  "id" , "~K", "sr"},
				{ "EPSILON" },
				{ "funcHead", "~Ksblock",  "funcBody",  "~Fsblock" , "~Ffuncdef" ,";" },
				{ "{", "REP7", "REP78", "}"}   ,                                                    //20
				{ "statement", "REP78"},
				{ "EPSILON" },
				{ "varDec", "REP7"},
				{ "EPSILON" },
				{ "statement", "REP8"},
				{ "EPSILON" },
				{ "type", "~K"  , "id", "~K" ,"REP9", "~FDim" , "~FVARrn3" ,";"},
				{ "arraySize", "REP9"},
				{ "EPSILON" },
				{ "for", "(", "type", "~Ktype", "id", "~K" ,"assignOp", "expr", ";" , "relExpr",  "~Freln2", ";", "assignStat", ")",  "~Ksblock" , "statBlock",  "~Fsblock" ,"~Ffor", ";" }, //30
				{ "if", "(", "expr" , ")", "then", "~Ksblock", "statBlock" , "~Fsblock", "else", "~Ksblock", "statBlock", "~Fsblock", "~Fif" , ";"  },
				{ "return", "(", "expr", ")",  "~Freturn" ,";"},
				{ "write", "(", "expr", ")",  "~Fwrite",  ";"},
				{ "read", "(", "variable", ")", "~Fread" ,";"},
				{ "assignStat", ";"},
				{ "variable", "assignOp", "expr" , "~F=n2"},
				{ "{", "REP10", "}" },
				{ "statement"},
				{ "EPSILON" },
				{ "statement", "REP10"},
				{ "EPSILON" },
				{ "arithExpr", "expr1"},
				{ "relOp", "arithExpr", "~Freln2"},
				{ "EPSILON" },
				{ "arithExpr", "relOp", "arithExpr"},
				{ "term", "arithExpr1"},
				{ "addOp","term", "~F+n2", "arithExpr1"},
				{ "EPSILON" },
				{ "factor", "term1"},
				{ "multOp", "factor", "~F*n2","term1"},
				{ "EPSILON" },
				{ "+"},
				{ "-"},
				{ "id","~K","REP11", "."},
				{ "id","~K","REP11", ".", "REP12"},
				{ "EPSILON" },
				{ "(", "arithExpr", ")"},
				{ "floatNum","~K"},
				{ "intNum","~K"},
				{ "not", "factor" , "~Not"}          ,                                 //60
				{ "FACTO1", ".", "REP12", "id", "~fac" ,  "factor1" , "~Attach", "~Att."},
				{ "sign", "factor" , "~Sign"},
				{ "REP12"},
				{ "~Params",  "(", "aParams", ")" , "~Attach"},
				{ "FACTO1", "." , "REP12", "id", "REP12"},
				{ "EPSILON" , "id" , "~K" ,  "REP12"},
				{ "indice", "REP12"},
				{ "EPSILON" },
				{ "id","~K", "REP11"},
				{ "FACTO1" , ".", "REP12" , "id", "(", "aParams", ")"},
				{ "EPSILON", "id", "(", "aParams", ")"},
				{ "[", "arithExpr","~Attach", "]"},
				{ "[", "intNum" ,"~Dim" ,"]"},
				{ "integer"},
				{ "float"},
				{ "id"},
				{ "type", "~Ktype" ,"id","~K" ,  "REP13", "~FDim", "~FVARrn3", "~Attach" , "REP14"},
				{ "EPSILON" },
				{ "arraySize", "REP13"},
				{ "EPSILON" },                                              //80
				{ "fParamsTail", "~FVARrn3", "~Attach" , "REP14"},
				{ "EPSILON" },
				{ "expr" , "~Attach" , "REP15"},
				{ "EPSILON" },
				{ "aParamsTail", "~Attach" , "REP15"},
				{ "EPSILON" },
				{ ",", "type", "~Ktype" , "id", "~K" , "REP13", "~FDim"},
				{ ",", "expr"},
				{ "="},
				{ "eq", "~Op"},                                                   //90
				{ "neq", "~Op"},
				{ "lt", "~Op"},
				{ "gt", "~Op"},
				{ "leq", "~Op"},
				{ "geq", "~Op"},
				{ "+", "~Op"},
				{ "-", "~Op"},
				{ "or", "~Op"},
				{ "*", "~Op"},
				{ "/", "~Op"},
				{ "and" , "~Op"},                                               // 101 
			  { "error" },
			  { "error" },
			  { "REP12"},
			  {".", "REP12", "id", "~fac" ,"factor1", "~Attach" , "term1"},
			  
	};
	

syntaxMatrix = {   {1,103,1,1,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,1,1,103,103,103,103,103,103,103,103,103,103,103,102},
				{3,103,2,3,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,3,3,103,103,103,103,103,103,103,103,103,103,103,103},
				{5,103,103,4,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,4,4,103,103,103,103,103,103,103,103,103,103,103,103},
				{102,103,6,102,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,102,102,103,103,103,103,103,103,103,103,103,103,103,103},
				{103,103,103,103,8,103,7,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103},
				{103,103,103,103,10,103,103,9,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103},
				{103,103,103,11,103,12,12,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,11,11,103,103,103,103,103,103,103,103,103,103,103,103},
				{103,103,103,13,103,14,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,13,13,103,103,103,103,103,103,103,103,103,103,103,103},
				{103,103,103,15,103,102,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,15,15,103,103,103,103,103,103,103,103,103,103,103,103},
				{103,103,103,16,102,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,16,16,103,103,103,103,103,103,103,103,103,103,103,103},
				{103,103,103,17,103,103,17,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103},
				{102,103,103,19,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,19,19,103,103,103,103,103,103,103,103,103,103,103,103},
				{103,102,103,103,20,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103},
				{103,103,103,21,103,22,103,103,103,103,103,21,21,103,103,21,21,21,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103},
				{103,103,103,24,103,26,103,103,103,103,103,24,24,103,103,24,24,24,103,103,103,103,103,103,103,103,23,23,103,103,103,103,103,103,103,103,103,103,103,103},
				{103,103,103,25,103,103,103,103,103,103,103,25,25,103,103,25,25,25,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103},
				{103,103,103,27,103,103,102,103,103,103,103,102,102,103,103,102,102,102,103,103,103,103,103,103,103,103,27,27,103,103,103,103,103,103,103,103,103,103,103,103},
				{103,29,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,28,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103},
				{103,102,103,35,103,102,103,103,103,103,103,30,31,103,102,32,33,34,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103},
				{103,102,103,36,103,103,103,103,103,102,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103},
				{103,39,103,38,37,103,103,103,103,103,103,38,38,103,39,38,38,38,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103},
				{103,103,103,40,103,41,103,103,103,103,103,40,40,103,103,40,40,40,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103},
				{103,102,103,42,103,103,103,102,42,102,103,103,103,103,103,103,103,103,42,42,103,42,42,42,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103},
				{103,44,103,103,103,103,103,44,103,44,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,43,43,43,43,43,43,103,103,103,103,103},
				{103,102,103,45,103,103,103,103,45,103,103,103,103,103,103,103,103,103,45,45,103,45,45,45,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103},
				{103,102,103,46,103,103,103,102,46,102,103,103,103,103,103,103,103,103,46,46,103,46,46,46,103,102,103,103,103,102,102,102,102,102,102,103,103,103,103,103},
				{103,48,103,103,103,103,103,48,103,48,103,103,103,103,103,103,103,103,47,47,103,103,103,103,103,48,103,103,103,48,48,48,48,48,48,47,103,103,103,103},
				{103,102,103,49,103,103,103,102,49,102,103,103,103,103,103,103,103,103,49,49,103,49,49,49,103,102,103,103,103,102,102,102,102,102,102,102,103,103,103,103},
				{103,51,103,103,103,103,103,51,103,51,103,103,103,103,103,103,103,103,51,51,105,103,103,103,103,51,103,103,3,51,51,51,51,51,51,51,50,50,50,103},
				{103,103,103,102,103,103,103,103,102,103,103,103,103,103,103,103,103,103,52,53,103,102,102,102,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103},
				{103,103,103,54,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103},
				{103,103,103,55,103,103,103,3,103,103,103,103,103,103,103,103,103,103,103,103,56,103,103,103,104,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103},
				{103,102,103,61,103,103,103,102,57,102,103,103,103,103,103,103,103,103,62,62,103,58,59,60,103,102,103,103,103,102,102,102,102,102,102,102,102,102,102,103},
				{103,3,103,103,103,103,103,102,64,3,103,103,103,103,103,103,103,103,3,3,3,103,103,103,63,102,103,103,103,102,102,102,102,102,102,102,3,102,102,103},
				{103,103,103,66,103,103,103,103,103,66,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,66,103,103,103,103,103,103,103,103,103,103,103},
				{103,68,103,68,103,103,103,68,64,68,103,103,103,103,103,103,103,103,68,68,68,103,103,103,67,68,103,103,68,68,68,68,68,68,68,68,68,68,68,103},
				{103,103,103,69,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,102,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103},
				{103,103,103,71,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103},
				{103,102,103,102,103,103,103,102,103,102,103,103,103,103,103,103,103,103,102,102,102,103,103,103,72,102,103,103,102,102,102,102,102,102,102,102,102,102,102,103},
				{103,102,103,103,103,103,103,102,103,102,103,103,103,103,103,103,103,103,103,103,103,103,103,103,73,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103},
				{103,103,103,76,103,103,102,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,74,75,103,103,103,103,103,103,103,103,103,103,103,103},
				{103,103,103,77,103,103,103,103,103,78,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,77,77,103,103,103,103,103,103,103,103,103,103,103,103},
				{103,103,103,103,103,103,103,80,103,80,103,103,103,103,103,103,103,103,103,103,103,103,103,103,79,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103},
				{103,103,103,103,103,103,103,81,103,82,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103},
				{103,103,103,83,103,103,103,103,83,84,103,103,103,103,103,103,103,103,83,83,103,83,83,83,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103},
				{103,103,103,103,103,103,103,85,103,86,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103},
				{103,103,103,103,103,103,103,87,103,102,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103},
				{103,103,103,103,103,103,103,88,103,102,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103},
				{103,103,103,102,103,103,103,103,102,103,103,103,103,103,103,103,103,103,102,102,103,102,102,102,103,103,103,103,89,103,103,103,103,103,103,103,103,103,103,103},
				{103,103,103,102,103,103,103,103,102,103,103,103,103,103,103,103,103,103,102,102,103,102,102,102,103,103,103,103,103,90,91,92,93,94,95,103,103,103,103,103},
				{103,103,103,102,103,103,103,103,102,103,103,103,103,103,103,103,103,103,96,97,103,102,102,102,103,103,103,103,103,103,103,103,103,103,103,98,103,103,103,103},
				{103,103,103,102,103,103,103,103,102,103,103,103,103,103,103,103,103,103,102,102,103,102,102,102,103,103,103,103,103,103,103,103,103,103,103,103,99,100,101,103}}; 
									   
			}


Parser::Parser() {
	
}

void Parser::skipErrors(Token a){
	
	bool semicolFound = false;
	Token token = nextWord();
	
	ofstream error;
	error.open("error.txt",std::ios_base::app);
	error << "  ERROR at : \n";
	error.close();
	a.printToken(true) ;
    
					
	while (semicolFound == false){
		
		if (token.getLexeme() == ";"){
			semicolFound = true;
		} 
		token = nextWord();
	}
	
	wordPosition = wordPosition - 2;

	while (parseStack.top() != ";"  && parseStack.top() != "REP78" ){
		parseStack.pop();
	}
}

Parser::Parser(vector<Token> wordL){
	initialize(wordL);
}
 
bool Parser::isTerminal(string w){

	for(int i = 0; i < terminalSet.size(); ++i) {
		if(w == terminalSet.at(i)){return true;}
	}
	
	return false;
	
}

void Parser::showstack(){ 
	cout << "STATE OF STACK : ";
	vector<string> temp;
	
    while (!parseStack.empty()) 
    { 
		temp.push_back(parseStack.top()); 
		parseStack.pop();
	
    } 
	
	cout << temp.size() ;
	
	for (unsigned i = temp.size(); i-- > 0; ){
		cout << '\t' << temp.at(i);
		parseStack.push(temp.at(i));
	}
	
    cout << '\n'; 
} 

void Parser::showSEMstack(){ 
	cout << "STATE OF SEMANTIC STACK : ";
	vector<Node*> temp;
	
    while (!semanticStack.empty()) 
    { 
		temp.push_back(semanticStack.top()); 
		semanticStack.pop();
	
    } 
	
	cout << temp.size() ;
	
	for (unsigned i = temp.size(); i-- > 0; ){
		cout << '\t' << temp.at(i)->getType();
		semanticStack.push(temp.at(i));
	}
	
    cout << '\n'; 
} 

Token Parser::nextWord(){
	wordPosition++;
	return getWordAt(wordPosition);
}

Token Parser::prevWord(int n){
	return getWordAt(wordPosition - n);
}

Token Parser::goBackNwords(int n){
	
	for ( int i = 0 ; i < n; i++ ){ wordPosition--;}
	return getWordAt(wordPosition);
	
}

int Parser::getProdIndex(string prod){
	
	if (prod == "prog"){return 0;}
	if (prod == "REP1"){return 1;}
	if (prod == "REP2"){return 2;}
	if (prod == "classDec"){return 3;}
	if (prod == "OP1"){return 4;}
	if (prod == "REP6"){return 5;}
	if (prod == "REP3"){return 6;}
	if (prod == "REP4"){return 7;}
	if (prod == "funcDec"){return 8;}
	if (prod == "funcHead"){return 9;}
	if (prod == "OP2"){return 10;}
	if (prod == "funcDef"){return 11;}
	if (prod == "funcBody"){return 12;}
	if (prod == "REP78"){return 13;}
	if (prod == "REP7"){return 14;}
	if (prod == "REP8"){return 15;}
	if (prod == "varDec"){return 16;}
	if (prod == "REP9"){return 17;}
	if (prod == "statement"){return 18;}
	if (prod == "assignStat"){return 19;}
	if (prod == "statBlock"){return 20;}
	if (prod == "REP10"){return 21;}
	if (prod == "expr"){return 22;}
	if (prod == "expr1"){return 23;}
	if (prod == "relExpr"){return 24;}
	if (prod == "arithExpr"){return 25;}
	if (prod == "arithExpr1"){return 26;}
	if (prod == "term"){return 27;}
	if (prod == "term1"){return 28;}
	if (prod == "sign"){return 29;}
	if (prod == "idnest"){return 30;}
	if (prod == "REP11"){return 31;}
	if (prod == "factor"){return 32;}
	if (prod == "factor1"){return 33;}
	if (prod == "variable"){return 34;}
	if (prod == "REP12"){return 35;}
	if (prod == "FACTO1"){return 36;}
	if (prod == "functionCall"){return 37;}
	if (prod == "indice"){return 38;}
	if (prod == "arraySize"){return 39;}
	if (prod == "type"){return 40;}
	if (prod == "fParams"){return 41;}
	if (prod == "REP13"){return 42;}
	if (prod == "REP14"){return 43;}
	if (prod == "aParams"){return 44;}
	if (prod == "REP15"){return 45;}
	if (prod == "fParamsTail"){return 46;}
	if (prod == "aParamsTail"){return 47;}
	if (prod == "assignOp"){return 48;}
	if (prod == "relOp"){return 49;}
	if (prod == "addOp"){return 50;}
	if (prod == "multOp"){return 51 ;}
}

int Parser::getWordIndex(string w){
	
	if (w == "main"){return 0;}
	if (w == ";"){return 1;}
	if (w == "class"){return 2;}
	if (w == "id"){return 3;}
	if (w == "{"){return 4;}
	if (w == "}"){return 5;}
	if (w == ":"){return 6;}
	if (w == ","){return 7;}
	if (w == "("){return 8;}
	if (w == ")"){return 9;}
	if (w == "sr"){return 10;}
	if (w == "for"){return 11;}
	if (w == "if"){return 12;}
	if (w == "then"){return 13;}
	if (w == "else"){return 14;}
	if (w == "return"){ return 15;}
	if (w == "write"){return 16;}
	if (w == "read"){return 17;}
	if (w == "+"){return 18;}
	if (w == "-"){return 19;}
	if (w == "."){return 20;}
	if (w == "floatNum"){return 21;}
	if (w == "intNum"){return 22;}
	if (w == "not"){return 23;}
	if (w == "["){return 24;}
	if (w == "]"){return 25;}
	if (w == "integer"){return 26;}
	if (w == "float"){return 27;}
	if (w == "="){return 28;}
	if (w == "eq"){return 29;}
	if (w == "neq"){return 30;}
	if (w == "lt"){return 31;}
	if (w == "gt"){return 32;}
	if (w == "leq"){return 33;}
	if (w == "geq"){return 34;}
	if (w == "or"){return 35;}
	if (w == "*"){return 36;}
	if (w == "/"){return 37;}
	if (w == "and"){return 38;}
	if (w == "$"){return 39;}


	
}

vector<string> Parser::syntaxTable(string production , string word){
	
	int pindex = getProdIndex(production);
	int windex = getWordIndex(word);
	
	return rules.at(syntaxMatrix.at(pindex).at(windex));	
}

void Parser::pushRHS(vector<string> rhs){
	for (unsigned i = rhs.size(); i-- > 0; ){
		 push(rhs.at(i));
	}
}

bool Parser::backTack(Token &a, string &x){
	
	
	if ((x == "assignOp"  || x == ")") && (a.getWord() == ".")){  
		parseStack.push("~Att.");
		parseStack.push("~Attach");
		parseStack.push("REP12");
		parseStack.push("~fac");
		parseStack.push("id");
		parseStack.push("REP12");
		parseStack.push(".");
		return true;
		}
	
	
	
	if ((x == "assignOp") && (a.getWord() == "id") ){              
		for ( int i = 0 ; i < 4; i++ ){ parseStack.pop();}
		parseStack.push("REP7");
		parseStack.push("varDec");
		semanticStack.pop();
		a = goBackNwords(1);
	   // cout << "backtracking to REP7 (varDec instead of statement\n";
		return true;
	}
	
	
	if ((x == "sr") && (a.getWord() == "(") ){              
		for ( int i = 0 ; i < 2; i++ ){ parseStack.pop();}
	  //  cout << "backtracking to (\n";
		return true;
	}
	
	if ((x == ".") && (a.getWord() == "eq" || a.getWord() == "neq" || a.getWord() == "lt" ||
		a.getWord() == "leq" || a.getWord() == "geq" ||a.getWord() == "gt" ||  a.getWord() == "+" 
		||  a.getWord() == "-" ||  a.getWord() == "/" ||  a.getWord() == "*" || a.getWord() == "]" 
		||a.getWord() == ";" ||a.getWord() == "," || a.getWord() == ")" )){
		for ( int i = 0 ; i < 6; i++ ){ parseStack.pop();}
		//cout << "backtracking  \n" ;
		return true;
		}
	
	if ((x == "REP11") && (a.getWord() == "eq" || a.getWord() == "neq" || a.getWord() == "lt" ||
		a.getWord() == "leq" || a.getWord() == "geq" ||a.getWord() == "gt" ||  a.getWord() == "+" 
		||  a.getWord() == "-" ||  a.getWord() == "/" ||  a.getWord() == "*" || a.getWord() == "]"  
		|| a.getWord() == ")" || a.getWord() == ";" || a.getWord() == "and" )){
		for ( int i = 0 ; i < 8; i++ ){ parseStack.pop();}
	//	cout << "backtracking to relop \n" ;
		return true;
		}
	
	
	if ((x == "REP11") && (a.getWord() == "(") ){               // giving priority to funcDec
		for ( int i = 0 ; i < 7; i++ ){ parseStack.pop();}
	  //  cout << "backtracking to factor1\n";
	    parseStack.push("factor1");
	    parseStack.push("~Attach");
		parseStack.push(")");
		parseStack.push("aParams");
		parseStack.push("(");
		parseStack.push("~Params");
		return true;
	}
	
	
	if ((x == "REP9") && (a.getWord() == "(") ){               // giving priority to funcDec
		for ( int i = 0 ; i < 5; i++ ){ parseStack.pop();}
		a = goBackNwords(2);
		for ( int i = 0 ; i < 2; i++ ){ semanticStack.pop();}
	    cout << "backtracking to funcDec\n";
		return true;
	}
	return false;
}

void Parser::writeDerivation(string x , vector<string> rhs ){
	
	ofstream o;
	o.open("derivation1.txt",std::ios_base::app);
	
	cout << "applied " << x << " --> " ;
	o   << "applied " << x << " --> " ;
	
	for (unsigned i = rhs.size(); i-- > 0; ){
		cout << '\t' << rhs.at(i);
		o << '\t' << rhs.at(i)  ;
		}
		
	o << "\n\n" ;
	o.close();
}

void Parser::traverse(Node * root) { 
	
  
  ofstream dot;
  dot.open("dotcode.txt",std::ios_base::app);
  
  cout << "Parent : " << root->getName() << " has children : \t";
  dot << root->getID() << "[label = \" " <<   root->getName() << "(" << root->getType() << ") \" ];\n";
  for (int i =  root->nbOfChildren() - 1 ; i >= 0; --i) {
	   dot << root->getID() << "--" << root->getChildAt(i)->getID() << ";\n" ;
	   cout << root->getID() << "--" << root->getChildAt(i)->getID() << ";\n" ;
	   cout << root->getChildAt(i)->getName() << "("<<  root->getChildAt(i)->getType() << ") "<< "\t";
  }
  cout << "\n\n";
  dot.close();
  
  for (int i =  root->nbOfChildren() - 1 ; i >= 0; --i) {
	  traverse(root->getChildAt(i));
  }
} 

void Parser::semanticRecord(string x, Token a){
	
	if (x == "~F+n2"){
		makeFamilyN2(opStack.top(), "artm1");
		opStack.pop();
	}
	
	if (x == "~Freln2"){
		makeFamilyN2(opStack.top(), "relexpr");
		opStack.pop();
	}
	
	if (x == "~F*n2"){
		makeFamilyN2(opStack.top(), "T1s");
		opStack.pop();
	}
	
	if (x == "~F=n2"){
		makeFamilyN2("=", "assignStat");
	}
	
	if (x == "~FVARelmt2"){
		makeFamilyN2("var", "var");
	}
	
	if (x == "~FDim"){
		makeFamilyN("dimensions", "id" , "func", -1 );
	}
	
	if (x == "~Fsblock"){
		makeFamilyN("statblock", "statblock" , "statblock", -1 );
		Node * parent = semanticStack.top();
		semanticStack.pop();
		semanticStack.pop();
		semanticStack.push(parent);
	}
	
	if (x == "~FVARrn3"){
		makeFamilyN("varDec", "varDec", "varDec", 3);
	}
	
	if (x == "~Fprog"){
		makeFamilyN("prog", "$", "$", -1);
	}
	
	
	if (x == "~FfuncN3"){
		makeFamilyN("funcDec", "funcDec", "funcDec", 3);
	}	
	
	
	if (x == "~FbodyN" ){
		makeFamilyN("funcBody", "funcHead", "main", -1);
	}
	
	if (x == "~Fif" ){
		makeFamilyN("if", "if", "if", 3);
	}
	
	if (x == "~Ffor" ){
		makeFamilyN("for", "for", "for", 6);
	}
	
	if (x == "~Ffuncdef" ){
		makeFamilyN("funcdef", "funcdef", "fucdef", 5);
	}
	
	if (x == "~Fclass" ){
		makeFamilyN("classDec", "classDec", "classDec", 3);
	}

	if (x == "~FVRB" ){
		makeFamilyN("vrb", "vrb", "main", -1);
	}

	if (x == "~K"){
		makeNode(a, a.getWord());
	}
	
	if (x == "~Dim"){
		makeNode(a, "dim");
	}

	if (x == "~fac"){
		makeNode(a, "fac");
	}
	
	if (x == "~Ksblock"){
		makeNode(a, "statblock");
	}
	
	if (x == "~Kmain"){
		makeNode(a, "main");
	}
	
	if (x == "~Kfunchead"){
		makeNode(a, "funchead");
	}
	
	if (x == "~Params"){
		makeNode(a, "params");
	}
	
	if (x == "~inherList"){
		makeNode(a, "inherList");
	}

	if (x == "~Ktype"){
		makeNode(a, "type");
	}
	
	if (x == "~Attach"){
		attach(a, "fac");
	}
	
	if (x == "~Sign"){
		injectSign(a, "sign");
	}
	
	if (x == "~Not"){
		injectSign(a, "not");
	}
	
	if (x == "~Fread"){
		injectSign(a, "read");
	}
	
	if (x == "~Fwrite"){
		injectSign(a, "write");
	}
	
	if (x == "~Freturn"){
		injectSign(a, "return");
	}
	
	if (x == "~Op"){
		opStack.push(a.getWord());
	}
	
	if (x == "~Att." &&  getWordAt(wordPosition).getLexeme() == "." ){
		parseStack.push("term1");
	}
}

void Parser::injectSign(Token a, string namerec){

	showSEMstack();

	Node * sign = new Node(namerec, namerec ,namerec);
	sign->addChild(semanticStack.top());
	semanticStack.pop();
	semanticStack.push(sign);
	
	showSEMstack();
	
	}

void Parser::attach(Token a, string namerec){
	
	showSEMstack();
	
	Node * child = semanticStack.top();
	semanticStack.pop();
	semanticStack.top()->addChild(child);
	
	showSEMstack();
	}
	
void Parser::makeNode(Token a, string namerec){
	
	showSEMstack();
	
	Node * child = new Node(a.getLexeme(), namerec , a.getLexeme(), a.getLine());
	semanticStack.push(child);
    
	showSEMstack();
}

void Parser::makeFamilyN2(string op, string namerec){
	
	showSEMstack();
	
	Node * parent = new Node(op , namerec , op);

	parent->addChild(semanticStack.top());
	semanticStack.pop();
	parent->addChild(semanticStack.top());
	semanticStack.pop();
	
	semanticStack.push(parent);
	
	showSEMstack();
	
}

void Parser::makeFamilyN(string op, string namerec, string namerec2, int limit){
	
	Node * parent = new Node(op , op , op);
	cout << endl;
	
	while (semanticStack.top()->getType() != namerec && semanticStack.top()->getType() != namerec2  && limit != 0 ){
		showSEMstack();
		parent->addChild(semanticStack.top());
		semanticStack.pop();
		limit--;
	}
	
	semanticStack.push(parent);
	cout << endl;
	
}

bool Parser::run(){
	
	semanticStack.push(new Node("$","$","$"));
	parseStack.push("$");
	parseStack.push("prog");

	Token a = nextWord();
	string x = "";
	bool inVardec = false;
	
	while ( parseStack.top() != "$" ){
		
		cout << "\na is now " << a.getWord() << "\n";
		cout << "X is now " << x << "\n";
		a.printToken(false) ;
		cout << "\n\n" ;
		cout << "\n";
    	showstack();
		//cin.get();
		
		x = parseStack.top();
		
		if (x[0] == '~'){
			parseStack.pop();
			semanticRecord(x, prevWord(1));
			showSEMstack() ;
			showstack();
			continue;
		}
		
		if (isTerminal(x)){
			
			if (x == a.getWord()) {

				parseStack.pop();
			 	cout << "consumed : " << a.getWord() << endl;
				if(parseStack.top() != "$"){ a = nextWord();}
					
			}else{
				
			    if (backTack(a,x) == false ){ 
					
					cout << "\nSYNTAX ERROR DETECTED\n";
					skipErrors(a);
					error = true;
					a = nextWord();
					
					
				}
			}
			
		}else{
			
			if (syntaxTable(x,a.getWord()).at(0) != "error" ) {
				
				parseStack.pop();
				
				if ( syntaxTable(x,a.getWord()).at(0) != "EPSILON") {
					pushRHS(syntaxTable(x,a.getWord()));
					
				}
				
				if ( syntaxTable(x,a.getWord()).at(0) == "EPSILON" && x == "variable"){
						pushRHS(syntaxTable(x,a.getWord())); 
						parseStack.pop();
				}
				
				writeDerivation(x, syntaxTable(x,a.getWord()) );
				
			}else{

				if (backTack(a,x) == false ){ 
					
					cout << "\nSYNTAX ERROR DETECTED\n";
					skipErrors(a);
					error = true;
					a = nextWord();
					
				}
			}
		}
	}
	
	if (parseStack.top() != "$" || error == true){
		
		cout << "\n SYNTAX ERROR "<< endl;
		
	}else {
		
		cout << "\nAST Tree preorder traversal : "<< endl;
		setASTroot(semanticStack.top());
		
		return true;
		
		}
}