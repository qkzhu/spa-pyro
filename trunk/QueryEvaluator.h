#include <iostream>
#include <vector>
#include <map>
#include <algorithm>


#include "PQLParserAPIs.h"
#include "PKB.h"
#include "QueryResult.h"
#include "TupleOperations.h"

//QueryEvaluator accepts PKB data structure and Query data structure and evaluate the result by them.
/************************Special API required, Need to sync with PKB group OR PQL Parser*********************************
PKB:
	bool hasStmt(int stmt#); //input a stmt# and check whether the program has this stmt#

	bool hasVar(int var); //input a PKB-encoded var and check whether the program has this var

	bool hasCons(int consV); //input a constant value and check whether the program has any constant with this value

	bool hasProc(int proc); //input a PKB-encode procedure name and check whether the program has this name of procedure

PQL:


***********************************************************************************************************/
class QueryEvaluator{
	
private:
	PKB *mPKBObject;  //Connect to PKB side
	QueryParser *mQueryTree;   //Connect to PQL parser side
	QueryResult mResult;  //The final result in QueryResult.
	std::vector<int> mgTupleIndexing; //The global indexing for tuple evaluation, corresponding to the current tuple result
	

	bool mgNullClause; //If any clause evaluate to erroe, just jump out and return NULL for this evaluation.

	std::vector<std::vector<int> > getRel(int type1, int type2, int para1, int para2, int relType);

	//std::vector<std::vector<int> > *getRelStar(int type1, int type2, int para1, int para2, int relType); 

	std::vector<int> getChildStar(int stmtN);
	std::vector<int> getFollowsStar(int stmtN);
	std::vector<int> getCallsStar(int procN);

public:
	QueryEvaluator(PKB *, QueryParser *);  //Constructor
	void evaluate();//Preprocessor call evaluate to pass Query tree to Evaluator

	QueryResult getResult();//After calling evaluate, the result stored inside mResult and can be retrieved by this function.
};

