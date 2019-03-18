 

#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <fstream>
#include <iterator>
#include <algorithm> 
#include "token.h"


State::State() {
	
}

void State::setFinalBool(bool b){
	isFinal = b;
}

void State::setBacktrack(bool b){
	backtrack = b;
}