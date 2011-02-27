#include <map>
#include <algorithm>


#include "PQLParserAPIs.h"
#include "PKB_APIs.h"
#include "QueryResult.h"
#include "TupleOperations.h"

class QueryEvaluator{
	
private:
	PKB *mPKBObject;  //Connect to PKB side
	QueryParser *mQueryTree;   //Connect to PQL parser side
	QueryResult mResult;  //The final result in QueryResult.
	std::vector<int> mgTupleIndexing; //The global indexing for tuple evaluation, corresponding to the current tuple result
	

	bool mgNullClause; //If any clause evaluate to erroe, just jump out and return NULL for this evaluation.

	

	//std::vector<std::vector<int> > *getRelStar(int type1, int type2, int para1, int para2, int relType); 

	std::vector<int> getChildStar(int stmtN);
	std::vector<int> getFollowsStar(int stmtN);
	std::vector<int> getCallsStar(int procN);

	string PQL_procDecode(int);
	int PQL_procEncode(string);
	string PQL_varDecode(int);
	int PQL_varEncode(string);

	int PKB_procEncode(string);
	string PKB_procDecode(int);
	int PKB_varEncode(string);
	string PKB_varDecode(int);

public:
	QueryEvaluator(PKB *, QueryParser *);  //Constructor
	void evaluate();//Preprocessor call evaluate to pass Query tree to Evaluator
	std::vector<std::vector<int> > getRel(int type1, int type2, int para1, int para2, int relType);
	QueryResult getResult();//After calling evaluate, the result stored inside mResult and can be retrieved by this function.

	void printResult();
};

