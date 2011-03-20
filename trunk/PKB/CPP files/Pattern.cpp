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
		if (s[i] != ' ' && s[i] != '\t' && s[i] != '\n' && s[i] != '\'')
			output += s[i];

	return output;
}

void Pattern::addOperator(vector<Node*> &tree, char op, PKB& pkb)
{
	if (tree.size() < 2)
		throw new string("Error in pattern.");

	Node::NodeType type;

	if (op == '+')
		type = Node::PLUS;
	else if (op == '-')
		type = Node::MINUS;
	else if (op == '*')
		type = Node::TIMES;

	//create a tree for this operator
	Node *curr = pkb.ast_CreateNode(type, -1, -1);
	
	//adds the top two trees in the results stack to the current tree's 
	//left and right child
	pkb.ast_AddDown(curr, tree[tree.size() - 2]);
	pkb.ast_AddDown(curr, tree[tree.size() - 1]);

	//pops the top two trees
	tree.pop_back();
	tree.pop_back();
				
	//and replace with the current tree
	tree.push_back(curr);
}

Node *Pattern::generateNode(string input, PKB& pkb)
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
				addOperator(result, ops.top(), pkb);

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
			curr = pkb.ast_CreateNode(Node::CONST, -1, atoi(next.c_str()));
			result.push_back(curr);
		} //if constant

		else if (isValidName(next))
		{
			int var_index = pkb.vTable_GetVarIndex(next);
			curr = pkb.ast_CreateNode(Node::VAR, -1, var_index);
			result.push_back(curr);
		} //else if valid name

		else
			throw new string("Invalid pattern string:" + input);
	}

		//pops off all remaining operators on the stack
	while (ops.size() > 0)
	{
		addOperator(result, ops.top(), pkb);
		ops.pop();
	}

	if (result.size() != 1)
		throw new string("Invalid pattern string: " + input);

	return result[0];
}

bool Pattern::patternAssign(int stmtNum, string patternLeft, string patternRight, PKB& pkb)
{
	Node *assign = pkb.ast_GetNodeByStatementNum(stmtNum);

	if (assign->type != Node::ASSIGN)
		return false;

	//check that it has two bottom nodes, and the left is a variable
	vector<Node*> bottomNodes = pkb.ast_GetDown(assign);
	if (bottomNodes.size() < 2 || bottomNodes[0]->type != Node::VAR || 
		bottomNodes[0]->stmtNum != stmtNum)
		return false;

	//check that the modified variable corresponds to the pattern on the left
	int var_index = pkb.vTable_GetVarIndex(patternLeft);
	if (var_index != bottomNodes[0]->id)
		return false;

	//check the pattern on the right.
	if (patternRight.size() == 0)
		return false;
	
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

	Node *inputNode = generateNode(patternRight, pkb);
	Node *existingNode = bottomNodes[1];

	return match(inputNode, existingNode, matchFront, matchEnd);
}

//matches input and existing string using postfix.
bool Pattern::match(Node* inputNode, Node* existingNode, bool matchFront, bool matchEnd)
{


	return false;
}

void Pattern::printNode(Node* n, int level, PKB &pkb)
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
		cout << pkb.vTable_GetVarName(n->id) << endl;
		break;

	default:
		cout << "Unidentified node type." << endl;
		break;
	}

	//print children
	for  (unsigned int i = 0; i < n->bottomNodeList.size(); i++)
		printNode(n->bottomNodeList[i], level + 1, pkb);

	//print following node if it exists
	if (n->followNode != NULL)
		printNode(n->followNode, level, pkb);
}

//generates the postfix expression from a given string
string Pattern::stringToPostFix(string input)
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
					throw new string("Invalid pattern string: " + input);
				
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
			throw new string("Invalid pattern string:" + input);
	}

	//pops off all remaining operators on the stack
	while (ops.size() > 0)
	{
		result += ops.top();
		ops.pop();
	}

	return result;
}

//generates the postfix expression from a given node
string Pattern::nodeToPostFix(Node *node, PKB& pkb)
{
	vector<Node*> children = pkb.ast_GetDown(node);
	string result = "";

	if (children.size() != 0 && children.size() != 2)
		throw new string("Invalid expression tree.");

	//a given node in an expression tree has either two children or none
	if (children.size() == 2)
	{
		//adds the left child
		result += nodeToPostFix(children[0], pkb);
		//adds the right child
		result += nodeToPostFix(children[1], pkb);
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
		if (!pkb.vTable_IsVarIndexExist(node->id))
			throw new string("Variable with var index " + pkb.intToString(node->id));

		result += pkb.vTable_GetVarName(node->id);
		break;
	default:
		throw new string("Invalid node type in expression");
	}

	return result;
}