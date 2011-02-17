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

void AST::addProcedure(Node* d)
{
	proList.push_back(d);
}

Node* AST::getChild(Node* d)
{
	return d->childNode;
}

Node* AST::createNode(Node::NodeType nt, int statNum, string name)
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

