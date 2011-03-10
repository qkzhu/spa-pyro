#include "CFGNode.h"

using namespace std;

//TODO: Check stmtNum
CFGNode::CFGNode(int stmtNum){
	this->stmtNumList->insert(stmtNum);
}


//TODO: Check stmtNumList
CFGNode::CFGNode(vector<int> &stmtNumList){
	for(vector<int>::iterator it = stmtNumList.begin(); it != stmtNumList.end(); it++)
		this->stmtNumList->insert(*it);
}