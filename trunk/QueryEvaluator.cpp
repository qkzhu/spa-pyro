#include "QueryEvaluator.h"

using namespace std;


//Controller passes the PKB and Query Parser to evaluator, and trigger evaluator to start.
QueryEvaluator::QueryEvaluator(PKB *p, QueryParser *q)
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
	vector<int> equal_vars;  //for use when in with clause, there is sth like v.varName = p.varName
	map<int, vector<int> > var_value_table; //maps the variable to the value assigned in With clause, attr 1 is the variable string in its code

	int var_code_ending = 501; //I need to use it when I meet '_' in relation to create new variable


	//No tupled selection yet
	int select_element = mQueryTree->selectAt(0);  //Elements to be selected


	//Start evaluating With clauses                                  
	int with_size = mQueryTree->withSize();
	for(int i = 0; i<with_size; i++){
		vector<int> clause = mQueryTree->withAt(i);
		int clause_size = clause.size();
		int ref = clause.back();

		//Read the with clause and divide it
		vector<int> part1;
		vector<int> part2;
		int withCount = 0;
		for(; withCount<clause_size; withCount++)
		{
			if(clause.at(withCount) != 155)
				part1.push_back(clause.at(withCount));
			break;
		}
		for(; withCount<clause_size; withCount++){
			part2.push_back(clause.at(withCount));
		}
		//clause dividing END

		int part2_size = part2.size();

		if(part2_size = 2)  //Variable 
		{
			int type = clause.at(0);
			int syn = clause.at(1);
			int dot = clause.at(2);
			//int attr_name = clause.at(3); No need to check this
			int eq = clause.at(4);
			
			if(dot != 156 || eq != 155) throw "With clause bad formed";
			else {
				var_value_table.insert(syn, part2);	
			}
		}//if: normal case END
		//For the case when a.stmt# = b.stmt#
		else if(part1.size() == part2_size)
		{
			int p1_name = part1.at(1);
			int p2_name = part2.at(1);
			equal_vars.push_back(p1_name);
			equal_vars.push_back(p2_name);
		}
		else throw "With clause bad formed";
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
		if(clause[1] = 157)// if the first para is _
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
		if(clause[next_indx] = 157)// if the second para is _
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
				para2Type = 51;
				para2 = var_code_ending++;
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
		vector<int>::iterator it;
		it = find(mgTupleIndexing.begin(), mgTupleIndexing.end(), para1);
		int same1Tuple1 = 0;
		int same2Tuple1 = 0;
		int numOfCommonElement = 0;
		//When no common attr for new tuple with older old one, just do nothing, cause no relation inside
		//But this requires the query passed to me 
		//NEWEST: this way turned out to be unmature, it has error when there is no relation has attr of select element. I come up with a way of adding a new relation to add select element evaluation, but this is not so workable for multiple select.
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
			numOfCommonElement = 4;
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

		if(rel == 5 ||rel==6|| rel==7 || rel==8 ||rel==9 || rel==10 || rel==11||rel==12)
			relResult = getRel(para1Type, para2Type, para1, para2, rel);
		else 
			throw "Relation not exists";

		
		if(numOfCommonElement == 2)
			eva_tuple = TupleOperations.tupleJoinOneC(same1Tuple1, eva_tuple, relResult);
		else if(numOfCommonElement == 4)
			eva_tuple = TupleOperations.tupleJoinTwoC(same1Tuple1, same2Tuple1, eva_tuple, relResult);
	}//while: such that evaluation END


	//This is to deal with when the with clause have v.varName = p.procName
	int equal_ele_size = equal_vars.size();
	for(int i = 0; i<equal_ele_size; i++){
		int same1 = equal_vars.at(i);
		int same2 = equal_vars.at(i++);
		int found1 = -1;
		int found2 = -1;
		for(int k = 0; k<eva_tuple.at(0).size(); k++){
			if(mgTupleIndexing.at(k) == same1) found1 = k;
			if(mgTupleIndexing.at(k) == same2) found2 = k;
		}
		if(found1 == -1 || found2 == -1) break;
		for(int k = 0; k<eva_tuple.size(); k++){
			if(eva_tuple.at(k).at(found1) != eva_tuple.at(k).at(found2)) 
				eva_tuple.erase(eva_tuple.begin() + k);
		}
	}


	if(select_element == 60) //If the select is boolean
	{
		if(mgTupleIndexing.empty()) mResult.setBoolValue(false);
		else mResult.setBoolValue(true);
		return;
	}

	int indx = 0;
	for(; indx< mgTupleIndexing.size(); indx++)
	{
		if(select_element == mgTupleIndexing.at(indx)) break;
	}
	mResult.addInTuple(&eva_tuple.at(indx));
	//Store result inside mResult


}//End of evaluate

QueryResult QueryEvaluator::getResult()
{
	return mResult;
}



vector<vector<int> > QueryEvaluator::getRel(int type1, int type2, int para1, int para2, int relType)
{
	vector<vector<int> > evalResult;	
	switch(relType)
	{
		case 5: case 6: //Relation is parent or parent*
		{
			vector<int> para1List;
			if(type1 = 201) para1List.push_back(para1);
			else if(type1 == 51 || type1 == 52)
				para1List = mPKBObject->getAllStmts();
			else if(type1 == 54 || type1 == 55)
				para1List = mPKBObject->getAllWhile();
			else throw "Parent parameter type mismatch!";
			for(vector<int>::iterator i=para1List.begin(); i<para1List.end(); i++){
				vector<int> result;
				if(relType == 5)
					 result = mPKBObject->getChild(*i);
				else if(relType == 6)
					result = getChildStar(*i);
				if(*(result.begin()) == -1) continue;    //continue this loop if this candidate has no child
				for(vector<int>::iterator k=result.begin(); k<result.end(); k++){
					vector<int> para2List;
					if(type2 == 201) para2List.push_back(para2);
					else if(type2 == 53) 
						para2List = mPKBObject->getAllAssign();
					else if(type2 == 54)
						para2List = mPKBObject->getAllWhile();
					else if(type2 == 55)
						para2List = mPKBObject->getAllIf();
					else if(type2 == 59)
						para2List = mPKBObject->getAllCall();
					else throw "Your follows relation has unpaired second parameters";
					vector<int>::iterator it = find(para2List.begin(), para2List.end(), *k);
					if(it == para2List.end()) continue; 
					vector<int> tmp;
					int t[] = {201,para1,201, *k};
					tmp.insert(tmp.end(), t, t+4);
					evalResult.push_back(tmp);
				}
			}
			break;
		}//Parent END
		case 7: case 8://relation is follows or follows*
		{
			vector<int> para1List; 
			if(type1 = 201) para1List.push_back(para1);
			else if(type1 = 51 || type1 == 52)
				para1List = mPKBObject->getAllStmts();
			else if(type1 == 53)
				para1List = mPKBObject->getAllAssign();
			else if(type1 == 54)
				para1List = mPKBObject->getAllWhile();
			else if(type1 == 55)
				para1List = mPKBObject->getAllIf();
			else if(type1 == 59)
				para1List = mPKBObject->getAllCall();
			else throw "Your follows relation has unpaired parameters";
			for(vector<int>::iterator i=para1List.begin(); i<para1List.end(); i++){
				vector<int> result;
				if(relType == 7) 
					result.push_back(mPKBObject->getFollowingStatement(*i));
				else result = getFollowsStar(*i);
				if(result.at(0) == -1) continue;    //continue this loop without eva this iteration
				for(vector<int>::iterator k = result.begin(); k<result.end(); k++){
					vector<int> para2List;
					if(type2 == 201) para2List.push_back(para2);
					else if(type2 == 53) para2List = mPKBObject->getAllAssign();
					else if(type2 == 54)
					para2List = mPKBObject->getAllWhile();
					else if(type2 == 55)
					para2List = mPKBObject->getAllIf();
					else if(type2 == 59)
					para2List = mPKBObject->getAllCall();
					else throw "Your follows relation has unpaired second parameters";
					vector<int>::iterator it = find(para2List.begin(), para2List.end(), *k);
					if(it == para2List.end()) continue; 
					vector<int> tmp;
					int t[] = {201,para1, 201,*k};
					tmp.insert(tmp.end(), t, t+4);
					evalResult.push_back(tmp);
				}
			}
			break;
		}//Follows END
	case 9: case 10: //relation is uses or modifies
		{
			if(type1 == 201 || type1 == 51 || type1 == 52 || type1 == 53 || type1 == 54 || type1 == 55 || type1 == 59) //the first parameter is a constant stmt#
			{
				vector<int> para1List;
				if(type1 == 201)
					para1List.push_back(para1);
				else if(type1 == 51 || type1 == 52)
					para1List = mPKBObject->getAllStmts();
				else if(type1 == 53) //assign var
					para1List = mPKBObject->getAllAssign();
				else if(type1 == 54) //while var
					para1List = mPKBObject->getAllWhile();
				else if(type1 == 55) //if var
					para1List = mPKBObject->getAllIf();
				else if(type1 == 59) //
					para1List = mPKBObject->getAllCall();
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
						int t[] = {201,para1, 202, *k};
						tmp.insert(tmp.begin(), t, t+4);
						evalResult.push_back(tmp);
					}
				}
			}
			else if(type1 == 203 || type1 == 58) //the first parameter is a constant procName or proc var
			{
				vector<int> para1List;
				if(type1 == 203)
					para1List.push_back(para1);
				else para1List = mPKBObject->getAllProc();
				for(vector<int>::iterator i=para1List.begin(); i<para1List.end(); i++){
					vector<int> result;
					if(relType == 9)
						result = mPKBObject->uTable_getUsedVarPI(*i);
					else  result = mPKBObject->mTable_getModifiedVarPI(*i);
					if(*(result.begin()) == -1) continue;    //continue this loop without eva this iteration
					for(vector<int>::iterator k=result.begin(); k<result.end(); k++){
						if(type2 == 202 && para2 != *k) continue;
						vector<int> tmp;
						int t[] = {203, para1, 202, *k};
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
	case 11: case 12://relation is calls
		{
			if(type1 == 203 || type1 == 58) //proc var
			{
				vector<int> para1List;
				if(type1 == 203) para1List.push_back(para1);
				else para1List = mPKBObject->getAllProc();
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
						int t[] = {203, para1, 203, *k};
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
	vector<int> descendant = mPKBObject->getChild(stmtN);
	if(descendant.at(0) != -1){
		for(vector<int>::iterator i = descendant.begin(); i < descendant.end(); i++){
			vector<int> k = mPKBObject->getChild(*i);
			if(k.at(0) != -1)descendant.insert(descendant.end(), k.begin(), k.end());
		}
	}
	return descendant;
}

vector<int> QueryEvaluator::getFollowsStar(int stmtN){
	vector<int> following;
	int follow = mPKBObject->getFollowingStatement(stmtN);
	while(follow != -1){
		following.push_back(follow);
		follow = mPKBObject->getFollowingStatement(follow);	
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


