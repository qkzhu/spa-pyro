#ifndef QUERY_EVALUATOR_H
#define QUERY_EVALUATOR_H

#include <map>
#include <algorithm>


#include "PqlPreprocessor.h"
#include "PKB.h"
#include "QueryResult.h"
#include "TupleOperations.h"

#define ENCODE_ENDING 500  //I need to use it when I meet '_' in relation to create new variable


class QueryEvaluator{
	
private:
	PKB *mPKBObject;  //Connect to PKB side
	PqlPreprocessor *mQueryTree;   //Connect to PQL parser side
	QueryResult mResult;  //The final result in QueryResult.
	std::vector<int> mgTupleIndexing; //The global indexing for tuple evaluation, corresponding to the current tuple result

	std::vector<int> getChildStar(int stmtN);
	std::vector<int> getFollowsStar(int stmtN);
	std::vector<int> getCallsStar(int procN);

	vector<int> getAllStmts();

	

	string PQL_procDecode(int);
	int PQL_procEncode(string);
	string PQL_varDecode(int);
	int PQL_varEncode(string);

	int PKB_procEncode(string);
	string PKB_procDecode(int);
	int PKB_varEncode(string);
	string PKB_varDecode(int);

public:
	QueryEvaluator(PKB *, PqlPreprocessor *);  //Constructor
	void evaluate();//Preprocessor call evaluate to pass Query tree to Evaluator
	std::vector<std::vector<int> > getRel(int type1, int type2, int para1, int para2, int relType);
	QueryResult getResult();//After calling evaluate, the result stored inside mResult and can be retrieved by this function.


	int PQL_getIndex(string);
	void printResult();
};

#endif
