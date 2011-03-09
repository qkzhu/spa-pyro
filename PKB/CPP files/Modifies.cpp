#include "Modifies.h"
using namespace std;

set<int> Modifies::getModifiedVar(int stmtNum)
{
	set<int> varIndexes; // empty set of varIndexes
	if(mStmtModMap.find(stmtNum) != mStmtModMap.end())
		return mStmtModMap[stmtNum];
	varIndexes.insert(-1);
	return varIndexes; //return -1 if set does not contain any varIndex
}// end getModifiedVar

set<int> Modifies::getModifiedVarPI(int procIndex)
{
	set<int> varIndexes; // empty set of varIndexes
	if(mProcModMap.find(procIndex) != mProcModMap.end())
	{
		return mProcModMap[procIndex];
	}
	varIndexes.insert(-1);
	return varIndexes; //return -1 if set does not contain any varIndex
}// end getModifiedVarPI

set<int> Modifies::getStmtModifies(int varIndex)
{
	set<int> stmts;
	if(mVarStmtMap.find(varIndex) != mVarStmtMap.end())
	{
		return mVarStmtMap[varIndex];
	}
	stmts.insert(-1);
	return stmts; //return -1 if set does not contain any stmts
}// end getStmtModifies

set<int> Modifies::getProcModifies(int varIndex)
{
	set<int> procIndexes;
	if(mVarProcMap.find(varIndex) != mVarProcMap.end())
	{
		return mVarProcMap[varIndex];
	}
	procIndexes.insert(-1);
	return procIndexes; //return -1 if set does not contain any procIndex
}// end getProcModifies

void Modifies::setModify(int stmtNum, int varIndex)
{
	//update mStmtModMap. Mapping each stmt# to modified variable. 
	//can modify multiple variables if stmt is a container.
	mStmtModMap[stmtNum].insert(varIndex);

	//update mVarStmtMap. Mapping each variable to stmt# that modify the variable
	mVarStmtMap[varIndex].insert(stmtNum);
}// end setModify

void Modifies::setModifyPV(int procIndex, int varIndex)
{
	//update mProcModMap. Mapping each procedure to modified variables
	mProcModMap[procIndex].insert(varIndex);

	//update mVarProcMap. Mapping each variables to a set of procedures that modifies it.
	mVarProcMap[varIndex].insert(procIndex);
}// end setModifyPV


//For Debugging
void Modifies::printStmtModVar(VarTable* varTable){
	if(mStmtModMap.size() == 0) 
		cout<<"Statement modifies Variable table is empty!"<<endl;
	else{
		set<int> varSet;
		for(map<int, set<int>>::iterator it = mStmtModMap.begin(); it != mStmtModMap.end(); it++)
		{
			cout<<"Statment Number "<<it->first<<" Modifies: ";
			varSet = it->second;

			for(set<int>::iterator itVar = varSet.begin(); itVar != varSet.end(); itVar++)
			{
				cout<<varTable->getVarName(*itVar) << ", ";
			}
			cout<<endl;
		}
	}
}//end printStmtModVar

void Modifies::printProcModVar(ProcTable* pTable, VarTable* varTable){
	if(mProcModMap.size() == 0) 
		cout<<"Procedure modifies Variable table is empty!"<<endl;
	else{
		set<int> varSet;
		for(map<int, set<int>>::iterator itPro = mProcModMap.begin(); itPro != mProcModMap.end(); itPro++)
		{
			cout<<"Procedure "<<pTable->getProcName(itPro->first)<<" modifies variable: ";

			varSet = itPro->second;

			for(set<int>::iterator itVar = varSet.begin(); itVar != varSet.end(); itVar++)
			{
				cout<<varTable->getVarName(*itVar)<<", ";
			}
			cout<<endl;
		}
	}
}//end printProcModVar

void Modifies::printVarModifiedByStmt(VarTable* varTable){
	if(mVarStmtMap.size() == 0) 
		cout<<"Variable modified by Statement table is empty!"<<endl;
	else{
		set<int> stmtSet;
		for(map<int, set<int>>::iterator itVar = mVarStmtMap.begin(); itVar != mVarStmtMap.end(); itVar++)
		{
			cout<<"Variable "<<varTable->getVarName(itVar->first)<<" is modified in statement#: ";
			stmtSet = itVar->second;
			for(set<int>::iterator itStmt = stmtSet.begin(); itStmt != stmtSet.end(); itStmt++)
				cout<<*itStmt<<", ";
			cout<<endl;
		}
	}
}//end printVarModifiedByStmt

void Modifies::printVarModifiedByProc(ProcTable* pTable, VarTable* varTable){
	if(mVarProcMap.size() == 0) 
		cout<<"Variable modified by Procedure table is empty!"<<endl;
	else{
		set<int> procSet;
		for(map<int, set<int>>::iterator itVar = mVarProcMap.begin(); itVar != mVarProcMap.end(); itVar++)
		{
			cout<<"Variable "<<varTable->getVarName(itVar->first)<<" is modified by procedure: ";
			procSet = itVar->second;
			for(set<int>::iterator itProc = procSet.begin(); itProc != procSet.end(); itProc++)
				cout<<pTable->getProcName(*itProc)<<", ";
			cout<<endl;
		}
	}
}//end printVarModifiedByProc