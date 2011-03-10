#ifndef CFG_H
#define CFG_H

#include "CFGNode.h"

using namespace std;

class CFG
{
public:
	CFGNode* addCFGNode(int stmtNum);
	CFGNode* addCFGNode(vector<int> &stmtNumList);
	void addProcToCFGNodeMap(int procIndex, CFGNode* rootNode);

	void getNext(vector<int>& theNext, int stmt);		//(for different nodes, insert a -1 inside.)
	void getNextUp(vector<int>& theNextUp, int stmt);	//(for different nodes, insert a -1 inside.)

private: 
	map<int procIndex, CFGNode* rootNode>	procToCFGNodeMap;
};


#endif