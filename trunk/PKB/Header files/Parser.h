#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <sstream>
#include <stack>
#include <utility>
#include <tuple>
#include "PKB.h"

using namespace std;

typedef int ProcIndex;
typedef int LineNum;

class Parser 
{
public:
	Parser(PKB &pkb, std::string filename);
	void parseProgram();
	void loadFile(std::string filename); //loads a new source file
	~Parser();

private:
	int mLineNum;
	int mStatNum;
	int mCurrProcIndex; //tracks the current procedure being parsed.

	//functions to hold pkb instances
	PKB &mPkb;

	//functions for tokenizing
	string getToken();		//extracts next token, returns empty string if none found.
	string peekToken();	//peeks next token, empty string if none found
	bool hasToken();		//checks whether there are more tokens to be read
	void skipSpaces();		//Skips all spaces until the next non-space char is encountered

	//functions that print error msgs
	void error(std::string expected, std::string received);	//prints the error msg and exits
	void match(std::string token);							//
	string intToString(int n);

	//functions to parse the source program
	void parseProcedure();
	Node *parseStmtList(Node* parentNode);
	Node *parseStmt(Node* parentNode);
	Node *parseAssignment(Node* parentNode);
	Node *parseWhile(Node* parentNode);
	Node *parseIf(Node* parentNode);
	Node *parseCall(Node* parentNode);

	//functions to check whether tokens are constants or delimiters
	bool isConstant(string tok);
	bool isDelimiter(char tok);

	//for parsing expression trees e.g. (1+2) * 3.
	bool isBracket(string s);
	bool isOperator(string s);
	int getPriority(string s);
	void addOperator(vector<Node*> &tree, string op);

	//functions to check validity
	bool isExistingVariable(std::string var_name);
	void checkValidName(std::string var_name);
	bool isValidName(std::string var_name);
	void checkValidFile();

	//function to update all ur ancestors
	void updateModify(Node* parent, int varIndex);
	void updateUse(Node* parent, int varIndex);

	//functions for post-processing
	void processCalls();
	void processModifyUse();

	//member variables to hold files
	string mFilename;
	ifstream *mpFile;

	stringstream *mStringBuf;

	static const string mKeyWords[];

	//buffers
	map<string, vector<tuple<ProcIndex, LineNum, Node*> > > mProcCallsBuf; //store procedures undeclared at the point of parsing
	vector<pair<ProcIndex, Node*>> mProcNodesBuf;
};

#endif