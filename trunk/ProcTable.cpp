#include "SymbolTable.h"
using namespace std;


void ProcTable::insertProc(string s)
{
	this->insertSymbol(s);
}// end InsertProc

string ProcTable::getProcName(int index)
{
	return this->getSymbolName(index);
}// end GetProcName

int ProcTable::getProcIndex(string s)
{
	return this->getSymbolIndex(s);
}// end GetProcIndex

int ProcTable::getSize()
{
	return this->getSymbolMapSize();
}// end getProcTableSize

bool ProcTable::containsProc(string s)
{
	return this->containsSymbol(s);
}// end ContainsProc

set<string> ProcTable::getAllProc()
{
	return this->getAllSymbol();
}// end GetAllProc


/**
 * For call tables
 */

/* procIndex1 calls procIndex2 */
void ProcTable::addCall(int procIndex1, int procIndex2) {
	set<int> tmp;

	/* check if the procIndex1 has already existed in callTable */
	if(callTable.find(procIndex1) != callTable.end())	//procIndex1 exists
	{
		tmp = callTable[procIndex1];
		tmp.insert(procIndex2);
		callTable[procIndex1] = tmp;
	}
	else	//procIndex1 does not exist
	{
		tmp.insert(procIndex2);
		callTable.insert(pair<int, set<int>>(procIndex1, tmp));
	}

	addCalled(procIndex1, procIndex2);
}//end addCall(int procIndex1, int procIndex2)


/* procIndex1 calls procList */
void ProcTable::addCall(int procIndex1, vector<int> procList){
	set<int> tmp;
	int indexTmp;

	/* check if the procIndex1 has already existed in callTable */
	if(callTable.find(procIndex1) != callTable.end())	//procIndex1 exists
	{
		tmp = callTable[procIndex1];
		for(indexTmp = 0; indexTmp < (int)procList.size(); indexTmp++)
			tmp.insert(procList[indexTmp]);
		callTable[procIndex1] = tmp;
	}
	else	//procIndex1 does not exist
	{
		/* copy element from list<int> to set<int> */
		for(indexTmp = 0; indexTmp < (int)procList.size(); indexTmp++)
			tmp.insert(procList[indexTmp]);
		callTable.insert(pair<int, set<int>>(procIndex1, tmp));
	}
}//end addCall(int procIndex1, vector<int> procList)


/* procIndex1 was called by procIndex2 */
void ProcTable::addCalled(int procIndex1, int procIndex2){
	set<int> tmp;

	/* check if the procIndex1 has already existed in calledTable */
	if(calledTable.find(procIndex1) != calledTable.end())	//procIndex1 exists
	{
		tmp = calledTable[procIndex1];
		tmp.insert(procIndex2);
		calledTable[procIndex1] = tmp;
	}
	else	//procIndex1 does not exist
	{
		tmp.insert(procIndex2);
		calledTable.insert(pair<int, set<int>>(procIndex1, tmp));
	}
}

/* procIndex1 was called procList */
void ProcTable::addCalled(int procIndex1, vector<int> procList){
	set<int> tmp;
	int indexTmp;

	/* check if the procIndex1 has already existed in calledTable */
	if(calledTable.find(procIndex1) != calledTable.end())	//procIndex1 exists
	{
		tmp = calledTable[procIndex1];
		for(indexTmp = 0; indexTmp < (int)procList.size(); indexTmp++)
			tmp.insert(procList[indexTmp]);
		calledTable[procIndex1] = tmp;
	}
	else	//procIndex1 does not exist
	{
		/* copy element from list<int> to set<int> */
		for(indexTmp = 0; indexTmp < (int)procList.size(); indexTmp++)
			tmp.insert(procList[indexTmp]);
		calledTable.insert(pair<int, set<int>>(procIndex1, tmp));
	}
}//end addCalled(int procIndex1, vector<int> procList)


/* get all procedures called by procIndex */
vector<int> ProcTable::getCall(int procIndex){
	vector<int> tmpVec;
	set<int>	tmpSet;

	/* verify procIndex */
	if(procIndex <= 0){
		tmpVec.push_back(-1);
		return tmpVec;
	}

	/* Check if procIndex exists in exists */
	if(callTable.find(procIndex) != callTable.end())	//procIndex exists
	{
		tmpSet = callTable[procIndex];
		/* In case procIndex calls no procdure */
		if(tmpSet.size() <= 0){
			tmpVec.push_back(-1);
			return tmpVec;
		}

		/* copy element from set<int> to list<int> */
		for(set<int>::iterator it = tmpSet.begin(); it != tmpSet.end(); it++)
			tmpVec.push_back(*it);
	}
	else	//procIndex does not exist
	{
		tmpVec.push_back(-1);
	}

	return tmpVec;
}//end getCall


/* get all procedures who call procIndex */
vector<int> ProcTable::getCalled(int procIndex){
	vector<int> tmpVec;
	set<int>	tmpSet;

	/* verify procIndex */
	if(procIndex <= 0){
		tmpVec.push_back(-1);
		return tmpVec;
	}

	/* Check if procIndex exists in exists */
	if(calledTable.find(procIndex) != calledTable.end())	//procIndex exists
	{
		tmpSet = calledTable[procIndex];
		/* In case procIndex was called by no procdure */
		if(tmpSet.size() <= 0){
			tmpVec.push_back(-1);
			return tmpVec;
		}

		/* copy element from set<int> to list<int> */
		for(set<int>::iterator it = tmpSet.begin(); it != tmpSet.end(); it++)
			tmpVec.push_back(*it);
	}
	else	//procIndex does not exist
	{
		tmpVec.push_back(-1);
	}

	return tmpVec;
}//end getCalled