#pragma once
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <fstream>
#include <iterator>
#include <algorithm> 

 

using namespace std; 
 

int gcount = 0;
 
class Node 
{ 
	
	
public:
        //constructors
		Node(string data, string type, string name, int line) 
						{this->data = data; this->type = type;
						 this->name = name; gcount++; 
						 this->nodeID = gcount; this->line =line ; } 
		
		Node(string data, string type, string name) 
						{this->data = data; this->type = type;
						 this->name = name; gcount++; 
						 this->nodeID = gcount; } 
		
		//methods
		void setParent(Node * node){parent = node;}
		Node* getParent(){return parent;};
		void addChild(Node *  child);
		string getName(){return name;}
		string getType(){return type;}
		void setType(string s){type = s;};
		int getID(){return nodeID;}
		Node* getChildAt(int i){return children.at(i);}
		int nbOfChildren(){return children.size();}
		string print();
		int getLine(){return line;};

private:
        //member var
		string name;
		string data; 
		string type;
		int line;
		int nodeID;
		Node * parent;
		vector<Node*> children;

}; 