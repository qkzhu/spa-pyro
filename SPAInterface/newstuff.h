#include "stdafx.h"

#ifndef NEWSTUFF_H
#define NEWSTUFF_H

#include <string>

int getPriority(Node::NodeType type);
string printNode(Node* n, int level, PKB& pkb);
string printProgram(PKB& pkb);
string nodeToInfix(Node *node, PKB& pkb);

#endif