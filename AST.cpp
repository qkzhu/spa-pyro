#include "AST.h"
#include <iostream>

using namespace std;

//QianKun
AST::AST(){}

void AST::addChild(Node* d, Node* childNode)
{
	d->childNode = childNode;
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

	if(nt == Node::ASSIGN) {
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
	return this->StatNumAndNodeList.max_size();
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
	vector<int> result;

	for(map<int, Node*>::iterator it = astList.begin(); it != astList.end(); it++)
	{
		result.push_back(it->first);
	}

	return result;
}

vector<int>	AST::getAllCall(){
	return this->getAllType(Node::CALL);
}

vector<int> AST::getAllType(Node::NodeType nt){
	int index, maxStmtNum = this->getMaxStmtNum();
	vector<int> result;

	for(index = 1; index <= maxStmtNum; index++){
		Node* currNode = this->getNodeByStatementNum(index);
		if(currNode->type == nt)
			result.push_back(index);
	}

	/* If no result found, return a vector with only one element: -1. */
	if(result.size() == 0) result.push_back(-1);

	return result;
}

void AST::printTree()
{
	for (int i = 1; i <= getMaxStmtNum(); i++)
	{
		Node* n = astList[i];
		cout << "Proc: " << n->name << endl;

		if (n->bottomNodeList.size() != 0)
			n = n->bottomNodeList[0]->bottomNodeList[0];

		for (; n != NULL; n = n->followNode)
		{
			cout << n->bottomNodeList[0]->name << " = " << 
				n->bottomNodeList[1]->name << endl;
		}		
	}
}