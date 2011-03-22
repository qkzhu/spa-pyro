#include "QueryEvaluator.h"
#include <string>

using namespace std;

int const UP = 1;
int const DOWN = 0;

int const STAR = 1;
int const NOSTAR = 0;

int const MOD = 1;
int const USE = 0;

int const PROC = 1;
int const STMT_QE = 0;


//Controller passes the PKB and Query Parser to evaluator, and trigger evaluator to start.
QueryEvaluator::QueryEvaluator(PKB *p, PqlPreprocessor *q)
{
	mPKBObject = p;
	mQueryTree = q;
}

//*********Noting before coding*****************
//In the current implementation, I just have basic evaluator. Evaluate one clause after one. No complex optimization.
//For relational linking, it's to be implemented after the basic one.
void QueryEvaluator::evaluate()
{
	//evaluation tuple, this is the final and medium evaluation tuple
	vector<vector<int> > eva_tuple;   

	int var_code_ending = ENCODE_ENDING+1; //I need to use it when I meet '_' in relation to create new variable

	vector<int> select;
	mQueryTree->selectAt(select, 0);  //Elements to be selected
	bool is_bool_sel = false;
	if(select[0] == mQueryTree->getIndex("BOOLEAN")) is_bool_sel = true;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////FOR DEBUGGING
	cout<< "PQL parser checking"<< endl;
	cout << "Pattern clauses: " << endl;
	for(int i=0; i< mQueryTree->patternSize(); i++){
		vector<int> tmp_pattern;
		mQueryTree->patternAt(tmp_pattern, i);
		for(int k = 0; k < (int)tmp_pattern.size(); k++){
			cout << tmp_pattern.at(k) << " ";
		}
		cout << endl;
	}
	cout << "With clauses: " << endl;
	for(int i=0; i< mQueryTree->withSize(); i++){
		vector<int> tmp_with;
		mQueryTree->withAt(tmp_with, i);
		for(int k = 0; k < (int)tmp_with.size(); k++){
			cout << tmp_with.at(k) << " ";
		}
		cout << endl;
	}
	cout << "Such that clauses: " << endl;
	for(int i=0; i< mQueryTree->suchThatSize(); i++){
		vector<int> tmp_suchthat;
		mQueryTree->suchThatAt(tmp_suchthat, i);
		for(int k = 0; k < (int)tmp_suchthat.size(); k++){
			cout << tmp_suchthat.at(k) << " ";
		}
		cout << endl;
	}
	cout << "PQL PARSER checking FINISH." << endl;
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//QE evaluate the With clause, then pattern clause, and in the end Such That clause.

	//Start evaluating With clauses                                  
	int with_size = mQueryTree->withSize();
	for(int i = 0; i<with_size; i++){
		vector<int> clause;
		mQueryTree->withAt(clause, i);
		int clause_size = (int)clause.size();
		int ref = clause.back();

		//Read the with clause and divide it
		vector<int> part1;
		vector<int> part2;
		int withCount = 0;
		for(; withCount<clause_size; withCount++){
			if(clause.at(withCount) != mQueryTree->getIndex("="))
				part1.push_back(clause.at(withCount));
			else break;
		}
		withCount++;
		for(; withCount<clause_size; withCount++){
			part2.push_back(clause.at(withCount));
		}
		//clause dividing END

		int part2_size = (int) part2.size();

		int p1_type = part1.at(0);
		int p1_name = part1.at(1);
		int p2_type ;
		int p2_name ;
		if(part2_size == 2)  //Variable 
		{
			p2_type = part2.at(0);
			p2_name = var_code_ending++;
		}
		//For the case when a.stmt# = b.stmt#
		else if((int)part1.size() == part2_size)
		{
			p2_type = part2.at(0);
			p2_name = part2.at(1);
		}	
		vector<vector<int> > with_result;
		vector<int> tmp;
		if(p1_type == mQueryTree->getIndex("integer")||p1_type == mQueryTree->getIndex("procOfSimpl") || p1_type == mQueryTree->getIndex("varOfSimpl"))
			throw new string("QueryEvaluator, getAllType function can not take constant type!");
		else getAllType(tmp, p1_type);
		
		vector<int> tmp2;
		if(p2_type == mQueryTree->getIndex("integer")) tmp2.push_back(part2.at(1));
		else if(p2_type == mQueryTree->getIndex("varOfSimpl")) {
			int tmp_code = PKB_varEncode(PQL_varDecode(part2.at(1)));
			if(tmp_code == -1) {
				mResult.addInType(-1);
				return;
			}
			tmp2.push_back(tmp_code);
		}
		else if(p2_type == mQueryTree->getIndex("procOfSimpl")) {
			int tmp_code = PKB_procEncode(PQL_procDecode(part2.at(1)));
			if(tmp_code == -1){
				mResult.addInType(-1);
				return;
			}
			tmp2.push_back(tmp_code);
		}
		else if(p2_type == mQueryTree->getIndex("integer")||p2_type == mQueryTree->getIndex("procOfSimpl") || p2_type == mQueryTree->getIndex("varOfSimpl"))
			throw new string("QueryEvaluator, getAllType function can not take constant type!");
		else getAllType(tmp2, p2_type);
		
		int entry_type;
		if(p1_type == mQueryTree->getIndex("stmt")|| p1_type==mQueryTree->getIndex("assign")|| p1_type==mQueryTree->getIndex("while")|| p1_type==mQueryTree->getIndex("if")||p1_type==mQueryTree->getIndex("call") ||p1_type==mQueryTree->getIndex("constant") || p1_type==mQueryTree->getIndex("prog_line"))
			entry_type = mQueryTree->getIndex("integer");
		else if(p1_type==mQueryTree->getIndex("variable"))
			entry_type = mQueryTree->getIndex("varOfSimpl");
		else if(p1_type==mQueryTree->getIndex("procedure"))
			entry_type = mQueryTree->getIndex("procOfSimpl");
		else throw new string("with clause error type");

		for(vector<int>::iterator i=tmp.begin(); i< tmp.end(); i++)
			for(vector<int>::iterator k = tmp2.begin(); k<tmp2.end(); k++){
				if(*i == *k) {
					vector<int> entry;
					int entry_array [] = {entry_type, *i, entry_type, *k};
					entry.insert(entry.end(), entry_array, entry_array+4);
					with_result.push_back(entry);
				}
			}

		//Merging different evaluation tuples
		vector<int>::iterator it;
		it = find(mgTupleIndexing.begin(), mgTupleIndexing.end(), p1_name);
		int same1Tuple1 = 0;
		int same2Tuple1 = 0;
		int numOfCommonElement = 0;
		if(it == mgTupleIndexing.end())
			mgTupleIndexing.push_back(p1_name);
		else
		{
			for(vector<int>::iterator i = mgTupleIndexing.begin(); i<=it; i++){
				same1Tuple1++;
			}
			numOfCommonElement = 2;
		}
		it = find(mgTupleIndexing.begin(), mgTupleIndexing.end(), p2_name);
		if(it == mgTupleIndexing.end())
			if(p2_type == mQueryTree->getIndex("integer") || p2_type == mQueryTree->getIndex("varOfSimpl") || p2_type == mQueryTree->getIndex("varOfSimpl"))
				mgTupleIndexing.push_back(var_code_ending++);
			else mgTupleIndexing.push_back(p2_name);
		else
		{
			for(vector<int>::iterator i = mgTupleIndexing.begin(); i<=it; i++){
				same2Tuple1++;
			}
			numOfCommonElement = numOfCommonElement+2;
		}

		if(with_result.empty()){
			if(is_bool_sel)
				mResult.setBoolValue(false);
			else mResult.addInType(-1);
			return;
		}

		joinTuples(eva_tuple, with_result, numOfCommonElement, same1Tuple1, same2Tuple1);
	}//while: With clause evaluation End

	

	//Pattern Evaluation Start
	int patternSize = mQueryTree->patternSize();
	for(int i = 0; i < patternSize; i++){
		vector<int> clause;
		mQueryTree->patternAt(clause, i);
		int var_type = clause[0];
		int var = clause[1];
		int pattern1_type;
		int pattern1;
		int pattern2_type;
		int pattern2;
		
		//Read in pattern clause
		int next = 2;
		if(clause[next] == mQueryTree->getIndex("_")){
			pattern1_type = clause[next];
			pattern1 = clause[next++];
		}else{
			pattern1_type = clause[next];
			pattern1 = clause[next++];
		}
		if(clause[next++] != mQueryTree->getIndex(",")) {
			throw new string("QueryEvaluator::evaluate, pattern format error, no \",\"!");
		}
		
		pattern2_type = clause[next++];
		pattern2 = clause[next++];

		if(var_type == mQueryTree->getIndex("if"))
			if(clause[next] != mQueryTree->getIndex("_"))
				throw new string("QueryEvaluator::evaluate, pattern format error, if third parameter!");
		//pattern clause read-in finish//
      
		vector<int> pattern_result;
		evalPattern(eva_tuple, pattern_result, var, var_type, pattern1, pattern2);

		int it = find_ele(mgTupleIndexing, var);
		if(it == (int)mgTupleIndexing.size()){
			mgTupleIndexing.push_back(var);
			for(int row = 0; row < (int)eva_tuple.size(); row++){
				for(int indx = 0; indx < (int)pattern_result.size(); indx++)
					eva_tuple[row].push_back(pattern_result[indx]);
			}
		}else{
			for(int row = 0; row < (int)eva_tuple.size(); row++){
				int entry = eva_tuple[row][it];
				int found = find_ele(pattern_result, entry);
				if(found == (int)pattern_result.size())
					eva_tuple.erase(eva_tuple.begin() + row);
			}
		}
	}//Pattern Evaluation Finish

	//Start evaluating SuchThat clauses
	int suchThatSize = mQueryTree->suchThatSize();
	for(int i= 0; i<suchThatSize; i++)
	{
		vector<int> clause;
		mQueryTree->suchThatAt(clause, i);
		int rel = clause[0];
		int para1Type;
		int para1;
		int para2Type;
		int para2;

		
		underScore(rel, clause, para1, para1Type, para2, para2Type, var_code_ending);

		/*
		///////////////////////////////////////////TESTING//////////////////////////////////////////////////
		cout << "underScore check:" << endl;
		cout << para1 << endl;
		cout << para1Type << endl;
		cout << para2 << endl;
		cout << para2Type << endl;
		cout << "underScore check finish" << endl;
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		*/

		//Evaluating Relation
		vector<vector<int> > relResult;
		map<int,vector<int> >::iterator it1 = var_value_table.find(para1);
		map<int,vector<int> >::iterator it2 = var_value_table.find(para2);

		//If this variable is assigned with a value in with clause, do the substitution.
		if(it1 != var_value_table.end()){
			para1Type = (it1->second).at(0);
			para1 = (it1->second).at(1);
		}
		if(it2 != var_value_table.end()){
			para2Type = (it2->second).at(0);
			para2 = (it2->second).at(1);
		}

		
		//Convert all "a", "b", "c" 's code from PQL code to PKB code
		if(para1Type == mQueryTree->getIndex("varOfSimpl")) para1 = PKB_varEncode(PQL_varDecode(para1));
		else if(para1Type == mQueryTree->getIndex("procOfSimpl")) para1 = PKB_procEncode(PQL_procDecode(para1));

		if(para2Type == mQueryTree->getIndex("varOfSimpl")) para2 = PKB_varEncode(PQL_varDecode(para2));
		else if(para2Type == mQueryTree->getIndex("procOfSimpl")) para2 = PKB_procEncode(PQL_procDecode(para2));
	
		//When the parameter of relation is not in simple, the query just evaluate to null
		if(para1 == -1 || para2 == -1){
			if(is_bool_sel)
				mResult.setBoolValue(false);
			else mResult.addInType(-1);
			return;
		}

		//Check the input argument candidates
		vector<int> para1_collection;
		vector<int> para2_collection;

		if(para1Type == mQueryTree->getIndex("integer")) para1_collection.push_back(para1);
		else if(para1Type == mQueryTree->getIndex("procOfSimple")) para1_collection.push_back(para1);
		else if(para1Type == mQueryTree->getIndex("varOfSimple")) para1_collection.push_back(para1);
		else getAllType(para1_collection, para1Type);

		if(para2Type == mQueryTree->getIndex("integer")) para2_collection.push_back(para2);
		else if(para2Type == mQueryTree->getIndex("procOfSimple")) para2_collection.push_back(para2);
		else if(para2Type == mQueryTree->getIndex("varOfSimple")) para2_collection.push_back(para2);
		else getAllType(para2_collection, para2Type);

		/*
		//////////////////////////////////////TESTING /////////////////////////////////////////////////////////
		cout << "para1_collection = " << endl;
		for(int p = 0; p < (int)para1_collection.size(); p++){
			cout << para1_collection.at(p) << " ";
		}
		cout << endl;
		cout << "para2_collection = " << endl;
		for(int p = 0; p < (int)para2_collection.size(); p++){
			cout << para2_collection.at(p) << " ";
		}
		cout << endl;
		/////////////////////////////////////////////////////////////////////////////////////////////////////////
		*/

		if(rel == mQueryTree->getIndex("parent")){	
			evalParent(NOSTAR, relResult, para1_collection, para2_collection);
		}else if(rel==mQueryTree->getIndex("parent*")){
			evalParent(STAR, relResult, para1_collection, para2_collection);
		}else if(rel==mQueryTree->getIndex("follows")){
			evalFollows(NOSTAR, relResult, para1_collection, para2_collection);
		}else if(rel==mQueryTree->getIndex("follows*")){
			evalFollows(STAR, relResult, para1_collection, para2_collection);
		}else if(rel==mQueryTree->getIndex("uses")){
			if(para1Type == mQueryTree->getIndex("procedure"))
				evalMU(USE, relResult, mQueryTree->getIndex("procOfSimpl"), para1_collection, para2_collection);
			else if(para1Type == mQueryTree->getIndex("stmt") || para1Type == mQueryTree->getIndex("assign")|| para1Type == mQueryTree->getIndex("if")|| para1Type == mQueryTree->getIndex("while")|| para1Type == mQueryTree->getIndex("call")||para1Type == mQueryTree->getIndex("integer"))
				evalMU(USE, relResult, mQueryTree->getIndex("integer"), para1_collection, para2_collection);
			else throw new string("QueryEvaluator::evaluate, for evaluating uses, no such para type");
		}else if(rel==mQueryTree->getIndex("modifies")){
			if(para1Type == mQueryTree->getIndex("procedure"))
				evalMU(MOD, relResult, mQueryTree->getIndex("procOfSimpl"), para1_collection, para2_collection);
			else if(para1Type == mQueryTree->getIndex("stmt") || para1Type == mQueryTree->getIndex("assign")|| para1Type == mQueryTree->getIndex("if")|| para1Type == mQueryTree->getIndex("while")|| para1Type == mQueryTree->getIndex("call")||para1Type == mQueryTree->getIndex("integer"))
				evalMU(MOD, relResult, mQueryTree->getIndex("integer"), para1_collection, para2_collection);
			else throw new string("QueryEvaluator::evaluate, for evaluating modifies, no such para type");
		}else if(rel==mQueryTree->getIndex("calls")){
			evalCalls(NOSTAR, relResult, para1_collection, para2_collection);
		}else if(rel==mQueryTree->getIndex("calls*")){
			evalCalls(STAR, relResult, para1_collection, para2_collection);
		}else if(rel==mQueryTree->getIndex("next")){
			evalNext(NOSTAR, relResult, para1_collection, para2_collection);
		}else if(rel==mQueryTree->getIndex("next*")){
			evalNext(STAR, relResult, para1_collection, para2_collection);
		}else if(rel==mQueryTree->getIndex("affects")){
			//Check whether the parameters are assign
			vector<int> assigns;
			mPKBObject->ast_GetAllAssign(assigns);
			for(int i = 0; i< (int)para1_collection.size(); i++){
				int found = find_ele(assigns, para1_collection[i]);
				if(found == (int)assigns.size()){
					para1_collection.erase(para1_collection.begin() + i);
				}
			}
			for(int i = 0; i< (int)para2_collection.size(); i++){
				int found = find_ele(assigns, para2_collection[i]);
				if(found == (int)assigns.size()){
					para2_collection.erase(para2_collection.begin() + i);
				}
			}
			//evaluate affects
			evalAffects(relResult, para1_collection, para2_collection);
		}else if(rel==mQueryTree->getIndex("affects*")){
			//Check whether the parameters are assign
			vector<int> assigns;
			mPKBObject->ast_GetAllAssign(assigns);
			for(int i = 0; i< (int)para1_collection.size(); i++){
				int found = find_ele(assigns, para1_collection[i]);
				if(found == (int)assigns.size()){
					para1_collection.erase(para1_collection.begin() + i);
				}
			}
			for(int i = 0; i< (int)para2_collection.size(); i++){
				int found = find_ele(assigns, para2_collection[i]);
				if(found == (int)assigns.size()){
					para2_collection.erase(para2_collection.begin() + i);
				}
			}
			//evaluate affects star
			evalAffectsStar(relResult, para1_collection, para2_collection);
		}else throw new string("Relation not exists");
		
		if(relResult.empty()){
			if(is_bool_sel)
				mResult.setBoolValue(false);
			else mResult.addInType(-1);
			return;
		}

		//Manipulation of indexing issue, adjust the mgTupleIndexing and find common index to pass
		int it = find_ele(mgTupleIndexing, para1);
		int same1Tuple1 = 0;
		int same2Tuple1 = 0;
		int numOfCommonElement = 0;
		if(it == (int)mgTupleIndexing.size())
			if(para1Type == mQueryTree->getIndex("integer") || para1Type == mQueryTree->getIndex("varOfSimpl") || para1Type == mQueryTree->getIndex("procOfSimpl"))
				mgTupleIndexing.push_back(var_code_ending++);
			else mgTupleIndexing.push_back(para1);
		else
		{
			for(int i = 0; i<=it; i++){
				same1Tuple1++;
			}
			numOfCommonElement = 2;
		}
		it = find_ele(mgTupleIndexing, para2);
		if(it == (int)mgTupleIndexing.size())
			if(para2Type == mQueryTree->getIndex("integer") || para2Type == mQueryTree->getIndex("varOfSimpl") || para2Type == mQueryTree->getIndex("procOfSimpl"))
				mgTupleIndexing.push_back(var_code_ending++);
			else mgTupleIndexing.push_back(para2);
		else
		{
			for(int i = 0; i<=it; i++){
				same2Tuple1++;
			}
			numOfCommonElement = numOfCommonElement+2;
		}

		cout << "relResult = " << endl;
		for(int p = 0; p < (int)relResult.size(); p++){
			vector<int> tmp_store = relResult[p];
			for(int j = 0; j < (int)tmp_store.size(); j++){
				cout << tmp_store.at(j) << " ";
			}
			cout << endl;
		}

		cout << "Checking mgTupleIndexing: " << endl;
		for(int k = 0; k < (int)mgTupleIndexing.size(); k++){
			cout << mgTupleIndexing[k] << " ";
		}
		cout << endl;
		cout << numOfCommonElement <<" "<< same1Tuple1 << " " <<  same2Tuple1 << endl;

		joinTuples(eva_tuple, relResult, numOfCommonElement, same1Tuple1, same2Tuple1);

		/*
		cout << "eva_tuple = " << endl;
		for(int p = 0; p < (int)eva_tuple.size(); p++){
			vector<int> tmp_store = eva_tuple[p];
			for(int j = 0; j < (int)tmp_store.size(); j++){
				cout << tmp_store.at(j) << " ";
			}
			cout << endl;
		}*/

	}//while: such that evaluation END

	if(is_bool_sel){  //If the select is boolean
		if(with_size == 0 && suchThatSize == 0 && patternSize == 0){
			mResult.setBoolValue(true);
			return;
		}

		if(mgTupleIndexing.empty()) mResult.setBoolValue(false);
		else mResult.setBoolValue(true);
		return;
	}

	//find which of the selected elements are inside the evaluated result tuple
	//1. no select element is in the result tuple
	//2. partial select elements are in the result tuple
	//3.. all select elements are in the result tuple
	bool is_selected = true;
	vector<int> indexes;
	for(int i = 0; i < mQueryTree->selectSize(); i++){
		vector<int> tmp_selected;
		mQueryTree->selectAt(tmp_selected, i);
		int select = tmp_selected[1];
		int indx = find_ele(mgTupleIndexing, select);
		if(indx == (int)mgTupleIndexing.size()){
			indexes.push_back(-1);
			is_selected = false;
		}else indexes.push_back(indx);
	}
	


	/*
	cout << "eval_tuple check:" << endl;
	for(int i = 0; i < (int)eva_tuple.size(); i++){
		vector<int> ele = eva_tuple.at(i);
		for(int p = 0; p < (int)ele.size(); p++){
			cout << ele[p] << " ";
		}
		cout << endl;
	}
	cout << "Checking finish" << endl;
	*/

	//if no with and such that clause, just return;
	//Or if the selected element is not inside evaluated result tuple, depend on the tuple
	if((with_size == 0 && suchThatSize == 0)|| ((int)mgTupleIndexing.size()!=0 && !is_selected)){
		
		//if the selected element is not inside evaluated result tuple and result tuple is null
		if((int)mgTupleIndexing.size() != 0){
			if(eva_tuple.empty()) {
				mResult.addInType(-1);
				return;
			}
		}
		
		//Get all selection variable candidates
		vector<vector<int> > selection_candidates;
		for(int i = 0; i < mQueryTree->selectSize(); i++){
			vector<int> tmp_selected;
			mQueryTree->selectAt(tmp_selected, i);
			int select_type = tmp_selected[0];
			vector<int> tmp;
			if(select_type == mQueryTree->getIndex("integer") || select_type == mQueryTree->getIndex("procOfSimpl") || select_type == mQueryTree->getIndex("varOfSimpl"))  
				throw new string("QueryEvaluator, getAllType function can not take constant type!");
			else getAllType(tmp, select_type);
			selection_candidates.push_back(tmp);
			if(select_type == mQueryTree->getIndex("procedure"))
				mResult.addInType(mQueryTree->getIndex("procOfSimpl"));
			else if(select_type == mQueryTree->getIndex("variable"))
				mResult.addInType(mQueryTree->getIndex("varOfSimpl"));
			else mResult.addInType(mQueryTree->getIndex("integer")); 
		}

		/* ///////////////////////////////////////////////////////////////TESTING ///////////////////////////
		cout << "selection_candidate check" << endl;
		for(int i = 0; i < (int)selection_candidates.size(); i++){
			vector<int> tmp_sto = selection_candidates[i];
			for(int j = 0; j < (int)tmp_sto.size(); j++){
				cout << tmp_sto[j] << " ";
			}
			cout << endl;
		}
		cout << "Checking finish!!!" << endl;
		*/

		//Join all the variable candidates
		vector<vector<int> > final_result;
		joinSelection(final_result, selection_candidates);
		
		/* //////////////////////////////////////////////////TESTING/////////////////////////////////////
		cout << "Final_result check" << endl;
		for(int i = 0; i < (int)final_result.size(); i++){
			vector<int> tmp_sto = final_result[i];
			for(int j = 0; j < (int)tmp_sto.size(); j++){
				cout << tmp_sto[j] << " ";
			}
			cout << endl;
		}
		cout << "Checking FINISH!!!" << endl;*/

		for(int i = 0; i < (int)final_result.size(); i++){
			vector<int> new_tuple_tmp;
			for(int k = 0; ; k++){
				if( (k*2+1) >= (int)final_result[i].size()) break;
				new_tuple_tmp.push_back(final_result[i][k*2+1]);
			}
			mResult.addInTuple(new_tuple_tmp);
		}
		//mResult.print();   // TESTING
		//candidates join finishes
	}else{
		if(eva_tuple.size() == 0){
			mResult.addInType(-1);
			return;
		}

		int size = eva_tuple[0].size();
		for(int i = 0; i < (int)indexes.size(); i++){
			int type = eva_tuple[0][indexes[i]*2];
			mResult.addInType(type);
		}

		for(int i = 0; i< (int)eva_tuple.size(); i++){
			vector<int> tmp_entry;
			for(int j = 0; j < (int)mQueryTree->selectSize(); j++){
				int v = eva_tuple[i][indexes[j]*2+1];
				tmp_entry.push_back(v);
			}
			mResult.addInTuple(tmp_entry);
		}
	}

	//In case the returned mResult is empty, then insert -1 to the result.
	vector<int> resultType = mResult.getTypes();
	if(resultType.empty()) mResult.addInType(-1);
}//End of evaluate


void QueryEvaluator::joinSelection(vector<vector<int> >& pre_tuple, vector<vector<int> >& candidates){
	int size = candidates.size();
	for(int i = 0; i < size; i++){
		int type = mResult.getTypeAt(i);
		vector<int> tmp_tuple = candidates.at(i);
		vector<vector<int> > new_tuple;
		for(int j = 0; j < (int)tmp_tuple.size(); j++){
			vector<int> tmp;
			tmp.push_back(type);
			tmp.push_back(tmp_tuple.at(j));
			new_tuple.push_back(tmp);
		}
		joinTuples(pre_tuple, new_tuple, 0, 0, 0);
	}
}

void QueryEvaluator::underScore(int rel, vector<int> clause, int& para1, int& para1_type, int& para2, int& para2_type, int& var_code_ending){
	int next_indx = 0;
	if(clause[1] == mQueryTree->getIndex("_"))// if the first para is _
	{ 
		para1 = clause[1];
		next_indx = 2;
	}
	else 
	{
		para1_type = clause[1];
		para1 = clause[2];
		next_indx = 3;
	}
	if(clause[next_indx] == mQueryTree->getIndex("_"))// if the second para is _
	{ 
		para2 = clause[next_indx];
	}
	else 
	{
		para2_type = clause[next_indx];
		para2 = clause[next_indx+1];
	}

	if(rel == mQueryTree->getIndex("parent") || rel == mQueryTree->getIndex("parent*") || rel == mQueryTree->getIndex("follows") || rel == mQueryTree->getIndex("follows*") || rel == mQueryTree->getIndex("next") || rel == mQueryTree->getIndex("next*") || rel == mQueryTree->getIndex("affects") || rel == mQueryTree->getIndex("affects*")) //relation is parent, parent*, follows, follows*
	{
		if(para1 == mQueryTree->getIndex("_")){  //If _ , then replace with (stmt newest)
			para1_type = mQueryTree->getIndex("stmt");
			para1 = var_code_ending++;
		}if(para2 == mQueryTree->getIndex("_")){
			para2_type = mQueryTree->getIndex("stmt");
			para2 = var_code_ending++;
		}
	}
	else if(rel == mQueryTree->getIndex("uses") || rel == mQueryTree->getIndex("modifies")) // uses and modifies
	{		
		if(para2 == mQueryTree->getIndex("_")){ //replace with (variable newest)
			para2_type = mQueryTree->getIndex("variable");
			para2 = var_code_ending++;
		}
	}
	else if(rel == mQueryTree->getIndex("calls") || rel == mQueryTree->getIndex("calls*")) //relation is calls, calls*
	{
		if(para1 == mQueryTree->getIndex("_")){ //replace with (proc newest)
			para1_type = mQueryTree->getIndex("procedure");
			para1 = var_code_ending++;
		}if(para2 == mQueryTree->getIndex("_")){
			para2 = var_code_ending++;
			para2_type = mQueryTree->getIndex("procedure");
		}
	}else throw new string("QueryEvaluator::underScore, no such relation type!");  //do nothing 
}

void QueryEvaluator::evalPattern(vector<vector<int> >& result_tuple, vector<int>& result, int var, int var_type, int pattern1, int pattern2){
	int indx = find_ele(mgTupleIndexing, var);
	int found = 0;
	if(indx != (int)mgTupleIndexing.size()){
		found = 1;
		for(int i = 0; i < (int)result_tuple.size(); i++)
			result.push_back(result_tuple[i][indx]);
	}

	if(var_type == mQueryTree->getIndex("assign")){
		if(found == 0)
			mPKBObject->ast_GetAllAssign(result);
		getPatternAssign(result, mQueryTree->getContent(pattern1), mQueryTree->getContent(pattern2));
	}else if(var_type == mQueryTree->getIndex("if") || var_type == mQueryTree->getIndex("while")){
		if(found == 0)
			if(var_type == mQueryTree->getIndex("if"))
				mPKBObject->ast_GetAllIf(result);
			else if(var_type == mQueryTree->getIndex("while"))
				mPKBObject->ast_GetAllWhile(result);
		getPatternCond(result, var_type, mQueryTree->getContent(pattern1));
	}else 
		throw new string("QueryEvaluator::evalPattern, no such variable type!");
}

void QueryEvaluator::joinTuples(vector<vector<int> >& eva_tuple, vector<vector<int> >& pre_tuple, int common_num, int same1_tuple1, int same2_tuple1){
		if(common_num == 2)
			if(same1_tuple1 != 0)
				eva_tuple = TupleOperations::tupleJoinOneC(same1_tuple1, 0, eva_tuple, pre_tuple);
			else eva_tuple = TupleOperations::tupleJoinOneC(same2_tuple1, 1, eva_tuple, pre_tuple);
		else if(common_num == 4){
			eva_tuple = TupleOperations::tupleJoinTwoC(same1_tuple1, same2_tuple1, eva_tuple, pre_tuple);
		}else if((int) eva_tuple.size() == 0)
			eva_tuple = pre_tuple;
		else{
			vector<vector<int> > tmp_store;
			for(vector<vector<int> >::iterator i = eva_tuple.begin(); i<eva_tuple.end(); i++)
				for(vector<vector<int> >::iterator k = pre_tuple.begin(); k<pre_tuple.end(); k++){
					vector<int> join_entry;
					join_entry.insert(join_entry.end(), (*i).begin(), (*i).end());
					join_entry.insert(join_entry.end(), (*k).begin(), (*k).end());
					tmp_store.push_back(join_entry);
				}				
			eva_tuple = tmp_store;
		}
}

QueryResult QueryEvaluator::getResult()
{
	return mResult;
}

void QueryEvaluator::printResult(){
	if(mResult.isBoolSet()){
		cout << "Query evaluate: ";
		if(mResult.getBoolValue())
			cout << "true" << endl;
		else cout << "false" << endl;
	}else{
		vector<vector<int> > resultTuple = mResult.getTuple();
		vector<int> resultTupleType = mResult.getTypes();
		cout << "Query evaluate: ";

		if((int)resultTupleType.at(0) == -1){
			cout << "query result size: " << 0 << endl ;
			cout << "Null" << endl;
			return;
		}
		cout << "query result size: " << (int)resultTuple.size() << endl ;
		for(int i= 0; i< (int)resultTuple.size(); i++){
			int select_size = mQueryTree->selectSize();
			for(int j = 0; j < select_size; j++){
				int type = resultTupleType.at(j);
				if(type == mQueryTree->getIndex("integer"))
					cout << resultTuple[i][j] << " ";
				else if(type == mQueryTree->getIndex("varOfSimpl")) 
					cout << PKB_varDecode(resultTuple[i][j]) << " ";	
				else if(type == mQueryTree->getIndex("procOfSimpl")) 
					cout << PKB_procDecode(resultTuple[i][j]) << " ";
				else 
					throw new string("No type match when decode result for query result");
			}
			cout << endl;
		}
		cout << endl;
	}
}
	
void QueryEvaluator::evalParent(int star, vector<std::vector<int> >& result, const vector<int>& para1, const vector<int>& para2){
	if(para1.size() < para2.size()){
		for(int l = 0; l < (int)para1.size(); l++){
			int in1 = para1[l];
			vector<int> tmp1;
			if(star == NOSTAR) mPKBObject->ast_GetChild(tmp1, in1);
			else getChildStar(DOWN, tmp1, in1);
			if(tmp1[0] == -1) continue;    //continue this loop without eva this iteration
			for(int i = 0; i < (int)para2.size(); i++)
			{
				int in2 = para2[i];
				int found = find_ele(tmp1, in2);
				if(found == tmp1.size()) continue;
				vector<int> tmp2;
				int t[] = {mQueryTree->getIndex("integer"), in1, mQueryTree->getIndex("integer"), in2};
				tmp2.insert(tmp2.begin(), t, t+4);
				result.push_back(tmp2);
			}
		}
	}else{
		for(int l = 0; l < (int)para2.size(); l++){
			int in2 = para2[l];
			vector<int> tmp2;
			if(star == NOSTAR) tmp2.push_back(mPKBObject->ast_getParent(in2));
			else getChildStar(UP, tmp2, in2);
			if(tmp2[0] == -1) continue;    //continue this loop without eva this iteration
			for(int i = 0; i < (int)para1.size(); i++)
			{
				int in1 = para1[i];
				int found = find_ele(tmp2, in1);
				if(found == tmp2.size()) continue;
				vector<int> tmp1;
				int t[] = {mQueryTree->getIndex("integer"), in1, mQueryTree->getIndex("integer"), in2};
				tmp1.insert(tmp1.begin(), t, t+4);
				result.push_back(tmp1);
			}
		}
	}
}

//Given the first stmt, return all parent*(stmt, s) result of s
void QueryEvaluator::getChildStar(int up, vector<int>& result, int stmtN){
	if(up == DOWN){
		mPKBObject->ast_GetChild(result, stmtN);
		//It is important that I don't use iterator to loop descendant
		//It will cause some error
		if(result.at(0) != -1){
			for(int i = 0; i < (int)result.size(); i++){
				vector<int> k;
				mPKBObject->ast_GetChild(k, result[i]);
				if(k.at(0) != -1){
					result.insert(result.end(), k.begin(), k.end());
				}
			}
		}
	}else if(up == UP){
		result.push_back(mPKBObject->ast_getParent(stmtN));
		if(result.at(0) != -1){
			for(int i = 0; i < (int)result.size(); i++){
				int k = mPKBObject->ast_getParent(result[i]);
				if(k != -1){
					result.push_back(k);
				}
			}
		}
	}else throw new string("QueryEvaluator::getChildStar, no such up type.");
}

void QueryEvaluator::evalFollows(int star, vector<std::vector<int> >& result, const vector<int>& para1, const vector<int>& para2){
	if(para1.size() < para2.size()){
		for(int l = 0; l < (int)para1.size(); l++){
			int in1 = para1[l];
			vector<int> tmp1;
			if(star == NOSTAR) tmp1.push_back(mPKBObject->ast_GetFollowingStatementNum(in1));
			else getFollowsStar(DOWN, tmp1, in1);
			if(tmp1[0] == -1) continue;    //continue this loop without eva this iteration
			for(int i = 0; i < (int)para2.size(); i++)
			{
				int in2 = para2[i];
				int found = find_ele(tmp1, in2);
				if(found == (int)tmp1.size()) continue;
				vector<int> tmp2;
				int t[] = {mQueryTree->getIndex("integer"), in1, mQueryTree->getIndex("integer"), in2};
				tmp2.insert(tmp2.begin(), t, t+4);
				result.push_back(tmp2);
			}
		}
	}else{
		for(int l = 0; l < (int)para2.size(); l++){
			int in2 = para2[l];
			vector<int> tmp2;
			if(star == NOSTAR) tmp2.push_back(mPKBObject->ast_GetPreviousStatementNum(in2));
			else getFollowsStar(UP, tmp2, in2);
			if(tmp2[0] == -1) continue;    //continue this loop without eva this iteration
			for(int i = 0; i < (int)para1.size(); i++)
			{
				int in1 = para1[i];
				int found = find_ele(tmp2, in1);
				if(found == (int)tmp2.size()) continue;
				vector<int> tmp1;
				int t[] = {mQueryTree->getIndex("integer"), in1, mQueryTree->getIndex("integer"), in2};
				tmp1.insert(tmp1.begin(), t, t+4);
				result.push_back(tmp1);
			}
		}
	}
}

void QueryEvaluator::getFollowsStar(int up, vector<int>& result, int stmtN){
	if(up == DOWN){
		int follow = mPKBObject->ast_GetFollowingStatementNum(stmtN);
		if(follow == -1){
			result.push_back(-1);
		}
		while(follow != -1){
			result.push_back(follow);
			follow = mPKBObject->ast_GetFollowingStatementNum(follow);	
		}
	}else if(up == UP){
		int follow = mPKBObject->ast_GetPreviousStatementNum(stmtN);
		if(follow == -1){
			result.push_back(-1);
		}
		while(follow != -1){
			result.push_back(follow);
			follow = mPKBObject->ast_GetPreviousStatementNum(follow);	
		}
	}else throw new string("QueryEvaluator::getFollowsStar, no such up type");
}

void QueryEvaluator::evalCalls(int star, vector<std::vector<int> >& result, const vector<int>& para1, const vector<int>& para2){
	if(para1.size() < para2.size()){
		for(int l = 0; l < (int)para1.size(); l++){
			int in1 = para1[l];
			vector<int> tmp1;
			if(star == NOSTAR) mPKBObject->pTable_getCall(tmp1, in1);
			else getCallsStar(DOWN, tmp1, in1);
			if(tmp1[0] == -1) continue;    //continue this loop without eva this iteration
			for(int i = 0; i < (int)para2.size(); i++){
				int in2 = para2[i];
				int found = find_ele(tmp1, in2);
				if(found == (int)tmp1.size()) continue;
				vector<int> tmp2;
				int t[] = {mQueryTree->getIndex("procOfSimpl"), in1, mQueryTree->getIndex("procOfSimpl"), in2};
				tmp2.insert(tmp2.begin(), t, t+4);
				result.push_back(tmp2);
			}
		}
	}else{
		for(int l = 0; l < (int)para2.size(); l++){
			int in2 = para2[l];
			vector<int> tmp2;
			if(star == NOSTAR) mPKBObject->pTable_getCalled(tmp2, in2);
			else getCallsStar(UP, tmp2, in2);
			if(tmp2[0] == -1) continue;    //continue this loop without eva this iteration
			for(int i = 0; i < (int)para1.size(); i++)
			{
				int in1 = para1[i];
				int found = find_ele(tmp2, in1);
				if(found == tmp2.size()) continue;
				vector<int> tmp1;
				int t[] = {mQueryTree->getIndex("procOfSimpl"), in1, mQueryTree->getIndex("procOfSimpl"), in2};
				tmp1.insert(tmp1.begin(), t, t+4);
				result.push_back(tmp1);
			}
		}
	}
}
//There may be loop call, take care
void QueryEvaluator::getCallsStar(int up, vector<int>& result, int procNameCode){
	if(up == DOWN){
		mPKBObject->pTable_getCall(result, procNameCode);
		if(result.at(0) != -1){
			for(int i = 0; i < (int)result.size(); i++){
				vector<int> k;
				mPKBObject->pTable_getCall(k, result[i]);
				if(k.at(0) != -1){
					for(int j = 0; j < (int)k.size(); j++){
						int found = find_ele(result, k[j]);
						if(found != (int)result.size())	result.push_back(k[j]);
					}
				}
			}
		}
	}else if(up == UP){
		mPKBObject->pTable_getCalled(result, procNameCode);
		if(result.at(0) != -1){
			for(int i = 0; i < (int)result.size(); i++){
				vector<int> k;
				mPKBObject->pTable_getCalled(k, result[i]);
				if(k.at(0) != -1){
					for(int j = 0; j < (int)k.size(); j++){
						int found = find_ele(result, k[j]);
						if(found != (int)result.size())	result.push_back(k[j]);
					}
				}
			}
		}
	}else throw new string("QueryEvaluator::getCallsStar, no such up type.");
}

void QueryEvaluator::evalMU(int modOrUse, vector<vector<int> >& result, int type1, const vector<int>& para1, const vector<int>& para2){	
	if(para1.size() <= para2.size()){
		for(int i = 0; i < (int)para1.size(); i++){
			int in1 = para1[i];
			vector<int> tmp1;
			if(modOrUse == MOD)
				if(type1 == mQueryTree->getIndex("procOfSimpl"))
					mPKBObject->mTable_getModifiedVarPI(tmp1, in1);
				else if(type1 == mQueryTree->getIndex("integer"))
					mPKBObject->mTable_getModifiedVar(tmp1, in1);
				else throw new string("QueryEvaluator::evalMU, no such para type!");
			else if(modOrUse == USE)
				if(type1 == mQueryTree->getIndex("procOfSimpl"))
					mPKBObject->uTable_getUsedVarPI(tmp1, in1);
				else if(type1 == mQueryTree->getIndex("integer"))
					mPKBObject->uTable_getUsedVar(tmp1, in1);
				else throw new string("QueryEvaluator::evalMU, no such type.");
			else throw new string("QueryEvaluator::evalMU, no such relation type.");
			for(int j = 0; j < (int)para2.size(); j++){
				int in2 = para2[j];
				vector<int> tmp2;
				int found = find_ele(tmp1, in2);
				if(found == (int)tmp1.size()) continue;
				int t[4];
				if(type1 == mQueryTree->getIndex("procOfSimpl")){
					t[0] = mQueryTree->getIndex("procOfSimpl");
					t[1] = in1;
					t[2] = mQueryTree->getIndex("varOfSimpl");
					t[3] = in2;
				}else if(type1 == mQueryTree->getIndex("integer")){
					t[0] = mQueryTree->getIndex("integer");
					t[1] = in1;
					t[2] = mQueryTree->getIndex("varOfSimpl");
					t[3] = in2;
				}else throw new string("QueryEvaluator::evalMU, no such para type.");
				tmp2.insert(tmp2.end(), t, t+4);
				result.push_back(tmp2);
			}
		}
	}else {
		for(int i = 0; i < (int)para2.size(); i++){
			int in2 = para2[i];
			vector<int> tmp2;
			if(modOrUse == MOD)
				if(type1 == mQueryTree->getIndex("procOfSimpl"))
					mPKBObject->mTable_getProcModifies(tmp2, in2);
				else if(type1 == mQueryTree->getIndex("integer"))
					mPKBObject->mTable_getStmtModifies(tmp2, in2);
				else throw new string("QueryEvaluator::evalMU, no such para type.");
			else if(modOrUse == USE)
				if(type1 == mQueryTree->getIndex("procOfSimpl"))
					mPKBObject->uTable_getProcUses(tmp2, in2);
				else if(type1 == mQueryTree->getIndex("integer"))
					mPKBObject->uTable_getStmtUses(tmp2, in2);
				else throw new string("QueryEvaluator::evalMU, no such type.");
			else throw new string("QueryEvaluator::evalMU, no such relation type.");
			for(int j = 0; j < (int)para1.size(); j++){
				int in1 = para1[j];
				vector<int> tmp1;
				int found = find_ele(tmp2, in1);
				if(found == (int)tmp2.size()) continue;
				int t[4];
				if(type1 == mQueryTree->getIndex("procOfSimpl")){
					t[0] = mQueryTree->getIndex("procOfSimpl");
					t[1] = in1;
					t[2] = mQueryTree->getIndex("varOfSimpl");
					t[3] = in2;
				}else if(type1 == mQueryTree->getIndex("integer")){
					t[0] = mQueryTree->getIndex("integer");
					t[1] = in1;
					t[2] = mQueryTree->getIndex("varOfSimpl");
					t[3] = in2;
				}else throw new string("QueryEvaluator::evalMU, no such para type.");
				tmp1.insert(tmp1.end(), t, t+4);
				result.push_back(tmp1);
			}
		}
	}
}//evalMU finish

void QueryEvaluator::evalNext(int star, vector<vector<int> >& result, const vector<int>& para1, const vector<int>& para2){
	if(para1.size() <= para2.size()){
		for(int l = 0; l < (int)para1.size(); l++){
			int stmtN = para1[l];
			vector<int> nexts;
			if(star == NOSTAR) mPKBObject->getNext(nexts, stmtN);
			else if(star == STAR) getNextStar(DOWN, nexts, stmtN);
			else throw new string("QueryEvaluator::evalNext, no such star type!");
			
			for(int i = 0; i < (int)nexts.size() ; i++){
				if(i != -1){
					int found = find_ele(para2, nexts[i]);
					if(found != (int)para2.size()){
						int tmp[4] = {mQueryTree->getIndex("integer"), stmtN, mQueryTree->getIndex("integer"), nexts[i]};
						vector<int> entry;
						entry.insert(entry.end(), tmp, tmp+4);
						result.push_back(entry);
					}
				}
			}
		}
	}else{
		for(int l = 0; l < (int)para2.size(); l++){
			int stmtN = para2[l];
			vector<int> nexts;
			if(star == NOSTAR) mPKBObject->getNextUp(nexts, stmtN);
			else if(star == STAR) getNextStar(UP, nexts, stmtN);
			else throw new string("QueryEvaluator::evalNext, no such star type!");
			for(int i = 0; i < (int)nexts.size() ; i++){
				if(i != -1){
					int found = find_ele(para1, nexts[i]);
					if(found != para1.size()){
						int tmp[4] = {mQueryTree->getIndex("integer"),nexts[i] , mQueryTree->getIndex("integer"), stmtN};
						vector<int> entry;
						entry.insert(entry.end(), tmp, tmp+4);
						result.push_back(entry);
					}
				}
			}
		}
	}
}

//Take care of next loop
void QueryEvaluator::getNextStar(int up, vector<int>& result, int para){
	getNextPure(up, result, para);
	if(result[0] != -1){
		for(int i = 0; i < (int)result.size(); i++){
			int ele = result[i];
			vector<int> mid_result;
			getNextPure(up, mid_result, ele);
			for(int j = 0; j < (int)mid_result.size(); j++){
				int found = find_ele(result, mid_result[j]);
				if(found == result.size())
					result.push_back(mid_result[j]);
			}
		}
	}
}

void QueryEvaluator::getNextPure(int up, vector<int>& result, int para){
	if(up == DOWN){
		mPKBObject->getNext(result, para);
		if((int)result.size() == 3){
			int next1 = result[0];
			int next2 = result[2];
			result.clear();
			result.push_back(next1);
			result.push_back(next2);
		}
	}else if(up == UP){
		mPKBObject->getNextUp(result, para);
		if((int)result.size() == 3){
			int next1 = result[0];
			int next2 = result[2];
			result.clear();
			result.push_back(next1);
			result.push_back(next2);
		}
	}else throw new string("QueryEvaluator::getNextStar, no such up type");
}

//The para passed inside is checked to be assign
void QueryEvaluator::evalAffects(vector<vector<int> >& result, const vector<int>& para1, const vector<int>& para2){
	for(int i = 0; i < (int)para1.size(); i++){
		int in1 = para1[i];
		for(int p = 0; p < (int)para2.size(); p++){
			int in2 = para2[p];
			if(affects(in1, in2)){
				int tmp[4] = {mQueryTree->getIndex("integer"), in1, mQueryTree->getIndex("integer"), in2};
				vector<int> entry;
				entry.insert(entry.end(), tmp, tmp+4);
				result.push_back(entry);
			}
		}
	}
}

//The para passed inside is checked to be assign
void QueryEvaluator::evalAffectsStar(vector<vector<int> >& result, const vector<int>& para1, const vector<int>& para2){
	if(para1.size() <= para2.size()){
		for(int i = 0; i < (int)para1.size(); i++){
			int in1 = para1[i];
			vector<int> affectsStar;
			getAffectsStar(DOWN, affectsStar, in1);
			for(int j = 0; j < (int)para2.size(); j++){
				int in2 = para2[j];
				int found = find_ele(affectsStar, in2);
				if(found == (int)affectsStar.size()){
					int t[4] = {mQueryTree->getIndex("integer"), in1, mQueryTree->getIndex("integer"), in2};
					vector<int> tmp;
					tmp.insert(tmp.end(), t, t+4);
					result.push_back(tmp);
				}
			}
		}
	}
}

//If no affectsStar exists, return original result vector.
void QueryEvaluator::getAffectsStar(int up, vector<int>& result, int para){
	getAffects(up, result, para);
	for(int i = 0; i< (int)result.size(); i++){
		int next = result[i];
		getAffects(up, result, next);
	}
}

//If no affects exists, return the original result vector
void QueryEvaluator::getAffects(int up, vector<int>& result, int para){
	vector<int> tmp;
	mPKBObject->mTable_getModifiedVar(tmp, para);
	int modified = tmp[0];
	vector<int> used;
	mPKBObject->mTable_getStmtModifies(used, modified);
	if(up == DOWN){
		if(used[0] != -1){
			for(int i = 0 ; i< (int) used.size(); i++){
				int next = used[i];
				int found = find_ele(result, next);
				if(found == (int)result.size())
					if(affects(para, next))
						result.push_back(next);
			}
		}
	}else if(up == UP){
		if(used[0] != -1){
			for(int i = 0 ; i< (int) used.size(); i++){
				int next = used[i];
				int found = find_ele(result, next);
				if(found == (int)result.size())
					if(affects(next, para))
						result.push_back(next);
			}
		}
	}else throw new string("QueryEvaluator::getAffects, No such up type!");
}


bool QueryEvaluator::affects(int stmt1, int stmt2){
	//check the modifies var of stmt1 is in the used var of stmt2
	vector<int> tmp;
	mPKBObject->mTable_getModifiedVar(tmp, stmt1);
	int modified = tmp[0];
	vector<int> used;
	mPKBObject->uTable_getUsedVar(used, stmt2);
	int found1 = find_ele(used, modified);

	//Check next relation
	vector<int> nexts;
	getNextStar(DOWN, nexts, stmt1);
	int found2 = find_ele(nexts, stmt2);

	if(found1 != (int)used.size() && found2 != (int)nexts.size()){
		bool find_dest = false;
		bool has_non_mod_path = nonModPath(stmt1, modified, stmt2, stmt2, find_dest);
		if(has_non_mod_path) return true;
		else return false;
	}else return false;
}//affects END;

void QueryEvaluator::getPatternAssign(vector<int>& result, string patternLeft, string patternRight){
	vector<int> tmp;
	for(int i = 0; i < (int)result.size(); i++){
		if(mPKBObject->patternAssign(result[i], patternLeft, patternRight))
			tmp.push_back(result[i]);
	}
	result.clear();
	result.insert(result.end(), tmp.begin(), tmp.end());
}

void QueryEvaluator::getPatternCond(vector<int>& result, int type, string patternCond){
	vector<int> tmp;
	
	for(int i = 0; i < (int)result.size(); i++){
		if(type = mQueryTree->getIndex("if")){
			if(mPKBObject->condIf(result[i]) == PKB_varEncode(patternCond))
				tmp.push_back(result[i]);
		}else if(type = mQueryTree->getIndex("while")){
			if(mPKBObject->condWhile(result[i]) == PKB_varEncode(patternCond))
				tmp.push_back(result[i]);
		}else
			throw new string("QueryEvaluator::getPatternCond, no such type!");
	}
	result.clear();
	result.insert(result.end(), tmp.begin(), tmp.end());
}



/////////////////////////////////Augmenting Functions///////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void QueryEvaluator::getAllStmts(vector<int>& result){
	int max = mPKBObject->ast_getMaxStmtNum();
	for(int i = 1; i< max+1; i++){
		result.push_back(i);
	}
}

string QueryEvaluator::PQL_procDecode(int i){
	return mQueryTree->procDecode(i);
}
int QueryEvaluator::PQL_procEncode(string s){
	return mQueryTree->procCode(s);
}
int QueryEvaluator::PQL_varEncode(string s){
	return mQueryTree->varCode(s);
}
string QueryEvaluator::PQL_varDecode(int i){
	return mQueryTree->varDecode(i);
}

int QueryEvaluator::PKB_procEncode(string s){
	return mPKBObject->pTable_GetProcIndex(s);
}
string QueryEvaluator::PKB_procDecode(int i){
	//cout << mPKBObject->pTable_GetProcName(i) << endl;
	return mPKBObject->pTable_GetProcName(i);
}
int QueryEvaluator::PKB_varEncode(string s){
	return mPKBObject->vTable_GetVarIndex(s);
}
string QueryEvaluator::PKB_varDecode(int i){
	return mPKBObject->vTable_GetVarName(i);
}

int QueryEvaluator::PQL_getIndex(string s){
	return mQueryTree->getIndex(s);
}

int QueryEvaluator::find_ele(const vector<int>& in, const int ele){
	for(int i = 0; i< (int)in.size(); i++){
		if(in[i] == ele)
			return i;
	}
	return (int)in.size();
}


bool QueryEvaluator::isIf(int stmt){
	vector<int> ifs;
	mPKBObject->ast_GetAllIf(ifs);
	int found = find_ele(ifs, stmt);
	if(found == (int)ifs.size())
		return false;
	else return true;
}

bool QueryEvaluator::isWhile(int stmt){
	vector<int> ws;
	mPKBObject->ast_GetAllWhile(ws);
	int found = find_ele(ws, stmt);
	if(found == (int)ws.size())
		return false;
	else return true;
}

bool QueryEvaluator::isInsideWhile(int w, int stmt){
	vector<int> descadent;
	getChildStar(DOWN, descadent, w);
	int found = find_ele(descadent, stmt);
	if(found == (int)descadent.size())
		return false;
	else return true;
}

bool QueryEvaluator::isInsideIf(int ifstat, int stmt){
	vector<int> descadent;
	getChildStar(DOWN, descadent, ifstat);
	int found = find_ele(descadent, stmt);
	if(found == (int)descadent.size())
		return false;
	else return true;

}

//Check whether there is a non-mod path for variable mod in the stmt
bool QueryEvaluator::nonModPath(int s, int mod, int dest, int final, bool& find_dest){
	find_dest = false;

	int next = s;
	if(s <= 0)
		return true;
	
	if(next == final) {
		find_dest = true;
		return true;
	}
	if(s == dest) return true;

	bool is_if = isIf(next);
	bool is_while = isWhile(next);
	if(is_if){
		vector<int> tmp_nexts;
		getNextPure(DOWN, tmp_nexts, next);
		int thenC = tmp_nexts[0];
		int elseC = tmp_nexts[1];
		
		bool is_in_if = isInsideIf(next, final);
		int joint_node = mPKBObject->ast_GetFollowingStatementNum(s);
		bool find_dest1;
		bool find_dest2;
		bool non_mod_path1;
		bool non_mod_path2;

		if((!is_in_if) || is_in_if && joint_node != -1){
			non_mod_path1 = nonModPath(thenC, mod, joint_node, final, find_dest1);
			non_mod_path2 = nonModPath(elseC, mod, joint_node, final, find_dest2);
		}else{
			non_mod_path1 = nonModPath(thenC, mod, final, final, find_dest1);
			non_mod_path2 = nonModPath(elseC, mod, final, final, find_dest2);
			if(find_dest1){
				find_dest = true;
				return non_mod_path1;
			}
			if(find_dest2){
				find_dest = true;
				return non_mod_path2;
			}
			throw new string("QueryEvaluator::nonModPath, either find_dest1 or find_dest2 is true, inconsistency!");
		}

		if(find_dest1 && !find_dest2) {
			find_dest = true;
			return non_mod_path1;
		}
		if(find_dest2){
			find_dest = true;
			return non_mod_path2;
		}
		if(!non_mod_path1 && !non_mod_path2) return false;
		else return nonModPath(joint_node, mod, dest, final, find_dest);
	}else if(is_while){
		bool is_in_while = isInsideWhile(next, dest);
		vector<int> tmp_nexts;
		getNextPure(DOWN, tmp_nexts, next);
		int size = tmp_nexts.size();
		int inner;
		int outer;
		if(size == 2){
			outer = tmp_nexts[0];
			inner = tmp_nexts[1];
		}else inner = tmp_nexts[0];
	
		if(is_in_while)
			return nonModPath(inner, mod, dest, final, find_dest);
		else if(size == 2) return nonModPath(outer, mod, dest, final, find_dest);
		else return true;
	}else{
		vector<int> modifies;
		vector<int> tmp_nexts;
		getNextPure(DOWN, tmp_nexts, next);
		next = tmp_nexts[0];
		if(next == final){
			find_dest = true;
			return true;
		}

		mPKBObject->mTable_getModifiedVar(modifies, next); 
		int found = find_ele(modifies, mod);
		if(found != (int)modifies.size())
			return false;
		else return nonModPath(next, mod, dest, final, find_dest);
	}
	return true;
}

void QueryEvaluator::getAllType(vector<int>& result, int type){
	if(type == mQueryTree->getIndex("stmt"))  getAllStmts(result);
	else if(type == mQueryTree->getIndex("prog_line")) getAllStmts(result);
	else if(type == mQueryTree->getIndex("assign")) mPKBObject->ast_GetAllAssign(result);
	else if(type == mQueryTree->getIndex("while")) mPKBObject->ast_GetAllWhile(result);
	else if(type == mQueryTree->getIndex("if")) mPKBObject->ast_GetAllIf(result);
	else if(type == mQueryTree->getIndex("call")) mPKBObject->ast_GetAllCall(result);
	else if(type == mQueryTree->getIndex("variable")) mPKBObject->vTable_GetAllVar(result);
	else if(type == mQueryTree->getIndex("procedure")) mPKBObject->pTable_GetAllProc(result);
	else if(type == mQueryTree->getIndex("constant")) mPKBObject->cTable_GetAllConstants(result);
	else throw new string("QueryEvaluator::getAllType, type not exist!");
}