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
	
	//for use when in with clause, there is sth like v.varName = p.varName .... OLD WAY
	//If contains only the pair of variable codes, no type needed.
	vector<int> equal_vars;

	//maps the variable to the value assigned in With clause, attr 1 is the variable string in its code
	//The value must have the type indicated
	map<int, vector<int> > var_value_table;


	//Evaluating query functions

	//returned result will not be very large
	void evalWith(vector<vector<int> >& result, vector<int>& equal_var, vector<int>& var_value_table);
	
	//The result_tuple is passed down for optimisation purpose, result is initially empty
	void evalPattern_PQL(vector<vector<int> >& result_tuple, vector<int>& result, int var, int var_type, int pattern1, int pattern2);

	//read such that clause, deal with _ specifically
	void underScore(int rel, vector<int> clause, int& para1, int& para1_type, int& para2, int& para2_type, int& var_code_ending);
	//
	void joinTuples(vector<vector<int> >& result, vector<vector<int> >& pre_tuple, int commonNum, int same1Tuple1, int same2Tuple1);
	
	//Deal with no selected elements in the result_tuple and patial case
	void joinSelection(vector<vector<int> >& pre_tuple, vector<vector<int> >& candidates);
	
	
	//Functions for evaluating such that relations
	
	//For function with star, if star == NOSTAR, means no star; if star == STAR, means eval star.
	//I should think about collect all relation evaluation into this format. Don't forget
	//Iteration 2
	void evalNext(int star, std::vector<std::vector<int> >& result, const std::vector<int>& para1, const std::vector<int>& para2);
	//If up == UP, means search from down to up
	void getNextStar(int up, std::vector<int>& result, int para);
	void getNextPure(int up, std::vector<int>& result, int para);

	//For efficiency of getAffects, I do not retrieve all tuples, but pass the tuple candidate and decide whether they affect each other.
	//Candidates of the two parameters is passed inside. Have to be assign. Return well-formed tuple result
	void evalAffects(std::vector<std::vector<int> >& result, const std::vector<int>& para1, const std::vector<int>& para2);
	void evalAffectsStar(std::vector<std::vector<int> >& result, const std::vector<int>& para1, const std::vector<int>& para2);
	//If up == UP, means search from down to up
	void getAffectsStar(int up, std::vector<int>& result, int para);
	void getAffects(int up, std::vector<int>& result, int para);
	bool affects(int stmt1, int stmt2);

	//For evaluating pattern
	//result passed in is initially filled with all candidates ///////////////IMPORTANT
	void getPattern_PQLAssign(std::vector<int>& result, string patternLeft, string patternRight);
	void getPattern_PQLCond(std::vector<int>& result, int type, string patternCond);

	//For evaluation other iteration 1 relations
	void evalFollows(int star, std::vector<std::vector<int> >& result, const std::vector<int>& para1, const std::vector<int>& para2);
	void evalParent(int star, std::vector<std::vector<int> >& result, const std::vector<int>& para1, const std::vector<int>& para2);
	void evalCalls(int star, std::vector<std::vector<int> >& result, const std::vector<int>& para1, const std::vector<int>& para2);
	void evalMU(int modOrUse, std::vector<std::vector<int> >& result, int type1, const std::vector<int>& para1, const std::vector<int>& para2);

	//If up == UP, mean it's bottom-down search
	void getChildStar(int up, std::vector<int> &, int stmtN);
	void getFollowsStar(int up, std::vector<int> &, int stmtN);
	void getCallsStar(int up, std::vector<int> &, int procN);
	
	
	//Other augmental functions
	void getAllStmts(std::vector<int>& result);
	void getAllVar(std::vector<int>& result);
	void getAllProc(std::vector<int>& result);

	string PQL_procDecode(int);
	int PQL_procEncode(string);
	string PQL_varDecode(int);
	int PQL_varEncode(string);

	int PKB_procEncode(string);
	string PKB_procDecode(int);
	int PKB_varEncode(string);
	string PKB_varDecode(int);

	void getAllType(vector<int>& result, int type);

	bool isIf(int stmt);
	
	bool isWhile(int stmt);
	bool isInsideWhile(int parent, int stmt); //check if this stmt is inside the while loop
	bool isInsideIf(int ifstat, int stmt);

	int find_ele(const vector<int>& in, const int ele);//If the element is found in the vector, return its index; otherwise return the size of the vector.

	bool nonModPath(int s, int mod, int dest, int final, bool& find_dest); //Check whether there is a non-mod path for variable mod in the stmt


public:
	QueryEvaluator(PKB *, PqlPreprocessor *);  //Constructor
	void evaluate();//Preprocessor call evaluate to pass Query tree to Evaluator
	QueryResult getResult();//After calling evaluate, the result stored inside mResult and can be retrieved by this function.

	int PQL_getIndex(string);
	void printResult();
};

#endif
