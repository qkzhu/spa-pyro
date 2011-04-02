#ifndef AST_H
#define AST_H

#include "SymbolTable.h"
#include <hash_map>
#include <list>
#include "Node.h"

using namespace std;

/**
 * The AST class is used to provide method to construct a AST tree, and easy to query data from the created AST tree.
 */
class AST
{
public:
	void addChild(Node *d, Node *childNode);			// assign the child node to the child of given node d.
	void addDown(Node *upperNode, Node *bottomNode);	// establish a bottom relation to upperNode, and the same time, establish a up realtion to upperNode.
	void addFollow(Node *d, Node *followNode);			// establish follow relation to the given node d, and also establish a followed by relation to followNode internally.


	/**
	 * In the AST, there is a table used to store all the root node of each procedure, this table is called proList, it's used
	 * for retrive the root node of a procedure, and the procedure name can be obtanined from d.name.
	 */
	void addProcedure(int index, Node *d);
	Node* getNodeByProcdureIndex(int procIndex);

	/**
	 * Create a new node and return the pointer of the node. If the given type is assignment, the new node will also be stored
	 * into a vector called 'StatNumAndNodeList'. This vector stores the statment number of each assignment node, and it's used
	 * for retrive the node by using statment number.
	 */
	Node*		createNode(Node::NodeType nt, int statNum, int name);	
	void		getChild(int stmtNum, vector<int>&);// get the child node of given node statement number stmtNum.
	int			getParent(int stmtNum);				// get the parent stmt number of given node statement number stmtNum.
	Node*		getParent(Node *d);					// get the parent node of given node statement number stmtNum.
	Node*		getPreviousStatement(Node* d);		// Get the previous node of given node d, with follow relation.
	Node*		getFollowingStatement(Node* d);		// Get the follow node of given node d, withe the follow relation.
	Node*		getNodeByStatementNum(int index);	// Return a node corresponding to the given statement number.
	int			getStatementNumByNode(Node* d);		// Return statement number corresponding to the given node.
	int			getMaxStmtNum();

	void getAllAssign(vector<int>&);
	void getAllWhile(vector<int>&);
	void getAllIf(vector<int>&);
	void getAllCall(vector<int>&);

	bool isIf(int stmt);
	bool isWhile(int stmt);
	bool isAssign(int stmt);
	bool isCall(int stmt);

	//for CFG
	vector<Node*> getAllProc();
	vector<Node*> getAllDown(Node* n);

	void printTree(ProcTable &procTable, VarTable &varTable);
	void printNode(Node* n, int level, ProcTable &procTable, VarTable &varTable);

	AST();

private:
	hash_map<int, Node*>	astList;					// procedure list, it stores the root of each procedure.
	hash_map<int, Node*>	StatNumAndNodeList;			// StatNumAndNodeList stores the relation between statement number and node.
	hash_map<Node*, int>	NodeAndStatNumList;			// NodeAndStatNumList stores the relation between node and statement number.

	void AST::getAllType(Node::NodeType nt, vector<int>& result);	// Return all statement number with the given node type.
	bool isType(int stmt, Node::NodeType nt);						// Check if the given stmt index is the given Node type
};	

#endif