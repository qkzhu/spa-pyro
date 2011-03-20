#include "Uses.h"

using namespace std;

set<int> Uses::getUsedVar(int stmtNum)
{
	set<int> varIndexes; // empty set of varIndexes
	if(mStmtUseMap.find(stmtNum) != mStmtUseMap.end())
	{
		return mStmtUseMap[stmtNum];
	}
	varIndexes.insert(-1);
	return varIndexes;  // return -1 if set does not contain any varIndex
}// end getUsedVar

set<int> Uses::getUsedVarPI(int procIndex)
{
	set<int> varIndexes; // empty set of varIndexes
	if(mProcUseMap.find(procIndex) != mProcUseMap.end())
	{
		return mProcUseMap[procIndex];
	}
	varIndexes.insert(-1);
	return varIndexes;  // return -1 if set does not contain any varIndex
}// end getUsedVarPI

set<int> Uses::getStmtUses(int varIndex)
{
	set<int> stmts; // empty set of stmts
	if(mVarStmtMap.find(varIndex) != mVarStmtMap.end())
	{
		return mVarStmtMap[varIndex];
	}
	stmts.insert(-1);
	return stmts; //return -1 if set does not contain any stmt
}// end getStmtUses

set<int> Uses::getProcUses(int varIndex)
{
	set<int> procIndexes; //empty set of procIndexes
	if(mVarProcMap.find(varIndex) != mVarProcMap.end())
	{
		return mVarProcMap[varIndex];
	}
	procIndexes.insert(-1);
	return procIndexes; //return -1 if set does not contain any procIndex
}// end getProcUses

void Uses::setUses(int stmtNum, int varIndex)
{
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

//*********For Debugging************
void Uses::printStmtUseMap(VarTable* varTable){
	if(mStmtUseMap.size() == 0) 
		cout<<"Statement uses Variable table is empty!"<<endl;
	else{
		set<int> varSet;
		for(map<int, set<int>>::iterator it = mStmtUseMap.begin(); it != mStmtUseMap.end(); it++)
		{
			cout<<"Statment Number "<<it->first<<" Uses: ";
			varSet = it->second;

			for(set<int>::iterator itVar = varSet.begin(); itVar != varSet.end(); itVar++)
			{
				cout<<varTable->getVarName(*itVar)<<", ";
			}
			cout<<endl;
		}
	}
}//end printStmtUseMap

void Uses::printProcUseMap(ProcTable* pTable, VarTable* varTable){
	if(mProcUseMap.size() == 0) 
		cout<<"Procedure uses Variable table is empty!"<<endl;
	else{
		set<int> varSet;
		for(map<int, set<int>>::iterator itPro = mProcUseMap.begin(); itPro != mProcUseMap.end(); itPro++)
		{
			cout<<"Procedure "<<pTable->getProcName(itPro->first)<<" uses variable: ";

			varSet = itPro->second;

			for(set<int>::iterator itVar = varSet.begin(); itVar != varSet.end(); itVar++)
			{
				cout<<varTable->getVarName(*itVar)<<", ";
			}
			cout<<endl;
		}
	}
}//end printProcUseMap

void Uses::printVarStmtMap(VarTable* varTable){
	if(mVarStmtMap.size() == 0) 
		cout<<"Variable used by Statement table is empty!"<<endl;
	else{
		set<int> stmtSet;
		for(map<int, set<int>>::iterator itVar = mVarStmtMap.begin(); itVar != mVarStmtMap.end(); itVar++)
		{
			cout<<"Variable "<<varTable->getVarName(itVar->first)<<" is used by statement#: ";
			stmtSet = itVar->second;
			for(set<int>::iterator itStmt = stmtSet.begin(); itStmt != stmtSet.end(); itStmt++)
				cout<<*itStmt<<", ";
			cout<<endl;
		}
	}
}//end printVarStmtMap

void Uses::printVarProcMap(ProcTable* pTable, VarTable* varTable){
	if(mVarProcMap.size() == 0) 
		cout<<"Variable used by Procedure table is empty!"<<endl;
	else{
		set<int> procSet;
		for(map<int, set<int>>::iterator itVar = mVarProcMap.begin(); itVar != mVarProcMap.end(); itVar++)
		{
			cout<<"Variable "<<varTable->getVarName(itVar->first)<<" is used by procedure: ";
			procSet = itVar->second;
			for(set<int>::iterator itProc = procSet.begin(); itProc != procSet.end(); itProc++)
				cout<<pTable->getProcName(*itProc)<<", ";
			cout<<endl;
		}
	}
}//end printVarProcMap
//For Unit Testing
/*
int main(){
	Uses M;
	M.setUses(1, 2); //stmt1 uses varIndex 2
	M.setUses(1, 3); //stmt1 uses varIndex 3
	cout << "Stmt1 Uses varIndex = ";
	set<int> test1 = M.getUsedVar(1);
	for(set<int>::iterator it = test1.begin(); it != test1.end(); it++)
	{
		cout << *it << " ";
	}
	cout << endl;
	for (int stmt = 2; stmt <= 12; stmt++)
	{
		M.setUses(stmt, 3);  //stmt2 to stmt12 all uses varIndex 3
	}
	cout << "varIndex = 3 is used by stmt#: ";
	set<int> test = M.getStmtUses(3);
	for(set<int>::iterator it = test.begin(); it != test.end(); it++)
	{
		cout << *it << " ";
	}
	cout << endl;
	cout << "varIndex = 109 is used by stmt#: ";
	set<int> test5 = M.getStmtUses(109);	// No stmt# uses varIndex 109
	for(set<int>::iterator it = test5.begin(); it != test5.end(); it++)
	{
		cout << *it << " ";
	}
	cout << endl;

	M.setUsesPV(1,3);  //procIndex 1 uses varIndex 3
	M.setUsesPV(1,2); //procIndex 1 uses varIndex 2
	M.setUsesPV(1,33); //procIndex 1 uses varIndex 2
	cout << "ProcIndex1 uses varIndex = ";
	set<int> test2 = M.getUsedVarPI(1);
	for(set<int>::iterator it = test2.begin(); it != test2.end(); it++)
	{
		cout << *it << " ";
	}
	cout << endl;
	for (int proc = 2; proc <= 12; proc++)
	{
		M.setUsesPV(proc, 2);  //procIndex1 to procIndex12 all uses varIndex 2 becoz procIndex1 already used varIndex 2!
	}
	cout << "varIndex = 2 is used by procIndex: ";
	set<int> test3 = M.getProcUses(2);
	for(set<int>::iterator it = test3.begin(); it != test3.end(); it++)
	{
		cout << *it << " ";
	}
	cout << endl;
	cout << "varIndex = 14 is used by procIndex: ";
	set<int> test4 = M.getProcUses(14);		//No procIndex uses varIndex 14
	for(set<int>::iterator it = test4.begin(); it != test4.end(); it++)
	{
		cout << *it << " ";
	}
	string readin;
	cin >> readin;
}*/