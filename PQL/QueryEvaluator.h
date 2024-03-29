#ifndef QUERY_EVALUATOR_H
#define QUERY_EVALUATOR_H

#include <map>
#include <algorithm>
#include <cstring>

//#include "AbstractWrapper.h"
#include "PqlPreprocessor.h"
#include "PKB.h"
#include "QueryResult.h"
#include "TupleOperations.h"


#define ENCODE_ENDING 500  //I need to use it when I meet '_' and constant in relation to create new variable for join

typedef struct{

		int initial_stmt;

		bool is_affects_table_built;

		//affects table
		vector<vector<int> > table_body_down;
		vector<vector<int> > table_body_up;

		//map for assign stmt# to index in affects table
		map<int, int> affects_index_down;
		map<int, int> affects_index_up;

} affectsTableStructure;

class QueryEvaluator{
	
private:
	PKB *mPKBObject; 
	PqlPreprocessor *mQueryTree;  

	//The final result in QueryResult
	QueryResult mResult;  

	//The global indexing for tuple evaluation, corresponding to the current tuple result
	std::vector<int> mgTupleIndexing; 

	//use it when meet '_' or constant in relation to create new variable
	int varCodeEnding;  

	//evaluation tuple, this is the final and medium evaluation tuple
	vector<vector<int> > evalTuple; 

	bool isBoolSelected;

	affectsTableStructure affectsTable;

	//check affects in the whole query and build affects table dependently
	void initialAffectsTable();

	//Evaluating query functions
	void initialize();

	void evaluateWith(bool& unrelated_finish, int& last_point, int threshold);

	void evaluatePattern(bool& unrelated_finish, int& last_point, int threshold);

	void evaluateSuchThat(bool& unrelated_finish, int& last_point, int threshold);

	//generate result based on selection elements and evaluation result
	void generateResult();

	//after a partition's evaluation, check whether it's null
	void checkValid();


	//remove unpaired tuple in relation result when the relation of same parameter, like ***(a, a), is evaluated
	void removeInequal(vector<vector<int> >& tuple);
	
	//result is initially empty, it always result returns tuple with the code of the first variable code;
	void evalPattern_PQL(vector<vector<int> >& result, int var, int var_type, int pattern1_type, int pattern1, int pattern2);

	//read such that clause, deal with _ specifically
	void underScore(int rel, vector<int> clause, int& para1, int& para1_type, int& para2, int& para2_type, int& varCodeEnding);
	//
	void joinTuples(vector<vector<int> >& result, vector<vector<int> >& pre_tuple, int commonNum, int same1Tuple1, int same2Tuple1, int first_time);
	
	//Deal with no selected elements in the result_tuple and patial case
	void transform(vector<vector<int> >& pre_tuple, vector<vector<int> >& candidates, vector<int>& select_index, vector<int> non_evaled_selects);
	
	//load candidates for relation evaluation. candidates needs no type.
	void checkCandidates(int var, int var_type, vector<int>& candidates);


	//Functions for evaluating such that relations
	
	//For function with star, if star == NOSTAR, means no star; if star == STAR, means eval star.
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

	void removeDuplicates(vector<int>& v);

	void getAllType(vector<int>& result, int type);

	//If the element is found in the vector, return its index; otherwise return the size of the vector.
	int find_ele(const vector<int>& in, const int ele);

	//find the next stmt of the if stmt's ending of then, notice this is not the last stmt
	int nextOfIf(int first);

	//get all the affected stmts from s
	bool nonModPath(int s, int mod, int dest, int start, vector<int>& affect_result,bool init); 


public:
	QueryEvaluator(PKB *, PqlPreprocessor *);  //Constructor
	void evaluate();//Preprocessor call evaluate to pass Query tree to Evaluator
	QueryResult getResult();//After calling evaluate, the result stored inside mResult and can be retrieved by this function.


	//Put these functions public for autotester uses
	int PKB_procEncode(string);
	string PKB_procDecode(int);
	int PKB_varEncode(string);
	string PKB_varDecode(int);


	int PQL_getIndex(string);
	void printResult();
};

#endif
