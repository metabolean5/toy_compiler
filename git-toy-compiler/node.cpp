
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <fstream>
#include <iterator>
#include <algorithm> 
#include "node.h"




using namespace std;


void Node::addChild(Node * child){
	
	children.push_back(child);
	
}

string Node::print(){
	
	stringstream ss;
	ss <<  "position : [line=" << line << "] id : '" << data << "'";
	return ss.str();
	
}

