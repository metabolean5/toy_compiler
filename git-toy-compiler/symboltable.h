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


 
class SymbolTable
{
public:
	
	//CONSTRUCTORS
	SymbolTable();
	SymbolTable(string name, string kind , string type, Node * node);

	//METHODS
	void setParent(SymbolTable * node){parent = node;};
	SymbolTable* getParent(){return parent;};
	void addEntry(SymbolTable * entry);
	void deleteEntry(int i);
	vector<SymbolTable*> getEntries(){return entries;};
	void print();
	string printEntry();
	void outEntry();
	void setLeaf(){leaf = true;};
	bool isLeaf(){return leaf;};
	string getName(){return name;};
	string getType(){return type;};
	void setName(string nname){name = nname;};
	void setMemberClass(string cname){className = cname;};
	string getClassName(){return className;};
	Node* getNode(){return node;};
	int getLine(){return line;};
	void setLine(int l){line = l;};
	int getID(){return id;};
	
	//FOR FUNCTION TYPE
	bool isDefined(){return defined;};
	bool isFreeFunc(){return freeFunc;};
	void setDefined(){defined = true;};
	void setFreeFunc(){freeFunc = true;};

	//FOR CLASS TYPE
	void addInherited(SymbolTable * parentClass);
	vector<SymbolTable*> getInheritance(){return inheritance;};
	
	
private:

   SymbolTable * tablesRoot;
   SymbolTable * parent = NULL;
   string name;
   string kind;
   string type;
   vector<SymbolTable*> entries;
   vector<SymbolTable*> hierarchy;
   int nbOfRows = 0;
   bool leaf = false;
   string className;
   Node * node;
   int line;
   int id;

   //FOR FUNCTION TYPE
   bool defined = false;
   bool freeFunc = false;
   
   //FOR CLASS TYPE
   vector<SymbolTable*> inheritance;
   

  
};