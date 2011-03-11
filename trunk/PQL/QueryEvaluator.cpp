#include "QueryEvaluator.h"
#include <string>

using namespace std;


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
	vector<vector<int> > eva_tuple;  //evalation tuple

	//for use when in with clause, there is sth like v.varName = p.varName .... OLD WAY
	//If contains only the pair of variable codes, no type needed.
	vector<int> equal_vars;

	//maps the variable to the value assigned in With clause, attr 1 is the variable string in its code
	//The value must have the type indicated
	map<int, vector<int> > var_value_table; 

	int var_code_ending = ENCODE_ENDING+1; //I need to use it when I meet '_' in relation to create new variable


	//No tupled selection yet
	int select_element = (mQueryTree->selectAt(0)).at(0);  //Elements to be selected
	if(select_element != mQueryTree->getIndex("BOOLEAN")) select_element = (mQueryTree->selectAt(0)).at(1);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////FOR DEBUGGING
	cout<< "PQL parser checking"<< endl;
	cout << "Select element: " << select_element << endl;; 
	for(int i=0; i< mQueryTree->withSize(); i++){
		cout << "With clauses: " << endl;
		vector<int> tmp_with =  mQueryTree->withAt(i);
		for(int k = 0; k < tmp_with.size(); k++){
			cout << tmp_with.at(k) << " ";
		}
		cout << endl;
	}
	for(int i=0; i< mQueryTree->suchThatSize(); i++){
		cout << "Such that clauses: " << endl;
		vector<int> tmp_suchthat =  mQueryTree->suchThatAt(i);
		for(int k = 0; k < tmp_suchthat.size(); k++){
			cout << tmp_suchthat.at(k) << " ";
		}
		cout << endl;
	}
	cout << "PQL PARSER checking FINISH." << endl;
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//Start evaluating With clauses                                  
	int with_size = mQueryTree->withSize();
	for(int i = 0; i<with_size; i++){
		vector<int> clause = mQueryTree->withAt(i);
		int clause_size = (int)clause.size();
		int ref = clause.back();

		//Read the with clause and divide it
		vector<int> part1;
		vector<int> part2;
		int withCount = 0;
		for(; withCount<clause_size; withCount++)
		{
			if(clause.at(withCount) != mQueryTree->getIndex("="))
				part1.push_back(clause.at(withCount));
			else break;
		}
		withCount++;
		for(; withCount<clause_size; withCount++){
			part2.push_back(clause.at(withCount));
		}
		//clause dividing END

		int part2_size =(int) part2.size();

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
		if(p1_type == mQueryTree->getIndex("stmt"))  tmp = getAllStmts();
		else if(p1_type == mQueryTree->getIndex("assign")) tmp = mPKBObject->ast_GetAllAssign();
		else if(p1_type == mQueryTree->getIndex("while")) tmp = mPKBObject->ast_GetAllWhile();
		else if(p1_type == mQueryTree->getIndex("if")) tmp = mPKBObject->ast_GetAllIf();
		else if(p1_type == mQueryTree->getIndex("constant")) tmp = mPKBObject->cTable_GetAllConstants();
		else if(p1_type == mQueryTree->getIndex("variable")) tmp = mPKBObject->vTable_GetAllVar();
		else if(p1_type == mQueryTree->getIndex("procedure")) tmp = mPKBObject->pTable_GetAllProc();
		else if(p1_type == mQueryTree->getIndex("call")) tmp = mPKBObject->ast_GetAllCall();
		
		vector<int> tmp2;
		if(p2_type == mQueryTree->getIndex("integer")) tmp2.push_back(part2.at(1));
		else if(p2_type == mQueryTree->getIndex("varOfSimpl")) {
			int tmp_code = PKB_varEncode(PQL_varDecode(part2.at(1)));
			if(tmp_code == -1) {
				mResult.addInTuple(-1);
				return;
			}
			tmp2.push_back(tmp_code);
		}
		else if(p2_type == mQueryTree->getIndex("procOfSimpl")) {
			int tmp_code = PKB_procEncode(PQL_procDecode(part2.at(1)));
			if(tmp_code == -1){
				mResult.addInTuple(-1);
				return;
			}
			tmp2.push_back(tmp_code);
		}
		else if(p2_type == mQueryTree->getIndex("stmt"))  tmp2 = getAllStmts();
		else if(p2_type == mQueryTree->getIndex("assign")) tmp2 = mPKBObject->ast_GetAllAssign();
		else if(p2_type == mQueryTree->getIndex("while")) tmp2 = mPKBObject->ast_GetAllWhile();
		else if(p2_type == mQueryTree->getIndex("if")) tmp2 = mPKBObject->ast_GetAllIf();
		else if(p2_type == mQueryTree->getIndex("constant")) tmp2 = mPKBObject->cTable_GetAllConstants();
		else if(p2_type == mQueryTree->getIndex("variable")) tmp2 = mPKBObject->vTable_GetAllVar();
		else if(p2_type == mQueryTree->getIndex("procedure")) tmp2 = mPKBObject->pTable_GetAllProc();
		else if(p2_type == mQueryTree->getIndex("call")) tmp2 = mPKBObject->ast_GetAllCall();
		
		int entry_type;
		if(p1_type == mQueryTree->getIndex("stmt")|| p1_type==mQueryTree->getIndex("assign")|| p1_type==mQueryTree->getIndex("while")|| p1_type==mQueryTree->getIndex("if")||p1_type==mQueryTree->getIndex("call") ||p1_type==mQueryTree->getIndex("constant"))
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
			if(select_element == mQueryTree->getIndex("BOOLEAN"))
				mResult.setBoolValue(false);
			else mResult.addInTuple(-1);
			return;
		}

		if(numOfCommonElement == 2)
			if(same1Tuple1 != 0)
				eva_tuple = TupleOperations::tupleJoinOneC(same1Tuple1, 0,eva_tuple, with_result);
			else eva_tuple = TupleOperations::tupleJoinOneC(same2Tuple1, 1, eva_tuple, with_result);
		else if(numOfCommonElement == 4)
			eva_tuple = TupleOperations::tupleJoinTwoC(same1Tuple1, same2Tuple1, eva_tuple, with_result);
		else if((int) eva_tuple.size() == 0)
			eva_tuple = with_result;
		else{
			vector<vector<int> > tmp_store;
			for(vector<vector<int> >::iterator i = eva_tuple.begin(); i<eva_tuple.end(); i++)
				for(vector<vector<int> >::iterator k = with_result.begin(); k<with_result.end(); k++){
					vector<int> join_entry;
					join_entry.insert(join_entry.end(), (*i).begin(), (*i).end());
					join_entry.insert(join_entry.end(), (*k).begin(), (*k).end());
					tmp_store.push_back(join_entry);
				}				
			eva_tuple = tmp_store;
		}
	}//while: With clause evaluation End
	
	//Start evaluating SuchThat clauses
	int suchThatSize = mQueryTree->suchThatSize();
	for(int i= 0; i<suchThatSize; i++)
	{
		vector<int> clause = mQueryTree->suchThatAt(i);
		int rel = clause[0];
		int para1Type;
		int para1;
		int para2Type;
		int para2;

		int next_indx = 0;
		if(clause[1] == mQueryTree->getIndex("_"))// if the first para is _
		{ 
			para1 = clause[1];
			next_indx = 2;
		}
		else 
		{
			para1Type = clause[1];
			para1 = clause[2];
			next_indx = 3;
		}
		if(clause[next_indx] == mQueryTree->getIndex("_"))// if the second para is _
		{ 
			para2 = clause[next_indx];
		}
		else 
		{
			para2Type = clause[next_indx];
			para2 = clause[next_indx+1];
		}

		
		if(rel == mQueryTree->getIndex("parent") || rel == mQueryTree->getIndex("parent*") || rel == mQueryTree->getIndex("follows") || rel == mQueryTree->getIndex("follows*")) //relation is parent, parent*, follows, follows*
		{
			if(para1 == mQueryTree->getIndex("_"))  //If _ , then replace with (stmt newest)
			{
				para1Type = mQueryTree->getIndex("stmt");
				para1 = var_code_ending++;
			}
			if(para2 == mQueryTree->getIndex("_"))
			{
				para2Type = mQueryTree->getIndex("stmt");
				para2 = var_code_ending++;
			}
		}
		else if(rel == mQueryTree->getIndex("uses") || rel == mQueryTree->getIndex("modifies")) // uses and modifies
		{
			if(para2 == mQueryTree->getIndex("_")) //replace with (variable newest)
			{
				para2Type = mQueryTree->getIndex("variable");
				para2 = var_code_ending++;
			}
		}
		else if(rel == mQueryTree->getIndex("calls") || rel == mQueryTree->getIndex("calls*")) //relation is calls, calls*
		{
			if(para1 == mQueryTree->getIndex("_")) //replace with (proc newest)
			{
				para1Type = mQueryTree->getIndex("procedure");
				para1 = var_code_ending++;
			}
			if(para2 == mQueryTree->getIndex("_"))
			{
				para2Type = mQueryTree->getIndex("procedure");
				para2 = var_code_ending++;
			}
		}else;  //do nothing 

		//Manipulation of indexing issue, adjust the mgTupleIndexing and find common index to pass
		//When no common attr for new tuple with older old one, just do nothing, cause no relation inside
		//But this requires the query passed to me 
		//NEWEST: this way turned out to be unmature, it has error when there is no relation has attr of select element. I come up with a way of adding a new relation to add select element evaluation, but this is not so workable for multiple select.
		vector<int>::iterator it;
		it = find(mgTupleIndexing.begin(), mgTupleIndexing.end(), para1);
		int same1Tuple1 = 0;
		int same2Tuple1 = 0;
		int numOfCommonElement = 0;
		if(it == mgTupleIndexing.end())
			if(para1Type == mQueryTree->getIndex("integer") || para1Type == mQueryTree->getIndex("varOfSimpl") || para1Type == mQueryTree->getIndex("procOfSimpl"))
				mgTupleIndexing.push_back(var_code_ending++);
			else mgTupleIndexing.push_back(para1);
		else
		{
			for(vector<int>::iterator i = mgTupleIndexing.begin(); i<=it; i++){
				same1Tuple1++;
			}
			numOfCommonElement = 2;
		}
		it = find(mgTupleIndexing.begin(), mgTupleIndexing.end(), para2);
		if(it == mgTupleIndexing.end())
			if(para2Type == mQueryTree->getIndex("integer") || para2Type == mQueryTree->getIndex("varOfSimpl") || para2Type == mQueryTree->getIndex("procOfSimpl"))
				mgTupleIndexing.push_back(var_code_ending++);
			else mgTupleIndexing.push_back(para2);
		else
		{
			for(vector<int>::iterator i = mgTupleIndexing.begin(); i<=it; i++){
				same2Tuple1++;
			}
			numOfCommonElement = numOfCommonElement+2;
		}

		

		//Evaluating Relation
		vector<vector<int> > relResult;
		map<int,vector<int> >::iterator it1 = var_value_table.find(para1);
		map<int,vector<int> >::iterator it2 = var_value_table.find(para2);
		if(it1 != var_value_table.end())
		{
			para1Type = (it1->second).at(0);
			para1 = (it1->second).at(1);
		}
		if(it2 != var_value_table.end())
		{
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
			if(select_element == mQueryTree->getIndex("BOOLEAN"))
				mResult.setBoolValue(false);
			else mResult.addInTuple(-1);
			return;
		}

		if(rel == mQueryTree->getIndex("parent") ||rel==mQueryTree->getIndex("parent*")|| rel==mQueryTree->getIndex("follows") || rel==mQueryTree->getIndex("follows*") ||rel==mQueryTree->getIndex("uses") || rel==mQueryTree->getIndex("modifies") || rel==mQueryTree->getIndex("calls")||rel==mQueryTree->getIndex("calls*"))
			relResult = getRel(para1Type, para2Type, para1, para2, rel);
		else 
			throw new string("Relation not exists");
		if(relResult.empty()){
			if(select_element == mQueryTree->getIndex("BOOLEAN"))
				mResult.setBoolValue(false);
			else mResult.addInTuple(-1);
			return;
		}

		if(numOfCommonElement == 2)
				if(same1Tuple1 != 0)
					eva_tuple = TupleOperations::tupleJoinOneC(same1Tuple1, 0,eva_tuple, relResult);
				else eva_tuple = TupleOperations::tupleJoinOneC(same2Tuple1, 1, eva_tuple, relResult);
		else if(numOfCommonElement == 4)
			eva_tuple = TupleOperations::tupleJoinTwoC(same1Tuple1, same2Tuple1, eva_tuple, relResult);
		else if((int) eva_tuple.size() == 0)
			eva_tuple = relResult;
		else{
			vector<vector<int> > tmp_store;
			for(vector<vector<int> >::iterator i = eva_tuple.begin(); i<eva_tuple.end(); i++)
				for(vector<vector<int> >::iterator k = relResult.begin(); k<relResult.end(); k++){
					vector<int> join_entry;
					join_entry.insert(join_entry.end(), (*i).begin(), (*i).end());
					join_entry.insert(join_entry.end(), (*k).begin(), (*k).end());
					tmp_store.push_back(join_entry);
				}				
			eva_tuple = tmp_store;
		}

	}//while: such that evaluation END


	if(select_element == mQueryTree->getIndex("BOOLEAN")) //If the select is boolean
	{
		if(mgTupleIndexing.empty()) mResult.setBoolValue(false);
		else mResult.setBoolValue(true);
		return;
	}

	//find whether the selected element is inside the evaluated result tuple
	int indx = 0;
	for(; indx< (int) mgTupleIndexing.size(); indx++)
		if(select_element == (int)mgTupleIndexing.at(indx)) break;

	//if no with and such that clause, just return;
	//Or if the selected element is not inside evaluated result tuple, depend on the tuple
	if((with_size == 0 && suchThatSize == 0)|| (mgTupleIndexing.size()!=0 && indx == mgTupleIndexing.size())){
		
		//if the selected element is not inside evaluated result tuple and result tuple is null
		if(mgTupleIndexing.size() != 0){
			if(eva_tuple.empty()) {
				mResult.addInTuple(-1);
				return;
			}
		}

		int select_type = (mQueryTree->selectAt(0)).at(0);
		vector<int> tmp;
		if(select_type == mQueryTree->getIndex("stmt"))  tmp = getAllStmts();
		else if(select_type == mQueryTree->getIndex("assign")) tmp = mPKBObject->ast_GetAllAssign();
		else if(select_type == mQueryTree->getIndex("while")) tmp = mPKBObject->ast_GetAllWhile();
		else if(select_type == mQueryTree->getIndex("if")) tmp = mPKBObject->ast_GetAllIf();
		else if(select_type == mQueryTree->getIndex("constant")) tmp = mPKBObject->cTable_GetAllConstants();
		else if(select_type == mQueryTree->getIndex("variable")) tmp = mPKBObject->vTable_GetAllVar();
		else if(select_type == mQueryTree->getIndex("procedure")) tmp = mPKBObject->pTable_GetAllProc();
		else if(select_type == mQueryTree->getIndex("call")) tmp = mPKBObject->ast_GetAllCall();
		else throw new string("Select type error!");

		for(vector<int>::iterator i=tmp.begin(); i<tmp.end(); i++){
			if(select_type == mQueryTree->getIndex("variable"))
				mResult.addInType(mQueryTree->getIndex("varOfSimpl"));
			else if (select_type == mQueryTree->getIndex("procedure"))
				mResult.addInType(mQueryTree->getIndex("procOfSimpl"));
			else mResult.addInType(mQueryTree->getIndex("integer"));
			mResult.addInTuple(*i);
		}
	}else{
		for(vector<vector<int> >::iterator i= eva_tuple.begin(); i< eva_tuple.end(); i++){
			mResult.addInType((*i).at(indx*2));
			mResult.addInTuple((*i).at(indx*2+1));
		}
	}

	//In case the returned mResult is empty, then insert -1 inside.
	vector<int> resultTuple = mResult.getTuple();
	if(resultTuple.empty()) mResult.addInTuple(-1);

	//Store result inside mResult
	//cout << "eval finish!" << endl;

}//End of evaluate

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
		vector<int> resultTuple = mResult.getTuple();
		vector<int> resultTupleType = mResult.getTypes();
		cout << "Query evaluate: ";
		int count = 0;
		if((int)resultTuple.at(0) == -1){
			cout << "query result size: " << 0 << endl ;
			cout << "Null" << endl;
			return;
		}
		cout << "query result size: " << (int)resultTuple.size() << endl ;
		for(vector<int>::iterator i=resultTuple.begin(); i<resultTuple.end(); i++){
			int type = resultTupleType.at(count++);
			if(type == mQueryTree->getIndex("integer"))
				cout << *i << " ";
			else if(type == mQueryTree->getIndex("varOfSimpl")) 
				cout << PKB_varDecode(*i) << " ";	
			else if(type == mQueryTree->getIndex("procOfSimpl")) 
				cout << PKB_procDecode(*i) << " ";
			else 
				throw new string("No type match when decode result for query result");
		}
		cout << endl;
	}
}


//The varName and procName code has been converted to PKB side code
vector<vector<int> > QueryEvaluator::getRel(int type1, int type2, int para1, int para2, int relType)
{
	////////////////////////Crucial for debugging, don't delete //////////////////////////////////////////////////////
	cout << "gerRel info: relType = " << relType << " ";
	cout << "type1 = "<<type1 << " ";
	cout << "type2 = " <<type2 << " ";
	cout << "para1 = " <<para1 << " ";
	cout << "para2 = "<<para2 << endl;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	vector<vector<int> > evalResult;
	switch(relType)
	{
		case 5: case 6: //Relation is parent or parent*
		{
			vector<int> para1List;
			if(type1 == mQueryTree->getIndex("integer")) para1List.push_back(para1);
			else if(type1 == mQueryTree->getIndex("stmt")) para1List = getAllStmts();
			else if(type1 == mQueryTree->getIndex("assign")) para1List = mPKBObject->ast_GetAllAssign();
			else if(type1 == mQueryTree->getIndex("while")) para1List = mPKBObject->ast_GetAllWhile();
			else if(type1 == mQueryTree->getIndex("if")) para1List = mPKBObject->ast_GetAllIf();
			else if(type1 == mQueryTree->getIndex("call")) para1List = mPKBObject->ast_GetAllCall();
			else throw new string("Parent parameter type mismatch!");

			vector<int> para2List;
			if(type2 == mQueryTree->getIndex("integer")) para2List.push_back(para2);
			else if(type2 == mQueryTree->getIndex("stmt")) para2List = getAllStmts();
			else if(type2 == mQueryTree->getIndex("assign")) para2List = mPKBObject->ast_GetAllAssign();
			else if(type2 == mQueryTree->getIndex("while")) para2List = mPKBObject->ast_GetAllWhile();
			else if(type2 == mQueryTree->getIndex("if")) para2List = mPKBObject->ast_GetAllIf();
			else if(type2 == mQueryTree->getIndex("call")) para2List = mPKBObject->ast_GetAllCall();
			else throw new string("Your follows relation has unpaired second parameters");

			for(vector<int>::iterator i=para1List.begin(); i<para1List.end(); i++){
				vector<int> result;
				if(relType == mQueryTree->getIndex("parent"))
					 result = mPKBObject->ast_GetChild(*i);
				else if(relType == mQueryTree->getIndex("parent*"))
					result = getChildStar(*i);
				if(*(result.begin()) == -1) continue;    //continue this loop if this candidate has no child
				for(vector<int>::iterator k=result.begin(); k<result.end(); k++){
					vector<int>::iterator it = find(para2List.begin(), para2List.end(), *k);
					if(it == para2List.end()) continue; 
					vector<int> tmp;
					int t[] = {mQueryTree->getIndex("integer"),*i,mQueryTree->getIndex("integer"), *k};
					tmp.insert(tmp.end(), t, t+4);
					evalResult.push_back(tmp);
				}
			}
			break;
		}//Parent END
		case 7: case 8://relation is follows or follows*
		{
			vector<int> para1List; 
			
			if(type1 == mQueryTree->getIndex("integer")) para1List.push_back(para1);
			else if(type1 == mQueryTree->getIndex("stmt")) para1List = getAllStmts();
			else if(type1 == mQueryTree->getIndex("assign")) para1List = mPKBObject->ast_GetAllAssign();
			else if(type1 == mQueryTree->getIndex("while")) para1List = mPKBObject->ast_GetAllWhile();
			else if(type1 == mQueryTree->getIndex("if")) para1List = mPKBObject->ast_GetAllIf();
			else if(type1 == mQueryTree->getIndex("call")) para1List = mPKBObject->ast_GetAllCall();
			else throw new string("Your follows relation has unpaired parameters");

			vector<int> para2List;
			if(type2 == mQueryTree->getIndex("integer")) para2List.push_back(para2);
			else if(type2 == mQueryTree->getIndex("stmt")) para2List = getAllStmts();
			else if(type2 == mQueryTree->getIndex("assign")) para2List = mPKBObject->ast_GetAllAssign();
			else if(type2 == mQueryTree->getIndex("while")) para2List = mPKBObject->ast_GetAllWhile();
			else if(type2 == mQueryTree->getIndex("if")) para2List = mPKBObject->ast_GetAllIf();
			else if(type2 == mQueryTree->getIndex("call")) para2List = mPKBObject->ast_GetAllCall();
			else throw new string("Your follows relation has unpaired second parameters");

			for(vector<int>::iterator i=para1List.begin(); i<para1List.end(); i++){
				vector<int> result;
				if(relType == mQueryTree->getIndex("follows")) 
					result.push_back(mPKBObject->ast_GetFollowingStatementNum(*i));
				else result = getFollowsStar(*i);
				
				if( (int)(result.at(0))== -1) continue;    //continue this loop without eva this iteration
				for(vector<int>::iterator k = result.begin(); k<result.end(); k++){
					vector<int>::iterator it = find(para2List.begin(), para2List.end(), *k);
					if(it == para2List.end()) continue; 
					vector<int> tmp;
					int t[] = {mQueryTree->getIndex("integer"),*i, mQueryTree->getIndex("integer"),*k};
					tmp.insert(tmp.end(), t, t+4);
					evalResult.push_back(tmp);
				}
			}
			break;
		}//Follows END
	case 9: case 10: //relation is uses or modifies
		{
			//mQueryTree->getIndex("procOfSimpl")
			if(type1 == mQueryTree->getIndex("integer") || type1 == mQueryTree->getIndex("stmt") || type1 == mQueryTree->getIndex("assign") || type1 == mQueryTree->getIndex("while") || type1 == mQueryTree->getIndex("if") ) //the first parameter is a constant stmt#
			{
				vector<int> para1List;
				if(type1 == mQueryTree->getIndex("integer")) para1List.push_back(para1);
				else if(type1 == mQueryTree->getIndex("stmt")) para1List = getAllStmts();
				else if(type1 == mQueryTree->getIndex("assign")) para1List = mPKBObject->ast_GetAllAssign();
				else if(type1 == mQueryTree->getIndex("while")) para1List = mPKBObject->ast_GetAllWhile();
				else if(type1 == mQueryTree->getIndex("if")) para1List = mPKBObject->ast_GetAllIf();
				else if(type1 == mQueryTree->getIndex("call")) para1List = mPKBObject->ast_GetAllCall();
				else break; //cannot happen
				
				for(vector<int>::iterator i=para1List.begin(); i<para1List.end(); i++){
					vector<int> result;
					if(relType == mQueryTree->getIndex("uses"))
						result = mPKBObject->uTable_getUsedVar(*i);
					else  result = mPKBObject->mTable_getModifiedVar(*i);
					
					//vector<int> asdkfaldsk= mPKBObject->uTable_getUsedVar(4);
					if(result.at(0) == -1) continue;    //continue this loop without eva this iteration
					for(vector<int>::iterator k=result.begin(); k<result.end(); k++){
						if(type2 == mQueryTree->getIndex("varOfSimpl") && para2 != *k) continue;
						vector<int> tmp;
						int t[] = {mQueryTree->getIndex("integer"),*i, mQueryTree->getIndex("varOfSimpl"), *k};
						tmp.insert(tmp.begin(), t, t+4);
						evalResult.push_back(tmp);
					}
				}
			}
			else if(type1 == mQueryTree->getIndex("procOfSimpl") || type1 == mQueryTree->getIndex("procedure")) //the first parameter is a constant procName or proc var
			{
				vector<int> para1List;
				if(type1 == mQueryTree->getIndex("procOfSimpl")) para1List.push_back(para1);
				else para1List = mPKBObject->pTable_GetAllProc();
				for(vector<int>::iterator i=para1List.begin(); i<para1List.end(); i++){
					vector<int> result;
					if(relType == mQueryTree->getIndex("uses"))
						result = mPKBObject->uTable_getUsedVarPI(*i);
					else  result = mPKBObject->mTable_getModifiedVarPI(*i);
					if(*(result.begin()) == -1) continue;    //continue this loop without eva this iteration
					for(vector<int>::iterator k=result.begin(); k<result.end(); k++){
						if(type2 == mQueryTree->getIndex("varOfSimpl") && para2 != *k) continue;
						vector<int> tmp;
						int t[] = {mQueryTree->getIndex("procOfSimpl"), *i, mQueryTree->getIndex("varOfSimpl"), *k};
						tmp.insert(tmp.begin(), t, t+4);
						evalResult.push_back(tmp);
					}
				}
			}
			else//Error Case
			{
				throw new string("Your uses relation has unpaired parameters");
			}
			break;
		}//Uses END
	case 11: case 12://relation is calls, calls*
		{
			if((type1 == mQueryTree->getIndex("procOfSimpl") || type1 == mQueryTree->getIndex("procedure"))&&(type2 == mQueryTree->getIndex("procOfSimpl") || type2 == mQueryTree->getIndex("procedure"))) //proc var
			{
				vector<int> para1List;
				if(type1 == mQueryTree->getIndex("procOfSimpl")) para1List.push_back(para1);
				else para1List = mPKBObject->pTable_GetAllProc();
				for(vector<int>::iterator i=para1List.begin(); i<para1List.end(); i++)
				{
					vector<int> result;
					if(relType == mQueryTree->getIndex("calls")) result = mPKBObject->pTable_getCall(*i);
					else result = getCallsStar(*i);
					if(*(result.begin()) == -1) continue;    //continue this loop without eva this iteration
					for(vector<int>::iterator k = result.begin(); k<result.end(); k++)
					{
						if(type2 == mQueryTree->getIndex("procOfSimpl") && para2 != *k) continue;
						vector<int> tmp;
						int t[] = {mQueryTree->getIndex("procOfSimpl"), *i, mQueryTree->getIndex("procOfSimpl"), *k};
						tmp.insert(tmp.begin(), t, t+4);
						evalResult.push_back(tmp);
					}
				}
			}
			else//Error Case
			{
				throw new string("Your calls relation has unpaired parameters");
			}
			break;
		}//Calls END
	default:
		{
			throw new string("No such relation implementation yet");
		}//Error case END
	}//getRel END

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////HQ DEBUGGING
	cout << "FOR DEBUGGING" << endl;
	for(vector<vector<int> >::iterator i=evalResult.begin(); i< evalResult.end(); i++){
		for(vector<int>::iterator k = (*i).begin(); k< (*i).end(); k++){
			cout << *k << " ";
		}
		cout << endl;
	}
	cout << "DEBUGGING FINISH" << endl;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	return evalResult;
}
	

vector<int> QueryEvaluator::getChildStar(int stmtN){
	cout << "stmtN1 = " << stmtN << endl;
	vector<int> descendant = mPKBObject->ast_GetChild(stmtN);
	cout << "stmtN3 = " << stmtN << endl;
	
	//It is important that I don't use iterator to loop descendant
	//It will cause some error
	if( descendant.at(0) != -1){
		for(int i = 0; i < descendant.size(); i++){
			vector<int> k = mPKBObject->ast_GetChild(descendant[i]);
			if(k.at(0) != -1){
				descendant.insert(descendant.end(), k.begin(), k.end());
			}
		}
	}
	return descendant;
}

vector<int> QueryEvaluator::getFollowsStar(int stmtN){
	vector<int> following;
	int follow = mPKBObject->ast_GetFollowingStatementNum(stmtN);
	if(follow == -1){
		following.push_back(-1);
		return following;
	}
	while(follow != -1){
		following.push_back(follow);
		follow = mPKBObject->ast_GetFollowingStatementNum(follow);	
	}
	return following;
}

vector<int> QueryEvaluator::getCallsStar(int procNameCode){
	vector<int> descendant = mPKBObject->pTable_getCall(procNameCode);
	if(descendant.at(0) != -1){
		for(vector<int>::iterator i = descendant.begin(); i < descendant.end(); i++){
			vector<int> k = mPKBObject->pTable_getCall(*i);
			if(k.at(0) != -1)descendant.insert(descendant.end(), k.begin(), k.end());
		}
	}
	return descendant;
}


vector<int> QueryEvaluator::getAllStmts(){
	int max = mPKBObject->ast_getMaxStmtNum();
	vector<int> tmp;
	for(int i = 1; i< max+1; i++){
		tmp.push_back(i);
	}
	return tmp;
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