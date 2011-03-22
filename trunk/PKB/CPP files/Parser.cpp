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
 Parser::Parser(PKB &pkb, string filename) : 
mPkb(pkb), mLineNum(0), mStatNum(1),
	 mCurrProcIndex(0), mFilename(filename), mpFile(new ifstream()), 
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

	 if (hasToken())
	 {
		string s = mStringBuf->str();

		unsigned int start  = (unsigned int)mStringBuf->tellg();
		unsigned int pos = start;

		if (isDelimiter(s[pos]))
			return s.substr(pos, 1);
		
		//obtains characters from the file until a space/delimiter/EOF is encountered
		while (pos < s.size() && !isspace(s[pos]) && 
			!isDelimiter(s[pos]))
			pos++;

		return s.substr(start, pos-start);
	 }

	 return "";
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
		 throw new string("Error opening file: " + mFilename);
 }

//prints an error msg and quits the program
void Parser::error(string expected, string received) 
{
	throw new string("Error at line " + intToString(mLineNum) +  ": expected - \"" + 
		expected + "\" received - \"" + received + "\"");
	//exit(1);
}

//makes sure that the next token matches the specified token.
void Parser::match(string token) 
{
	string received = getToken();

	if (token != received)
		error(token, received);
}

//post process all calls to procedures undefined at the time of parsing.
// map["procedure name"] = (mCurrProcIndex, (mLineNum, Node)
void Parser::processCalls()
{
	for (map<string, vector<tuple<ProcIndex, LineNum, Node*> > >::iterator it = mProcCallsBuf.begin(); it != mProcCallsBuf.end(); it++)
	{
		int proc_index = mPkb.pTable_GetProcIndex(it->first); 
		vector<tuple<ProcIndex, LineNum, Node*> > results = it->second;
	
		//stores into call buffer, check whether the procedure exists when program ends.
		if (!mPkb.pTable_isProcIndexExist(proc_index))
		{
			string output = "Attempt to call non-existing procedure " + it->first + " at line(s) ";
			for (unsigned int i = 0; i < results.size(); i++)
				output += intToString(get<1>(results[i])) + " ";
			throw new string(output);
		}

		//records the proc call in the proc table
		else
		{
			for (unsigned int i = 0; i < results.size(); i++)
			{
				mPkb.pTable_AddCall(get<0>(results[i]), proc_index);
				get<2>(results[i])->id = proc_index;
			}
		}
	}
}

void Parser::processModifyUse()
{
	//updates modifies!
	for (unsigned int i = 0; i < mProcNodesBuf.size(); i++)
	{
		pair<ProcIndex, Node*> curr = mProcNodesBuf[i];

		/***obtains all the variables modified and used in the next procedure***/

		//get all the variables modified in the particular (indirect) call
		vector<int> nextModVars;
		mPkb.mTable_getModifiedVarPI(curr.second->id, nextModVars);

		//add them to the current procedure, statement and statement's ancestors
		for (unsigned int k = 0; k < nextModVars.size(); k++)
		{
			if (!mPkb.vTable_IsVarIndexExist(nextModVars[k]))
				continue;
			mPkb.mTable_setModifyPV(curr.first, nextModVars[k]);
			mPkb.mTable_setModify(mPkb.ast_getStmtNum(curr.second), nextModVars[k]);
			updateModify(mPkb.ast_getParent(curr.second), nextModVars[k]);
		}

		//get all the variables used in the particular (indirect) call
		vector<int> nextUsedVars;
		mPkb.uTable_getUsedVarPI(curr.second->id, nextUsedVars);

		//add them to the current procedure, statement and statement's ancestors
		for (unsigned int k = 0; k < nextUsedVars.size(); k++)
		{
			if (!mPkb.vTable_IsVarIndexExist(nextUsedVars[k]))
				continue;
			mPkb.uTable_setUsesPV(curr.first, nextUsedVars[k]);
			mPkb.uTable_setUses(mPkb.ast_getStmtNum(curr.second), nextUsedVars[k]);
			updateUse(mPkb.ast_getParent(curr.second), nextUsedVars[k]);
		}


		//**** obtains all variables in recursive calls ****/
		vector<ProcIndex> recursiveCall;
		mPkb.pTable_getCall_(curr.second->id, recursiveCall);
		
		//get all the direct and indirect calls
		for (unsigned int j = 0; j < recursiveCall.size(); j++)
		{
			//get all the variables modified in the particular (indirect) call
			vector<int> modVariables;
			mPkb.mTable_getModifiedVarPI(recursiveCall[j], modVariables);

			//add them to the current procedure, statement and statement's ancestors
			for (unsigned int k = 0; k < modVariables.size(); k++)
			{
				if (!mPkb.vTable_IsVarIndexExist(modVariables[k]))
					continue;
				mPkb.mTable_setModifyPV(curr.first, modVariables[k]);
				mPkb.mTable_setModify(mPkb.ast_getStmtNum(curr.second), modVariables[k]);
				updateModify(mPkb.ast_getParent(curr.second), modVariables[k]);
			}

			//get all the variables used in the particular (indirect) call
			vector<int> usedVariables;
			mPkb.uTable_getUsedVarPI(recursiveCall[j], usedVariables);

			//add them to the current procedure, statement and statement's ancestors
			for (unsigned int k = 0; k < usedVariables.size(); k++)
			{
				if (!mPkb.vTable_IsVarIndexExist(usedVariables[k]))
					continue;
				mPkb.uTable_setUsesPV(curr.first, usedVariables[k]);
				mPkb.uTable_setUses(mPkb.ast_getStmtNum(curr.second), usedVariables[k]);
				updateUse(mPkb.ast_getParent(curr.second), usedVariables[k]);
			}
		}
	}
}

void Parser::updateModify(Node* parent, int varIndex)
{
	while (parent != NULL)
	{
		mPkb.mTable_setModify(parent->stmtNum, varIndex);
		parent = mPkb.ast_getParent(parent);
	}
}
void Parser::updateUse(Node* parent, int varIndex)
{
	while (parent != NULL)
	{
		mPkb.uTable_setUses(parent->stmtNum, varIndex);
		parent = mPkb.ast_getParent(parent);
	}
}

//parses the source program
void Parser::parseProgram() 
{
	while(hasToken())
		parseProcedure();

	//post-processing
	processCalls();
	processModifyUse();
}

void Parser::parseProcedure() 
{
	match("procedure");

	string proc_name = getToken();
	checkValidName(proc_name);

	//inserts procedure name into proc table
	mPkb.pTable_InsertProc(proc_name); 
	//obtains procedure index
	int proc_index = mPkb.pTable_GetProcIndex(proc_name);  
	//creates procedure node
	Node *node = mPkb.ast_CreateNode(Node::PROC, mLineNum, proc_index); 

	//records current procedure
	mCurrProcIndex = proc_index;
	
	match("{");
	
	mPkb.ast_AddDown(node, parseStmtList(NULL));
	
	match("}");

	//adds procedure to AST
	mPkb.ast_AddProcedure(proc_index, node);
}

Node *Parser::parseStmtList(Node* parentNode) 
{
	string tok =peekToken();
	Node *stmt_list = mPkb.ast_CreateNode(Node::STMT_LIST, mStatNum, -1);
	Node *prev_node = NULL; 
	
	if (tok != "" && tok != ";" && tok != "}")
	{
		prev_node = parseStmt(parentNode);
		mPkb.ast_AddDown(stmt_list, prev_node); 
	}

	//continue parsing more statements while
	//1. there are more tokens
	//2. closing brace is not encountered
	while (peekToken() != "" && peekToken() != "}") 
	{
		Node *new_node = parseStmt(parentNode);
		mPkb.ast_AddFollow(prev_node, new_node);
		prev_node = new_node;

		////adds the current node as the child of the parent if there is a parent.
		//if (parentNode != NULL)
		//	mPkb.ast_AddChild(parentNode, prev_node);
	}

	if (prev_node == NULL)
		throw new string("There must be at least a statement within the braces. (Line " + intToString(mLineNum) + ")");
	
	return stmt_list;
}

Node *Parser::parseStmt(Node* parentNode)
{
	string next_tok = peekToken();

	if (next_tok == "while")
		return parseWhile(parentNode);

	else if (next_tok == "if")
		return parseIf(parentNode);

	else if (next_tok == "call")
		return parseCall(parentNode);

	else
		return parseAssignment(parentNode);
}


Node *Parser::parseWhile(Node* parentNode)
{
	match("while");
	Node *while_node = mPkb.ast_CreateNode(Node::WHILE, mStatNum, -1);

	//adds the current node as the child of the parent if there is a parent.
	if (parentNode != NULL)
		mPkb.ast_AddChild(parentNode, while_node);

	//obtains variable token, creates var node and adds it under while node.
	string var_name = getToken();
	checkValidName(var_name);
	if (!isExistingVariable(var_name))
		mPkb.vTable_InsertVar(var_name);
	int var_index = mPkb.vTable_GetVarIndex(var_name);
	Node *var_node = mPkb.ast_CreateNode(Node::VAR, mStatNum, var_index);
	mPkb.ast_AddDown(while_node, var_node);
	
	//update useTable
	mPkb.uTable_setUses(mStatNum, var_index);
	mPkb.uTable_setUsesPV(mCurrProcIndex, var_index);
	//update ancestors
	updateUse(parentNode, var_index);

	mStatNum++;

	//while body goes here
	match("{");
	Node* body_node = parseStmtList(while_node);
	mPkb.ast_AddDown(while_node, body_node);
	match("}");

	return while_node;
}


Node *Parser::parseIf(Node* parentNode)
{
	//creates if node.
	match("if");
	Node *if_node = mPkb.ast_CreateNode(Node::IF, mStatNum, -1);

	//adds the current node as the child of the parent if there is a parent.
	if (parentNode != NULL)
		mPkb.ast_AddChild(parentNode, if_node);

	//obtains variable token, creates var node and adds it under if node.
	string var_name = getToken();
	if (!isExistingVariable(var_name))
		mPkb.vTable_InsertVar(var_name);
	int var_index = mPkb.vTable_GetVarIndex(var_name);
	Node *var_node = mPkb.ast_CreateNode(Node::VAR, mStatNum, var_index);
	mPkb.ast_AddDown(if_node, var_node);
	
	//update useTable
	mPkb.uTable_setUses(mStatNum, var_index);
	mPkb.uTable_setUsesPV(mCurrProcIndex, var_index);
	//update ancestors
	updateUse(parentNode, var_index);

	mStatNum++;

	//creates a then node then adds it under the down link of if node
	match("then");
	match("{");
	Node* then_node = parseStmtList(if_node);
	mPkb.ast_AddDown(if_node, then_node);
	match("}");
	
	//creates an else node then adds it under the down link of if node
	match("else");
	match("{");
	Node* else_node = parseStmtList(if_node);
	mPkb.ast_AddDown(if_node, else_node);
	match("}");

	return if_node;
}


Node *Parser::parseCall(Node* parentNode)
{
	match("call");

	string proc_name = getToken();

	int proc_index = mPkb.pTable_GetProcIndex(proc_name); 

	Node* curr = mPkb.ast_CreateNode(Node::CALL, mStatNum++, proc_index);

	//adds the current node as the child of the parent if there is a parent.
	if (parentNode != NULL)
		mPkb.ast_AddChild(parentNode, curr);

	//stores the call and node pair for post-processing modify and use.
	mProcNodesBuf.push_back(make_pair(mCurrProcIndex, curr));

	//stores into call buffer, check whether the procedure exists when program ends.
	if (!mPkb.pTable_isProcIndexExist(proc_index))
	{
		tuple<ProcIndex, LineNum, Node*> curr_tuple(mCurrProcIndex, mLineNum, curr);
		if (mProcCallsBuf.find(proc_name) == mProcCallsBuf.end())
		{
			vector<tuple<ProcIndex, LineNum, Node*>> vec;
			vec.push_back(curr_tuple);
			mProcCallsBuf[proc_name] = vec;
		}
		else
			mProcCallsBuf[proc_name].push_back(curr_tuple);
	}
	//records the proc call in the proc table
	else
		mPkb.pTable_AddCall(mCurrProcIndex, proc_index);
	
	match(";");

	return curr;
}

void Parser::addOperator(vector<Node*> &tree, string op)
{
	if (tree.size() < 2)
		throw new string("Error in expression at line " + intToString(mLineNum));

	Node::NodeType type;

	if (op == "+")
		type = Node::PLUS;
	else if (op == "-")
		type = Node::MINUS;
	else if (op == "*")
		type = Node::TIMES;

	//create a tree for this operator
	Node *curr = mPkb.ast_CreateNode(type, mStatNum, -1);
	
	//adds the top two trees in the results stack to the current tree's 
	//left and right child
	mPkb.ast_AddDown(curr, tree[tree.size() - 2]);
	mPkb.ast_AddDown(curr, tree[tree.size() - 1]);

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

Node *Parser::parseAssignment(Node* parentNode) 
{
	string var_name = getToken();
	checkValidName(var_name);
	
	Node *assign = mPkb.ast_CreateNode(Node::ASSIGN, mStatNum, -1);

	//adds the current node as the child of the parent if there is a parent.
	if (parentNode != NULL)
		mPkb.ast_AddChild(parentNode, assign);

	//update vartable
	mPkb.vTable_InsertVar(var_name);
	int var_index = mPkb.vTable_GetVarIndex(var_name);
	Node *lhs = mPkb.ast_CreateNode(Node::VAR, mStatNum, var_index);
	
	//update modify tables
	mPkb.mTable_setModify(mStatNum, var_index);
	mPkb.mTable_setModifyPV(mCurrProcIndex, var_index);
	//recursively update modify tables
	updateModify(parentNode, var_index);

	mPkb.ast_AddDown(assign, lhs);
	
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
			mPkb.cTable_AddConstant(atoi(next.c_str()));
			curr = mPkb.ast_CreateNode(Node::CONST, mStatNum, atoi(next.c_str()));
			result.push_back(curr);
		} //if constant

		else if (isValidName(next))
		{
			if (!isExistingVariable(next))
				mPkb.vTable_InsertVar(next);
			int var_index = mPkb.vTable_GetVarIndex(next);
			curr = mPkb.ast_CreateNode(Node::VAR, mStatNum, var_index);
			result.push_back(curr);
			
			//Update use tables.
			mPkb.uTable_setUses(mStatNum, var_index);
			mPkb.uTable_setUsesPV(mCurrProcIndex, var_index);

			//recursively update use tables.
			updateUse(parentNode, var_index);

		} //else if valid name

		else if (isOperator(next))
		{
			int curr_priority = getPriority(next);
			while (ops.size() > 0  && curr_priority <= getPriority(ops.top()))
			{
				if (result.size() < 2)
					throw new string("Error in expression.");
				
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
					throw string("No corresponding '(' for ')'");

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
		throw new string("Error in expression at line " + intToString(mLineNum));
	
	mPkb.ast_AddDown(assign, result[0]);

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
	static regex exp("[{};\\-+*()=]"); //dash must be escaped
	string tok(&c);
	return regex_match(tok.begin(), tok.end(), exp);
}

bool Parser::isExistingVariable(string var_name) 
{
	return mPkb.vTable_IsVarNameExist(var_name);
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
		throw new string("Error at line " + intToString(mLineNum) + ": Invalid variable name \"" + 
			var_name + "\"");
		//cout << "Error at line " << mLineNum << ": Invalid variable name \"" << 
		//	var_name << "\"" << endl;
	}
	else
	{
		//checks for matches with keywords
		for (int i = 0; i < keywords_len; i++)
		{
			if (strcmp(var_name.c_str(), mKeyWords[i].c_str()) == 0)
			{
				throw new string("Cannot use \"" + var_name + "\" as a variable. " +
					" (line " + intToString(mLineNum) + ")");

				//cout << "Cannot use \"" << var_name << "\" as a variable. " <<
				//	" (line " << mLineNum << ")" << endl;
				//exit(1);
			}
		}
	}
}

string Parser::intToString(int n)
{
	char buf[256];
	_itoa_s(n, buf, 10);
	return string(buf);
}