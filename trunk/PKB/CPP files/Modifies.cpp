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
	//update mStmtModMap. Mapping each stmt# to only one modified variable. 
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
		for(hash_map<int, set<int>>::iterator it = mStmtModMap.begin(); it != mStmtModMap.end(); it++)
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
		for(hash_map<int, set<int>>::iterator itPro = mProcModMap.begin(); itPro != mProcModMap.end(); itPro++)
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
		for(hash_map<int, set<int>>::iterator itVar = mVarStmtMap.begin(); itVar != mVarStmtMap.end(); itVar++)
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
		for(hash_map<int, set<int>>::iterator itVar = mVarProcMap.begin(); itVar != mVarProcMap.end(); itVar++)
		{
			cout<<"Variable "<<varTable->getVarName(itVar->first)<<" is modified by procedure: ";
			procSet = itVar->second;
			for(set<int>::iterator itProc = procSet.begin(); itProc != procSet.end(); itProc++)
				cout<<pTable->getProcName(*itProc)<<", ";
			cout<<endl;
		}
	}
}//end printVarModifiedByProc
/*
int main(){
	Modifies M;
	M.setModify(1, 2); //stmt1 modifies varIndex 2
	M.setModify(1, 3); //stmt1 modifies varIndex 3
	cout << "Stmt1 modifies varIndex = ";
	set<int> test1 = M.getModifiedVar(1);
	for(set<int>::iterator it = test1.begin(); it != test1.end(); it++)
	{
		cout << *it << " ";
	}
	cout << endl;
	for (int stmt = 2; stmt <= 12; stmt++)
	{
		M.setModify(stmt, 3);  //stmt2 to stmt12 all modifies varIndex 3
	}
	cout << "varIndex = 3 is modified by stmt#: ";
	set<int> test = M.getStmtModifies(3);
	for(set<int>::iterator it = test.begin(); it != test.end(); it++)
	{
		cout << *it << " ";
	}
	cout << endl;
	cout << "varIndex = 109 is modified by stmt#: ";
	set<int> test5 = M.getStmtModifies(109);	// No stmt# modify varIndex 109
	for(set<int>::iterator it = test5.begin(); it != test5.end(); it++)
	{
		cout << *it << " ";
	}
	cout << endl;

	M.setModifyPV(1,3);  //procIndex 1 modifies varIndex 3
	M.setModifyPV(1,2); //procIndex 1 modifies varIndex 2
	M.setModifyPV(1,33); //procIndex 1 modifies varIndex 2
	cout << "ProcIndex1 modifies varIndex = ";
	set<int> test2 = M.getModifiedVarPI(1);
	for(set<int>::iterator it = test2.begin(); it != test2.end(); it++)
	{
		cout << *it << " ";
	}
	cout << endl;
	for (int proc = 2; proc <= 12; proc++)
	{
		M.setModifyPV(proc, 2);  //procIndex1 to procIndex12 all modifies varIndex 2 becoz procIndex1 already modified varIndex 2!
	}
	cout << "varIndex = 2 is modified by procIndex: ";
	set<int> test3 = M.getProcModifies(2);
	for(set<int>::iterator it = test3.begin(); it != test3.end(); it++)
	{
		cout << *it << " ";
	}
	cout << endl;
	cout << "varIndex = 14 is modified by procIndex: ";
	set<int> test4 = M.getProcModifies(14);		//No procIndex modifies varIndex 14
	for(set<int>::iterator it = test4.begin(); it != test4.end(); it++)
	{
		cout << *it << " ";
	}
	string readin;
	cin >> readin;
}*/