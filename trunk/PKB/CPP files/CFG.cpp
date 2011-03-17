#include "CFG.h"
#include <iostream>

using namespace std;

CFG::CFG(PKB& pkb):mPkb(pkb) 
{ 
	generateCFG(); 
}

//input node is the starting statement list to process

//parent node is for adding a link back to the WHILE node 
//for its last child

//following node is for adding a following node for the 
//last child of an if node
void CFG::processStatement(Node *inputNode, Node *parentNode, Node* followingNode)
{
	if (inputNode == NULL)
		throw new string("CFG (processStatement)- inputNode is NULL");

	vector<Node*> bottomNodes = mPkb.ast_GetDown(inputNode);

	if (bottomNodes.size() == 0)
		throw new string("CFG (processStatement)- statement list has no body");

	Node *startNode = bottomNodes[0];
}

void CFG::generateCFG()
{
	vector<Node*> procs = mPkb.ast_GetAllProc();

	for (int i = 0; i < procs.size(); i++)
	{
		vector<Node*> bottomNodes = mPkb.ast_GetDown(procs[i]);
				
		//check whether procedure has a node below
		if (bottomNodes.size() == 0)
			throw new string("CFG- procedure with index: " + intToString(procs[i]->id) + 
				" has no statement list.");

		Node *stmtList = bottomNodes[0];
		
		//checks whether the procedure really has a statement list
		if (stmtList->type != Node::STMT_LIST)
			throw new string("CFG- procedure with index: " + intToString(procs[i]->id) + 
				" has a node that is not a statement list below it.");

		processStatement(stmtList, NULL, NULL);
	}
}

string CFG::intToString(int n)
{
	char buf[256];
	_itoa_s(n, buf, 10);
	return string(buf);
}

//(for different nodes, insert a -1 inside.)
void CFG::getNext(vector<int>& theNext, int stmt){
}		

//(for different nodes, insert a -1 inside.)
void CFG::getNextUp(vector<int>& theNextUp, int stmt){
}