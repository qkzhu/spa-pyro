#ifndef PATTERN_H
#define PATTERN_H

#include "AST.h"
#include "SymbolTable.h"
#include <stack>
#include <regex>

class Pattern
{
public:
	//return the condition variable 		of the while stmt in its code
	int condWhile(int stmtNum, AST& ast);
	int condIf(int stmtNum, AST& ast);

	//matching	
	bool patternAssign(int stmtNum, string patternLeft, string patternRight, AST& ast, VarTable& varTable);
	bool match(const string& input, const string& existing, bool matchFront, bool matchEnd);
	
	//conversion to prefix for matching
	string stringToPrefix(string& input);
	string nodeToPrefix(Node *node, AST& ast, VarTable& varTable);

	//debugging
	void printNode(Node* n, int level, AST& ast, VarTable& varTable);
	Node *generateNode(string s, AST& ast, VarTable& varTable);
	
	//conversion to postfix for matching
	string nodeToPostFix(Node *node, AST& ast, VarTable& varTable);
	string stringToPostFix(string& input);

private:
	bool isConstant(string tok);
	bool isOperator(char c);
	bool isDelimiter(char c);
	bool isValidName(string var_name);
	string removeSpacesAndQuotes(string s);
	int getPriority(char c);
	void addOperator(vector<Node*> &tree, char op, AST& ast, VarTable& varTable);
};
#endif