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
	vector<vector<int> > *eva_tuple;  //evalation tuple
	map<int, int> *var_value_table; //maps the variable to the value assigned in With clause, attr 1 is the variable string in its code

	//No tupled selection yet
	int select_element = mQueryTree->selectAt(0);  //Elements to be selected

	//Start evaluating With clauses
	int with_size = mQueryTree->withSize();
	for(int i = 0; i<with_size; i++){
		vector<int> clause = mQueryTree->withAt(i);
		int clause_size = clause.size();
		int ref = clause.back();
		if(clause_size == 6)  //Variable 
		{
			int type = clause.at(0);
			int syn = clause.at(1);
			int dot = clause.at(2);
			//int attr_name = clause.at(3); No need to check this
			int eq = clause.at(4);
			if(type != 203) throw "With clause bad formed"; //Check whether the first token is query var
			if(dot != 156 || eq != 155) throw "With clause bad formed";
			else {
				var_value_table->insert(syn, ref);	
			}
		}//if: normal case END
		else if(clause_size == 5) //variable is synonmy, just check whether this stmt hold
		{
			int syn = clause.at(0);
			int dot = clause.at(1);
			int attr_name = clause.at(2);
			int eq = clause.at(3);
			if(syn != 157) throw "With clause bad formed"; //Check whether the first token is _
			if(dot != 156 || eq != 155) throw "With clause bad formed";
			else{
				if(attr_name = 101)  //value attr
				{
					if(mPKBObject->hasStmt(ref) || mPKBObject->hasCons(ref)) break;
					else 
					{
						mgNullClause = true;
						return;
					}
				}
				else if(attr_name = 102) //varName attr
				{
					if(mPKBObject->hasVar(mPKBObject->varCode(mQueryTree->varDecode(ref)))) break;
					else
					{
						mgNullClause = true;
						return;
					}
				}
				else if(attr_name = 103) //procName attr
				{
					if(mPKBObject->hasProc(mPKBObject->procCode(mQueryTree->procDecode(ref)))) break;
					else
					{
						mgNullClause = true;
						return;
					}
				}
				else if(attr_name = 104) //stmt# attr
				{
					if(mPKBObject->hasStmt(ref)) break;
					else
					{
						mgNullClause = true;
						return;
					}
				}
				else throw "With clause bad formed";
			}//if: check attr END
		}//if: synonmy END
		else throw "With clause bad formed";
	}//while: With clause evaluation End

	//Start evaluating SuchThat clauses
	int suchThatSize = mQueryTree->suchThatSize();
	for(int i= 0; i<suchThatSize; i++)
	{
		int *clause = mQueryTree->suchThatAt(i);
		int rel = clause[0];
		int para1Type = clause[1];
		int para1 = clause[2];
		int para2Type = clause[3];
		int para2 = clause[4];

		//Manipulation of indexing issue, adjust the mgTupleIndexing and find common index to pass
		vector<int>::iterator it;
		it = find(mgTupleIndexing->begin(), mgTupleIndexing->end(), para1);
		vector<vector<int> >::iterator same1Tuple1;
		vector<vector<int> >::iterator same2Tuple1;
		vector<vector<int> >::iterator same1Tuple2;
		vector<vector<int> >::iterator same2Tuple2;  // Need to be assigned when the new result tuple is found
		int numOfCommonElement = 0;
		if(it == mgTupleIndexing->end()) 
			mgTupleIndexing->push_back(para1);
		else 
		{
			same1Tuple1 = eva_tuple->begin();
			for(vector<int>::iterator i = mgTupleIndexing->begin(); i<=it; i++){
				same1Tuple1++;
			}
			numOfCommonElement = 2;
		}
		it = find(mgTupleIndexing->begin(), mgTupleIndexing->end(), para2);
		if(it == mgTupleIndexing->end())
			mgTupleIndexing->push_back(para2);
		else
		{
			same1Tuple1 = eva_tuple->begin();
			for(vector<int>::iterator i = mgTupleIndexing->begin(); i<=it; i++){
				same1Tuple1++;
			}
			numOfCommonElement = 4;
		}


		//Evaluating Relation
		vector<vector<int> > *relResult;
		if(rel == 5 || rel==7 || rel==9 || rel==10 || rel==11)
			relResult = getRel(para1Type, para2Type, para1, para2, rel);
		else if (rel == 6 || rel == 8 || rel == 12)
			relResult = getRelStar(para1Type, para2Type, para1, para2, rel);
		else 
			throw "No Such Relation: " + rel + "\n";
		same1Tuple2 = relResult->begin();
		same2Tuple2 = relResult->end();

		if(numOfCommonElement == 2)
			eva_tuple = TupleOperations.tupleJoinOneC(same1Tuple1, same2Tuple1, eva_tuple, relResult);
		else if(numOfCommonElement == 4)
			eva_tuple = TupleOperations.tupleJoinTwoC(same1Tuple1, same2Tuple1, same2Tuple1, same2Tuple2, eva_tuple, relResult);
	}//while: such that evaluation END


	if(select_element == 60) //If the select is boolean
	{
		if(mgTupleIndexing->empty()) mResult->setBoolValue(false);
		else mResult->setBoolValue(true);
		return;
	}

	int indx = 0;
	for(; indx< mgTupleIndexing->size(); indx++)
	{
		if(select_element == mgTupleIndexing->at(indx)) break;
	}
	mResult->addInTuple(&eva_tuple->at(indx));
	//Store result inside mResult


}//End of evaluate

QueryResult *QueryEvaluator::getResult()
{
	return mResult;
}


vector<vector<int> > *QueryEvaluator::getRel(int type1, int type2, int para1, int para2, int relType)
{
	vector<vector<int> > *evalResult;	
	switch(relType)
	{
	case 5: //Relation is parent
		{
			if(type1 == 201) //the first parameter is a constant stmt#
			{
				int result = mPKBObject->getChild(para1);
				if(result == -1) break;
				vector<int> tmp;
				int t[] = {201,para1, 201,result};
				tmp.insert(tmp.begin(), t, t+4);
				evalResult->push_back(tmp);
			}
			else if(type1 == 51 || type1 == 52)// first para is a stmt var
			{
				vector<int> para1List = mPKBObject->getAllStmts();
				for(vector<int>::iterator i=para1List.begin(); i<para1List.end(); i++){
					int result = mPKBObject->getChild(*i);
					if(result == -1) continue;    //continue this loop without eva this iteration
					if(type2 == 201 && para2 != result) continue;
					vector<int> tmp;
					int t[] = {201,para1,201, result};
					tmp.insert(tmp.begin(), t, t+4);
					evalResult->push_back(tmp);
				}
			}
			else if(type1 == 54)//while var
			{
				vector<int> para1List = mPKBObject->getAllWhile();
				for(vector<int>::iterator i=para1List.begin(); i<para1List.end(); i++){
					int result = mPKBObject->getChild(*i);
					if(result == -1) continue;    //continue this loop without eva this iteration
					if(type2 == 201 && para2 != result) continue;
					vector<int> tmp;
					int t[] = {201,para1, 201,result};
					tmp.insert(tmp.begin(), t, t+4);
					evalResult->push_back(tmp);
				}
			}
			else if(type1 == 55)//if var
			{
				vector<int> para1List = mPKBObject->getAllIf();
				for(vector<int>::iterator i=para1List.begin(); i<para1List.end(); i++){
					int result = mPKBObject->getChild(*i);
					if(result == -1) continue;    //continue this loop without eva this iteration
					if(type2 == 201 && para2 != result) continue;
					vector<int> tmp;
					int t[] = {201, para1, 201,result};
					tmp.insert(tmp.begin(), t, t+4);
					evalResult->push_back(tmp);
				}
			}
			else//Error Case
			{
				throw "Your parent relation has unpaired parameters, para1 type = "+ type1 +"\n";
			}
			break;
		}//Parent END
	case 7: //relation is follows
		{
			if(type1 == 201) //the first parameter is a constant stmt#
			{
				int result = mPKBObject->getFollowingStatement(para1);
				if(result == -1) break;
				if(type2 == 201 && para2 != result) continue;
				vector<int> tmp;
				int t[] = {201,para1,201, result};
				tmp.insert(tmp.begin(), t, t+4);
				evalResult->push_back(tmp);
			}
			else if(type1 == 51 || type1 == 52)// first para is a stmt var
			{
				vector<int> para1List = mPKBObject->getAllStmts();
				for(vector<int>::iterator i=para1List.begin(); i<para1List.end(); i++){
					int result = mPKBObject->getFollowingStatement(*i);
					if(result == -1) continue;    //continue this loop without eva this iteration
					if(type2 == 201 && para2 != result) continue;
					vector<int> tmp;
					int t[] = {201,para1, 201,result};
					tmp.insert(tmp.begin(), t, t+4);
					evalResult->push_back(tmp);
				}
			}
			else if(type1 == 53)//assign var
			{
				vector<int> para1List = mPKBObject->getAllAssign();
				for(vector<int>::iterator i=para1List.begin(); i<para1List.end(); i++){
					int result = mPKBObject->getFollowingStatement(*i);
					if(result == -1) continue;    //continue this loop without eva this iteration
					if(type2 == 201 && para2 != result) continue;
					vector<int> tmp;
					int t[] = {201,para1, 201,result};
					tmp.insert(tmp.begin(), t, t+4);
					evalResult->push_back(tmp);
				}
			}
			else if(type1 == 54)//while var
			{
				vector<int> para1List = mPKBObject->getAllWhile();
				for(vector<int>::iterator i=para1List.begin(); i<para1List.end(); i++){
					int result = mPKBObject->getFollowingStatement(*i);
					if(result == -1) continue;    //continue this loop without eva this iteration
					if(type2 == 201 && para2 != result) continue;
					vector<int> tmp;
					int t[] = {201,para1, 201,result};
					tmp.insert(tmp.begin(), t, t+4);
					evalResult->push_back(tmp);
				}
			}
			else if(type1 == 55)//if var
			{
				vector<int> para1List = mPKBObject->getAllIf();
				for(vector<int>::iterator i=para1List.begin(); i<para1List.end(); i++){
					int result = mPKBObject->getFollowingStatement(*i);
					if(result == -1) continue;    //continue this loop without eva this iteration
					if(type2 == 201 && para2 != result) continue;
					vector<int> tmp;
					int t[] = {201,para1, 201,result};
					tmp.insert(tmp.begin(), t, t+4);
					evalResult->push_back(tmp);
				}
			}
			else if(type1 == 59)//call var
			{
				vector<int> para1List = mPKBObject->getAllCall();
				for(vector<int>::iterator i=para1List.begin(); i<para1List.end(); i++){
					int result = mPKBObject->getFollowingStatement(*i);
					if(result == -1) continue;    //continue this loop without eva this iteration
					if(type2 == 201 && para2 != result) continue;
					vector<int> tmp;
					int t[] = {201,para1,201, result};
					tmp.insert(tmp.begin(), t, t+4);
					evalResult->push_back(tmp);
				}
			}else//Error Case
			{
				throw "Your follows relation has unpaired parameters";
			}
			break;
		}//Follows END
	case 9: //relation is uses
		{
			if(type1 == 201) //the first parameter is a constant stmt#
			{
				int result = mPKBObject->getUsedVar(para1);
				if(result == -1) continue;
				if(type2 == 202 && para2 != result) continue;
				vector<int> tmp;
				int t[] = {201,para1, 202, result};
				tmp.insert(tmp.begin(), t, t+4);
				evalResult->push_back(tmp);
			}
			else if(type1 == 203) //the first parameter is a constant procName
			{
				int result = mPKBObject->getUsedVarPI(para1);
				if(result == -1) continue;
				if(type2 == 202 && para2 != result) continue;
				vector<int> tmp;
				int t[] = {203, para1,202, result};
				tmp.insert(tmp.begin(), t, t+4);
				evalResult->push_back(tmp);
			}
			else if(type1 == 51 || type1 == 52)// first para is a stmt var
			{
				vector<int> para1List = mPKBObject->getAllStmts();
				for(vector<int>::iterator i=para1List.begin(); i<para1List.end(); i++){
					int result = mPKBObject->getUsedVar(*i);
					if(result == -1) continue;    //continue this loop without eva this iteration
					if(type2 == 202 && para2 != result) continue;
					vector<int> tmp;
					int t[] = {201,para1, 202, result};
					tmp.insert(tmp.begin(), t, t+4);
					evalResult->push_back(tmp);
				}
			}
			else if(type1 == 53)//assign var
			{
				vector<int> para1List = mPKBObject->getAllAssign();
				for(vector<int>::iterator i=para1List.begin(); i<para1List.end(); i++){
					int result = mPKBObject->getUsedVar(*i);
					if(result == -1) continue;    //continue this loop without eva this iteration
					if(type2 == 202 && para2 != result) continue;
					vector<int> tmp;
					int t[] = {201,para1, 202, result};
					tmp.insert(tmp.begin(), t, t+4);
					evalResult->push_back(tmp);
				}
			}
			else if(type1 == 54)//while var
			{
				vector<int> para1List = mPKBObject->getAllWhile();
				for(vector<int>::iterator i=para1List.begin(); i<para1List.end(); i++){
					int result = mPKBObject->getUsedVar(*i);
					if(result == -1) continue;    //continue this loop without eva this iteration
					if(type2 == 202 && para2 != result) continue;
					vector<int> tmp;
					int t[] = {201,para1, 202, result};
					tmp.insert(tmp.begin(), t, t+4);
					evalResult->push_back(tmp);
				}
			}
			else if(type1 == 55)//if var
			{
				vector<int> para1List = mPKBObject->getAllIf();
				for(vector<int>::iterator i=para1List.begin(); i<para1List.end(); i++){
					int result = mPKBObject->getUsedVar(*i);
					if(result == -1) continue;    //continue this loop without eva this iteration
					if(type2 == 202 && para2 != result) continue;
					vector<int> tmp;
					int t[] = {201,para1, 202, result};
					tmp.insert(tmp.begin(), t, t+4);
					evalResult->push_back(tmp);
				}
			}
			else if(type1 == 59)//call var
			{
				vector<int> para1List = mPKBObject->getAllCall();
				for(vector<int>::iterator i=para1List.begin(); i<para1List.end(); i++){
					int result = mPKBObject->getUsedVar(*i);
					if(result == -1) continue;    //continue this loop without eva this iteration
					if(type2 == 202 && para2 != result) continue;
					vector<int> tmp;
					int t[] = {201,para1, 202, result};
					tmp.insert(tmp.begin(), t, t+4);
					evalResult->push_back(tmp);
				}
			}
			else if(type1 == 58) //proc var
			{
				vector<int> para1List = mPKBObject->getAllProc();
				for(vector<int>::iterator i=para1List.begin(); i<para1List.end(); i++){
					int result = mPKBObject->getUsedVarPI(*i);
					if(result == -1) continue;    //continue this loop without eva this iteration
					if(type2 == 202 && para2 != result) continue;
					vector<int> tmp;
					int t[] = {203, para1, 202, result};
					tmp.insert(tmp.begin(), t, t+4);
					evalResult->push_back(tmp);
				}
			}
			else//Error Case
			{
				throw "Your uses relation has unpaired parameters" + "\n";
			}
			break;
		}//Uses END
	case 10: //relation is modifies
		{
			if(type1 == 201) //the first parameter is a constant stmt#
			{
				int result = mPKBObject->getModifiedVar(para1);
				if(result == -1) continue;
				if(type2 == 202 && para2 != result) continue;
				vector<int> tmp;
				int t[] = {201,para1, 202, result};
				tmp.insert(tmp.begin(), t, t+4);
				evalResult->push_back(tmp);
			}
			else if(type1 == 203) //the first parameter is a constant procName
			{
				int result = mPKBObject->getModifiedVarPI(para1);
				if(result == -1) continue;
				if(type2 == 202 && para2 != result) continue;
				vector<int> tmp;
				int t[] = {203, para1,202, result};
				tmp.insert(tmp.begin(), t, t+4);
				evalResult->push_back(tmp);
			}
			else if(type1 == 51 || type1 == 52)// first para is a stmt var
			{
				vector<int> para1List = mPKBObject->getAllStmts();
				for(vector<int>::iterator i=para1List.begin(); i<para1List.end(); i++){
					int result = mPKBObject->getModifiedVar(*i);
					if(result == -1) continue;    //continue this loop without eva this iteration
					if(type2 == 202 && para2 != result) continue;
					vector<int> tmp;
					int t[] = {201,para1, 202, result};
					tmp.insert(tmp.begin(), t, t+4);
					evalResult->push_back(tmp);
				}
			}
			else if(type1 == 53)//assign var
			{
				vector<int> para1List = mPKBObject->getAllAssign();
				for(vector<int>::iterator i=para1List.begin(); i<para1List.end(); i++){
					int result = mPKBObject->getModifiedVar(*i);
					if(result == -1) continue;    //continue this loop without eva this iteration
					if(type2 == 202 && para2 != result) continue;
					vector<int> tmp;
					int t[] = {201,para1, 202, result};
					tmp.insert(tmp.begin(), t, t+4);
					evalResult->push_back(tmp);
				}
			}
			else if(type1 == 54)//while var
			{
				vector<int> para1List = mPKBObject->getAllWhile();
				for(vector<int>::iterator i=para1List.begin(); i<para1List.end(); i++){
					int result = mPKBObject->getModifiedVar(*i);
					if(result == -1) continue;    //continue this loop without eva this iteration
					if(type2 == 202 && para2 != result) continue;
					vector<int> tmp;
					int t[] = {201,para1, 202, result};
					tmp.insert(tmp.begin(), t, t+4);
					evalResult->push_back(tmp);
				}
			}
			else if(type1 == 55)//if var
			{
				vector<int> para1List = mPKBObject->getAllIf();
				for(vector<int>::iterator i=para1List.begin(); i<para1List.end(); i++){
					int result = mPKBObject->getModifiedVar(*i);
					if(result == -1) continue;    //continue this loop without eva this iteration
					if(type2 == 202 && para2 != result) continue;
					vector<int> tmp;
					int t[] = {201,para1, 202, result};
					tmp.insert(tmp.begin(), t, t+4);
					evalResult->push_back(tmp);
				}
			}
			else if(type1 == 59)//call var
			{
				vector<int> para1List = mPKBObject->getAllCall();
				for(vector<int>::iterator i=para1List.begin(); i<para1List.end(); i++){
					int result = mPKBObject->getModifiedVar(*i);
					if(result == -1) continue;    //continue this loop without eva this iteration
					if(type2 == 202 && para2 != result) continue;
					vector<int> tmp;
					int t[] = {201,para1, 202, result};
					tmp.insert(tmp.begin(), t, t+4);
					evalResult->push_back(tmp);
				}
			}
			else if(type1 == 58) //proc var
			{
				vector<int> para1List = mPKBObject->getAllProc();
				for(vector<int>::iterator i=para1List.begin(); i<para1List.end(); i++){
					int result = mPKBObject->getModifiedVarPI(*i);
					if(result == -1) continue;    //continue this loop without eva this iteration
					if(type2 == 202 && para2 != result) continue;
					vector<int> tmp;
					int t[] = {203, para1, 202, result};
					tmp.insert(tmp.begin(), t, t+4);
					evalResult->push_back(tmp);
				}
			}
			else//Error Case
			{
				throw "Your modifies relation has unpaired parameters" + "\n";
			}
			break;
		}//Modifies END
	case 11: //relation is calls
		{
			if(type1 == 203) //the first parameter is a constant procName
			{
				int result = mPKBObject->getCalls(para1);
				if(result == -1) continue;
				if(type2 == 203 && para2 != result) continue;
				vector<int> tmp;
				int t[] = {203, para1,203, result};
				tmp.insert(tmp.begin(), t, t+4);
				evalResult->push_back(tmp);
			}
			else if(type1 == 58) //proc var
			{
				vector<int> para1List = mPKBObject->getAllProc();
				for(vector<int>::iterator i=para1List.begin(); i<para1List.end(); i++){
					int result = mPKBObject->getCalls(*i);
					if(result == -1) continue;    //continue this loop without eva this iteration
					if(type2 == 203 && para2 != result) continue;
					vector<int> tmp;
					int t[] = {203, para1, 203, result};
					tmp.insert(tmp.begin(), t, t+4);
					evalResult->push_back(tmp);
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
			throw "No such relation implementation yet: "+relType+ "\n";
		}//Error case END
	}//getRel END

	std::vector<std::vector<int> > *getRelStar(int type1, int type2, int para1, int para2, int relType)
	{
		switch(relType)
		{
		case 6: //parent*
			{
				if(type1 == 201) //the first parameter is a constant stmt#
				{
					throw "Your calls relation has parameter mismatch!";
				}
				else if(type1 == 51 || type1 == 52)// first para is a stmt var
				{
					throw "Your calls relation has parameter mismatch!";
				}
				else if(type1 == 53)//assign var
				{
					throw "Your calls relation has parameter mismatch!";
				}
				else if(type1 == 54)//while var
				{
					throw "Your calls relation has parameter mismatch!";
				}
				else if(type1 == 55)//if var
				{
					throw "Your calls relation has parameter mismatch!";
				}
				else//Error Case
				{
					throw "Your parent* relation has unpaired parameters";
				}
				break;
			}//Parent* END
		case 8: //follows*
			{
				if(type1 == 201) //the first parameter is a constant stmt#
				{
					throw "Your calls relation has parameter mismatch!";
				}
				else if(type1 == 51 || type1 == 52)// first para is a stmt var
				{
					throw "Your calls relation has parameter mismatch!";
				}
				else if(type1 == 53)//assign var
				{
					throw "Your calls relation has parameter mismatch!";
				}
				else if(type1 == 54)//while var
				{
					throw "Your calls relation has parameter mismatch!";
				}
				else if(type1 == 55)//if var
				{
					throw "Your calls relation has parameter mismatch!";
				}
				else//Error Case
				{
					throw "Your follows* relation has unpaired parameters";
				}
				break;
			}//follows* END
		case 12: //calls*
			{
				if(type1 == 203) //the first parameter is a constant procName
				{
					int result = mPKBObject->getCalls(para1);
					if(result == -1) continue;
					if(type2 == 203 && para2 != result) continue;
					vector<int> tmp;
					int t[] = {203, para1,203, result};
					tmp.insert(tmp.begin(), t, t+4);
					evalResult->push_back(tmp);
				}
				else if(type1 == 58) //proc var
				{
					vector<int> para1List = mPKBObject->getAllProc();
					for(vector<int>::iterator i=para1List.begin(); i<para1List.end(); i++){
						int result = mPKBObject->getCalls(*i);
						if(result == -1) continue;    //continue this loop without eva this iteration
						if(type2 == 203 && para2 != result) continue;
						vector<int> tmp;
						int t[] = {203, para1, 203, result};
						tmp.insert(tmp.begin(), t, t+4);
						evalResult->push_back(tmp);
					}
				}
				else//Error Case
				{
					throw "Your calls* relation has unpaired parameters";
				}
				break;
			}
		default:
			throw "The rel* has wrong relation stated!";

		}//switch END


	}//getRelStar END
}

	



