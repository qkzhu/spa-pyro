#include "Modifies.h"
//#include <iostream>
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

/******Driver program*******
int main()
{
	Modifies M;
	int stmt = 1;
	for(stmt = 1; stmt <= 5; stmt++)
	{
		if(stmt == 5)
			M.setModify(5, 12);
		else
			M.setModify(stmt, 223); // 5 procedures modified varIndex = 223
		
	}
	M.setModify(1, 2); //error message to occur
	cout << "Stmt1 modifies varIndex = ";
	set<int> test1 = M.getModifiedVar(1);
	for(set<int>::iterator it = test1.begin(); it != test1.end(); it++)
	{
		cout << *it << " ";
	}
	cout << endl;
	cout << "Stmt2 modifies varIndex = " << *M.getModifiedVar(2).begin() << endl;
	cout << "Stmt5 modifies varIndex = " << *M.getModifiedVar(5).begin() << endl;
	cout << "Size of stmts that modifies varIndex = 223 : " << M.getStmtModifies(223).size() << endl;
	cout << "varIndex = 223 is modified by stmt#: ";
	set<int> test = M.getStmtModifies(223);
	for(set<int>::iterator it = test.begin(); it != test.end(); it++)
	{
		cout << *it << " ";
	}
	cout << endl;
	//cout << "VarIndex = 223 is modified by procedures: " << M.getProcModifies(223) << endl;
	string readin;
	cin >> readin;
}*/