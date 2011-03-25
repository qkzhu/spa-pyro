#ifndef MODIFIES_H
#define MODIFIES_H

#include <set>
#include <hash_map>
#include <iostream>			//For Debugging
#include "SymbolTable.h"	//For Debugging
using namespace std;

class Modifies
{
public:
	set<int> getModifiedVar(int);	//takes in a statNum; return varIndex
	set<int> getModifiedVarPI(int); // takes in a procIndex; return varIndex
	set<int> getStmtModifies(int);	// takes in a varIndex
	set<int> getProcModifies(int);	// takes in a varIndex; return procIndex
	void setModify(int stmtNum, int varIndex);
	void setModifyPV(int procIndex, int varIndex);

	//For debugging
	void printStmtModVar(VarTable* varTable);							// map each stmt# to a set of modified varIndex
	void printProcModVar(ProcTable* pTable, VarTable* varTable);		// map each ProcIndex to a set of modified varIndex
	void printVarModifiedByStmt(VarTable* varTable);					// map each modified varIndex to a set of stmt#
	void printVarModifiedByProc(ProcTable* pTable, VarTable* varTable);	// map each modified varIndex to a set of procIndex

private:
	hash_map<int, set<int>> mStmtModMap; // map each stmt# to a set of modified varIndex
	hash_map<int, set<int>> mProcModMap; // map each ProcIndex to a set of modified varIndex
	hash_map<int, set<int>> mVarStmtMap; // map each modified varIndex to a set of stmt#
	hash_map<int, set<int>> mVarProcMap; // map each modified varIndex to a set of procIndex
};

#endif