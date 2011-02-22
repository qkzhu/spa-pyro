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

list<int> AST::getChild(int stmtNum)
{
	//TODO in case of stmtNum == 0;
	Node* currNode = getNodeByStatementNum(stmtNum);
	vector<Node*> children = currNode->childList;
	list<int> result;

	unsigned int i;
	for(i = 0; i < children.size(); i++)
		result.push_back(children[i]->stmtNum);

	return result;
}

Node* AST::createNode(Node::NodeType nt, int statNum, int name)
{
	Node* newNode = new Node(nt, statNum, name);

	if(nt == Node::ASSIGN) StatNumAndNodeList[statNum] = newNode;

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