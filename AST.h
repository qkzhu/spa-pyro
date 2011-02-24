#include <map>
#include <list>
#include "Node.h"

using namespace std;

/**
 * The AST class is used to provide method to construct a AST tree, and easy to query data from the created AST tree.
 */
class AST
{
public:
	void addChild(Node *d, Node *childNode);						// assign the child node to the child of given node d.
	void addDown(Node *upperNode, Node *bottomNode);				// establish a bottom relation to upperNode, and the same time, establish a up realtion to upperNode.
	void addFollow(Node *d, Node *followNode);						// establish follow relation to the given node d, and also establish a followed by relation to followNode internally.

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
	vector<int> getChild(int stmtNum);				// get the child node of given node statement number stmtNum.
	int			getParent(int stmtNum);				// get the parent node of given node statement number stmtNum.
	Node*		getPreviousStatement(Node* d);		// Get the previous node of given node d, with follow relation.
	Node*		getFollowingStatement(Node* d);		// Get the follow node of given node d, withe the follow relation.
	Node*		getNodeByStatementNum(int index);	// Return a node corresponding to the given statement number.
	int			getStatementNumByNode(Node* d);		// Return statement number corresponding to the given node.

	AST();

private:
	map<int, Node*>	astList;				// procedure list, it stores the root of each procedure.
	map<int, Node*> StatNumAndNodeList;		// StatNumAndNodeList stores the relation between statement number and node.
	map<Node*, int>	NodeAndStatNumList;		// StatNumAndNodeList stores the relation between node and statement number.
};	