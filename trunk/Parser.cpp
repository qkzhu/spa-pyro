#include "Parser.h"

using namespace std;

const string Parser::mKeyWords[] = { "procedure", "while", "if", "then", "else", "call" };

//loads the file from the given filename
void Parser::loadFile(string name)
{
	//close and delete the reference if another file is open
	if (mpFile != NULL)
	{
		mpFile->close();
		delete mpFile;
		mpFile = NULL;
	}

	//records the new filename
	mFilename = name;

	//opens the new file and check whether it is valid
	mpFile = new ifstream(mFilename);
	checkValidFile();
}

//constructor for Parser takes a VarTable, AST, and the source code filename
 Parser::Parser(VarTable &mVarTable, AST &mAst, string filename) : 
	mVarTable(mVarTable), mAst(mAst), mLineNum(0), mStatNum(0),
	 mFilename(filename), mpFile(new ifstream()), 
	 mStringBuf(new stringstream(stringstream::in | stringstream::out))
 {
	 mpFile->open(filename, ios::in);
 }

//Destructor closes the file if it is open.
 Parser::~Parser()
 {
	 if (mpFile)
		 mpFile->close();

	 delete mpFile;
	 delete mStringBuf;
 }

 //peeks next token, empty string if none found
 string Parser::peekToken() 
 {
	 checkValidFile();

	 string output = getToken();
	 int size = output.size();
	 size;

	 mStringBuf->seekg(-size, ios_base::cur);

	 return output;
 }

 //extracts next token, returns empty string if none found.
 string Parser::getToken()
 {
	 //checks whether opened file is valid
	 checkValidFile();

	 stringstream output;	 
	 if (hasToken())
	 {
		char c = '\0';

		//if the next character is a delimiter, return the delimiter
		if (isDelimiter((char)mStringBuf->peek()))
		{
			mStringBuf->read(&c, sizeof(c));
			char output[2] = { c, '\0' };
			return string(output);
		}

		//obtains characters from the file until a space/delimiter/EOF is encountered
		while (!isspace(mStringBuf->peek()) && 
			!isDelimiter((char)mStringBuf->peek()) &&
			!mStringBuf->eof())
		{
			mStringBuf->read(&c, sizeof(c));
			output << (char)c;
		}
	 }

	 //returns the token found.
	 return output.str();
 }

 bool Parser::hasToken()
 {
	 checkValidFile();

	 skipSpaces();

	 if (!mStringBuf->eof())
		 return true;

	 //keeps reading until a non-space is found.
	 string s;
	 while (mStringBuf->eof() && getline(*mpFile, s))
	 {
		 delete mStringBuf;
		 mStringBuf = new stringstream(stringstream::in | stringstream::out);

		 mLineNum++;

		 *mStringBuf << s;
		 skipSpaces();
	 }
	 
	 //if EOF encountered, return false, else return true
	 return !mStringBuf->eof() || *mpFile;
 }

 //skips over all spaces in the source program
 void Parser::skipSpaces()
 {
	 //eats all the spaces in the stream
	 char dummy = '\0';
	 while (isspace(mStringBuf->peek()))
		 mStringBuf->read(&dummy, sizeof(dummy));
 }

 //check whether source file is still opened.
 void Parser::checkValidFile()
 {
	 if (!mpFile->is_open())
	 {
		 cout << "Error opening file: " << mFilename << endl;
		 exit(1);
	 }
 }

//prints an error msg and quits the program
void Parser::error(string expected, string received) 
{
	cout << "Error at line " << mLineNum << ": expected - \"" << 
		expected << "\" received - \"" << received << "\"" << endl;
	exit(1);
}

//makes sure that the next token matches the specified token.
void Parser::match(string token) 
{
	string received = getToken();

	if (token != received)
		error(token, received);
}

//parses the source program
void Parser::parseProgram() 
{
	while(hasToken())
	{
		Node *node = parseProcedure();
		mAst.addProcedure(node);
	}
}

Node *Parser::parseProcedure() 
{
	match("procedure");
	
	string proc_name = getToken();
	checkValidName(proc_name);
	Node *node = mAst.createNode(Node::PROC, mLineNum, proc_name);
	
	match("{");
	
	mAst.addDown(node, parseStmtList());
	
	match("}");

	return node;
}

Node *Parser::parseStmtList() 
{
	string tok =peekToken();
	Node *stmt_list = mAst.createNode(Node::STMT_LIST, mLineNum, "");
	Node *prev_node = NULL; 
	
	if (tok != "" && tok != ";" && tok != "}")
	{
		prev_node = parseAssignment();
		mAst.addDown(stmt_list, prev_node);
	}

	//continue parsing more statements while
	//1. there are more tokens
	//2. closing brace is not encountered
	while (strcmp(peekToken().c_str(), "") != 0 && 
		strcmp(peekToken().c_str(), "}") != 0) 
	{
		Node *new_node = parseStmt();
		mAst.addFollow(prev_node, new_node);
		prev_node = new_node;
	}
	
	return stmt_list;
}

Node *Parser::parseStmt()
{
	string next_tok = peekToken();

	if (next_tok == "while")
		return parseWhile();

	else if (next_tok == "if")
		return parseIf();

	else if (next_tok == "call")
		return parseCall();

	else
		return parseAssignment();
}

//TO-DO: implement parseWhile
Node *Parser::parseWhile()
{
	return NULL;
}

//TO-DO: implement parseIf
Node *Parser::parseIf()
{
	return NULL;
}

//TO-DO: implement parseCall
Node *Parser::parseCall()
{
	return NULL;
}

Node *Parser::parseAssignment() 
{
	string var_name = getToken();
	checkValidName(var_name);
	
	Node *assign = mAst.createNode(Node::ASSIGN, mStatNum, "");
	Node *lhs = mAst.createNode(Node::VAR, mStatNum, var_name);
	
	mAst.addDown(assign, lhs);
	
	match("=");
	
	Node *rhs = NULL;
	string rhs_token = getToken();

	//catch the closing semi-colon
	string closing = getToken();
	
	if (strcmp(closing.c_str(), ";") != 0)
		error(";", closing);
		
	if (isConstant(rhs_token))	
		rhs = mAst.createNode(Node::CONST, mStatNum, rhs_token);
		
	else if(checkVariableExists(var_name))
		rhs = mAst.createNode(Node::VAR, mStatNum, rhs_token);
	
	else
		error("existing variable or constant", rhs_token);
	
	mAst.addDown(assign, rhs);
	
	mVarTable.insertVar(var_name, mStatNum);
	
	return assign;
}

bool Parser::isConstant(string tok) 
{
	static regex exp("-?\\d+");
	return regex_match(tok.begin(), tok.end(), exp);
}

bool Parser::isDelimiter(char c)
{
	static regex exp("[{};()+-*]");
	string tok(&c);
	return regex_match(tok.begin(), tok.end(), exp);
}

bool Parser::checkVariableExists(string var_name) 
{
	return mVarTable.containsVar(var_name);
}

void Parser::checkValidName(string var_name)
{
	static regex exp("[A-Za-z][A-Za-z0-9]*");
	static int keywords_len = sizeof(mKeyWords) / sizeof(mKeyWords[0]);
	
	if (!regex_match(var_name.begin(), var_name.end(), exp))
	{
		cout << "Error at line " << mLineNum << ": Invalid variable name \"" << 
			var_name << "\"" << endl;
		exit(1);
	}
	else
	{
		//checks for matches with keywords
		for (int i = 0; i < keywords_len; i++)
		{
			if (strcmp(var_name.c_str(), mKeyWords[i].c_str()) == 0)
			{
				cout << "Cannot use \"" << var_name << "\" as a variable. " <<
					" (line " << mLineNum << ")" << endl;
				exit(1);
			}
		}
	}
}
