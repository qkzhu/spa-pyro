#ifndef CFGNODE_H
#define CFGNODE_H

#include <vector>
#include <set>

using namespace std;

class CFGNode
{
public:
	//enum NodeType{WHILE, IF};
	//NodeType type;		// note type, it can be procedure, assignment, statment list, variable or constant value

	// Constructor
	CFGNode(int stmtNum);
	CFGNode(vector<int> &stmtNumList);

private:
	set<int> *stmtNumList;	// statement number list
	vector<CFGNode> *next;;
	vector<CFGNode> *previsou;
};

#endif