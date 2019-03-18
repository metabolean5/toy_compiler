 
#pragma once
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <fstream>
#include <iterator>
#include <algorithm> 
#include "state.h"
#include "table.h"


using namespace std;

 
class Table
{
public:
	
	//CONSTRUCTORS
	Table();
	Table(ifstream &fsmTable);

	
	//METHODS
	void printTable();
	int transit(char next, int currState);
	State getState(int i);

	
private:
    
   vector<State*> states;
   int nbOfRows = 0;
   

	
};