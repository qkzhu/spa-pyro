#include "Modifies.h"
#include <iostream>
using namespace std;

set<int> Modifies::getModifiedVar(int stmtNum)
{
	set<int> varIndexes; // empty set of varIndexes
	if(mStmtModMap.find(stmtNum) != mStmtModMap.end())
		return mStmtModMap[stmtNum];
	return varIndexes; // return empty set of varIndexes if the enquired stmtNum has no modified variable
}// end getModifiedVar

set<int> Modifies::getModifiedVarPI(int procIndex)
{
	set<int> varIndexes; // empty set of varIndexes
	if(mProcModMap.find(procIndex) != mProcModMap.end())
	{
		return mProcModMap[procIndex];
	}
	return varIndexes; // return empty set of varIndexes if the enquired procIndex has no modified variable
}// end getModifiedVarPI

set<int> Modifies::getStmtModifies(int varIndex)
{
	set<int> stmts;
	if(mVarStmtMap.find(varIndex) != mVarStmtMap.end())
	{
		return mVarStmtMap[varIndex];
	}
	return stmts; // return empty set of stmst if the enquired varIndex has no stmts modifying it.
}// end getStmtModifies

set<int> Modifies::getProcModifies(int varIndex)
{
	set<int> procIndexes;
	if(mVarProcMap.find(varIndex) != mVarProcMap.end())
	{
		return mVarProcMap[varIndex];
	}
	return procIndexes; // return empty set of procIndexes if the enquired varIndex has no procIndex modiftying it
}// end getProcModifies

void Modifies::setModify(int stmtNum, int varIndex)
{
	//update mStmtModMap. Mapping each stmt# to modified variable. 1 stmt can only modify 1 variable
	set<int> varIndexes;
	mStmtModMap[stmtNum] = varIndexes;
	mStmtModMap[stmtNum].insert(varIndex);

	//update mVarStmtMap. Mapping each variable to stmt# that modify the variable
	set<int> stmts;
	if(mVarStmtMap.find(varIndex) != mVarStmtMap.end()) // varIndex does not point to any set
	{
		mVarStmtMap[varIndex] = stmts;
		mVarStmtMap[varIndex].insert(stmtNum);
	}
	else
	{
		mVarStmtMap[varIndex].insert(stmtNum);
	}
}// end setModify

void Modifies::setModifyPV(int procIndex, int varIndex)
{
	//update mProcModMap. Mapping each procedure to modified variables
	set<int> varIndexes;
	if(mProcModMap.find(procIndex) != mProcModMap.end()) // procIndex does not point to any set
	{
		mProcModMap[procIndex] = varIndexes;
		mProcModMap[procIndex].insert(varIndex);
	}
	else
	{
		mProcModMap[procIndex].insert(varIndex);
	}

	//update mVarProcMap. Mapping each variables to a set of procedures that modifies it.
	set<int> procIndexes;
	if(mVarProcMap.find(varIndex) != mVarProcMap.end())
	{
		mVarProcMap[varIndex] = procIndexes;
		mVarProcMap[varIndex].insert(procIndex);
	}
	else
	{
		mVarProcMap[varIndex].insert(procIndex);
	}
}// end setModifyPV

/******Driver program*******/
int main()
{
	Modifies M;
	int procIndex = 1;
	for(procIndex = 1; procIndex <= 12; procIndex++)
	{
		M.setModifyPV(procIndex, 223); // 10 procedures modified varIndex = 223
	}

	cout << "Procedure 1 modifies varIndex = " << *M.getModifiedVarPI(1).begin() << endl;
	cout << "Procedure 7 modifies varIndex = " << *M.getModifiedVarPI(7).begin() << endl;
	cout << "varIndex = 223 is modified by procedures: " << *M.getProcModifies(223).begin();
/*	for(set<int>::iterator it = M.getProcModifies(223).begin(); it != M.getProcModifies(223).end(); it++)
	{
		cout << *it << " ";
	}
	cout << endl;*/
	//cout << "VarIndex = 223 is modified by procedures: " << M.getProcModifies(223) << endl;
	string readin;
	cin >> readin;
}