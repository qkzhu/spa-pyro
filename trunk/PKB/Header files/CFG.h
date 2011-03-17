#ifndef CFG_H
#define CFG_H

#include "PKB.h"
#include <map>

using namespace std;

class CFG
{
public:
	CFG(PKB &pkb);

	void addNext(int stmtNum, int stmtNum);
	void addProcToCFGNodeMap(int procIndex, int stmtNum);

	void getNext(vector<int>& theNext, int stmt);		//(for different nodes, insert a -1 inside.)
	void getNextUp(vector<int>& theNextUp, int stmt);	//(for different nodes, insert a -1 inside.)

private:
	void generateCFG();
	void processStatement(Node *inputNode, Node *parentNode, Node* followingNode);
	string intToString(int n);

	map<int, int> procToCFGNodeMap;	//procIndex, stmtNum
	map<int, set<int>> forwardMap;	
	map<int, set<int>> reverseMap;
	PKB mPkb;
};


#endif