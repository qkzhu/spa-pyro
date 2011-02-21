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

	//TO-DO: put procedure name into procedure table.
	int proc_index = -1; //TO-DO: obtain the procedure index from proc table.
	Node *node = mAst.createNode(Node::PROC, mLineNum, proc_index);
	
	match("{");
	
	mAst.addDown(node, parseStmtList(NULL));
	
	match("}");

	return node;
}

Node *Parser::parseStmtList(Node* parentNode) 
{
	string tok =peekToken();
	Node *stmt_list = mAst.createNode(Node::STMT_LIST, mLineNum, -1);
	Node *prev_node = NULL; 
	
	if (tok != "" && tok != ";" && tok != "}")
	{
		prev_node = parseStmt();
		mAst.addDown(stmt_list, prev_node);

		//adds the current node as the child of the parent if there is a parent.
		if (parentNode != NULL)
			mAst.addChild(parentNode, prev_node);
	}

	//continue parsing more statements while
	//1. there are more tokens
	//2. closing brace is not encountered
	while (peekToken() != "" && peekToken() != "}") 
	{
		Node *new_node = parseStmt();
		mAst.addFollow(prev_node, new_node);
		prev_node = new_node;

		//adds the current node as the child of the parent if there is a parent.
		if (parentNode != NULL)
			mAst.addChild(parentNode, prev_node);
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


Node *Parser::parseWhile()
{
	match("while");
	Node *while_node = mAst.createNode(Node::WHILE, mStatNum, -1);

	//obtains variable token, creates var node and adds it under while node.
	string var_name = getToken();
	checkVariableExists(var_name);
	mVarTable.insertVar(var_name);
	Node *var_node = mAst.createNode(Node::VAR, mStatNum, mVarTable.getVarIndex(var_name));
	mAst.addDown(while_node, var_node);
	mStatNum++;

	//while body goes here
	match("{");
	Node* body_node = parseStmtList(while_node);
	mAst.addDown(while_node, body_node);
	match("}");

	return while_node;
}


Node *Parser::parseIf()
{
	//creates if node.
	match("if");
	Node *if_node = mAst.createNode(Node::IF, mStatNum, -1);

	//obtains variable token, creates var node and adds it under if node.
	string var_name = getToken();
	checkVariableExists(var_name);
	mVarTable.insertVar(var_name);
	Node *var_node = mAst.createNode(Node::VAR, mStatNum, mVarTable.getVarIndex(var_name));
	mAst.addDown(if_node, var_node);
	mStatNum++;
	
	//creates a then node then adds it under the down link of if node
	match("then");
	match("{");
	Node* then_node = parseStmtList(if_node);
	mAst.addDown(if_node, then_node);
	match("}");
	
	//creates an else node then adds it under the down link of if node
	match("else");
	match("{");
	Node* else_node = parseStmtList(if_node);
	mAst.addDown(else_node, else_node);
	match("}");

	return if_node;
}


Node *Parser::parseCall()
{
	match("call");

	string proc_name = getToken();
	//TO-DO: update procedure table
	int proc_index = -1; //TO-DO obtain proc index from procedure table
	Node* curr = mAst.createNode(Node::CALL, mStatNum, proc_index);
	

	match(";");

	return curr;
}

void Parser::addOperator(vector<Node*> &tree, string op)
{
	if (tree.size() < 2)
	{
		cout << "Error in expression at line " << mLineNum  << endl;
		exit(1);
	}
	Node::NodeType type;

	if (op == "+")
		type = Node::PLUS;
	else if (op == "-")
		type = Node::MINUS;
	else if (op == "*")
		type = Node::TIMES;

	//create a tree for this operator
	Node *curr = new Node(type, mStatNum, -1);
	
	//adds the top two trees in the results stack to the current tree's 
	//left and right child
	mAst.addDown(curr, tree[tree.size() - 2]);
	mAst.addDown(curr, tree[tree.size() - 1]);

	//pops the top two trees
	tree.pop_back();
	tree.pop_back();
				
	//and replace with the current tree
	tree.push_back(curr);
}

bool Parser::isBracket(string s)
{
	return s.length() == 1 && (s[0] == '(' || s[0] == ')');
}

bool Parser::isOperator(string s)
{
	return s.length() == 1 &&  (s[0] == '+' || s[0] == '-' || s[0] =='*');
}

int Parser::getPriority(string s)
{
	if (s.length() != 1)
		return 0;

	char c = s[0];
	switch (c)
	{
	case '+':
	case '-':
		return 1;
	case '*':
		return 2;
	default:
		return 0;
	}
}

Node *Parser::parseAssignment() 
{
	string var_name = getToken();
	checkValidName(var_name);
	
	Node *assign = mAst.createNode(Node::ASSIGN, mStatNum, -1);

	mVarTable.insertVar(var_name);
	Node *lhs = mAst.createNode(Node::VAR, mStatNum, mVarTable.getVarIndex(var_name));
	
	mAst.addDown(assign, lhs);
	
	match("=");
	
	vector<Node*> result;
	stack<string> ops;

	string next;
	while (peekToken() != "" && peekToken() != ";")
	{
		next = getToken();
		Node *curr;

		if (isConstant(next))
		{
			//TO-DO: add constant to constant table.
			curr = mAst.createNode(Node::CONST, mStatNum, atoi(next.c_str()));
			result.push_back(curr);
		} //if constant

		else if (isValidName(next))
		{
			checkVariableExists(next);
			curr = mAst.createNode(Node::CONST, mStatNum, mVarTable.getVarIndex(next));
			result.push_back(curr);
		} //else if valid name

		else if (isOperator(next))
		{
			int curr_priority = getPriority(next);
			while (ops.size() > 0  && curr_priority < getPriority(ops.top()))
			{
				if (result.size() < 2)
				{
					cout << "Error in expression." << endl;
					system("pause");
					exit(1);
				}
				
				//adds the operator to the result tree
				addOperator(result, ops.top());

				//pops off the current operator from the operator stack
				ops.pop();
			}

			ops.push(next);
		} //else if operator

		else if (isBracket(next))
		{
			if (next == "(")
				ops.push(next);
			else
			{
				while (ops.size() > 0 && ops.top() != "(")
				{
					//adds the operator to the result tree
					addOperator(result, ops.top());

					//pops off the current operator from the operator stack
					ops.pop();
				}

				if (ops.size() == 0 || ops.top() != "(")
				{
					cout << "No corresponding '(' for ')'" << endl;
					system("pause");
					exit(1);
				}

				ops.pop();
			} //end else
		} //if bracket
		else
		{
			error("Valid variable name, operator or constant", next);
		}
	} //end while

	match(";");

	//pops off all remaining operators on the stack
	while (ops.size() > 0)
	{
		addOperator(result, ops.top());
		ops.pop();
	}

	if (result.size() != 1)
	{
		cout << "Error in expression at line " << mLineNum << endl;
		exit(1);
	}

	mAst.addDown(assign, result[0]);

	mStatNum++;

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

bool Parser::isValidName(string var_name)
{
	static regex exp("[A-Za-z][A-Za-z0-9]*");
	static int keywords_len = sizeof(mKeyWords) / sizeof(mKeyWords[0]);
	
	if (!regex_match(var_name.begin(), var_name.end(), exp))
		return false;
	else
	{
		//checks for matches with keywords
		for (int i = 0; i < keywords_len; i++)
			if (strcmp(var_name.c_str(), mKeyWords[i].c_str()) == 0)
				return false;
	}

	return true;
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
