#include "QueryEvaluator.h"
#include <string>

using namespace std;


//Controller passes the PKB and Query Parser to evaluator, and trigger evaluator to start.
QueryEvaluator::QueryEvaluator(PKB *p, PqlParser *q)
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

	int var_code_ending = 501; //I need to use it when I meet '_' in relation to create new variable


	//No tupled selection yet
	int select_element = (mQueryTree->selectAt(0)).at(1);  //Elements to be selected


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
			if(clause.at(withCount) != 155)
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
		if(p1_type == 51)  tmp = getAllStmts();
		else if(p1_type == 53) tmp = mPKBObject->ast_GetAllAssign();
		else if(p1_type == 54) tmp = mPKBObject->ast_GetAllWhile();
		else if(p1_type == 55) tmp = mPKBObject->ast_GetAllIf();
		else if(p1_type == 56) tmp = mPKBObject->cTable_GetAllConstants();
		else if(p1_type == 57) tmp = mPKBObject->vTable_GetAllVar();
		else if(p1_type == 58) tmp = mPKBObject->ast_GetAllProc();
		else if(p1_type == 59) tmp = mPKBObject->ast_GetAllCall();
		
		vector<int> tmp2;
		if(p2_type == 201) tmp2.push_back(part2.at(1));
		else if(p2_type == 202) tmp2.push_back(PKB_varEncode(PQL_varDecode(part2.at(1))));
		else if(p2_type == 203) tmp2.push_back(PKB_procEncode(PQL_procDecode(part2.at(1))));
		else if(p2_type == 51)  tmp2 = getAllStmts();
		else if(p2_type == 53) tmp2 = mPKBObject->ast_GetAllAssign();
		else if(p2_type == 54) tmp2 = mPKBObject->ast_GetAllWhile();
		else if(p2_type == 55) tmp2 = mPKBObject->ast_GetAllIf();
		else if(p2_type == 56) tmp2 = mPKBObject->cTable_GetAllConstants();
		else if(p2_type == 57) tmp2 = mPKBObject->vTable_GetAllVar();
		else if(p2_type == 58) tmp2 = mPKBObject->ast_GetAllProc();
		else if(p2_type == 59) tmp2 = mPKBObject->ast_GetAllCall();
		
		int entry_type;
		if(p1_type == 51|| p1_type==53|| p1_type==54|| p1_type==55||p1_type==59 ||p1_type==56)
			entry_type = 201;
		else if(p1_type==57)
			entry_type = 202;
		else if(p1_type==58)
			entry_type = 203;
		else throw "with clause error type";
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
			if(p2_type == 201 || p2_type == 202 || p2_type == 203)
				mgTupleIndexing.push_back(var_code_ending++);
			else mgTupleIndexing.push_back(p2_name);
		else
		{
			for(vector<int>::iterator i = mgTupleIndexing.begin(); i<=it; i++){
				same2Tuple1++;
			}
			numOfCommonElement = numOfCommonElement+2;
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

		//for(vector<int>::iterator i= clause.begin(); i<clause.end(); i++){
		//	cout << *i << " ";
		//}
		//cout << endl;
		//system("pause");
		int next_indx = 0;
		if(clause[1] == 157)// if the first para is _
		{ 
			para1 = 157;
			next_indx = 2;
		}
		else 
		{
			para1Type = clause[1];
			para1 = clause[2];
			next_indx = 3;
		}
		if(clause[next_indx] == 157)// if the second para is _
		{ 
			para2 = 157;
		}
		else 
		{
			para2Type = clause[next_indx];
			para2 = clause[next_indx+1];
		}

		
		if(rel == 5 || rel == 6 || rel == 7 || rel == 8) //relation is parent, parent*, follows, follows*
		{
			if(para1 == 157)  //If _ , then replace with (stmt newest)
			{
				para1Type = 51;
				para1 = var_code_ending++;
			}
			if(para2 == 157)
			{
				para2Type = 51;
				para2 = var_code_ending++;
			}
		}
		else if(rel == 9 || rel == 10) // uses and modifies
		{
			if(para2 == 157) //replace with (variable newest)
			{
				para2Type = 57;
				para2 = var_code_ending++;
			}
		}
		else if(rel == 11 || rel == 12) //relation is calls, calls*
		{
			if(para1 == 157) //replace with (proc newest)
			{
				para1Type = 58;
				para1 = var_code_ending++;
			}
			if(para2 == 157)
			{
				para2Type = 58;
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
			if(para1Type == 201 || para1Type == 202 || para1Type == 203)
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
			if(para2Type == 201 || para2Type == 202 || para2Type == 203)
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
		if(para1Type == 202) para1 = PKB_varEncode(PQL_varDecode(para1));
		else if(para1Type == 203) para1 = PKB_procEncode(PQL_procDecode(para1));

		if(para2Type == 202) para2 = PKB_varEncode(PQL_varDecode(para2));
		else if(para2Type == 203) para2 = PKB_procEncode(PQL_procDecode(para2));

		if(rel == 5 ||rel==6|| rel==7 || rel==8 ||rel==9 || rel==10 || rel==11||rel==12)
			relResult = getRel(para1Type, para2Type, para1, para2, rel);
		else 
			throw "Relation not exists";

		/*
		///////////////////////////////////////////////////////////////////////////////////////////////////////DEBUGGING
		cout << "para1Type="<<para1Type;
		cout << " para2Type=" << para2Type;
		cout << " para1= " << para1;
		cout << " para2 = " << para2;
		cout << " rel= "<<rel;
		cout << endl;
		cout << "Join pre-condition check: " << endl;
		cout << "numOfCommonElement = " << numOfCommonElement << endl;
		cout << "same1Tuple1 = " << same1Tuple1 << endl;
		cout << "same2Tuple1 = " << same2Tuple1 << endl;

			//////////////////////////////////////////////////////////////////////////////////////////////////////////////DEBUGGING
	cout << "previous resulted tuple: " << endl;
	for(vector<vector<int> >::iterator i = eva_tuple.begin(); i< eva_tuple.end(); i++){
		for(vector<int>::iterator k = (*i).begin(); k<(*i).end(); k++){
			cout << *k << " ";
		}
		cout << endl;
	}
	cout << "The relation evaluation result: " << endl;
	for(vector<vector<int> >::iterator i = relResult.begin(); i< relResult.end(); i++){
		for(vector<int>::iterator k = (*i).begin(); k<(*i).end(); k++){
			cout << *k << " ";
		}
		cout << endl;
	}
	cout << "pre_Join result shown" << endl; */

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
/*
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////DEBUGGING
	cout << "Show join result:" << endl;
	for(vector<vector<int> >::iterator i = eva_tuple.begin(); i< eva_tuple.end(); i++){
		for(vector<int>::iterator k = (*i).begin(); k<(*i).end(); k++){
			cout << *k << " ";
		}
		cout << endl;
	}
	cout << "Join result shown" << endl;
	*/

	}//while: such that evaluation END

	
	/* Old way of working with a.stmt# = s.stmt#.

	//This is to deal with when the with clause have v.varName = p.procName
	int equal_ele_size = (int)equal_vars.size();
	for(int i = 0; i<equal_ele_size; i++){
		int same1 = equal_vars.at(i);
		int same2 = equal_vars.at(i++);
		int found1 = -1;
		int found2 = -1;
		for(int k = 0; k<(int) eva_tuple.at(0).size(); k++){
			if(mgTupleIndexing.at(k) == same1) found1 = k;
			if(mgTupleIndexing.at(k) == same2) found2 = k;
		}
		if(found1 == -1 || found2 == -1) break;
		for(int k = 0; k<(int) eva_tuple.size(); k++){
			if(eva_tuple.at(k).at(found1) != eva_tuple.at(k).at(found2)) 
				eva_tuple.erase(eva_tuple.begin() + k);
		}
	}*/



	if(select_element == 60) //If the select is boolean
	{
		if(mgTupleIndexing.empty()) mResult.setBoolValue(false);
		else mResult.setBoolValue(true);
		return;
	}


	if(with_size == 0 && suchThatSize == 0){
		int select_type = (mQueryTree->selectAt(0)).at(0);
		vector<int> tmp;
		if(select_type == 51)  tmp = getAllStmts();
		else if(select_type == 53) tmp = mPKBObject->ast_GetAllAssign();
		else if(select_type == 54) tmp = mPKBObject->ast_GetAllWhile();
		else if(select_type == 55) tmp = mPKBObject->ast_GetAllIf();
		else if(select_type == 56) tmp = mPKBObject->cTable_GetAllConstants();
		else if(select_type == 57) tmp = mPKBObject->vTable_GetAllVar();
		else if(select_type == 58) tmp = mPKBObject->ast_GetAllProc();
		else if(select_type == 59) tmp = mPKBObject->ast_GetAllCall();
		for(vector<int>::iterator i=tmp.begin(); i<tmp.end(); i++){
			mResult.addInType(201);
			mResult.addInTuple(*i);
		}
	}else{
		int indx = 0;
		for(; indx< (int) mgTupleIndexing.size(); indx++)
		{
			if(select_element == mgTupleIndexing.at(indx)) break;
		}
		for(vector<vector<int> >::iterator i= eva_tuple.begin(); i< eva_tuple.end(); i++){
			mResult.addInType((*i).at(indx*2));
			mResult.addInTuple((*i).at(indx*2+1));
		}
	}
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
		cout << mResult.getBoolValue() << endl;
	}else{
		vector<int> resultTuple = mResult.getTuple();
		vector<int> resultTupleType = mResult.getTypes();
		cout << "Query evaluate: ";
		cout << "query result size: " << (int)resultTuple.size() << endl ;
		int count = 0;
		for(vector<int>::iterator i=resultTuple.begin(); i<resultTuple.end(); i++){
			if(resultTupleType.at(count++) == 201) cout << *i << " ";
			else if(resultTupleType.at(count++) == 202) cout << PKB_varDecode(*i);	
			else if(resultTupleType.at(count++) == 203) cout << PKB_procDecode(*i);
			else throw "No type match when decode result";
		}
		cout << endl;
	}
}


//The varName and procName code has been converted to PKB side code
vector<vector<int> > QueryEvaluator::getRel(int type1, int type2, int para1, int para2, int relType)
{
	vector<vector<int> > evalResult;	
	switch(relType)
	{
		case 5: case 6: //Relation is parent or parent*
		{
			vector<int> para1List;
			if(type1 == 201) para1List.push_back(para1);
			else if(type1 == 51) para1List = getAllStmts();
			else if(type1 == 53) para1List = mPKBObject->ast_GetAllAssign();
			else if(type1 == 54) para1List = mPKBObject->ast_GetAllWhile();
			else if(type1 == 55) para1List = mPKBObject->ast_GetAllIf();
			else if(type1 == 59) para1List = mPKBObject->ast_GetAllCall();
			else throw "Parent parameter type mismatch!";

			vector<int> para2List;
			if(type2 == 201) para2List.push_back(para2);
			else if(type2 == 51) para2List = getAllStmts();
			else if(type2 == 53) para2List = mPKBObject->ast_GetAllAssign();
			else if(type2 == 54) para2List = mPKBObject->ast_GetAllWhile();
			else if(type2 == 55) para2List = mPKBObject->ast_GetAllIf();
			else if(type2 == 59) para2List = mPKBObject->ast_GetAllCall();
			else throw "Your follows relation has unpaired second parameters";

			for(vector<int>::iterator i=para1List.begin(); i<para1List.end(); i++){
				vector<int> result;
				if(relType == 5)
					 result = mPKBObject->ast_GetChild(*i);
				else if(relType == 6)
					result = getChildStar(*i);
				if(*(result.begin()) == -1) continue;    //continue this loop if this candidate has no child
				for(vector<int>::iterator k=result.begin(); k<result.end(); k++){
					vector<int>::iterator it = find(para2List.begin(), para2List.end(), *k);
					if(it == para2List.end()) continue; 
					vector<int> tmp;
					int t[] = {201,*i,201, *k};
					tmp.insert(tmp.end(), t, t+4);
					evalResult.push_back(tmp);
				}
			}
			break;
		}//Parent END
		case 7: case 8://relation is follows or follows*
		{
			vector<int> para1List; 
			if(type1 == 201) para1List.push_back(para1);
			else if(type1 == 51) para1List = getAllStmts();
			else if(type1 == 53) para1List = mPKBObject->ast_GetAllAssign();
			else if(type1 == 54) para1List = mPKBObject->ast_GetAllWhile();
			else if(type1 == 55) para1List = mPKBObject->ast_GetAllIf();
			else if(type1 == 59) para1List = mPKBObject->ast_GetAllCall();
			else throw "Your follows relation has unpaired parameters";

			vector<int> para2List;
			if(type2 == 201) para2List.push_back(para2);
			else if(type2 == 51) para2List = getAllStmts();
			else if(type2 == 53) para2List = mPKBObject->ast_GetAllAssign();
			else if(type2 == 54) para2List = mPKBObject->ast_GetAllWhile();
			else if(type2 == 55) para2List = mPKBObject->ast_GetAllIf();
			else if(type2 == 59) para2List = mPKBObject->ast_GetAllCall();
			else throw "Your follows relation has unpaired second parameters";
			
			for(vector<int>::iterator i=para1List.begin(); i<para1List.end(); i++){
				vector<int> result;
				if(relType == 7) 
					result.push_back(mPKBObject->ast_GetFollowingStatementNum(*i));
				else result = getFollowsStar(*i);
				if(result.at(0) == -1) continue;    //continue this loop without eva this iteration
				for(vector<int>::iterator k = result.begin(); k<result.end(); k++){
					vector<int>::iterator it = find(para2List.begin(), para2List.end(), *k);
					if(it == para2List.end()) continue; 
					vector<int> tmp;
					int t[] = {201,*i, 201,*k};
					tmp.insert(tmp.end(), t, t+4);
					evalResult.push_back(tmp);
				}
			}
			break;
		}//Follows END
	case 9: case 10: //relation is uses or modifies
		{
			if(type1 == 201 || type1 == 51 || type1 == 53 || type1 == 54 || type1 == 55 ) //the first parameter is a constant stmt#
			{
				vector<int> para1List;
				if(type1 == 201) para1List.push_back(para1);
				else if(type1 == 51) para1List = getAllStmts();
				else if(type1 == 53) para1List = mPKBObject->ast_GetAllAssign();
				else if(type1 == 54) para1List = mPKBObject->ast_GetAllWhile();
				else if(type1 == 55) para1List = mPKBObject->ast_GetAllIf();
				else if(type1 == 59) para1List = mPKBObject->ast_GetAllCall();
				else break; //cannot happen
				for(vector<int>::iterator i=para1List.begin(); i<para1List.end(); i++){
					vector<int> result;
					if(relType == 9)
						result = mPKBObject->uTable_getUsedVar(*i);
					else  result = mPKBObject->mTable_getModifiedVar(*i);
					if(*(result.begin()) == -1) continue;    //continue this loop without eva this iteration
					for(vector<int>::iterator k=result.begin(); k<result.end(); k++){
						if(type2 == 202 && para2 != *k) continue;
						vector<int> tmp;
						int t[] = {201,*i, 202, *k};
						tmp.insert(tmp.begin(), t, t+4);
						evalResult.push_back(tmp);
					}
				}
			}
			else if(type1 == 203 || type1 == 58) //the first parameter is a constant procName or proc var
			{
				vector<int> para1List;
				if(type1 == 203) para1List.push_back(para1);
				else para1List = mPKBObject->ast_GetAllProc();
				for(vector<int>::iterator i=para1List.begin(); i<para1List.end(); i++){
					vector<int> result;
					if(relType == 9)
						result = mPKBObject->uTable_getUsedVarPI(*i);
					else  result = mPKBObject->mTable_getModifiedVarPI(*i);
					if(*(result.begin()) == -1) continue;    //continue this loop without eva this iteration
					for(vector<int>::iterator k=result.begin(); k<result.end(); k++){
						if(type2 == 202 && para2 != *k) continue;
						vector<int> tmp;
						int t[] = {203, *i, 202, *k};
						tmp.insert(tmp.begin(), t, t+4);
						evalResult.push_back(tmp);
					}
				}
			}
			else//Error Case
			{
				throw "Your uses relation has unpaired parameters";
			}
			break;
		}//Uses END
	case 11: case 12://relation is calls, calls*
		{
			if(type1 == 203 || type1 == 58) //proc var
			{
				vector<int> para1List;
				if(type1 == 203) para1List.push_back(para1);
				else para1List = mPKBObject->ast_GetAllProc();
				for(vector<int>::iterator i=para1List.begin(); i<para1List.end(); i++)
				{
					vector<int> result;
					if(relType == 11) result = mPKBObject->pTable_getCall(*i);
					else result = getCallsStar(*i);
					if(*(result.begin()) == -1) continue;    //continue this loop without eva this iteration
					for(vector<int>::iterator k = result.begin(); k<result.end(); k++)
					{
						if(type2 == 203 && para2 != *k) continue;
						vector<int> tmp;
						int t[] = {203, *i, 203, *k};
						tmp.insert(tmp.begin(), t, t+4);
						evalResult.push_back(tmp);
					}
				}
			}
			else//Error Case
			{
				throw "Your calls relation has unpaired parameters";
			}
			break;
		}//Calls END
	default:
		{
			throw "No such relation implementation yet";
		}//Error case END
	}//getRel END
	return evalResult;
}
	

vector<int> QueryEvaluator::getChildStar(int stmtN){
	vector<int> descendant = mPKBObject->ast_GetChild(stmtN);
	if(descendant.at(0) != -1){
		for(vector<int>::iterator i = descendant.begin(); i < descendant.end(); i++){
			vector<int> k = mPKBObject->ast_GetChild(*i);
			if(k.at(0) != -1)descendant.insert(descendant.end(), k.begin(), k.end());
		}
	}
	return descendant;
}

vector<int> QueryEvaluator::getFollowsStar(int stmtN){
	vector<int> following;
	int follow = mPKBObject->ast_GetFollowingStatementNum(stmtN);
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
	for(int i = 0; i< max; i++){
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
	return mPKBObject->pTable_GetProcName(i);
}
int QueryEvaluator::PKB_varEncode(string s){
	return mPKBObject->vTable_GetVarIndex(s);
}
string QueryEvaluator::PKB_varDecode(int i){
	return mPKBObject->vTable_GetVarName(i);
}