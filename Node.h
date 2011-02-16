#include <vector>
#include <string>

using namespace std;

/**
 * This is a double linked list node, with this node, AST tree can be easily contructed.
 */
class Node
{
public:
	enum NodeType{PROC, ASSIGN, STMT_LIST, VAR, CONST};

	int stmtNum;	// statement number
	NodeType type;	// note type, it can be procedure, assignment, statment list, variable or constant value
	string name;	// it stores the name of a procedure or variable, or the value of a constant value

	Node *parentNode;				// parent node
	Node *childNode;				// child node
	vector<Node*> childList;		// a collection of child node
	vector<Node*> bottomNodeList;	// a collection of bottom node
	Node *followNode;				// node with follow relation
	Node *prevNode;					// node with followed relation
	Node *downNode;					// donw node
	Node *upNode;					// up node

	Node(NodeType type, int stmtNum, string name);
};