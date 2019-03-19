 
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <streambuf>
#include <algorithm>  
#include <stdio.h>
#include <stack>
#include "parser.h"
#include "token.h"
#include "node.h"
#include "symboltable.h"
#include "symboltable.cpp"



Visitor::Visitor(){
	
} 

void Visitor::showHIERstack(){ 
	
	cout << "STATE OF hierarchy STACK : ";
	
	vector<SymbolTable*> temp;
	
    while (!hierStack.empty()) 
    { 
		temp.push_back(hierStack.top()); 
		hierStack.pop();
	
    } 
	
	cout << temp.size() ;
	
	for (unsigned i = temp.size(); i-- > 0; ){
		cout << '\t' << temp.at(i)->getName();
		hierStack.push(temp.at(i));
	}
	
    cout << '\n'; 
} 


bool Visitor::checkParams(SymbolTable * symt, Node * node){
	

	string type = typeLookUp(symt->getParent(), node->getParent());
	stringstream ss;
	ss << type << ":" ;
	
	string prevCTYPE = CTYPE;

		for (int i =  node->nbOfChildren() - 1 ; i >= 0; i--) { // parameter type
			ss << typeCheck(node->getChildAt(i));
			ss << "," ;
		}
	
	CTYPE = prevCTYPE; //this is because CTYPE is set to the parameter type checking type on the line just above
	
	if (symt->getType() != ss.str()){
		SymbolTable * error = new SymbolTable(node->getName(), "variable" , node->getName(), node);
		error->setLine(node->getLine());
		reportError("invalid parameter list for function call ", error);
	}
	
	return true;
	
}

string Visitor::getReturnType(Node * node, bool freefunc){
	
	if(freefunc){
		for(int i = 0 ; i <  freeFuncs.size()  ; ++i) { // look for function def in free functions
			
			if(freeFuncs.at(i)->getName() == node->getName()){
				
				string s = freeFuncs.at(i)->getType();
				checkParams(freeFuncs.at(i) , node->getChildAt(0));
				FCALL = true;
				return s.substr(0, s.find(":"));
			}
		}
		
	}else{
		
		for(int i = 0 ; i <  funcDecs.size()  ; ++i) { // look for function def
			
			if((funcDecs.at(i)->getName() == node->getName()) && 
			   (typeLookUp(hierStack.top(), node->getParent()) == funcDecs.at(i)->getType().substr(0, funcDecs.at(i)->getType().find(":")))){
				
				checkParams(funcDecs.at(i), node->getChildAt(0));
				
				string s = funcDecs.at(i)->getType();
				FCALL = true;
				return s.substr(0, s.find(":"));
			}
		}
		
		for (int i = 0 ; i < classDecs.size() ; ++i){//look for class declaration in local table
		
			cout << "\n\n<3> " << node->getParent()->getName()   <<" " << typeLookUp(hierStack.top(), node->getParent()) << " " <<  classDecs.at(i)->getName() ;
			
			if( typeLookUp(hierStack.top(), node->getParent()) ==  classDecs.at(i)->getName()){
				
				for (int j = 0 ;  j  < classDecs.at(i)->getInheritance().size() ; ++j){ // look for inherited classes + class def
					
					for (int k = 0 ; k < classDecs.at(i)->getInheritance().at(j)->getEntries().size() ; ++k){//look for data member in inherited classes + class def
		
						if (classDecs.at(i)->getInheritance().at(j)->getEntries().at(k)->getName() == node->getName() ){
							
							string s = classDecs.at(i)->getInheritance().at(j)->getEntries().at(k)->getType();
							FCALL = true;
							return s.substr(0, s.find(":"));
							
						}
					}
				}
			}
		}
	}
	
	cout << "\n\n<4>";
	
	SymbolTable * error = new SymbolTable(node->getName(), "variable" , node->getName(), node);
	error->setLine(node->getLine());

	reportError("invalid function call or data member", error );
	
	return "invalid";
	
			
}

void Visitor::checkIfFuncCall(Node * node){
			
	for (int i = 0 ; i < node->nbOfChildren() ; ++i){
		
		
		if( node->getChildAt(i)->getType() == "params"){
			
			node->setType(getReturnType(node, true));
			FCALL = true;
			OPERAND = true;
		}

		if(node->getChildAt(i)->getType() == "fac"){
			
			node->setType(getReturnType(node->getChildAt(i), false));
			FCALL = true;
			OPERAND = true;
		}
		
		
		
	}	
}

SymbolTable* Visitor::getSymt(int id){
	
	for(int i = 0 ; i <  funcDecs.size()  ; ++i) {
		
		if(funcDecs.at(i)->getID() == id){
			
			funcDecs.at(i)->print();
			return funcDecs.at(i);
		}
	}
}

void Visitor::printFreeFuncs(){
	
	for(int i = 0 ; i <  freeFuncs.size()  ; ++i) {
	
		cout << freeFuncs.at(i)->printEntry();
	}

}

void Visitor::printErrors(){
		
    int count = 0; 	
	
	for(int i = 0 ; i <  errors.size() ; ++i){
		cout << errors.at(i) << endl;
		count++;
	}
	
	if(count > 0){exit(1);}
}

void Visitor::reportError(string errorType, SymbolTable * symt){
	
	stringstream ss;
	ss << "ERROR : ["<< errorType << "] " << symt->printEntry() ;
	
	string error = ss.str();
	errors.push_back(error);
}

bool Visitor::multipleIDcheck(SymbolTable * parent, string id){
	
	if (parent == NULL){return true;}
	
	for(int i = 0 ; i <  parent->getEntries().size(); ++i) {
		
		if (parent->getEntries().at(i)->getName() == id){
			reportError("multiple identifier", parent->getEntries().at(i));
		}
	}
	
	multipleIDcheck(parent->getParent(), id);

	
}

void Visitor::deleteFuncDec(SymbolTable * symt){
	
	for(int i = 0 ; i <  funcDecs.size()  ; ++i) {
		
		if(funcDecs.at(i)->getName() == symt->getName() && funcDecs.at(i)->getClassName() ==  symt->getClassName() ){
			funcDecs.erase(funcDecs.begin() + i);
		}
	}

}

bool Visitor::checkIfDefined(Node * node){

	for(int h = 0 ; h  < node->nbOfChildren() ; ++h){
		
		if(node->getChildAt(h)->getType() == "id"){
			
			bool berr = true;
	
			if(hierStack.top()->isFreeFunc()){ //loop inside free function scope
			
				for(int i = 0; i < hierStack.top()->getEntries().size() ; ++i){
					
					if (node->getChildAt(h)->getName() == hierStack.top()->getEntries().at(i)->getName()){
						berr = false;
					}
				}
					
				for(int i = 0 ; i <  freeFuncs.size()  ; ++i) { // look for id in free functions
			
					if(freeFuncs.at(i)->getName() == node->getChildAt(h)->getName()){
						berr = false;
					}
				}
				
			}else{ //inside member function, check for inherited data members + local scope
				
				for(int i = 0; i < hierStack.top()->getEntries().size() ; ++i){ //local scope
					
					if (node->getChildAt(h)->getName() == hierStack.top()->getEntries().at(i)->getName()){
						berr = false;
					}
				}
				
				for(int i = 0; i < hierStack.top()->getParent()->getInheritance().size() ; ++i){ // loop inside list of inheritance class
					
					for (int j = 0 ; j < hierStack.top()->getParent()->getInheritance().at(i)->getEntries().size(); ++j ) { // loop inside each inheritance class entry
						
						if(node->getChildAt(h)->getName() == hierStack.top()->getParent()->getInheritance().at(i)->getEntries().at(j)->getName()){
							berr = false;
						}
					}
				}
			}
			
			if(berr == true){
				SymbolTable * error = new SymbolTable(node->getChildAt(h)->getName() , "variable/function" , "invalid", node->getChildAt(h));
				error->setLine(node->getChildAt(h)->getLine());
				reportError("undeclared identifier", error);
			}
		}
		checkIfDefined(node->getChildAt(h));
	}
	return true;
}

void Visitor::def2dec(SymbolTable * symt){
	
	bool error1 = true;
	bool error2 = true;
	
	for(int i = 0 ; i <=  classDecs.size() - 1 ; ++i) { //look for belonging class
		
		if(symt->getClassName() == classDecs.at(i)->getName()){ 
	
			for(int j = 0 ; j <=  classDecs.at(i)->getEntries().size() - 1 ; ++j) { //look for matching function
				
				if(symt->getName() == classDecs.at(i)->getEntries().at(j)->getName()){
				
					if (symt->getType() != classDecs.at(i)->getEntries().at(j)->getType() ){
						reportError("could not match with function declaration", symt);
					}
					if (classDecs.at(i)->getEntries().at(j)->isDefined()){
						reportError("multiple definitions (member function)", symt);
					}else{
						
					symt->setParent(classDecs.at(i)->getEntries().at(j)->getParent());
					deleteFuncDec(classDecs.at(i)->getEntries().at(j));
					classDecs.at(i)->deleteEntry(j);
					classDecs.at(i)->addEntry(symt);
					symt->setDefined();
					
					}
					
					error2 = false;
					break;
				}
			}
			error1 = false;
			break;
		}
	}
	  
	if (error1 == true){reportError("member class does not exist", symt);}
	if (error2 == true){reportError("undeclared member function", symt);}

}

void Visitor::buildInheritance(SymbolTable * parent , SymbolTable * symt){
	
	for (int i = 0 ; i < parent->getNode()->getChildAt(1)->nbOfChildren() ; ++i)
		
		for(int j = 0 ; j <  classDecs.size()  ; ++j) {
			
			if(parent->getNode()->getChildAt(1)->getChildAt(i)->getName() == classDecs.at(j)->getName()){
				
				symt->addInherited(classDecs.at(j));
				buildInheritance(classDecs.at(j), symt);
			}
			
		}
}

void Visitor::inheritanceCheck(SymbolTable * symt){
	
	for(int i = 0 ; i < symt->getParent()->getInheritance().size() ; ++i){ // 
		
		for(int j = 0 ; j <  symt->getParent()->getInheritance().at(i)->getEntries().size(); ++j ){
			
			if (symt->getName() == symt->getParent()->getInheritance().at(i)->getEntries().at(j)->getName()){
				
				if( symt->getType() ==  symt->getParent()->getInheritance().at(i)->getEntries().at(j)->getType()){
					reportError("WARNING overwritten member", symt->getParent()->getInheritance().at(i)->getEntries().at(j));
				}else{
					reportError("inherited data member type mismatch", symt->getParent()->getInheritance().at(i)->getEntries().at(j));
				}
			}
		}
	}
}

void Visitor::funcDecCheck(SymbolTable * symt){
	
	
	for(int i = 0 ; i <  funcDecs.size() ; ++i) {
		
		if(funcDecs.at(i)->getName() == symt->getName()){
			reportError("multiple definition (free function)", symt);
		}
		
	}
}

void Visitor::funcDefCheck(){
	
	for(int i = 0 ; i <  funcDecs.size() ; ++i) {
		
		if(!funcDecs.at(i)->isDefined()){
			reportError("undefined function declaration", funcDecs.at(i));
		}
		
	}
}

void Visitor::classDecCheck(SymbolTable * symt){
	
	for(int i = 0 ; i <  classDecs.size()  ; ++i) {
		
		if(classDecs.at(i)->getName() == symt->getName()){
			reportError("multiple definition (class)", symt);
		}
		
	}
}

void Visitor::hierStackPop(Node * node){
	
	if (node->getType() == "classDec" || node->getType() == "funcdef" ||   node->getType() == "main" ){ 
		hierStack.pop();
		}
}

void Visitor::printSymbolTables(){
	 cout << "SYMBOL TABLES\n\n";
	 traverseSYMT(global);
}

void Visitor::traverseSYMT(SymbolTable * table){
	
	if(table->isLeaf() == false){
		
		table->print();
		for (int i = 0 ; i < table->getEntries().size() ; i++){
			
			traverseSYMT(table->getEntries().at(i));
			
		}
	}
}

string Visitor::buildType(Node * node){
	
	if (node->getType() == "funcdef"){
		
		string type = node->getChildAt(4)->getName();
		stringstream ss;
		ss << type << ":" ;
		
		for (int i =  node->getChildAt(1)->nbOfChildren() - 1 ; i >= 0; i--) { // parameter type
			
			ss << node->getChildAt(1)->getChildAt(i)->getChildAt(2)->getName();
			
			for (int j = node->getChildAt(1)->getChildAt(i)->getChildAt(0)->nbOfChildren() -1 ; j >= 0 ; j--){ // for dimensions
			
				ss << "[" << node->getChildAt(1)->getChildAt(i)->getChildAt(0)->getChildAt(j)->getName() << "]";
			}
		
			ss << "," ;
		}
		
		return ss.str();
	}
	
	//BUILD TYPE FOR FUNDEC
	
	if (node->getType() == "funcDec"){
		string type = node->getChildAt(2)->getName();
		
		stringstream ss;
		ss << type << ":" ;
		
		for (int i =  node->getChildAt(0)->nbOfChildren() - 1 ; i >= 0; i--) { // parameter type
			
			ss << node->getChildAt(0)->getChildAt(i)->getChildAt(2)->getName();
			
			for (int j = node->getChildAt(0)->getChildAt(i)->getChildAt(0)->nbOfChildren() -1 ; j >= 0 ; j--){ // for dimensions
			
				ss << "[" << node->getChildAt(0)->getChildAt(i)->getChildAt(0)->getChildAt(j)->getName() << "]";
			}
			
			ss << "," ;
		}
		
		return ss.str();
	}
	
	
	//BUILD TYPE FOR VARIABLE
	if (node->getType() == "varDec"){
		string type = node->getChildAt(2)->getName();
		
		stringstream ss;
		ss << type << ":" ;
		
		for (int i =  node->getChildAt(0)->nbOfChildren() - 1 ; i >= 0; i--) { // parameter type
				ss << "[" << node->getChildAt(0)->getChildAt(i)->getName() << "]";
			}
			
			ss << "," ;
		
		return ss.str();
	}
}

void Visitor::traverse(Node * node){

	semanticAction(node);
	 
	cout << "Parent : " << node->getName() << " has children : \t";
	  
	for (int i =  node->nbOfChildren() - 1 ; i >= 0; --i) {
		cout << node->getChildAt(i)->getName() << "("<<  node->getChildAt(i)->getType() << ") "<< "\t";
	  }
	  
	cout << "\n\n"; 
	  
	for (int i =  node->nbOfChildren() - 1 ; i >= 0; --i) {
		  node->getChildAt(i)->setParent(node);
		  traverse(node->getChildAt(i));
	  }

	hierStackPop(node);
	
		
} 

void Visitor::traverse2(Node * node){
	
	semanticAction2(node);
	
	cout << "Parent : " << node->getName() << " has children : \t";
	  
	for (int i =  node->nbOfChildren() - 1 ; i >= 0; --i) {
		cout << node->getChildAt(i)->getName() << "("<<  node->getChildAt(i)->getType() << ") "<< "\t";
	  }
	  
	cout << "\n\n"; 
	  
	for (int i =  node->nbOfChildren() - 1 ; i >= 0; --i) {
		  node->getChildAt(i)->setParent(node);
		  traverse2(node->getChildAt(i));
	  }
	  
	hierStackPop(node);
	
}

void Visitor::semanticAction(Node * node){
	
	if (node->getType() == "prog"){
		SymbolTable * symt = new SymbolTable("Global", "Global" , "", node);
			cout << "\n\n ADDED " << symt->printEntry() << "\n\n";	hierStack.push(symt);
		global = symt;
	}
	
	if (node->getType() == "classDec"){
		
		SymbolTable * symt = new SymbolTable(node->getChildAt(2)->getName(), "class" ,"" , node);
		symt->setLine(node->getChildAt(2)->getLine());
		symt->setParent(hierStack.top());
		hierStack.top()->addEntry(symt);
		hierStack.push(symt);
		symt->addInherited(symt);
		buildInheritance(symt,symt);
		classDecCheck(symt);
		classDecs.push_back(symt);
	}
	
	if (node->getType() == "funcdef"){
		
		
		SymbolTable * symt = new SymbolTable(node->getChildAt(2)->getName(), "function" , buildType(node) , node);
		symt->setLine(node->getChildAt(2)->getLine());
		
		
		if (node->getChildAt(2)->getName() == node->getChildAt(3)->getName()){
			
			symt->setFreeFunc();
			symt->setParent(hierStack.top());
			symt->setDefined();
			freeFuncs.push_back(symt);
			hierStack.top()->addEntry(symt);
			hierStack.push(symt);
			funcDecCheck(symt);
			
		}else{
			
			symt->setMemberClass(node->getChildAt(3)->getName());
			def2dec(symt);
			hierStack.push(symt);
		}
		funcDecs.push_back(symt);
	}
	
	if (node->getType() == "main"){
		
		funcDefCheck();//verify if all functions declarations are defined.
		
		SymbolTable * symt = new SymbolTable("main", "function" ,"" , node);
		symt->setFreeFunc();
		symt->setLine(node->getLine());
		symt->setParent(hierStack.top());
		funcDecs.push_back(symt);
		hierStack.top()->addEntry(symt);
		hierStack.push(symt);
	}
	
	if (node->getType() == "funcDec"){
		
		SymbolTable * symt = new SymbolTable(node->getChildAt(1)->getName(), "function" , buildType(node), node);
		symt->setLine(node->getChildAt(1)->getLine());
		symt->setParent(hierStack.top());
		symt->setMemberClass(node->getParent()->getParent()->getChildAt(1)->getName());
		inheritanceCheck(symt);
		multipleIDcheck(symt->getParent(), symt->getName());
		hierStack.top()->addEntry(symt);
		funcDecs.push_back(symt);
		
	}
	
	if (node->getType() == "varDec" && node->getParent()->getParent()->getName() != "funcDec" ){
		
		SymbolTable * symt = new SymbolTable(node->getChildAt(1)->getName(), "variable" , buildType(node), node);
		symt->setLine(node->getChildAt(1)->getLine());
		symt->setParent(hierStack.top());
		inheritanceCheck(symt);
		multipleIDcheck(symt->getParent(), symt->getName());
		hierStack.top()->addEntry(symt);
		symt->setLeaf();
		}
	
	if (node->getType() == "read" || node->getType() == "write" || node->getType() == "return" || node->getType() == "assignStat" ){
		checkIfDefined(node);
	}
		
}

string Visitor::typeLookUp(SymbolTable * symt, Node * node){
	
	if(node->getType() == "float"){return "float";}
	if(node->getType() == "integer"){return "integer";}
	
	for(int i = 0; i < symt->getEntries().size() ; ++i){ //local scope
		
		if (node->getName() == symt->getEntries().at(i)->getName()){
			
			string s = symt->getEntries().at(i)->getType();
			return s.substr(0, s.find(":"));
		}
	}
			
	for(int i = 0; i < symt->getParent()->getInheritance().size() ; ++i){ // loop inside list of inheritance class
					
		for (int j = 0 ; j < symt->getParent()->getInheritance().at(i)->getEntries().size(); ++j ) { // loop inside each inheritance class entry
			
			if(node->getName() == symt->getParent()->getInheritance().at(i)->getEntries().at(j)->getName()){
				
				string s = symt->getEntries().at(i)->getType();
				return s.substr(0, s.find(":"));
			}
		}
	}
}

string Visitor::typeCheck(Node * node){
	
	bool error = false;
	string prevType = node->getType(); 
	
	cout << "<1> " << node->getName();
	checkIfFuncCall(node);
	if(node->getType() == "floatNum"){node->setType("float"); OPERAND = true; }
	if(node->getType() == "intNum"){node->setType("integer"); OPERAND = true; }
	if(node->getType() == "id"){node->setType(typeLookUp(hierStack.top(), node)); OPERAND = true;}
	if(node->getType() == "dimensions"){ FCALL = true; }
	
	
	if(CTYPE == "~" && OPERAND){
		CTYPE = node->getType();
	}else{
		if ((node->getType() != CTYPE) && OPERAND){
			error = true;
		} 
	}

	if(error == true){
		
		SymbolTable * error = new SymbolTable(node->getName(), "variable" , node->getType(), node);
		error->setLine(node->getLine());
		
		stringstream ss;
		ss << "type mismatch in statement between : " << node->getType() << " and " << CTYPE << " ";
		reportError(ss.str(), error);
		}
	
	
	OPERAND = false;
	node->setType(prevType);
	
	if (FCALL == false){
		for (int i = 0 ; i < node->nbOfChildren() ; ++i){
			typeCheck(node->getChildAt(i));                 //recursive call
		}
	}else{
		FCALL = false;
	}
	
	return CTYPE;
}


void Visitor::checkIndex(Node * node){
	
	
	for (int i = 0 ; i < node->nbOfChildren() ; ++i){
			
			if(node->getChildAt(i)->getType() == "dimensions"){
				
				for (int j = 0 ; j < node->getChildAt(i)->nbOfChildren() ; ++j){
				
					CTYPE = "~";
					if (typeCheck(node->getChildAt(i)->getChildAt(j)) != "integer"){
						SymbolTable * error = new SymbolTable(node->getChildAt(i)->getChildAt(j)->getName(), "index expression" , node->getChildAt(i)->getChildAt(j)->getType(), node);
						error->setLine(node->getLine());
						reportError("index must be of type integer", error) ;
					}
				}
			}
		} 	
		
}


void Visitor::semanticAction2(Node * node){

	if ( node->getType() == "id" ){ // check index type
		checkIndex(node);	
	}
	
	if (node->getType() == "main" || node->getType() == "funcdef" || node->getType() == "classDec" ){
		hierStack.push(getSymt(node->getID()));
	}
	
	if (node->getType() == "assignStat" || node->getType() == "T1s" || node->getType() == "relexpr" || node->getType() == "artm1"){
		CTYPE = "~";
		typeCheck(node);
	}
}
