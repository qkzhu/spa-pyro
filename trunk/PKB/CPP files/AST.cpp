#include "AST.h"
#include <iostream>

using namespace std;

//QianKun
AST::AST(){}

void AST::addChild(Node* d, Node* childNode)
{
	d->childList.push_back(childNode);
	childNode->parentNode = d;
}

void AST::addDown(Node* upperNode, Node* bottomNode)
{
	upperNode->bottomNodeList.push_back(bottomNode);
	bottomNode->upNode = upperNode;//addUpB
}

void AST::addFollow(Node* d, Node* followNode)
{
	d->followNode = followNode;
	followNode->prevNode = d;//add previousNode
}

void AST::addProcedure(int index, Node* d)
{
	astList.insert(pair<int, Node*>(index,d));
}

Node* AST::getNodeByProcdureIndex(int procIndex){
	if(astList.find(procIndex) != astList.end())
		return astList[procIndex];
	else return NULL;
}

vector<int> AST::getChild(int stmtNum)
{
	vector<int> result;
	if(stmtNum <= 0){
		result.push_back(-1);
		return result;
	}

	Node* currNode = getNodeByStatementNum(stmtNum);
	vector<Node*> children = currNode->childList;

	unsigned int i;

	if(children.size() == 0) result.push_back(-1);
	else{
		for(i = 0; i < children.size(); i++)
			result.push_back(children[i]->stmtNum);
	}

	return result;
}

Node* AST::getParent(Node* d)
{
	return d->parentNode;
}

int AST::getParent(int stmtNum)
{
	
	if(stmtNum <= 0) return -1;
	
	int result;
	Node* currNode = getNodeByStatementNum(stmtNum);
	Node* parentNode = currNode->parentNode;

	if(parentNode == NULL) result = -1;
	else result = parentNode->stmtNum;

	return result;
}


Node* AST::createNode(Node::NodeType nt, int statNum, int name)
{
	Node* newNode = new Node(nt, statNum, name);

	if(nt == Node::ASSIGN || nt == Node::CALL || nt == Node::WHILE || nt == Node::IF) {
		StatNumAndNodeList[statNum] = newNode;
		NodeAndStatNumList[newNode] = statNum;
	}

	return newNode;
}

Node* AST::getPreviousStatement(Node* d)
{
	return d->prevNode;
}

Node* AST::getFollowingStatement(Node* d)
{
	return d->followNode; 
}

Node* AST::getNodeByStatementNum(int index)
{
	if (AST::StatNumAndNodeList.find(index) == AST::StatNumAndNodeList.end())
		return NULL;

	return AST::StatNumAndNodeList[index];
}

int AST::getStatementNumByNode(Node* d){
	return NodeAndStatNumList[d];
}

int AST::getMaxStmtNum(){
	map<int, Node*>::iterator it = this->StatNumAndNodeList.end();

	if(it == this->StatNumAndNodeList.begin()) return 0;
	else return (int)(--it)->first;
}

vector<int> AST::getAllAssign(){
	return this->getAllType(Node::ASSIGN);
}

vector<int> AST::getAllWhile(){
	return this->getAllType(Node::WHILE);
}

vector<int> AST::getAllIf(){
	return this->getAllType(Node::IF);
}

vector<int> AST::getAllProc(){
	return this->getAllType(Node::PROC);
}

vector<int>	AST::getAllCall(){
	return this->getAllType(Node::CALL);
}

vector<int> AST::getAllType(Node::NodeType nt){
	int index, maxStmtNum = this->getMaxStmtNum();
	vector<int> result;

	for(index = 1; index <= maxStmtNum; index++){
		Node* currNode = this->getNodeByStatementNum(index);
		if(currNode != NULL && currNode->type == nt)
			result.push_back(index);
		//else if(currNode == NULL) throw new string("No node found");
	}

	/* If no result found, return a vector with only one element: -1. */
	if(result.size() == 0) result.push_back(-1);

	return result;
}

void AST::printNode(Node* n, int level, ProcTable &procTable, VarTable &varTable)
{
	for (int i = 0; i < level; i++)
		cout << "\t";

	switch (n->type)
	{
	case Node::STMT_LIST:
		cout << "=STMT_LIST=" << endl;
		break;

	case Node::ASSIGN:
		cout << "=ASSIGN=" << " (" << n->stmtNum << ") " << endl;
		break;

	case Node::PROC:
		cout << "Procedure: " << procTable.getProcName(n->id) << endl;
		break;

	case Node::CALL:
		cout << "Call: " << procTable.getProcName(n->id) << " (" << n->stmtNum << ") " << endl;
		break;

	case Node::CONST:
		cout << n->id << endl;
		break;

	case Node::IF:
		cout << "==IF==" << " (" << n->stmtNum << ") " << endl;
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

	case Node::WHILE:
		cout << "==WHILE==" << " (" << n->stmtNum << ") " << endl;
		break;

	default:
		cout << "Unidentified node type." << endl;
		break;
	}

	//print children
	for  (unsigned int i = 0; i < n->bottomNodeList.size(); i++)
		printNode(n->bottomNodeList[i], level + 1, procTable, varTable);

	//print following node if it exists
	if (n->followNode != NULL)
		printNode(n->followNode, level, procTable, varTable);
}

void AST::printTree(ProcTable &procTable, VarTable &varTable)
{
	for (unsigned int i = 0; i < astList.size(); i++)
		printNode(astList[i], 0, procTable, varTable);
}