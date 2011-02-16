#include "Node.h"
#include <vector>

using namespace std;

/**
 * Initialize parentNode, childNode, followNode, prevNode, downNode and upNode to NULL,
 * and also initialize the node type, statement number and name to the given value. 
 */
Node::Node(NodeType type, int stmtNum, string name)
	:parentNode(NULL),childNode(NULL), followNode(NULL),
	 prevNode(NULL), downNode(NULL), upNode(NULL)
{
	this->type = type;
	this->stmtNum = stmtNum;
	this->name = name;
}