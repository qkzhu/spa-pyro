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
	//update mStmtModMap. Mapping each stmt# to modified variable. 1 stmt can only modify 1 variable. Once set, cant overwrite
	if(mStmtModMap[stmtNum].size() == 1)
	{
		//To terminate program
	}
	else
	{
		mStmtModMap[stmtNum].insert(varIndex);
	}

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
