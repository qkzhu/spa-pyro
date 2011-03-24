#include "CFG.h"
#include <iostream>

using namespace std;

CFG::CFG():mAst(NULL)
{ 
}

//input node is the starting statement list to process

//parent node is for adding a link back to the WHILE node 
//for its last child

//following node is for adding a following node for the 
//last child of an if node
void CFG::processStatement(Node *inputNode, Node *parentNode, Node* followingNode)
{
	if (inputNode == NULL)
		throw new string("CFG (processStatement)- firstNode is NULL");

	vector<Node*> bottomNodes = mAst->getAllDown(inputNode);

	if (bottomNodes.size() == 0)
		throw new string("CFG (processStatement)- statement list has no body");

	Node *currNode = bottomNodes[0];
	
	//process all nodes chained by following links until a terminal node is reached
	while (currNode != NULL)
	{
		Node *nextNode = mAst->getFollowingStatement(currNode);
		
		if (nextNode != NULL && currNode->type != Node::IF)
			addNext(mAst->getStatementNumByNode(currNode), mAst->getStatementNumByNode(nextNode));
		
		//reached the end of the statement, perform adding of next statement 
		//either to the parent (for WHILE) or to the following statement (for IF)
		else
		{
			if (parentNode != NULL && currNode->type != Node::IF)
				addNext(mAst->getStatementNumByNode(currNode), mAst->getStatementNumByNode(parentNode));
			
			if (followingNode != NULL && currNode->type != Node::IF)
				addNext(mAst->getStatementNumByNode(currNode), mAst->getStatementNumByNode(followingNode));
		}

		if (currNode->type == Node::WHILE)
		{
			vector<Node*> bottomNodes = mAst->getAllDown(currNode);

			if (bottomNodes.size() < 2 || bottomNodes[1] == NULL)
				throw new string("CFG: WHILE has no statement list.");

			Node *stmtList = bottomNodes[1];

			vector<Node*> whileDownNodes = mAst->getAllDown(stmtList);
			if (whileDownNodes.size() == 0)
				throw new string("CFG: WHILE statement list is empty");

			Node* firstNode = whileDownNodes[0];
			addNext(mAst->getStatementNumByNode(currNode), mAst->getStatementNumByNode(firstNode));


			//sends the current WHILE node as parent to a recursive call
			processStatement(stmtList, currNode, NULL);
		}

		else if (currNode->type == Node::IF)
		{
			vector<Node*> bottomNodes = mAst->getAllDown(currNode);

			if (bottomNodes.size() < 3)
				throw new string("CFG (processStatement): IF is missing either a THEN or an ELSE statement list");

			Node *thenList = bottomNodes[1];
			Node *elseList = bottomNodes[2];

			//adds the next to the first then node
			vector<Node*> thenDownNodes = mAst->getAllDown(thenList);
			if (thenDownNodes.size() == 0)
				throw new string("CFG: THEN statement list is empty");

			Node* firstThenNode = thenDownNodes[0];
			addNext(mAst->getStatementNumByNode(currNode), mAst->getStatementNumByNode(firstThenNode));

			//adds the next to the first else node
			vector<Node*> elseDownNodes = mAst->getAllDown(elseList);
			if (elseDownNodes.size() == 0)
				throw new string("CFG: ELSE statement list is empty");

			Node* firstElseNode = elseDownNodes[0];
			addNext(mAst->getStatementNumByNode(currNode), mAst->getStatementNumByNode(firstElseNode));

			//sends the current IF node as parent to a recursive call
			processStatement(thenList, NULL, (nextNode != NULL) ? nextNode : parentNode);
			processStatement(elseList, NULL, (nextNode != NULL) ? nextNode : parentNode);
		}

		currNode = nextNode;
	}
}

void CFG::generateCFG()
{
	vector<Node*> procs = mAst->getAllProc();

	//process all the procedures inside the AST
	for (unsigned int i = 0; i < procs.size(); i++)
	{
		vector<Node*> bottomNodes = mAst->getAllDown(procs[i]);
				
		//check whether procedure has a node below
		if (bottomNodes.size() == 0)
			throw new string("CFG- procedure with index: " + intToString(procs[i]->id) + 
				" has no statement list.");

		Node *stmtList = bottomNodes[0];
		
		//checks whether the procedure really has a statement list
		if (stmtList->type != Node::STMT_LIST)
			throw new string("CFG- procedure with index: " + intToString(procs[i]->id) + 
				" has a node that is not a statement list below it.");
		
		processStatement(stmtList, NULL, NULL);
	}
}

string CFG::intToString(int n)
{
	char buf[256];
	_itoa_s(n, buf, 10);
	return string(buf);
}

void CFG::addNext(int firstStmt, int nextStmt)
{
	//Check the paramenter
	if(firstStmt <= 0 || nextStmt <= 0) throw new string("CFG::addNext - Invalid parameter.");

	//Check if the stmtNum already had 2 next stmts
	if(forwardMap[firstStmt].size() == 2) throw new string("CFG::addNext - Given Stmt already had two next stmt.");

	forwardMap[firstStmt].insert(nextStmt);
	reverseMap[nextStmt].insert(firstStmt);
}

//set direction to > 0 for forward link, to < 0 for reverse link 
void CFG::getNextLink(vector<int>& theNext, int stmt, map<int, set<int> >& theMap)
{
	if (theMap.find(stmt) == theMap.end()){
		theNext.push_back(-1);
		return;
	}

	for (set<int>::iterator it = theMap[stmt].begin(); it != theMap[stmt].end();)
	{
		theNext.push_back(*it);

		if (++it != theMap[stmt].end())
			theNext.push_back(-1);
	}
	if(theNext.size() == 3)
		reverseVec(theNext);
}

//(for different nodes, insert a -1 inside.)
void CFG::getNext(vector<int>& theNext, int stmt, AST* ast){
	if (mAst == NULL)
	{
		mAst = ast;
		generateCFG();
	}
	getNextLink(theNext, stmt, forwardMap);
}		

//(for different nodes, insert a -1 inside.)
void CFG::getNextUp(vector<int>& theNextUp, int stmt, AST* ast){
	if (mAst == NULL)
	{
		mAst = ast;
		generateCFG();
	}

	getNextLink(theNextUp, stmt, reverseMap);
}

//For HQ's reqest, the bigger stmt number should be at the first position of the vector,
//followed by -1, then followed by smaller stmt number.
void CFG::reverseVec(vector<int>& vec){
	vector<int>::iterator it = vec.begin();
	int firstStmt = *it;
	int secStmt = *(it+2);
	vec.clear();
	if(firstStmt > secStmt){
		vec.push_back(firstStmt);
		vec.push_back(-1);
		vec.push_back(secStmt);
	}else {
		vec.push_back(secStmt);
		vec.push_back(-1);
		vec.push_back(firstStmt);
	}
}

//For debugging
map<int, set<int>> CFG::getForwardMap(){ return this->forwardMap; }
map<int, set<int>> CFG::getReverseMap(){ return this->reverseMap; }

//For debugging:
void CFG::prtMap(map<int, set<int>> &currMap){
	for(map<int, set<int>>::iterator it = currMap.begin(); it != currMap.end(); it++){
		int a = it->first;
		set<int> tmp = currMap[a];
		cout<<"stmt "<<a<<": ";
		for(set<int>::iterator itSet = tmp.begin(); itSet != tmp.end(); itSet++)
			cout<<*itSet<<", ";
		cout<<endl;
	}
}//end prtMap