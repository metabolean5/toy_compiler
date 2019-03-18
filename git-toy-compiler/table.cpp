
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <fstream>
#include <iterator>
#include <algorithm> 
#include <regex>
#include "table.h"
#include "state.h"
#include "state.cpp"

using namespace std;



Table::Table() {
	
}


//PARSE TABLE IN CONSTRUCTOR
Table::Table(ifstream &fsmTable){

	string line;
	
	// count states
	bool end = false;
	
	while (getline(fsmTable, line) && (end==false) ){
		
		cout << line << endl;
		nbOfRows++;
		if (line == "END" ) {end = true;}
		
		}
		
	states.reserve(nbOfRows-1);
	cout << "\nNUMBER OF STATES IS " << nbOfRows - 1 << endl;
	
	
	fsmTable.clear();
	fsmTable.seekg(0, ios::beg);
	
	
	int snb = 0;
	end = false;
	while (getline(fsmTable, line) && (end==false) ){
		
		states.push_back(new State(snb));
		
		line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
		if (line == "END") {end = true; break;}
		
        // CHECK IF FINAL STATE				
	    line.erase(0, line.find("#")+1);
		string finalSS = line.substr(0, line.find("#"));
		
		
		if( finalSS == "1"){
			states.at(snb)->setFinalBool(true);
		}else{
			states.at(snb)->setFinalBool(false);
		}
		
		// CHECK IF BACKTRACK
		line.erase(0, line.find("#")+1);
		finalSS = line.substr(0, line.find("#"));
		
		if( finalSS == "1"){
			states.at(snb)->setBacktrack(true);
		}else{
			states.at(snb)->setBacktrack(false);
		}
		
	
		// TRANSITION FOR CHAR nonzero
		line.erase(0, line.find("#")+1);
		finalSS = line.substr(0, line.find("~"));
		int q = stoi(finalSS);
		states.at(snb)->addTransition(q);

		bool endline =  false;
		while (endline == false){
			line.erase(0, line.find("~")+1);
			finalSS = line.substr(0, line.find("~"));
			
			if (finalSS == "e16" || finalSS == "e26" ){
				endline = true;
				break;
				}
				
			q = stoi(finalSS);
			states.at(snb)->addTransition(q);
		}
		snb++;
	}
	
	//cout << "verify " << states.at(1)->getTransition(2) << endl;
}


int Table::transit(char next,int currState){
	
	string strChar(1, next);
	
    regex nonzero("[1-9]");
	regex nonEletter("[a-df-zA-Z]");

	if(regex_match(strChar, nonzero)) {return states.at(currState)->getTransition(0);}
	if (next == '0'){return states.at(currState)->getTransition(1);}
	if(regex_match(strChar, nonEletter)) {return states.at(currState)->getTransition(2);}
	if (next == '+'){return states.at(currState)->getTransition(3);}
	if (next == '_'){return states.at(currState)->getTransition(4);}
	if (next == '-'){return states.at(currState)->getTransition(5);}
	if (next == ' '){return states.at(currState)->getTransition(6);}
	if (next == '.'){return states.at(currState)->getTransition(7);}
	if (next == 'e'){return states.at(currState)->getTransition(8);}
	if (next == '\n'){return states.at(currState)->getTransition(9);}
	if (next == '='){return states.at(currState)->getTransition(10);}
	if (next == '<'){return states.at(currState)->getTransition(11);}
	if (next == '>'){return states.at(currState)->getTransition(12);}
	if (next == ';'){return states.at(currState)->getTransition(13);}
	if (next == ','){return states.at(currState)->getTransition(14);}
	if (next == '.'){return states.at(currState)->getTransition(15);}
	if (next == ':'){return states.at(currState)->getTransition(16);}
	if (next == '*'){return states.at(currState)->getTransition(17);}
	if (next == '!'){return states.at(currState)->getTransition(18);}
	if (next == '/'){return states.at(currState)->getTransition(19);}
	if (next == '&'){return states.at(currState)->getTransition(20);}
	if (next == '|'){return states.at(currState)->getTransition(21);}
	if (next == '('){return states.at(currState)->getTransition(22);}
	if (next == ')'){return states.at(currState)->getTransition(23);}
	if (next == '['){return states.at(currState)->getTransition(24);}
	if (next == ']'){return states.at(currState)->getTransition(25);}
	if (next == '{'){return states.at(currState)->getTransition(26);}
	if (next == '}'){return states.at(currState)->getTransition(27);}
}


State Table::getState(int i){
	return *states.at(i);
}
//void Table::printTable(){
//	
//	while (getline(infile, line)){
//		cout << line << endl;
//		}
//	
//	
//}
//
