#ifndef CFG_H
#define CFG_H

#include "CFGNode.h"
#include <map>

using namespace std;

class CFG
{
public:

	void addNext(int stmtNum, int stmtNum);
	void addProcToCFGNodeMap(int procIndex, int stmtNum);

	void getNext(vector<int>& theNext, int stmt);		//(for different nodes, insert a -1 inside.)
	void getNextUp(vector<int>& theNextUp, int stmt);	//(for different nodes, insert a -1 inside.)

private:
	map<int, int> procToCFGNodeMap;//procIndex, stmtNum
	map<int, set<int>> forwardMap;	
	map<int, set<int>> reverseMap;
};


#endif