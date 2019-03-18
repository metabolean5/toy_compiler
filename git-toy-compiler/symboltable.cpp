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
#include "symboltable.h"

using namespace std;

SymbolTable::SymbolTable(){
	
} 

SymbolTable::SymbolTable(string name, string kind , string type, Node * node){
	
	this->name = name;
	this->kind = kind;
	this->type = type;
	this->node = node;
	this->id = node->getID();
	
} 

void SymbolTable::addEntry(SymbolTable * entry){
	
	entries.push_back(entry);
	
}

void SymbolTable::addInherited(SymbolTable * parent){
	inheritance.push_back(parent);
	
}

void SymbolTable::deleteEntry(int i){
	
	entries.erase(entries.begin() + i);
	
}

void SymbolTable::outEntry(){
	
	cout << "[line] : " << line << "\t[name] : " << name << "\t[kind]: " << kind <<  "\t[type]: " << type  ;
	if (leaf == true) {cout << "\t X";}
	cout << endl;
	
}

string SymbolTable::printEntry(){
	
	stringstream ss;
	ss << "[line] : " << line << "\t[name] : " << name << "\t[kind]: " << kind <<  "\t[type]: " << type  ;
	return ss.str();
	
}
	
void SymbolTable::print(){
	
	cout << "-------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
	cout << "SYMBOL TABLE: " <<  name << endl;
	
	for (int i = 0 ; i < entries.size() ; i++){
		entries.at(i)->outEntry();
	}

    cout << "-------------------------------------------------------------------------------------------------------------------------------------------------------\n\n";
}