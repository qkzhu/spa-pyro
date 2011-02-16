#include <iostream>
#include <vector>
//#include "Tree.h"
#include "PQLpreprocessor.h"
#include "Driver.h"
#include "query_Result.h"
#include "Parser.h"

class query_Evaluator{
	//I assume the existence of some APIs.
	//PKB: TNode getFollowingStatement(TNode);
	//		TNode getPreviousStatement(TNode);
	//Preprocessor: QNode, ClauseNode APIs. 
	//PNode::List<PNode> getChild();  //
	//relNode:: string getType();
	//follows:: string getPara1();
	//			string getPara2();
	//followsS:: string getPara1();
	//			 string getPara2();
private:
	query_Result result;
	void getStar(AST *, Node*, string);

public:
	query_Evaluator();
	void evaluate(QueryTree*, AST *, VarTable *);//Preprocessor call evaluate to pass Query tree to Evaluator
	vector<int> *getResult();//After calling evaluate, the result stored inside and can be retrieved.
	

};

