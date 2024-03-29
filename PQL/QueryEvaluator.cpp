#include "QueryEvaluator.h"
#include <string>

using namespace std;

int const UP = 1;
int const DOWN = 0;

int const STAR = 1;
int const NOSTAR = 0;

int const MOD = 1;
int const USE = 0;

//Controller passes the PKB and Query Parser to evaluator, and trigger evaluator to start.
QueryEvaluator::QueryEvaluator(PKB *p, PqlPreprocessor *q)
{
	mPKBObject = p;
	mQueryTree = q;
	varCodeEnding = ENCODE_ENDING+1;
	isBoolSelected = false;
	initialAffectsTable();
}

void QueryEvaluator::evaluate(){
	vector<int> select;
	mQueryTree->selectAt(select, 0);  //Elements to be selected
	
	if(select[0] == mQueryTree->getIndex("BOOLEAN")) isBoolSelected = true;

	//QE evaluate the With clause, then pattern clause, and in the end Such That clause.
	bool unrelated_finish = false;
	int with_size = mQueryTree->withUnrelatedSize();
	int pattern_size = mQueryTree->patternUnrelatedSize();
	int suchthat_size = mQueryTree->suchThatUnrelatedSize();
	int with_point = 0;
	int pattern_point = 0;
	int suchthat_point = 0;

	while(!unrelated_finish){
		initialize();

		evaluateWith(unrelated_finish, with_point, with_size);
		if(mResult.isEmptyResult() || mResult.isBoolSet())
			return;
		//AutoTester Collaborative
		//if(AbstractWrapper::GlobalStop){
		//	throw new string("GlobalStop, time out!");
		//}
	
		evaluatePattern(unrelated_finish, pattern_point, pattern_size);
		if(mResult.isEmptyResult() || mResult.isBoolSet())
			return;
		//AutoTester Collaborative
		//if(AbstractWrapper::GlobalStop){
		//	throw new string("GlobalStop, time out!");
		//}
	
	
		evaluateSuchThat(unrelated_finish, suchthat_point, suchthat_size);
		if(mResult.isEmptyResult() || mResult.isBoolSet())
			return;
		//AutoTester Collaborative
		//if(AbstractWrapper::GlobalStop){
		//	throw new string("GlobalStop, time out!");
		//}
	}

	if(isBoolSelected){
		mResult.setBoolValue(true);
		return;
	}


	initialize();

	with_size = mQueryTree->withSize();
	pattern_size = mQueryTree->patternSize();
	suchthat_size = mQueryTree->suchThatSize();

	int last_point = 0;
	evaluateWith(unrelated_finish, last_point, with_size);
	if(mResult.isEmptyResult() || mResult.isBoolSet())
		return;
	//AutoTester Collaborative
	//if(AbstractWrapper::GlobalStop){
	//	throw new string("GlobalStop, time out!");
	//}
	
	last_point = 0;
	evaluatePattern(unrelated_finish, last_point, pattern_size);
	if(mResult.isEmptyResult() || mResult.isBoolSet())
		return;
	//AutoTester Collaborative
	//if(AbstractWrapper::GlobalStop){
	//	throw new string("GlobalStop, time out!");
	//}
	
	last_point = 0;
	evaluateSuchThat(unrelated_finish, last_point, suchthat_size);
	if(mResult.isEmptyResult() || mResult.isBoolSet())
		return;
	//AutoTester Collaborative
	//if(AbstractWrapper::GlobalStop){
	//	throw new string("GlobalStop, time out!");
	//}

	
	generateResult();
	if(mResult.isEmptyResult() || mResult.isBoolSet())
		return;

	//Sort and condense the result
	mResult.resultSort();

}//End of evaluate

void QueryEvaluator::evaluateWith(bool& unrelated_finish, int& last_point, int threshold){
	//Start evaluating With clauses 
	for(; last_point < threshold; last_point++){
		vector<int> clause;
		if(!unrelated_finish)
			mQueryTree->withUnrelatedAt(clause, last_point);
		else mQueryTree->withAt(clause, last_point);

		if(clause[0] == -1){
			last_point++;
			return;
		}else if(clause[0] == -2){
			last_point++;
			return;
		}

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

		bool isCallWithProc1 = false;
		int p1_type = part1.at(0);
		int p1_name = part1.at(1);
		bool isCallWithProc2 = false;
		int p2_type ;
		int p2_name ;
		if(p1_type == mQueryTree->getIndex("call"))
			if(part1[3] == mQueryTree->getIndex("procName"))
				isCallWithProc1 = true;
		if(part2_size == 2) { 
			p2_type = part2.at(0);
			p2_name = varCodeEnding++;
		}else if((int)part1.size() == part2_size){ //For the case when a.stmt# = b.stmt#
			p2_type = part2.at(0);
			p2_name = part2.at(1);
			if(p2_type == mQueryTree->getIndex("call"))
				if(part2[3] == mQueryTree->getIndex("procName"))
					isCallWithProc2 = true;


			if((part1[3] == mQueryTree->getIndex("varname") && part2[3] == mQueryTree->getIndex("procname")) ||
			(part1[3] == mQueryTree->getIndex("procname") && part2[3] == mQueryTree->getIndex("varname"))){
				if(isBoolSelected)
					mResult.setBoolValue(false);
				else mResult.addInType(-1);
				return;
			}
		}	
		vector<vector<int> > with_result;
		vector<int> tmp;
		checkCandidates(p1_name, p1_type, tmp);
		
		vector<int> tmp2;
		if(p2_type == mQueryTree->getIndex("integer")) tmp2.push_back(part2.at(1));
		else if(p2_type == mQueryTree->getIndex("varOfSimpl")){
			int tmp_code = PKB_varEncode(PQL_varDecode(part2.at(1)));
			if(tmp_code == -1) {
				if(isBoolSelected)
					mResult.setBoolValue(false);
				else mResult.addInType(-1);
				return;
			}
			tmp2.push_back(tmp_code);
		}else if(p2_type == mQueryTree->getIndex("procOfSimpl")){
			int tmp_code = PKB_procEncode(PQL_procDecode(part2.at(1)));
			if(tmp_code == -1){
				if(isBoolSelected)
					mResult.setBoolValue(false);
				else mResult.addInType(-1);
				return;
			}
			tmp2.push_back(tmp_code);
		}else checkCandidates(p2_name, p2_type, tmp2);
		
		int entry_type;
		if(p1_type == mQueryTree->getIndex("stmt")|| p1_type==mQueryTree->getIndex("assign")|| p1_type==mQueryTree->getIndex("while")|| p1_type==mQueryTree->getIndex("if")||p1_type==mQueryTree->getIndex("call") ||p1_type==mQueryTree->getIndex("constant") || p1_type==mQueryTree->getIndex("prog_line"))
			entry_type = mQueryTree->getIndex("integer");
		else if(p1_type==mQueryTree->getIndex("variable"))
			entry_type = mQueryTree->getIndex("varOfSimpl");
		else if(p1_type==mQueryTree->getIndex("procedure"))
			entry_type = mQueryTree->getIndex("procOfSimpl");
		else throw new string("with clause error type");

		for(vector<int>::iterator count_t=tmp.begin(); count_t< tmp.end(); count_t++)
			for(vector<int>::iterator k = tmp2.begin(); k<tmp2.end(); k++){
				int check1;
				int check2;
				if(isCallWithProc1)
					check1 = mPKBObject->ast_getCallProcIndex(*count_t);
				else check1 = *count_t;
				if(isCallWithProc2)
					check2 = mPKBObject->ast_getCallProcIndex(*k);
				else check2 = *k;
				if(check1 == check2){
					vector<int> entry;
					int entry_array [] = {entry_type, *count_t, entry_type, *k};
					entry.insert(entry.end(), entry_array, entry_array+4);
					with_result.push_back(entry);
				}
			}

		//Merging different evaluation tuples
		vector<int>::iterator it;
		it = find(mgTupleIndexing.begin(), mgTupleIndexing.end(), p1_name);
		int same1Tuple1 = -1;
		int same2Tuple1 = -1;
		int numOfCommonElement = 0;
		if(it == mgTupleIndexing.end()){
			mgTupleIndexing.push_back(p1_name);
		}else{
			for(vector<int>::iterator count_t = mgTupleIndexing.begin(); count_t<=it; count_t++){
				same1Tuple1++;
			}
			numOfCommonElement = 2;
		}
		it = find(mgTupleIndexing.begin(), mgTupleIndexing.end(), p2_name);
		if(it == mgTupleIndexing.end())
			if(p2_type == mQueryTree->getIndex("integer") || p2_type == mQueryTree->getIndex("varOfSimpl") || p2_type == mQueryTree->getIndex("varOfSimpl"))
				mgTupleIndexing.push_back(varCodeEnding++);
			else mgTupleIndexing.push_back(p2_name);
		else{
			for(vector<int>::iterator i = mgTupleIndexing.begin(); i<=it; i++){
				same2Tuple1++;
			}
			numOfCommonElement = numOfCommonElement+2;
		}
		
		if(with_result.empty()){
			if(isBoolSelected)
				mResult.setBoolValue(false);
			else mResult.addInType(-1);
			return;
		}

		joinTuples(evalTuple, with_result, numOfCommonElement, same1Tuple1, same2Tuple1, last_point);

		if(evalTuple.empty()){
			if(isBoolSelected)
				mResult.setBoolValue(false);
			else mResult.addInType(-1);
			return;
		}
	}//while: With clause evaluation End
}

void QueryEvaluator::evaluatePattern(bool& unrelated_finish, int& last_point, int threshold){
	//Pattern_PQL Evaluation Start
	for(; last_point < threshold; last_point++){
		vector<int> clause;
		if(!unrelated_finish)
			mQueryTree->patternUnrelatedAt(clause, last_point);
		else mQueryTree->patternAt(clause, last_point);

		if(clause[0] == -1){
			last_point++;
			return;
		}else if(clause[0] == -2){
			last_point++;
			return;
		}

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
			pattern1_type = clause[next++];
			pattern1 = clause[next++];
		}
		if(clause[next++] != mQueryTree->getIndex(",")) {
			throw new string("QueryEvaluator::evaluate, pattern format error, no \",\"!");
		}
		
		if(clause[next] == mQueryTree->getIndex("_")){
			pattern2_type = clause[next];
			pattern2 = clause[next++];
		}else{
			pattern2_type = clause[next++];
			pattern2 = clause[next++];
		}

		if(var_type == mQueryTree->getIndex("if"))
			if(clause[++next] != mQueryTree->getIndex("_")){
				throw new string("QueryEvaluator::evaluate, pattern format error, if third parameter!");
			}
		//pattern clause read-in finish//
      
		vector<vector<int> > pattern_result;
		evalPattern_PQL(pattern_result, var, var_type, pattern1_type, pattern1, pattern2);
		
		if(pattern_result.empty()){
			if(isBoolSelected)
				mResult.setBoolValue(false);
			else mResult.addInType(-1);
			return;
		}

		int it;
		int same1Tuple1 = -1;
		int same2Tuple1 = -1;
		int numOfCommonElement = 0;
		if(!mgTupleIndexing.empty()){
			it = find_ele(mgTupleIndexing, var);
			if(it == (int)mgTupleIndexing.size())
				mgTupleIndexing.push_back(var);
			else
			{
				same1Tuple1 = it;
				numOfCommonElement = 2;
			}
			it = find_ele(mgTupleIndexing, pattern1);
			if(it == (int)mgTupleIndexing.size()){
				if(pattern1_type != mQueryTree->getIndex("variable")){
					pattern1 = varCodeEnding;
					mgTupleIndexing.push_back(varCodeEnding++);
				}else mgTupleIndexing.push_back(pattern1);
			}else{
				for(int i = 0; i<=it; i++){
					same2Tuple1++;
				}
				numOfCommonElement = numOfCommonElement+2;
			}
		}else{
			numOfCommonElement = 0;
			mgTupleIndexing.push_back(var);
			if(pattern1_type != mQueryTree->getIndex("variable")){
				pattern1 = varCodeEnding;
				mgTupleIndexing.push_back(varCodeEnding++);
			}else mgTupleIndexing.push_back(pattern1);
		}
		
		joinTuples(evalTuple, pattern_result, numOfCommonElement, same1Tuple1, same2Tuple1, last_point);

		if(evalTuple.empty()){
			if(isBoolSelected)
				mResult.setBoolValue(false);
			else mResult.addInType(-1);
		}	
	}//Pattern_PQL Evaluation Finish
}

void QueryEvaluator::evaluateSuchThat(bool& unrelated_finish, int& last_point, int threshold){
	//Start evaluating SuchThat clauses
	if(last_point == threshold) unrelated_finish = true;
	for(; last_point < threshold; last_point++)
	{
		vector<int> clause;
		if(!unrelated_finish)
			mQueryTree->suchThatUnrelatedAt(clause, last_point);
		else mQueryTree->suchThatAt(clause, last_point);

		if(last_point == threshold - 1) unrelated_finish = true;
		if(clause[0] == -1){
			last_point++;
			return;
		}else if(clause[0] == -2){
			last_point++;
			return;
		}

		int rel = clause[0];
		int para1_type;
		int para1;
		int para2_type;
		int para2;

		
		underScore(rel, clause, para1, para1_type, para2, para2_type, varCodeEnding);


		//Convert all "a", "b", "c" 's code from PQL code to PKB code
		if(para1_type == mQueryTree->getIndex("varOfSimpl")) para1 = PKB_varEncode(PQL_varDecode(para1));
		else if(para1_type == mQueryTree->getIndex("procOfSimpl")) para1 = PKB_procEncode(PQL_procDecode(para1));

		if(para2_type == mQueryTree->getIndex("varOfSimpl")) para2 = PKB_varEncode(PQL_varDecode(para2));
		else if(para2_type == mQueryTree->getIndex("procOfSimpl")) para2 = PKB_procEncode(PQL_procDecode(para2));
	
		//When the parameter of relation is not in simple, the query just evaluate to null
		if(para1 == -1 || para2 == -1){
			if(isBoolSelected)
				mResult.setBoolValue(false);
			else mResult.addInType(-1);
			return;
		}

		//Check the input argument candidates
		vector<vector<int> > relResult;
		vector<int> para1_collection;
		vector<int> para2_collection;

		checkCandidates(para1, para1_type, para1_collection);
		checkCandidates(para2, para2_type, para2_collection);
		

		if(rel == mQueryTree->getIndex("parent")){	
			evalParent(NOSTAR, relResult, para1_collection, para2_collection);
		}else if(rel==mQueryTree->getIndex("parent*")){
			evalParent(STAR, relResult, para1_collection, para2_collection);
		}else if(rel==mQueryTree->getIndex("follows")){
			evalFollows(NOSTAR, relResult, para1_collection, para2_collection);
		}else if(rel==mQueryTree->getIndex("follows*")){
			evalFollows(STAR, relResult, para1_collection, para2_collection);
		}else if(rel==mQueryTree->getIndex("uses")){
			if(para1_type == mQueryTree->getIndex("procedure") || para1_type == mQueryTree->getIndex("procOfSimpl"))
				evalMU(USE, relResult, mQueryTree->getIndex("procOfSimpl"), para1_collection, para2_collection);
			else 
				evalMU(USE, relResult, mQueryTree->getIndex("integer"), para1_collection, para2_collection);
		}else if(rel==mQueryTree->getIndex("modifies")){
			if(para1_type == mQueryTree->getIndex("procedure") || para1_type == mQueryTree->getIndex("procOfSimpl"))
				evalMU(MOD, relResult, mQueryTree->getIndex("procOfSimpl"), para1_collection, para2_collection);
			else 
				evalMU(MOD, relResult, mQueryTree->getIndex("integer"), para1_collection, para2_collection);
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
			for(int i = 0; i< (int)para1_collection.size();){
				if(!mPKBObject->ast_IsAssign(para1_collection[i])){
					para1_collection.erase(para1_collection.begin() + i);
				}else i++;
			}
			for(int i = 0; i< (int)para2_collection.size();){
				if(!mPKBObject->ast_IsAssign(para2_collection[i])){
					para2_collection.erase(para2_collection.begin() + i);
				}else i++;
			}
			//evaluate affects
			evalAffects(relResult, para1_collection, para2_collection);
		}else if(rel==mQueryTree->getIndex("affects*")){
			//Check whether the parameters are assign
			for(int i = 0; i< (int)para1_collection.size();){
				if(!mPKBObject->ast_IsAssign(para1_collection[i])){
					para1_collection.erase(para1_collection.begin() + i);
				}else i++;
			}
			for(int i = 0; i< (int)para2_collection.size();){
				if(!mPKBObject->ast_IsAssign(para2_collection[i])){
					para2_collection.erase(para2_collection.begin() + i);
				}else i++;
			}
			//evaluate affects star
			evalAffectsStar(relResult, para1_collection, para2_collection);
		}else throw new string("Relation not exists");
		
		if(relResult.empty()){
			if(isBoolSelected)
				mResult.setBoolValue(false);
			else mResult.addInType(-1);
			return;
		}

		//Manipulation of indexing issue, adjust the mgTupleIndexing and find common index to pass
		int it;
		int same1Tuple1 = -1;
		int same2Tuple1 = -1;
		int numOfCommonElement = 0;
		if(!mgTupleIndexing.empty()){
			it = find_ele(mgTupleIndexing, para1);
			if(it == (int)mgTupleIndexing.size())
				if(para1_type == mQueryTree->getIndex("integer") || para1_type == mQueryTree->getIndex("varOfSimpl") || para1_type == mQueryTree->getIndex("procOfSimpl"))
					mgTupleIndexing.push_back(varCodeEnding++);
				else mgTupleIndexing.push_back(para1);
			else{
				same1Tuple1 = it;
				numOfCommonElement = 2;
			}
			it = find_ele(mgTupleIndexing, para2);
			if(it == (int)mgTupleIndexing.size())
				if(para2_type == mQueryTree->getIndex("integer") || para2_type == mQueryTree->getIndex("varOfSimpl") || para2_type == mQueryTree->getIndex("procOfSimpl")){
					para2 = varCodeEnding;
					mgTupleIndexing.push_back(varCodeEnding++);
				}else mgTupleIndexing.push_back(para2);
			else{
				same2Tuple1 = it;
				numOfCommonElement = numOfCommonElement+2;
			}


			if(para1 == para2){
				removeInequal(relResult);
				if(numOfCommonElement == 4)
					numOfCommonElement = 2;
				else{
					if(mgTupleIndexing.empty())
						mgTupleIndexing.push_back(para1);
					else
						mgTupleIndexing.pop_back();
				}
			}
		}else{
			numOfCommonElement = 0;
			if(para1 == para2){
				 removeInequal(relResult);
				mgTupleIndexing.push_back(para1);
			}else{
				mgTupleIndexing.push_back(para1);
				mgTupleIndexing.push_back(para2);
			}
		}
		
		
		if(relResult.empty()){
			if(isBoolSelected)
				mResult.setBoolValue(false);
			else mResult.addInType(-1);
		}

		joinTuples(evalTuple, relResult, numOfCommonElement, same1Tuple1, same2Tuple1, last_point);

		if(evalTuple.empty()){
			if(isBoolSelected)
				mResult.setBoolValue(false);
			else mResult.addInType(-1);
			return;
		}

	}//while: such that evaluation END
}


void QueryEvaluator::initialize(){
	mgTupleIndexing.clear();
	evalTuple.clear();
}

void QueryEvaluator::removeInequal(vector<vector<int> >& tuple){
	for(int i = 0; i < (int)tuple.size(); ){
		int value1_type = tuple[i][0];
		int value1 = tuple[i][1];
		int value2_type = tuple[i][2];
		int value2 = tuple[i][3];
		if(value1 != value2)
			tuple.erase(tuple.begin() + i);
		else{
			i++;
		}
	}
}

void QueryEvaluator::checkCandidates(int para, int para_type, vector<int>& candidates){
	if(para_type == mQueryTree->getIndex("integer")) {candidates.push_back(para); return;}
	else if(para_type == mQueryTree->getIndex("procOfSimpl")) {candidates.push_back(para); return;}
	else if(para_type == mQueryTree->getIndex("varOfSimpl")) {candidates.push_back(para); return;}

	for(int i = 0; i < (int)mgTupleIndexing.size(); i++){
		if(para == mgTupleIndexing[i]){
			for(int j = 0; j < (int)evalTuple.size(); j++){
				candidates.push_back(evalTuple[j][2*i+1]);
			}
			removeDuplicates(candidates);
			return;
		}
	}
	getAllType(candidates, para_type);
}

void QueryEvaluator::removeDuplicates(vector<int>& v){
	if(v.empty() || (int)v.size() == 1) return;

	sort(v.begin(), v.end());

	//Eliminate duplicates
	vector<int> tmp_store;
	vector<int> equilities; //keeps equal tuples' indexes
	equilities.push_back(0);
	for(int i = 1; i < (int)v.size(); i++){
		if(v[i-1] == v[i]){
			equilities.push_back(i);
			if(i == v.size() -1) 
				tmp_store.push_back(v[i]);
		}else{
			tmp_store.push_back(v[equilities[0]]);
			equilities.clear();
			equilities.push_back(i);
			if(i == v.size() -1) 
				tmp_store.push_back(v[i]);
		}
	}
	v = tmp_store;
}


void QueryEvaluator::transform(vector<vector<int> >& pre_tuple, vector<vector<int> >& candidates, vector<int>& select_index, vector<int> non_evaled_selects){
	int size = candidates.size();
	vector<vector<int> > tmp_result;
	if(candidates.empty()) return;
	if(pre_tuple.empty()){
		for(int k = 0; k < (int)candidates.size(); k++){
			if(tmp_result.empty()){
				for(int i = 0; i < (int)candidates[0].size(); i++){
					vector<int> tmp;
					tmp.push_back(candidates[0][i]);
					tmp_result.push_back(tmp);
				}
			}else{
				vector<vector<int> > medium_value;
				for(int j = 0; j < (int)tmp_result.size(); j++){
					for(int i = 0; i < (int)candidates[k].size(); i++){
						vector<int> join_entry = tmp_result[j];
						join_entry.push_back(candidates[k][i]);
						medium_value.push_back(join_entry);
					}	
				}
				tmp_result = medium_value;
			}
		}
		pre_tuple = tmp_result;
	}else{	
		for(int j = 0; j < (int)candidates.size(); j++){
			int insert_place = non_evaled_selects[j];
			for(int i = 0; i< (int)pre_tuple.size(); i++){
				for(int k = 0; k< (int)candidates[j].size(); k++){
					vector<int> join_entry;
					join_entry = pre_tuple[i];
					int insert_element = candidates[j][k];
					join_entry.insert(join_entry.begin()+insert_place, insert_element);
					tmp_result.push_back(join_entry);
				}
			}
			pre_tuple = tmp_result;
			tmp_result.clear();
		}
	}//tuple joining done


	vector<int> select_remain;
	for(int i = 0; i < (int)select_index.size(); i++){
		select_remain.push_back(i);
	}
	while(!select_index.empty()){
		vector<int> equalities;
		int equals = select_index[0];
		equalities.push_back(select_remain[0]);
		select_remain.erase(select_remain.begin());
		select_index.erase(select_index.begin());
		for(int i = 0; i < (int)select_index.size();){
			if(equals == select_index[i]){
				equalities.push_back(select_remain[i]);
				select_index.erase(select_index.begin()+i);
				select_remain.erase(select_remain.begin()+i);
			}else i++;
		}

		if((int)equalities.size() > 1){
			for(int j = 0; j < (int)pre_tuple.size(); j++){
				int value = pre_tuple[j][equalities[0]];
				for(int k = 1; k < (int)equalities.size(); k++){
					if(value != pre_tuple[j][equalities[k]]){
						pre_tuple.erase(pre_tuple.begin() + j);
						j--;
						break;
					}
				}
			}
		}
	}
}

void QueryEvaluator::generateResult(){
	vector<vector<int> > final_result;

	int with_size = mQueryTree->withSize();
	int pattern_size = mQueryTree->patternSize();
	int suchthat_size = mQueryTree->suchThatSize();

	if(isBoolSelected){  //If the select is boolean
		if(with_size == 0 && suchthat_size == 0 && pattern_size == 0){
			mResult.setBoolValue(true);
			return;
		}

		mResult.setBoolValue(true);
		return;
	}

	vector<vector<int> > selection_candidates;
	vector<vector<int> > suchthat_result;
	vector<int> non_evaled_selects;
	vector<int> select_index;
	for(int i = 0; i < mQueryTree->selectSize(); i++){
			vector<int> tmp_selected;
			mQueryTree->selectAt(tmp_selected, i);
			vector<int> tmp;
			int select_type = tmp_selected[0];
			int selected = tmp_selected[1];
			select_index.push_back(selected);
			if(select_type == mQueryTree->getIndex("call") && (int)tmp_selected.size() == 4)
				if(tmp_selected[3] == mQueryTree->getIndex("procname")){
					mResult.addCallWithProcIndex(i);
				}


			bool selected_in_st = false;

			for(int index = 0; index < (int)mgTupleIndexing.size(); index++){
				if(selected == mgTupleIndexing[index]){
					for(int eva_ind = 0; eva_ind < (int)evalTuple.size(); eva_ind++){
						tmp.push_back(evalTuple[eva_ind][index*2+1]);
					}
					selected_in_st = true;
					suchthat_result.push_back(tmp);
					break;
				}
			}

			if(!selected_in_st){
				getAllType(tmp, select_type);
				selection_candidates.push_back(tmp);
				non_evaled_selects.push_back(i);
			}
		
			if(select_type == mQueryTree->getIndex("procedure"))
				mResult.addInType(mQueryTree->getIndex("procOfSimpl"));
			else if(select_type == mQueryTree->getIndex("variable"))
				mResult.addInType(mQueryTree->getIndex("varOfSimpl"));
			else mResult.addInType(mQueryTree->getIndex("integer")); 
	}

	//Join all the variable candidates
	if(!suchthat_result.empty()){
		int size = suchthat_result[0].size();
		for(int ind = 0; ind < size; ind++){
			vector<int> tmp_result;
			for(int ind2 = 0; ind2 < (int)suchthat_result.size(); ind2++){
				tmp_result.push_back(suchthat_result[ind2][ind]);
			}
			final_result.push_back(tmp_result);
		}
	}

	//both tuple should have their type removed
	transform(final_result, selection_candidates, select_index, non_evaled_selects);

	for(int i = 0; i < (int)final_result.size(); i++){
		vector<int> new_tuple_tmp;
		int size = final_result[i].size();
		for(int k = 0; k < size; k++){
			new_tuple_tmp.push_back(final_result[i][k]);
		}
		mResult.addInTuple(new_tuple_tmp);
	}
}


void QueryEvaluator::initialAffectsTable(){
	int affects_num = mQueryTree->affectsSize();
	int affects_star_num = mQueryTree->affectsStarSize();
	if(affects_num >= 3 || affects_star_num > 0)
		affectsTable.is_affects_table_built = true;
	else affectsTable.is_affects_table_built = false;
	if(affectsTable.is_affects_table_built){
		vector<int> assigns;
		mPKBObject->ast_GetAllAssign(assigns);
		for(int i =0 ; i < (int)assigns.size(); i++){
			int stmt = assigns[i];
			if(i == 0) affectsTable.initial_stmt = stmt;
			vector<int> affects;
			vector<int> mods;
			mPKBObject->mTable_getModifiedVar(mods, stmt);
			if(mods.empty() || mods[0] == -1) 
				throw new string("QueryEvaluator::buildAffectsTable, shit! This should never happen!");
			int mod = mods[0];

			nonModPath(stmt, mod, -1, stmt, affects, true);

			affectsTable.affects_index_down.insert(pair<int, int>(stmt, i));
			affectsTable.table_body_down.push_back(affects);
			for(int j = 0;  j < (int)affects.size(); j++){
				pair<map<int,int>::iterator, bool> tmp = affectsTable.affects_index_up.insert(pair<int, int>(affects[j], (int)affectsTable.table_body_up.size()));
				if(tmp.second){
					vector<int> affects_up;
					affects_up.push_back(stmt);
					affectsTable.table_body_up.push_back(affects_up);
				}else{
					int index = affectsTable.affects_index_up[affects[j]];
					affectsTable.table_body_up[index].push_back(stmt);
				}
			}
		}
	}
}

void QueryEvaluator::underScore(int rel, vector<int> clause, int& para1, int& para1_type, int& para2, int& para2_type, int& varCodeEnding){
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
			para1 = varCodeEnding++;
		}if(para2 == mQueryTree->getIndex("_")){
			para2_type = mQueryTree->getIndex("stmt");
			para2 = varCodeEnding++;
		}
	}
	else if(rel == mQueryTree->getIndex("uses") || rel == mQueryTree->getIndex("modifies")) // uses and modifies
	{		
		if(para2 == mQueryTree->getIndex("_")){ //replace with (variable newest)
			para2_type = mQueryTree->getIndex("variable");
			para2 = varCodeEnding++;
		}
	}
	else if(rel == mQueryTree->getIndex("calls") || rel == mQueryTree->getIndex("calls*")) //relation is calls, calls*
	{
		if(para1 == mQueryTree->getIndex("_")){ //replace with (proc newest)
			para1_type = mQueryTree->getIndex("procedure");
			para1 = varCodeEnding++;
		}if(para2 == mQueryTree->getIndex("_")){
			para2 = varCodeEnding++;
			para2_type = mQueryTree->getIndex("procedure");
		}
	}else throw new string("QueryEvaluator::underScore, no such relation type!");  //do nothing 
}

void QueryEvaluator::evalPattern_PQL(vector<vector<int> >& result, int var, int var_type, int pattern1_type, int pattern1, int pattern2){
	int indx = find_ele(mgTupleIndexing, var);
	int found = 0;
	vector<int> var_candidates;
	if(indx != (int)mgTupleIndexing.size()){
		found = 1;
		for(int i = 0; i < (int)evalTuple.size(); i++){
			var_candidates.push_back(evalTuple[i][2*indx+1]);
		}
	}

	vector<int> pattern1_candidates;
	if(pattern1_type == mQueryTree->getIndex("variable")){
		checkCandidates(pattern1, pattern1_type, pattern1_candidates);
	}else if(pattern1_type != mQueryTree->getIndex("_")){
		string s = PQL_varDecode(pattern1);
		pattern1_candidates.push_back(PKB_varEncode(s));
	}

	if(var_type == mQueryTree->getIndex("assign")){
		if(found == 0){
			mPKBObject->ast_GetAllAssign(var_candidates);
		}
		if(pattern1_type == mQueryTree->getIndex("_")){
			for(int i = 0; i < (int)var_candidates.size(); i++){
				bool isAss = mPKBObject->patternAssign(var_candidates[i], "_", mQueryTree->getContent(pattern2));
				if(isAss){
					vector<int> tmp;
					tmp.push_back(mQueryTree->getIndex("integer"));
					tmp.push_back(var_candidates[i]);
					tmp.push_back(mQueryTree->getIndex("varofsimpl"));
					tmp.push_back(pattern1_type);
					result.push_back(tmp);
				}
			}
		}else{ 
			vector<vector<int> > tmp_result;
			for(int i = 0; i < (int)pattern1_candidates.size(); i++){
				for(int k = 0; k < (int)var_candidates.size(); k++){
					int stmt = var_candidates[k];
					string pattern1_s = PKB_varDecode(pattern1_candidates[i]);
					string pattern2_s = mQueryTree->getContent(pattern2);
					bool isAss = mPKBObject->patternAssign(stmt, pattern1_s, pattern2_s);
					if(isAss){
						vector<int> tmp;
						tmp.push_back(mQueryTree->getIndex("integer"));
						tmp.push_back(stmt);
						tmp.push_back(mQueryTree->getIndex("varofsimpl"));
						tmp.push_back(pattern1_candidates[i]);
						tmp_result.push_back(tmp);
					}
				}
			}
			result = tmp_result;
		}
	}else if(var_type == mQueryTree->getIndex("if") || var_type == mQueryTree->getIndex("while")){
		if(found == 0){
			if(var_type == mQueryTree->getIndex("if"))
				mPKBObject->ast_GetAllIf(var_candidates);
			else if(var_type == mQueryTree->getIndex("while"))
				mPKBObject->ast_GetAllWhile(var_candidates);
		}
		if(pattern1_type == mQueryTree->getIndex("_")){
			for(int i = 0; i < (int)var_candidates.size(); i++){
				vector<int> tmp;
				tmp.push_back(mQueryTree->getIndex("integer"));
				tmp.push_back(var_candidates[i]);
				tmp.push_back(mQueryTree->getIndex("varofsimpl"));
				tmp.push_back(pattern1_type);
				result.push_back(tmp);
			}
		}else{
			vector<int> current_result;
			for(int i = 0; i < (int)pattern1_candidates.size(); i++){
				for(int k = 0; k < (int)var_candidates.size(); k++){
					int pattern1_code = pattern1_candidates[i];
					int stmt = var_candidates[k];
					int stmt_left;
					if(var_type == mQueryTree->getIndex("if"))
						stmt_left = mPKBObject->condIf(stmt);
					else stmt_left = mPKBObject->condWhile(stmt);
					if(pattern1_code == stmt_left){
						vector<int> tmp;
						tmp.push_back(mQueryTree->getIndex("integer"));
						tmp.push_back(stmt);
						tmp.push_back(mQueryTree->getIndex("varofsimpl"));
						tmp.push_back(pattern1_code);
						result.push_back(tmp);
					}
				}
			}
		}
	}else 
		throw new string("QueryEvaluator::evalPattern_PQL, no such variable type!");
}

//same1_tuple1 means the equal index in tuple1, from 0 to tuple1 size; same for same2_tuple1
void QueryEvaluator::joinTuples(vector<vector<int> >& evalTuple, vector<vector<int> >& pre_tuple, int common_num, int same1_tuple1, int same2_tuple1, int first_time){
		if(common_num == 2){
			if(same1_tuple1 != -1)
				evalTuple = TupleOperations::tupleJoinOneC(same1_tuple1, 0, evalTuple, pre_tuple);
			else evalTuple = TupleOperations::tupleJoinOneC(same2_tuple1, 1, evalTuple, pre_tuple);
		}else if(common_num == 4){
			evalTuple = TupleOperations::tupleJoinTwoC(same1_tuple1, same2_tuple1, evalTuple, pre_tuple);
		}else if((int) evalTuple.size() == 0){
			//(first_time == 0)
			evalTuple = pre_tuple;
		}else{
			vector<vector<int> > tmp_store;
			for(vector<vector<int> >::iterator i = evalTuple.begin(); i<evalTuple.end(); i++)
				for(vector<vector<int> >::iterator k = pre_tuple.begin(); k<pre_tuple.end(); k++){
					vector<int> join_entry;
					join_entry.insert(join_entry.end(), (*i).begin(), (*i).end());
					join_entry.insert(join_entry.end(), (*k).begin(), (*k).end());
					tmp_store.push_back(join_entry);
				}				
			evalTuple = tmp_store;
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
				if(type == mQueryTree->getIndex("integer")){
					if(mResult.isElementCallWithProc(j)){
						int procIndex = mPKBObject->ast_getCallProcIndex(resultTuple[i][j]);
						cout << PKB_procDecode(procIndex) << endl;
					}else cout << resultTuple[i][j] << " ";
				}else if(type == mQueryTree->getIndex("varOfSimpl")) 
					cout << PKB_varDecode(resultTuple[i][j]) << " ";	
				else if(type == mQueryTree->getIndex("procOfSimpl")) 
					cout << PKB_procDecode(resultTuple[i][j]) << " ";
				else 
					throw new string("No type match when decode result for query result");
			}
			cout << ",";
		}
	}
}
	
void QueryEvaluator::evalParent(int star, vector<std::vector<int> >& result, const vector<int>& para1, const vector<int>& para2){
	if(para1.empty() || para2.empty())
		return;
	if(para1[0] == -1 || para2[0] == -1)
		return;
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
	if(para1.empty() || para2.empty())
		return;
	if(para1[0] == -1 || para2[0] == -1)
		return;
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
	if(para1.empty() || para2.empty())
		return;
	if(para1[0] == -1 || para2[0] == -1)
		return;
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
						if(found == (int)result.size())	result.push_back(k[j]);
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
						if(found == (int)result.size())	result.push_back(k[j]);
					}
				}
			}
		}
	}else throw new string("QueryEvaluator::getCallsStar, no such up type.");
}

void QueryEvaluator::evalMU(int modOrUse, vector<vector<int> >& result, int type1, const vector<int>& para1, const vector<int>& para2){
	if(para1.empty() || para2.empty())
		return;
	if(para1[0] == -1 || para2[0] == -1)
		return;

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
	if(para1.empty() || para2.empty())
		return;

	if(para1[0] == -1 || para2[0] == -1)
		return;

	if(para1.size() <= para2.size()){
		for(int l = 0; l < (int)para1.size(); l++){
			int stmtN = para1[l];
			vector<int> nexts;
			if(star == NOSTAR) mPKBObject->cfg_getNext(nexts, stmtN);
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
			if(star == NOSTAR) mPKBObject->cfg_getNextUp(nexts, stmtN);
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
		mPKBObject->cfg_getNext(result, para);
		if((int)result.size() == 3){
			int next1 = result[0];
			int next2 = result[2];
			result.clear();
			result.push_back(next1);
			result.push_back(next2);
		}
	}else if(up == UP){
		mPKBObject->cfg_getNextUp(result, para);
		for(int i = 0; i < (int)result.size();){
			if(result[i] == -1)
				result.erase(result.begin()+i);
			else i++;
		}
	}else throw new string("QueryEvaluator::getNextStar, no such up type");
}

//The para passed inside is checked to be assign
void QueryEvaluator::evalAffects(vector<vector<int> >& result, const vector<int>& para1, const vector<int>&  para2){
	if(para1.empty() || para2.empty())
		return;
	if(para1[0] == -1 || para2[0] == -1)
		return;

	if(affectsTable.is_affects_table_built){
		if(para1.size() <= para2.size()){
			for(int i = 0; i < (int)para1.size(); i++){
				int in1 = para1[i];
				vector<int> affected_para1;
				int stmt_index;
				int found = affectsTable.affects_index_down.count(in1);
				if(found > 0){
					stmt_index = affectsTable.affects_index_down[in1];
					affected_para1 = affectsTable.table_body_down[stmt_index];
				}
				for(int k = 0; k < (int)affected_para1.size(); k++){
					for(int p = 0; p < (int)para2.size(); p++){
						if(affected_para1[k] == para2[p]){
							int tmp[4] = {mQueryTree->getIndex("integer"), in1, mQueryTree->getIndex("integer"), para2[p]};
							vector<int> entry;
							entry.insert(entry.end(), tmp, tmp+4);
							result.push_back(entry);
							break;
						}
					}
				}
			}
		}else{
			for(int i = 0; i < (int)para2.size(); i++){
				int in2 = para2[i];
				vector<int> affected_up_para2;
				int found = affectsTable.affects_index_up.count(in2);
				int stmt_index = -1;
				if(found > 0){
					stmt_index = affectsTable.affects_index_up[in2];
					affected_up_para2 = affectsTable.table_body_up[stmt_index];
				}
				for(int k = 0; k < (int)affected_up_para2.size(); k++){
					for(int p = 0; p < (int)para1.size(); p++){
						if(affected_up_para2[k] == para1[p]){
							int tmp[4] = {mQueryTree->getIndex("integer"), para1[p], mQueryTree->getIndex("integer"), in2};
							vector<int> entry;
							entry.insert(entry.end(), tmp, tmp+4);
							result.push_back(entry);
							break;
						}
					}
				}
			}
		}
	}else{
		for(int i = 0; i < (int)para1.size(); i++){
			int in1 = para1[i];
			vector<int> affected_para1;

			vector<int> modifies;
			mPKBObject->mTable_getModifiedVar(modifies, in1);
			if(modifies.empty() || modifies[0]==-1) throw new string("QueryEvaluator::evalAffects, shit! This should never happen!");
			int mod = modifies[0];

			nonModPath(in1, mod, -1, in1, affected_para1, true);

			for(int k = 0; k < (int)affected_para1.size(); k++){
				for(int p = 0; p < (int)para2.size(); p++){
					if(affected_para1[k] == para2[p]){
						int tmp[4] = {mQueryTree->getIndex("integer"), in1, mQueryTree->getIndex("integer"), para2[p]};
						vector<int> entry;
						entry.insert(entry.end(), tmp, tmp+4);
						result.push_back(entry);
						break;
					}
				}
			}
		}
	}
}

//The para passed inside is checked to be assign
void QueryEvaluator::evalAffectsStar(vector<vector<int> >& result, const vector<int>& para1, const vector<int>& para2){
	if(para1.empty() || para2.empty())
		return;
	if(para1[0] == -1 || para2[0] == -1)
		return;

	if(!affectsTable.is_affects_table_built) throw new string("QueryEvaluator::evalAffectsStar, shit! How can table not built yet!");
	if(para1.size() <= para2.size()){
		for(int i = 0; i < (int)para1.size(); i++){
			int in1 = para1[i];
			vector<int> affectsStar;
			getAffectsStar(DOWN, affectsStar, in1);
			for(int j = 0; j < (int)para2.size(); j++){
				int in2 = para2[j];
				int found = find_ele(affectsStar, in2);
				if(found != (int)affectsStar.size()){
					int t[4] = {mQueryTree->getIndex("integer"), in1, mQueryTree->getIndex("integer"), in2};
					vector<int> tmp;
					tmp.insert(tmp.end(), t, t+4);
					result.push_back(tmp);
				}
			}
		}
	}else{
		for(int i = 0; i < (int)para2.size(); i++){
			int in2 = para2[i];
			vector<int> affectsStar;
			getAffectsStar(UP, affectsStar, in2);
			for(int j = 0; j < (int)para1.size(); j++){
				int in1 = para1[j];
				int found = find_ele(affectsStar, in1);
				if(found != (int)affectsStar.size()){
					int t[4] = {mQueryTree->getIndex("integer"), in1, mQueryTree->getIndex("integer"), in2};
					vector<int> tmp;
					tmp.insert(tmp.end(), t, t+4);
					result.push_back(tmp);
				}
			}
		}
	}
}

void QueryEvaluator::getAffectsStar(int up, vector<int>& result, int para){
	if(up == DOWN){
		int found = affectsTable.affects_index_down.count(para);
		int para_index;
		if(found > 0){
			para_index = affectsTable.affects_index_down[para];
			result = affectsTable.table_body_down[para_index];
		}
		for(int i = 0; i < (int)result.size(); i++){
			int next_stmt = result[i];
			found = affectsTable.affects_index_down.count(next_stmt);
			int next_index;
			vector<int> affects;
			if(found > 0){
				next_index = affectsTable.affects_index_down[next_stmt];
				affects = affectsTable.table_body_down[next_index];
			}
			for(int j = 0; j < (int)affects.size(); j++){
				int found = find_ele(result, affects[j]);
				if(found == (int)result.size())
					result.push_back(affects[j]);
			}
		}
	}else{
		int found = affectsTable.affects_index_up.count(para);
		int para_index;
		if(found > 0){
			para_index = affectsTable.affects_index_up[para];
			result = affectsTable.table_body_up[para_index];
		}
		for(int i = 0; i < (int)result.size(); i++){
			int next_stmt = result[i];
			found = affectsTable.affects_index_up.count(next_stmt);
			int next_index;
			vector<int> affects;
			if(found > 0){
				next_index = affectsTable.affects_index_up[next_stmt];
				affects = affectsTable.table_body_up[next_index];
			}
			for(int j = 0; j < (int)affects.size(); j++){
				int found = find_ele(result, affects[j]);
				if(found == (int)result.size())
					result.push_back(affects[j]);
			}
		}
	}
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

bool QueryEvaluator::nonModPath(int s, int mod, int dest, int start, vector<int>& affect_result, bool init){
	//AutoTester Collaborative
	//if(AbstractWrapper::GlobalStop){
	//	throw new string("GlobalStop, time out!");
	//}

	int next = s;

	if(!init){
		if(s == start){
			if(mPKBObject->ast_IsAssign(next)){
				vector<int> uses;
				mPKBObject->uTable_getUsedVar(uses, next);
				int found_uses = find_ele(uses, mod);
				if(found_uses != (int)uses.size())
				if(mPKBObject->ast_IsAssign(next))
					affect_result.push_back(next);
			}
			return false;
		}
	}

	if(s <= 0)
		return true;

	if(!init){
		if(next == dest){
			return true;
		}
	}

	bool is_if = mPKBObject->ast_IsIf(next);
	bool is_while = mPKBObject->ast_IsWhile(next);
	if(is_if){
		vector<int> tmp_nexts;
		getNextPure(DOWN, tmp_nexts, next);
		int thenC = tmp_nexts[1];
		int elseC = tmp_nexts[0];
		int joint_node = nextOfIf(thenC);

		bool non_mod_path1;
		bool non_mod_path2;
		bool join_result;

		non_mod_path1 = nonModPath(thenC, mod, joint_node, start, affect_result, false);
		non_mod_path2 = nonModPath(elseC, mod, joint_node, start, affect_result, false);

		if(!non_mod_path1 && !non_mod_path2) join_result =  false;
		else join_result = nonModPath(joint_node, mod, dest, start, affect_result, false);
		return join_result;
	}else if(is_while){
		vector<int> tmp_nexts;
		getNextPure(DOWN, tmp_nexts, next);
		int size = tmp_nexts.size();
		int inner;
		int outer;
		if(size == 2){
			outer = tmp_nexts[0];
			inner = tmp_nexts[1];
			vector<int> child;
			mPKBObject->ast_GetChild(child, next);
			int found = find_ele(child, outer);
			if(found != (int)child.size()){
				int tmp_s = outer;
				outer = inner;
				inner = tmp_s;
			}
			
			nonModPath(inner, mod, next, start, affect_result, false);
			return nonModPath(outer, mod, dest, start, affect_result, false);	
		}else{
			inner = tmp_nexts[0];

			nonModPath(inner, mod, next, start, affect_result, false);
		}
	}else{
		vector<int> tmp_nexts;
		
		if(init){
			getNextPure(DOWN, tmp_nexts, next);
			next = tmp_nexts[0];
			return nonModPath(next, mod, dest, start, affect_result, false);
		}

		vector<int> uses;
		mPKBObject->uTable_getUsedVar(uses, next);
		int found_uses = find_ele(uses, mod);
		if(found_uses != (int)uses.size())
			if(mPKBObject->ast_IsAssign(next))
				affect_result.push_back(next);

		vector<int> modifies;
		int found;
		mPKBObject->mTable_getModifiedVar(modifies, next); 
		found = find_ele(modifies, mod);
		getNextPure(DOWN, tmp_nexts, next);
		next = tmp_nexts[0];
		if(found != (int)modifies.size())
			return false;
		else return nonModPath(next, mod, dest, start, affect_result, false);	
		
	}
	return true;
}

int QueryEvaluator::nextOfIf(int first){
	int follows = first;
	int end;
	while(true){
		int next = mPKBObject->ast_GetFollowingStatementNum(follows);
		if(next == -1) {end = follows; break; }
		follows = next;
	}
	if(mPKBObject->ast_IsIf(end)){
		vector<int> nexts;
		getNextPure(DOWN, nexts, end);
		return nextOfIf(nexts[0]);
	}else if(mPKBObject->ast_IsWhile(end)){
		vector<int> nexts;
		getNextPure(DOWN, nexts, end);
		if((int)nexts.size() == 2){
			int outer = nexts[0];
			int inner = nexts[1];
			vector<int> child;
			mPKBObject->ast_GetChild(child, end);
			int found = find_ele(child, outer);
			if(found != (int)child.size()){
				int tmp_s = outer;
				outer = inner;
				inner = tmp_s;
			}
			return outer;
		}else return -1;
	}else{
		vector<int> nexts;
		getNextPure(DOWN, nexts, end);
		return nexts[0];
	}


	
}

void QueryEvaluator::getAllType(vector<int>& result, int type){
	if(type == mQueryTree->getIndex("stmt"))  getAllStmts(result);
	else if(type == mQueryTree->getIndex("prog_line")) getAllStmts(result);
	else if(type == mQueryTree->getIndex("assign")) mPKBObject->ast_GetAllAssign(result);
	else if(type == mQueryTree->getIndex("while")) mPKBObject->ast_GetAllWhile(result);
	else if(type == mQueryTree->getIndex("if")) mPKBObject->ast_GetAllIf(result);
	else if(type == mQueryTree->getIndex("call")) mPKBObject->ast_GetAllCall(result);
	else if(type == mQueryTree->getIndex("variable")) getAllVar(result);
	else if(type == mQueryTree->getIndex("procedure")) getAllProc(result);
	else if(type == mQueryTree->getIndex("constant")) mPKBObject->cTable_GetAllConstants(result);
	else throw new string("QueryEvaluator::getAllType, type not exist!");
	//cout << endl;
}

void QueryEvaluator::getAllProc(vector<int>& result){
	int max = mPKBObject->pTable_GetAllProc();
	for(int i = 0; i < max; i++){
		result.push_back(i);
	}
}

void QueryEvaluator::getAllVar(vector<int>& result){
	int max = mPKBObject->vTable_GetAllVar();
	for(int i = 0; i < max; i++){
		result.push_back(i);
	}
}