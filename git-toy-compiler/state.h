#pragma once
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <fstream>
#include <iterator>
#include <algorithm> 

 

using namespace std;

class State
{
public:
     
	 State();
	 State(int anb){stateNb = anb;};
	 
	 void setFinalBool(bool b);
	 void setBacktrack(bool b);
	 bool getFinal() { return isFinal;}
	 bool getBacktrack() { return backtrack;}
	 void addTransition(int i){ transitions.push_back(i);}
	 int getTransition(int i) { return transitions.at(i);}

private:
     
	 vector<int> transitions;
	 int stateNb;
	 bool isFinal;
	 bool backtrack;

};
