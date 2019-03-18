
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <fstream>
#include <iterator>
#include <algorithm> 
#include "token.h"


Token::Token() {
	
}

void Token::printToken(bool error){
	
	if (error == true){	
		
		ofstream error;
		error.open("error.txt",std::ios_base::app);
		
		error << "[" << type << "] position : [line=" << line << ",col =" << column << "] lexeme : '" << lexeme <<"'" << endl;
		
		error << "\n\n" ;
		error.close();
		
	}else{
		cout << "[" << type << "] position : [line=" << line << ",col =" << column << "] lexeme : '" << lexeme <<"'" << endl;
	}
}

void Token::assignType(int finalState){

	//TOKEN TYPES'
	if (finalState == 0){ type = "SOLO OPERATOR";}
	if ((finalState == 46) && (lexeme == "0" )){ type = "INTEGER"; word = "intNum";}
	if (finalState == 4){type = "INTEGER"; word = "intNum";}
	if (finalState == 5){type = "IDENTIFIER"; word = "id";}
	if (finalState == 10){type = "FRACTION"; word = "floatNum";}
	if (finalState == 8){type = "FLOAT";  word = "floatNum";}
	if (finalState == 17){type = "OPERATOR '=='"; word = "eq";}
	if (finalState == 20){type = "OPERATOR '<>'"; word = "neq";}
	if (finalState == 22){type = "OPERATOR '<='"; word = "geq";}
	if (finalState == 25){type = "OPERATOR '>='"; word = "leq";}
	if (finalState == 28){type = "OPERATOR '::'"; word = "sr";}
	if (finalState == 32){type = "PUNCTUATION '//'";}
	if (finalState == 33){type = "PUNCTUATION '/*'";}
	if (finalState == 36){type = "PUNCTUATION '*/'";}
	if (finalState == 39){type = "OPERATOR '&&'"; word = "and";}
	if (finalState == 43){type = "OPERATOR '||'"; word = "or";}
	
	//ERRORS
	if (finalState == 13){type = "ERROR INVALID NUMBER";}
	if (finalState == 9) {type = "ERROR INVALID NUMBER";}
	if (finalState == 40) {type = "ERROR INVALID CHARACTER";}
	if (finalState == 44) {type = "ERROR INVALID CHARACTER";}
	
	
}

void Token::symbolCheck(){
	
	if (type == "IDENTIFIER"){
		if(lexeme == "if"){type = "RESERVED 'if'"; word = "if";}
		if(lexeme == "then"){type = "RESERVED 'then'"; word = "then";}
		if(lexeme == "else"){type = "RESERVED 'else'";word = "else";}
		if(lexeme == "for"){type = "RESERVED 'for'"; word ="for";}
		if(lexeme == "class"){type = "RESERVED 'class'"; word = "class";}
		if(lexeme == "integer"){type = "RESERVED 'integer'"; word = "integer";}
		if(lexeme == "float"){type = "RESERVED 'float'"; word ="float";}
		if(lexeme == "read"){type = "RESERVED 'read'"; word = "read"; }
		if(lexeme == "write"){type = "RESERVED 'write'"; word = "write";}
		if(lexeme == "return"){type = "RESERVED 'return'"; word = "return";}
		if(lexeme == "main"){type = "RESERVED 'main'"; word = "main";}
	}
	
	if (type == "SOLO OPERATOR"){
		
		if(lexeme == "<"){ word = "lt" ;}
		if(lexeme == ">"){ word = "gt" ;}
		if(lexeme == "+"){ word = "+" ;}
		if(lexeme == "-"){ word = "-" ;}
		if(lexeme == "*"){ word = "*" ;}
		if(lexeme == "/"){ word = "/" ;}
		if(lexeme == "!"){ word = "not" ;}
		if(lexeme == "="){ word = "=" ;}
		if(lexeme == ":"){ word = ":" ;}
		if(lexeme == ","){ word = "," ;}
		if(lexeme == "."){ word = "." ;}
		if(lexeme == ";"){ word = ";" ;}
		if(lexeme == "["){ word = "[" ;}
		if(lexeme == "]"){ word = "]" ;}
		if(lexeme == "{"){ word = "{" ;}
		if(lexeme == "}"){ word = "}" ;}
		if(lexeme == "("){ word = "(" ;}
		if(lexeme == ")"){ word = ")" ;}
		
		
	}
}

void Token::concatLex(char c){
	lexeme += c;
}