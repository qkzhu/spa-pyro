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
	if(index < 0) throw new string("AST::addProcedure - Invalid procedure index.");
	astList.insert(pair<int, Node*>(index,d));
}

Node* AST::getNodeByProcdureIndex(int procIndex){
	if(procIndex < 0) throw new string("AST::getNodeByProcdureIndex - Invalid procedure index.");
	if(astList.find(procIndex) != astList.end())
		return astList[procIndex];
	else return NULL;
}

int AST::getProcRootStmtNumByProcdureIndex(int procIndex){
	Node* procRootStmt = getNodeByProcdureIndex(procIndex);
	if(procRootStmt == NULL) return -1;
	else return procRootStmt->stmtNum;
}

void AST::getChild(int stmtNum, vector<int>& result)
{
	if(stmtNum <= 0)
		result.push_back(-1);

	Node* currNode = getNodeByStatementNum(stmtNum);
	vector<Node*> children = currNode->childList;

	unsigned int i;

	if(children.size() == 0) result.push_back(-1);
	else{
		for(i = 0; i < children.size(); i++)
			result.push_back(children[i]->stmtNum);
	}
}

Node* AST::getParent(Node* d)
{
	return d->parentNode;
}

int AST::getParent(int stmtNum)
{
	
	if(stmtNum <= 0) throw new string("AST::getParent - Invalid procedure index.");;
	
	int result;
	Node* currNode = getNodeByStatementNum(stmtNum);
	Node* parentNode = currNode->parentNode;

	if(parentNode == NULL) result = -1;
	else result = parentNode->stmtNum;

	return result;
}


Node* AST::createNode(Node::NodeType nt, int statNum, int name)
{
	if(statNum <= 0) throw new string("AST::createNode - Invalid statNum or nameIndex.");
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

void AST::getAllAssign(vector<int>& result){
	getAllType(Node::ASSIGN, result);
}

void AST::getAllWhile(vector<int>& result){
	getAllType(Node::WHILE, result);
}

void AST::getAllIf(vector<int>& result){
	getAllType(Node::IF, result);
}

void AST::getAllCall(vector<int>& result){
	getAllType(Node::CALL, result);
}

void AST::getAllType(Node::NodeType nt, vector<int>& result){
	int index, maxStmtNum = this->getMaxStmtNum();

	for(index = 1; index <= maxStmtNum; index++){
		Node* currNode = this->getNodeByStatementNum(index);
		if(currNode != NULL && currNode->type == nt)
			result.push_back(index);
		//else if(currNode == NULL) throw new string("No node found");
	}

	/* If no result found, return a vector with only one element: -1. */
	if(result.size() == 0) result.push_back(-1);
}

vector<Node*> AST::getAllProc(){
	vector<Node*> result;
	for (map<int, Node*>::iterator it = astList.begin(); it != astList.end(); it++)
		result.push_back(it->second);
	return result;
}

vector<Node*> AST::getAllDown(Node* n)
{
	return n->bottomNodeList;
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