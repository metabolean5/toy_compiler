 
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


class Visitor
{
public:
	
	//CONSTRUCTORS
	Visitor();


	//METHODS
	void traverse(Node * parent);
	void traverse2(Node * parent);
	void semanticAction(Node * node);
	void semanticAction2(Node * node);
	string buildType(Node * node);
	void printSymbolTables();
	void traverseSYMT(SymbolTable * global);
	void reportError(string errorType, SymbolTable * symt);
	void hierStackPop(Node * node);
	void printErrors();
	SymbolTable* getSymt(int id);
	void printFreeFuncs();
	void showHIERstack();
	
	//MULTIPLE DEFINITIONS CHECKS
	bool multipleIDcheck(SymbolTable* parent, string id);
	void funcDecCheck(SymbolTable * symt);
	void classDecCheck(SymbolTable * symt);
	void inheritanceCheck(SymbolTable * symt);
	
	
	
	//FOR FUNCTIONS/CLASSES
	void def2dec(SymbolTable * symt);
	void buildInheritance(SymbolTable * parent ,SymbolTable * symt);
	void funcDefCheck();
	void deleteFuncDec(SymbolTable * symt);
	string getReturnType(Node * fname, bool freefunc);
	bool checkParams(SymbolTable * symt, Node * node);
	
	//FOR STATEMENTS
	bool checkIfDefined(Node * node);
	string typeCheck(Node * node);
	void checkIfFuncCall(Node * node);
	void checkIndex(Node * node);
	
	//FOR ALL ENTRIES
	string typeLookUp(SymbolTable * symt, Node * node);

	
	
private:

    SymbolTable * global;
	stack <SymbolTable*>  hierStack;
	vector<SymbolTable*> funcDecs;
	vector<SymbolTable*> classDecs;
	vector<string> errors;
	vector<SymbolTable*> freeFuncs;
	
	//UTILITIES / GLOBAL VAR
	
	string CTYPE;
	bool FCALL = false;
	bool OPERAND = false;

};