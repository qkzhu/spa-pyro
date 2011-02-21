#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <sstream>
#include <stack>
#include "PKB.h"

using namespace std;

class Parser 
{
public:
	Parser(VarTable &varTable, AST &ast, std::string filename);
	void parseProgram();
	void loadFile(std::string filename); //loads a new source file
	~Parser();

private:
	int mLineNum;
	int mStatNum;
	VarTable &mVarTable;
	AST &mAst;

	string getToken(); 	//extracts next token, returns empty string if none found.
	string peekToken();	//peeks next token, empty string if none found
	bool hasToken();	//checks whether there are more tokens to be read
	void skipSpaces();	//Skips all spaces until the next non-space char is encountered

	void error(std::string expected, std::string received);	//prints the error msg and exits
	void match(std::string token);							//

	Node *parseProcedure();
	Node *parseStmtList(Node* parentNode);
	Node *parseStmt();
	Node *parseAssignment();
	Node *parseWhile();
	Node *parseIf();
	Node *parseCall();

	bool isConstant(string tok);
	bool isDelimiter(char tok);

	//for parsing trees.
	bool isBracket(string s);
	bool isOperator(string s);
	int getPriority(string s);
	void addOperator(vector<Node*> &tree, string op);

	bool checkVariableExists(std::string var_name);
	void checkValidName(std::string var_name);
	bool isValidName(std::string var_name);
	void checkValidFile();

	string mFilename;
	ifstream *mpFile;
	stringstream *mStringBuf;

	static const string mKeyWords[];
};