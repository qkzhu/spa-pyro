#include "Pattern.h"

bool Pattern::isOperator(char c)
{
	return c == '+' || c == '-' || c =='*';
}

int Pattern::getPriority(char c)
{
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

int Pattern::getNodePriority(Node::NodeType type)
{
	if (type == Node::PLUS || type == Node::MINUS)
		return 1;
	
	if (type == Node::TIMES)
		return 2;

	return 3;
}

bool Pattern::isConstant(string tok) 
{
	static regex exp("-?\\d+");
	return regex_match(tok.begin(), tok.end(), exp);
}

//matches only +-* in this version.
bool Pattern::isDelimiter(char c)
{
	static regex exp("[\\-+*]"); //dash must be escaped
	string tok(&c);
	return regex_match(tok.begin(), tok.end(), exp);
}

bool Pattern::isValidName(string var_name)
{
	static regex exp("[A-Za-z][A-Za-z0-9]*");
	
	return regex_match(var_name.begin(), var_name.end(), exp);
}

string Pattern::removeSpacesAndQuotes(string s)
{
	int i = 0;
	string output = "";
	
	for (unsigned int i = 0; i < s.size(); i++)
		if (s[i] != ' ' && s[i] != '\t' && s[i] != '\n' && s[i] != '\"')
			output += s[i];

	return output;
}

void Pattern::addOperator(vector<Node*> &tree, char op, AST& ast, VarTable& varTable)
{
	if (tree.size() < 2)
		throw new string("Pattern: Error in pattern.");

	Node::NodeType type;

	if (op == '+')
		type = Node::PLUS;
	else if (op == '-')
		type = Node::MINUS;
	else if (op == '*')
		type = Node::TIMES;

	//create a tree for this operator 
	Node *curr = ast.createNode(type, 3333, -1);
	
	//adds the top two trees in the results stack to the current tree's 
	//left and right child
	ast.addDown(curr, tree[tree.size() - 2]);
	ast.addDown(curr, tree[tree.size() - 1]);

	//pops the top two trees
	tree.pop_back();
	tree.pop_back();
				
	//and replace with the current tree
	tree.push_back(curr);
}

Node *Pattern::generateNode(string input, AST& ast, VarTable& varTable)
{
	//removes all spaces and quotes inside the input first.
	input = removeSpacesAndQuotes(input);

	vector<Node*> result;
	stack<char> ops;

	unsigned int i = 0;
	unsigned int j = 0;
	Node *curr = NULL;
	while (i < input.size())
	{
		//implicitly encountered delimiter
		if (isOperator(input[i]))
		{
			int curr_priority = getPriority(input[i]);
			while (ops.size() > 0  && curr_priority <= getPriority(ops.top()))
			{
				if (result.size() < 2)
					throw new string("Invalid pattern string: " + input);
				
				//adds the operator to the result tree
				addOperator(result, ops.top(), ast, varTable);

				//pops off the current operator from the operator stack
				ops.pop();
			}

			ops.push(input[i]);
			i++;
		} //if delimiter encountered

		//gets the next token
		j = i;
		while (i < input.size() && !isDelimiter(input[i]))
			i++;
		string next = input.substr(j, i-j);

		if (isConstant(next))
		{
			curr = ast.createNode(Node::CONST, 1, atoi(next.c_str()));
			result.push_back(curr);
		} //if constant

		else if (isValidName(next))
		{
			int var_index = varTable.getVarIndex(next);
			curr = ast.createNode(Node::VAR, 3333, var_index);
			result.push_back(curr);
		} //else if valid name

		else
			throw new string("Pattern: Invalid pattern string:" + input);
	}

		//pops off all remaining operators on the stack
	while (ops.size() > 0)
	{
		addOperator(result, ops.top(), ast, varTable);
		ops.pop();
	}

	if (result.size() != 1)
		throw new string("Pattern: Invalid pattern string: " + input);

	return result[0];
}

bool Pattern::patternAssign(int stmtNum, string patternLeft, string patternRight, AST& ast, VarTable& varTable)
{
	if (stmtNum <= 0 || stmtNum > ast.getMaxStmtNum())
		throw new string("Pattern: Invalid statement number.");

	Node *assign = ast.getNodeByStatementNum(stmtNum);
	
	if (assign->type != Node::ASSIGN)
		return false;

	
	//check that it has two bottom nodes, and the left is a variable
	vector<Node*> bottomNodes = ast.getAllDown(assign);
	if (bottomNodes.size() < 2 || bottomNodes[0]->type != Node::VAR || 
		bottomNodes[0]->stmtNum != stmtNum)
		return false;

	//check that the modified variable corresponds to the pattern on the left
	int var_index = varTable.getVarIndex(patternLeft);

	if (patternLeft != "_")
	{
		checkValidName(patternLeft);

		if (var_index != bottomNodes[0]->id)
			return false;
	}

	//check the pattern on the right.
	if (patternRight.size() == 0)
		return false;

	if (patternRight.size() == 1 && patternRight[0] == '_')
		return true;
	
	bool matchFront = true;
	bool matchEnd = true;

	//remove the front underscore
	if (patternRight[0] == '_')
	{
		patternRight = patternRight.substr(1, patternRight.size()-1);
		matchFront = false;
	}

	//remove the rear underscore
	if (patternRight[patternRight.size()-1] == '_')
	{
		patternRight = patternRight.substr(0, patternRight.size()-1);
		matchEnd = false;
	}

	if(patternRight.size() == 0)
		throw new string("Pattern: Empty expression passed.");

	//building a tree to test whether patternRight is correct.
	Node* node = generateNode(patternRight, ast, varTable);

	string input = patternRight;
	string existing = nodeToInfix(bottomNodes[1], ast, varTable);

	return matchInfix(input, existing, matchFront, matchEnd);

	//return matchUsingNode(node, bottomNodes[1], matchFront, matchEnd);

	//string input = nodeToPrefix(node, ast, varTable);
	//string existing = nodeToPrefix(bottomNodes[1], ast, varTable);

	//return match(input, existing, matchFront, matchEnd);
}

bool Pattern::matchInfix(const string& input, const string& existing, bool matchFront, bool matchEnd)
{
	if (matchFront && matchEnd)
		return input == existing;

	if (matchFront)
	{
		char lastOp = ' ';
		int i = 0;

		for (; i < (int)input.size() && i < (int)existing.size(); i++)
		{
			if (input[i] != existing[i])
				return false;

			if (isOperator(input[i]))
				lastOp = input[i];
		}

		if (i < (int)input.size())
			return false;

		if (lastOp == ' ')
			return true;

		int currPriority = getPriority(lastOp);

		for (; i < (int)existing.size(); i++)
			if (isOperator(existing[i]) && getPriority(existing[i]) > currPriority)
				return false;

		return true;
	} //end matchFront

	else if (matchEnd)
	{
		if (input.size() > existing.size())
			return false;

		char lastOp = ' ';
		int i = input.size()-1;
		int j = existing.size() - 1;

		for (; i >= 0 && j >= 0; i--, j--)
		{
			if (input[i] != existing[j])
				return false;

			if (isOperator(input[i]))
				lastOp = input[i];
		}

		if (i >= 0)
			return false;

		if (lastOp == ' ')
			return true;

		int currPriority = getPriority(lastOp);

		for (; j >= 0; j--)
		{
			if (isOperator(existing[j]))
			{
			   if (getPriority(existing[j]) > currPriority)
				 return false;
			   else
				   break;
			}
		}

		return true;
	} //end matchEnd

	//match neither
	else
	{
		 int position = existing.find(input);

		 if (position == string::npos)
			 return false;

		 //checks the operator that comes before the matching expression
		 char firstOperator = ' ';

		 for (int i = 0; i < (int)input.size(); i++)
			 if (isOperator(input[i]))
			 {
				 firstOperator = input[i];
				 break;
			 }

		if (firstOperator != ' ')
		{
			int currPriority = getPriority(firstOperator);
			for (int j = position; j >= 0; j--)
				if (isOperator(existing[j]))
				{
					if (getPriority(existing[j]) > currPriority)
						return false;
					else
						break;
				}
		}

		//checks the operator that comes after the expression
		char lastOperator = ' ';

		 for (int i = (int)input.size() - 1; i >= 0; i--)
			 if (isOperator(input[i]))
			 {
				 lastOperator = input[i];
				 break;
			 }

		if (lastOperator != ' ')
		{
			int currPriority = getPriority(lastOperator);
			for (int j = position + input.size(); j < (int)existing.size(); j++)
				if (isOperator(existing[j]))
				{
					if (getPriority(existing[j]) > currPriority)
						return false;
					else
						break;
				}
		}

		 return true;
	}
}

//matches input and existing string using postfix.
bool Pattern::match(const string& input, const string& existing, bool matchFront, bool matchEnd)
{
	//exact match
	if (matchFront && matchEnd)
		return input == existing;

	//match from middle, ensuring that all characters before first the operator that matches
	//with the input are all operators themselves
	else if (matchFront)
	{
		for (unsigned int i = 0; i < existing.size() && existing.size() - i >= input.size(); i++)
		{
			if (existing.substr(i, input.size()) == input)
				return true;

			if (!isOperator(existing[i]))
				break;
		}
		return false;
	}

	//match the prefix substring from behind
	else if (matchEnd)
	{
		int i = existing.size();
		int j = input.size();
		while (j-- && i --)
			if (existing[i] != input[j])
				break;

		return input.size() > 0 && j < 0;
	}

	//input must be a substring of existing
	else
		return existing.find(input) != string::npos;

	//won't reach here.
}

bool Pattern::matchUsingNode(Node* input, Node* existing, bool matchFront, bool matchEnd)
{
	//either trees are null, return false
	if (input == NULL || existing == NULL)
		return false;

	//if both expression trees match, no questions asked.
	if (matchTree(input, existing))
		return true;

	if (matchFront)
	{
		//match front and left tree matches input
		if (existing->bottomNodeList.size() == 1 && 
			matchTree(input, existing->bottomNodeList[0]))
			return true;

		//match front and right tree matches input
		if (existing->bottomNodeList.size() == 2 && 
			matchTree(input, existing->bottomNodeList[1]))
			return false;
	}

	if (matchEnd)
	{
		//match end and right tree matches input
		if (existing->bottomNodeList.size() == 2 && 
				matchTree(input, existing->bottomNodeList[1]))
				return true;

		//match end and left tree matches input
		if (existing->bottomNodeList.size() >= 1 &&
			matchTree(input, existing->bottomNodeList[0]))
				return false;
	}

	//match the subtrees recursively
	return (existing->bottomNodeList.size() > 0 && matchUsingNode(input, existing->bottomNodeList[0], matchFront, matchEnd)) ||
		(existing->bottomNodeList.size() > 1 && matchUsingNode(input, existing->bottomNodeList[1], matchFront, matchEnd));
}

//matches input and existing string using prefix.
bool Pattern::matchUsingPrefix(const string& input, const string& existing, bool matchFront, bool matchEnd)
{
	//exact match
	if (matchFront && matchEnd)
		return input == existing;

	//match from middle, ensuring that all characters before first the operator that matches
	//with the input are all operators themselves
	else if (matchFront)
	{
		for (unsigned int i = 0; i < existing.size() && existing.size() - i >= input.size(); i++)
		{
			if (existing.substr(i, input.size()) == input)
				return true;

			if (!isOperator(existing[i]))
				break;
		}
		return false;
	}

	//match the prefix substring from behind
	else if (matchEnd)
	{
		int i = existing.size();
		int j = input.size();
		while (j-- && i --)
			if (existing[i] != input[j])
				break;

		return input.size() > 0 && j < 0;
	}

	//input must be a substring of existing
	else
		return existing.find(input) != string::npos;;

	//won't reach here.
}

void Pattern::printNode(Node* n, int level, AST& ast, VarTable& varTable)
{
	for (int i = 0; i < level; i++)
		cout << "\t";

	switch (n->type)
	{
	case Node::CONST:
		cout << n->id << endl;
		break;

	case Node::MINUS:
		cout << "==MINUS==" << endl;
		break;

	case Node::PLUS:
		cout << "==PLUS==" << endl;
		break;

	case Node::TIMES:
		cout << "==TIMES==" << endl;
		break;

	case Node::VAR:
		cout << varTable.getVarName(n->id) << endl;
		break;

	default:
		cout << "Pattern: Unidentified node type." << endl;
		break;
	}

	//print children
	for  (unsigned int i = 0; i < n->bottomNodeList.size(); i++)
		printNode(n->bottomNodeList[i], level + 1, ast, varTable);

	//print following node if it exists
	if (n->followNode != NULL)
		printNode(n->followNode, level, ast, varTable);
}

//converts from infix to prefix
string Pattern::stringToPrefix(string& input)
{
	//removes all spaces and quotes inside the input first.
	input = removeSpacesAndQuotes(input);

	string result;
	stack<char> ops;
	
	//prefix algorithm: same as postfix, except that expression
	//is processed is reverse order, and the result is reversed again.
	int i = input.size()-1;
	int j = 0;
	while (i >= 0)
	{
		//implicitly encountered delimiter
		if (isOperator(input[i]))
		{
			int curr_priority = getPriority(input[i]);
			while (ops.size() > 0  && curr_priority < getPriority(ops.top()))
			{
				if (result.size() < 2)
					throw new string("Pattern: Invalid pattern string: " + input);
				
				//adds the operator to the result tree
				result += ops.top();

				//pops off the current operator from the operator stack
				ops.pop();
			}

			ops.push(input[i]);
			i--;
		} //if delimiter encountered

		//gets the next token
		j = i;
		while (i >= 0 && !isDelimiter(input[i]))
			i--;
		string next = input.substr(i+1, j-i);

		if (isConstant(next) || isValidName(next))
			result += next;

		else
			throw new string("Pattern: Invalid pattern string:" + input);
	}

	//pops off all remaining operators on the stack
	while (ops.size() > 0)
	{
		result += ops.top();
		ops.pop();
	}

	string reversed = "";
	int k = result.size();
	while (k--)
		reversed += result[k];

	return reversed;
}

//generates the postfix expression from a given string
string Pattern::stringToPostFix(string& input)
{
	//removes all spaces and quotes inside the input first.
	input = removeSpacesAndQuotes(input);

	string result;
	stack<char> ops;

	unsigned int i = 0;
	unsigned int j = 0;
	while (i < input.size())
	{
		//implicitly encountered delimiter
		if (isOperator(input[i]))
		{
			int curr_priority = getPriority(input[i]);
			while (ops.size() > 0  && curr_priority <= getPriority(ops.top()))
			{
				if (result.size() < 2)
					throw new string("Pattern: Invalid pattern string: " + input);
				
				//adds the operator to the result tree
				result += ops.top();

				//pops off the current operator from the operator stack
				ops.pop();
			}

			ops.push(input[i]);
			i++;
		} //if delimiter encountered

		//gets the next token
		j = i;
		while (i < input.size() && !isDelimiter(input[i]))
			i++;
		string next = input.substr(j, i-j);

		if (isConstant(next) || isValidName(next))
		{
			result += next;
		} 

		else
			throw new string("Pattern: Invalid pattern string:" + input);
	}

	//pops off all remaining operators on the stack
	while (ops.size() > 0)
	{
		result += ops.top();
		ops.pop();
	}

	return result;
}

string Pattern::nodeToInfix(Node *node, AST& ast, VarTable& varTable)
{
	vector<Node*> children = ast.getAllDown(node);
	string result = "";
	stringstream ss;

	//process current node first.
	switch (node->type)
	{
	case Node::CONST:
		//create a stringstream
		ss << node->id;//add number to the stream
		result += ss.str();
		break;
	case Node::PLUS:
		result += "+";
		break;
	case Node::MINUS:
		result += "-";
		break;
	case Node::TIMES:
		result += "*";
		break;
	case Node::VAR:
		result += varTable.getVarName(node->id);
		break;
	default:
		throw new string("Pattern: Invalid node type in expression");
	}

	if (children.size() != 0 && children.size() != 2)
		throw new string("Pattern: Invalid expression tree.");

	//a given node in an expression tree has either two children or none
	if (children.size() == 2)
	{
		int currPriority = getNodePriority(node->type);

		//adds the left child
		if (getNodePriority(children[0]->type) < currPriority)
			result = "(" + nodeToInfix(children[0], ast, varTable) + ")" + result;
		else
			result = nodeToInfix(children[0], ast, varTable) + result;

		//adds the right child
		if (getNodePriority(children[1]->type) <= currPriority)
			result += "(" + nodeToInfix(children[1], ast, varTable) + ")";
		else
			result += nodeToInfix(children[1], ast, varTable);
	}

	return result;
}

string Pattern::nodeToPrefix(Node *node, AST& ast, VarTable& varTable)
{
	vector<Node*> children = ast.getAllDown(node);
	string result = "";
	stringstream ss;

	//process current node first.
	switch (node->type)
	{
	case Node::CONST:
		//create a stringstream
		ss << node->id;//add number to the stream
		result += ss.str();
		break;
	case Node::PLUS:
		result += "+";
		break;
	case Node::MINUS:
		result += "-";
		break;
	case Node::TIMES:
		result += "*";
		break;
	case Node::VAR:
		result += varTable.getVarName(node->id);
		break;
	default:
		throw new string("Pattern: Invalid node type in expression");
	}

	if (children.size() != 0 && children.size() != 2)
		throw new string("Pattern: Invalid expression tree.");

	//a given node in an expression tree has either two children or none
	if (children.size() == 2)
	{
		//adds the left child
		result += nodeToPrefix(children[0], ast, varTable);
		//adds the right child
		result += nodeToPrefix(children[1], ast, varTable);
	}

	return result;
}

//generates the postfix expression from a given node
string Pattern::nodeToPostFix(Node *node, AST& ast, VarTable& varTable)
{
	vector<Node*> children = ast.getAllDown(node);
	string result = "";

	if (children.size() != 0 && children.size() != 2)
		throw new string("Pattern: Invalid expression tree.");

	//a given node in an expression tree has either two children or none
	if (children.size() == 2)
	{
		//adds the left child
		result += nodeToPostFix(children[0], ast, varTable);
		//adds the right child
		result += nodeToPostFix(children[1], ast, varTable);
	}

	switch (node->type)
	{
	case Node::PLUS:
		result += "+";
		break;
	case Node::MINUS:
		result += "-";
		break;
	case Node::TIMES:
		result += "*";
		break;
	case Node::VAR:
		result += varTable.getVarName(node->id);
		break;
	default:
		throw new string("Pattern: Invalid node type in expression");
	}

	return result;
}

int Pattern::condWhile(int stmtNum, AST& ast)
{
	Node *whe = ast.getNodeByStatementNum(stmtNum);
	if (whe->type != Node::WHILE)
		throw new string("Pattern: Statement number does not correspond to a while statement.");

	vector<Node*> bottomNodes = whe->bottomNodeList;
	
	if (bottomNodes.size() == 0)
		throw new string("Pattern: While has no down nodes");

	return bottomNodes[0]->id;
}

int Pattern::condIf(int stmtNum, AST& ast)
{
	Node *iff = ast.getNodeByStatementNum(stmtNum);
	if (iff->type != Node::IF)
		throw new string("Pattern: Statement number does not correspond to a if statement.");

	vector<Node*> bottomNodes = iff->bottomNodeList;
	
	if (bottomNodes.size() == 0)
		throw new string("Pattern: If has no down nodes");

	return bottomNodes[0]->id;
}

bool Pattern::matchTree(Node* first, Node* second)
{
	//first level of check:
	//1. both node types must not be null
	//2. both nodes must have the same id
	//3. both nodes must have same number of bottom nodes
	if (first == NULL || second == NULL || first->type != second->type || 
		first->id != second->id || first->bottomNodeList.size() != second->bottomNodeList.size())
			return false;

	//second level of check. all bottom nodes must match
	for (unsigned int i = 0; i < first->bottomNodeList.size(); i++)
		if (!matchTree(first->bottomNodeList[i], second->bottomNodeList[i]))
			return false;

	//pass all checks - must be true.
	return true;
}

void Pattern::checkValidName(string& var_name)
{
	static regex exp("[A-Za-z][A-Za-z0-9]*");
	
	if (!regex_match(var_name.begin(), var_name.end(), exp))
		throw new string("Pattern: Invalid left pattern");
}