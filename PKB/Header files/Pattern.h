#ifndef PATTERN_H
#define PATTERN_H

#include "PKB.h"
#include <stack>
#include <regex>

class Pattern
{
public:
	//matching	
	bool patternAssign(int stmtNum, string patternLeft, string patternRight, PKB& pkb);
	bool match(Node* inputNode, Node* existingNode, bool matchFront, bool matchEnd);
	
	//conversion to postfix for matching
	string nodeToPostFix(Node *node, PKB& pkb);
	string stringToPostFix(string input);

	//debugging
	void printNode(Node* n, int level, PKB& pkb);
	Node *generateNode(string s, PKB& pkb);
	

private:
	bool isConstant(string tok);
	bool isOperator(char c);
	bool isDelimiter(char c);
	bool isValidName(string var_name);
	string removeSpacesAndQuotes(string s);
	int getPriority(char c);
	void addOperator(vector<Node*> &tree, char op, PKB& pkb);
};
#endif