#include "Uses.h"
using namespace std;

set<int> Uses::getUsedVar(int stmtNum)
{
	set<int> varIndexes; // empty set of varIndexes
	if(mStmtUseMap.find(stmtNum) != mStmtUseMap.end())
	{
		return mStmtUseMap[stmtNum];
	}
	return varIndexes;
}// end getUsedVar

set<int> Uses::getUsedVarPI(int procIndex)
{
	set<int> varIndexes; // empty set of varIndexes
	if(mProcUseMap.find(procIndex) != mProcUseMap.end())
	{
		return mProcUseMap[procIndex];
	}
	return varIndexes;
}// end getUsedVarPI

set<int> Uses::getStmtUses(int varIndex)
{
	set<int> stmts;
	if(mVarStmtMap.find(varIndex) != mVarStmtMap.end())
	{
		return mVarStmtMap[varIndex];
	}
	return stmts; 
}// end getStmtUses

set<int> Uses::getProcUses(int varIndex)
{
	set<int> procIndexes;
	if(mVarProcMap.find(varIndex) != mVarProcMap.end())
	{
		return mVarProcMap[varIndex];
	}
	return procIndexes;
}// end getProcUses

void Uses::setUses(int stmtNum, int varIndex)
{
	//wrong!!
	//update mStmtUseMap. Mapping each stmt# to used variable.
	mStmtUseMap[stmtNum].insert(varIndex);

	//update mVarStmtMap. Mapping each variable to stmt# that uses the variable
	mVarStmtMap[varIndex].insert(stmtNum);
}// end setUses

void Uses::setUsesPV(int procIndex, int varIndex)
{
	//update mProcUseMap. Mapping each procedure to used variables
	mProcUseMap[procIndex].insert(varIndex);

	//update mVarProcMap. Mapping each variables to a set of procedures that uses it.
	mVarProcMap[varIndex].insert(procIndex);
}// end setUsesPV