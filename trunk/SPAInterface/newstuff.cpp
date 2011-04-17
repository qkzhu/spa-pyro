#include "stdafx.h"

int getPriority(Node::NodeType type)
{
	if (type == Node::PLUS || type == Node::MINUS)
		return 1;
	
	if (type == Node::TIMES)
		return 2;

	return 3;
}

string printNode(Node* n, int level, PKB& pkb)
{
	stringstream output;
	//inserts statement number and tabs in front
	stringstream ss;
	ss << n->stmtNum;
	string stmtNo = ss.str();
	string tabs = "";
	for (int i = 0; i < level; i++)
		tabs += "\t";
	
	switch (n->type)
	{
	case Node::PROC:
	case Node::STMT_LIST:
		output << printNode(n->bottomNodeList[0], level, pkb);
		break;
	case Node::ASSIGN:
		output << stmtNo << tabs;
		output << pkb.vTable_GetVarName(n->bottomNodeList[0]->id) << " = ";
		output << nodeToInfix(n->bottomNodeList[1], pkb) << ";" << endl;
		break;

	case Node::CALL:
		output << stmtNo << tabs << "call " << pkb.pTable_GetProcName(n->id) << ";" << endl;
		break;

	case Node::IF:
		output << stmtNo << tabs << "if " << pkb.vTable_GetVarName(n->bottomNodeList[0]->id) << " then { " << endl;
		output << printNode(n->bottomNodeList[1], level+1, pkb);
		output << tabs << "} " << " else { " << endl;
		output << printNode(n->bottomNodeList[2], level+1, pkb);
		output << tabs << "}" << endl;
		break;

	case Node::WHILE:
		output << stmtNo << tabs << "while " << pkb.vTable_GetVarName(n->bottomNodeList[0]->id) << " { " << endl;
		output << printNode(n->bottomNodeList[1], level + 1, pkb);
		output << tabs << "}" << endl;
		break;
	}

	//print following node if it exists
	if (n->followNode != NULL)
		output << printNode(n->followNode, level, pkb);

	return output.str();
}

string printProgram(PKB& pkb)
{
	vector<Node*> procs = pkb.ast_GetAllProc();
	stringstream output;
	for (unsigned int i = 0; i < procs.size(); i++)
	{
		output << "procedure " << pkb.pTable_GetProcName(procs[i]->id) << " {" << endl;
		output << printNode(procs[i], 1, pkb);
		output << "}" << endl;
	}
	return output.str();
}

string nodeToInfix(Node *node, PKB& pkb)
{
	vector<Node*> children = pkb.ast_GetDown(node);
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
		result += pkb.vTable_GetVarName(node->id);
		break;
	default:
		throw new string("Pattern: Invalid node type in expression");
	}

	if (children.size() != 0 && children.size() != 2)
		throw new string("Pattern: Invalid expression tree.");

	//a given node in an expression tree has either two children or none
	if (children.size() == 2)
	{
		int currPriority = getPriority(node->type);

		//adds the left child
		if (getPriority(children[0]->type) < currPriority)
			result = "(" + nodeToInfix(children[0], pkb) + ")" + result;
		else
			result = nodeToInfix(children[0], pkb) + result;

		//adds the right child
		if (getPriority(children[1]->type) <= currPriority)
			result += "(" + nodeToInfix(children[1], pkb) + ")";
		else
			result += nodeToInfix(children[1], pkb);
	}

	return result;
}