#ifndef CFG_H
#define CFG_H

#include "AST.h"
#include <hash_map>

using namespace std;

class CFG
{
public:
	CFG();
	void generateCFG();

	void addNext(int firstStmt, int nextStmt);

	void getNext(vector<int>& theNext, int stmt, AST* ast);		//(for different nodes, insert a -1 inside.)
	void getNextUp(vector<int>& theNextUp, int stmt, AST* ast);	//(for different nodes, insert a -1 inside.)

	//For debugging
	hash_map<int, set<int>> CFG::getForwardMap();
	hash_map<int, set<int>> CFG::getReverseMap();
	void CFG::prtMap(hash_map<int, set<int>> &currMap);

private:
	void CFG::getNextLink(vector<int>& theNext, int stmt, hash_map<int, set<int> >& theMap);
	void processStatement(Node *inputNode, Node *parentNode, Node* followingNode);
	string intToString(int n);
	void reverseVec(vector<int>& vec);

	hash_map<int, set<int>> forwardMap;	
	hash_map<int, set<int>> reverseMap;
	AST* mAst;
};

#endif