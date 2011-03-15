#include "CFG.h"
#include <iostream>

using namespace std;

CFGNode* CFG::createCFGNode(int stmtNum){
	//TODO check if the sutmNum has already been used for the other node.
	CFGNode *newCFGNode  = new CFGNode(stmtNum);
	return newCFGNode;
}

CFGNode* CFG::createCFGNode(vector<int> &stmtNumList){
	//TODO check if the sutmNum has already been used for the other node.
	CFGNode *newCFGNode  = new CFGNode(stmtNumList);
	return newCFGNode;
}

void CFG::addCFGNode(CFGNode* cNode, CFGNode* nextCFGNode){
	cNode->next.push_back(nextCFGNode);
}

void CFG::addProcToCFGNodeMap(int procIndex, CFGNode* rootNode){
	//TODO: Check procIndex if it's valid
	procToCFGNodeMap.insert(pair<int, CFGNode*>(procIndex, rootNode));
}


//(for different nodes, insert a -1 inside.)
void CFG::getNext(vector<int>& theNext, int stmt){
}		

//(for different nodes, insert a -1 inside.)
void CFG::getNextUp(vector<int>& theNextUp, int stmt){
}