 
Project is still in progress

The lexical, syntax and semantic analysis are completed.
Up next is the machine code generation.

A little bit of info concerning the designs of the main phases:

-LEXER- [lexer.cpp/lexer.h]
----------------------------

The **Finite state machine** which recognizes the atomic elements 
of the language (lexemes/tokens) is implemented via a **table driven**
design. A stream of Token objects is returned.


-SYNTACTIC PARSER- [parser.cpp/parser.h]
------------------------------------------

The parser is also implemented via a table driven design.
It takes as input the previously computed stream of tokens.
All the errors contained in the program text file are signaled
as the parsing progresses.
The parser also builds an **abstract syntactic tree**. This provides
an intermediate representation of the program which is passed on
to the semantic analysis phase.


-SEMANTIC ANALYSIS- [visitor.cpp/visitor.h]
----------------------------------------------

The semantic analysis is done using a tree traversal of the
AST tree. The tree is traversed twice. Semantic actions are 
triggered when critical nodes are encountered.
On the first pass it generates **symbol tables** for the scope of the 
different functions and classes. It also checks for some possible
semantic errors. Here is a list :

			"multiple definition (member function)"
			"multiple definition (free function)"
			"multiple definition (class)"
			"multiple identifier"
			"undeclared member function"
			"undeclared identifier"
			"undefined function declaration"
			"invalid class"
			"WARNING overwritten member"
			"could not match with function declaration"
			"type mismatch in statement"
			"invalid function call or data member"
			"index must be of type integer"

The second pass procedes in a similar manner but it uses the
symbol table to check types and return types of statements
and function calls.


CODE RUNNING INSTRUCTIONS:
--------------------------
To run the code you must use the **g++**compiler.
You also need to change the path of the compiled program
in the main (line 24) method to whatever one you want.

FURTHER NOTES:
-------------
I apologize for the lack of clarity in my code.
I am probably not taking advantage of key c++ functionalities
And I may not even be using some of them in the right way.
Anyhow this is by far the most **amazing** project I have ever worked on.

