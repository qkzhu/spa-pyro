#include "SymbolTable.h"
using namespace std;


void ProcTable::insertProc(string s)
{
	this->insertSymbol(s);
}// end InsertProc

string ProcTable::getProcName(int index)
{
	if(isProcIndexExist(index)) return this->getSymbolName(index);
	else return "";
}// end GetProcName

int ProcTable::getProcIndex(string s)
{
	if(isProcNameExist(s)) return this->getSymbolIndex(s);
	else return -1;
}// end GetProcIndex

int ProcTable::getSize()
{
	return this->getSymbolMapSize();
}// end getProcTableSize

bool ProcTable::isProcNameExist(string procName)
{
	return this->isNameExist(procName);
}// end ContainsProc

bool ProcTable::isProcIndexExist(int procIndex)
{
	return this->isIndexExist(procIndex);
}// end ContainsProc

int ProcTable::getAllProc()
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

	addCalled(procIndex2, procIndex1);
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
void ProcTable::getCall(int procIndex, vector<int>& result){

	set<int> tmpSet;

	/* verify procIndex */
	if(procIndex < 0 || !isProcIndexExist(procIndex))
		result.push_back(-1);

	/* Check if procIndex exists in exists */
	if(callTable.find(procIndex) != callTable.end())	//procIndex exists
	{
		tmpSet = callTable[procIndex];
		/* In case procIndex calls no procdure */
		if(tmpSet.size() <= 0)
			result.push_back(-1);

		/* copy element from set<int> to list<int> */
		for(set<int>::iterator it = tmpSet.begin(); it != tmpSet.end(); it++)
			result.push_back(*it);
	}
	else	//procIndex does not exist
	{
		result.push_back(-1);
	}
}//end getCall


/* get all procedures who call procIndex */
void ProcTable::getCalled(int procIndex, vector<int>& result){

	set<int> tmpSet;

	/* verify procIndex */
	if(procIndex < 0 || !isProcIndexExist(procIndex))
		result.push_back(-1);

	/* Check if procIndex exists in exists */
	if(calledTable.find(procIndex) != calledTable.end())	//procIndex exists
	{
		tmpSet = calledTable[procIndex];
		/* In case procIndex was called by no procdure */
		if(tmpSet.size() <= 0)
			result.push_back(-1);

		/* copy element from set<int> to list<int> */
		for(set<int>::iterator it = tmpSet.begin(); it != tmpSet.end(); it++)
			result.push_back(*it);
	}
	else	//procIndex does not exist
	{
		result.push_back(-1);
	}

}//end getCalled

/* get all procedures called recursively by procIndex */
void ProcTable::getCall_(int procIndex, vector<int>& result){
	
	if (call_Table.find(procIndex) != call_Table.end()) {
		for(vector<int>::iterator it = call_Table[procIndex].begin(); it != call_Table[procIndex].end(); it++)
			result.push_back(*it);
	}

	if (!isIndexExist(procIndex)) 
		result.push_back(-1);

	set<int> visited;
	queue<int> curr_queue;

	vector<int> next_queue; 
	getCall(procIndex, next_queue);

	//minimum spanning tree
	for (unsigned int i = 0; i < next_queue.size(); i++)
		curr_queue.push(next_queue[i]);

	while (!curr_queue.empty())
	{
		int curr_proc = curr_queue.front();
		curr_queue.pop();

		if (visited.find(curr_proc) == visited.end())
		{
			visited.insert(curr_proc);
			vector<int> next_queue;
			getCall(curr_proc, next_queue);

			//ensures that -1 does not get creeped into result.
			if (next_queue.size() == 1 && next_queue[0] == -1)
				continue;

			for (unsigned int i = 0; i < next_queue.size(); i++)
				curr_queue.push(next_queue[i]);
		}
	}

	for(set<int>::iterator it = visited.begin(); it != visited.end(); it++)
		result.push_back(*it);

	call_Table[procIndex] = result;
}

/* get all procedures called recursively by procIndex */
void ProcTable::getCalled_(int procIndex, vector<int>& result){
	
	if (called_Table.find(procIndex) != called_Table.end())
		for(vector<int>::iterator it = called_Table[procIndex].begin(); it != called_Table[procIndex].end(); it++)
			result.push_back(*it);

	if (!isIndexExist(procIndex))
		result.push_back(-1);

	set<int> visited;
	queue<int> curr_queue;

	//minimum spanning tree
	vector<int> next_queue;
	getCalled(procIndex, next_queue);

	for (unsigned int i = 0; i < next_queue.size(); i++)
		curr_queue.push(next_queue[i]);

	while (!curr_queue.empty())
	{
		int curr_proc = curr_queue.front();
		curr_queue.pop();

		if (visited.find(curr_proc) == visited.end())
		{
			visited.insert(curr_proc);
			vector<int> next_queue;
			getCalled(curr_proc, next_queue);

			//ensures that -1 does not get creeped into result.
			if (next_queue.size() == 1 && next_queue[0] == -1)
				continue;

			for (unsigned int i = 0; i < next_queue.size(); i++)
				curr_queue.push(next_queue[i]);
		}
	}

	for(set<int>::iterator it = visited.begin(); it != visited.end(); it++)
		result.push_back(*it);

	called_Table[procIndex] = result;
}

//For Debugging
void ProcTable::printProcTable()
{
	this->printSymbolTable("Procedure");
}

void ProcTable::printCallTable(){
	if(callTable.size() == 0) 
		cout<<"CallTable table is empty!"<<endl;
	else{
		set<int> proc2Set;
		for(map<int, set<int>>::iterator itProc1 = callTable.begin(); itProc1 != callTable.end(); itProc1++)
		{
			cout<<"Procedure "<<this->getProcName(itProc1->first)<<" calls procedure: ";
			proc2Set = itProc1->second;
			for(set<int>::iterator itProc2 = proc2Set.begin(); itProc2 != proc2Set.end(); itProc2++)
				cout<<this->getProcName(*itProc2)<<", ";
			cout<<endl;
		}
	}
}//end printCallTable

void ProcTable::printCalledTable(){
	if(calledTable.size() == 0) 
		cout<<"CalledTable table is empty!"<<endl;
	else{
		set<int> proc2Set;
		for(map<int, set<int>>::iterator itProc1 = calledTable.begin(); itProc1 != calledTable.end(); itProc1++)
		{
			cout<<"Procedure "<<this->getProcName(itProc1->first)<<" is called by procedure: ";
			proc2Set = itProc1->second;
			for(set<int>::iterator itProc2 = proc2Set.begin(); itProc2 != proc2Set.end(); itProc2++)
				cout<<this->getProcName(*itProc2)<<", ";
			cout<<endl;
		}
	}
}//end printCalledTable

void ProcTable::printCall_Table(){
	//ensures that called* is applied on all procedures
	int maxProc = this->getAllProc();
	vector<int> tmp;
	for (int i = 0; i <= maxProc; i ++)
		getCall_(i, tmp);

	if(call_Table.size() == 0) 
		cout<<"Call*Table table is empty!"<<endl;
	else{
		vector<int> proc2Vec;
		for(map<int, vector<int>>::iterator itProc1 = call_Table.begin(); itProc1 != call_Table.end(); itProc1++)
		{
			if (itProc1->second.size() == 1 && itProc1->second[0] == -1)
			{
				cout<<"Procedure "<<this->getProcName(itProc1->first)<<" does not call any procedures. " << endl;
				continue;
			}
			cout<<"Procedure "<<this->getProcName(itProc1->first)<<" recursively calls procedure(s): ";
			proc2Vec = itProc1->second;
			for(unsigned int i = 0; i < proc2Vec.size(); i++)
				cout<<this->getProcName(proc2Vec[i])<<", ";
			cout<<endl;
		}
	}
}//end printCall_Table

void ProcTable::printCalled_Table(){
	//ensures that called* is applied on all procedures
	int maxProc = this->getAllProc();
	vector<int> tmp;
	for (int i = 0; i <= maxProc; i ++)
		getCalled_(i, tmp);

	if(called_Table.size() == 0) 
		cout<<"Called*Table table is empty!"<<endl;
	else{
		vector<int> proc2Vec;
		for(map<int, vector<int>>::iterator itProc1 = called_Table.begin(); itProc1 != called_Table.end(); itProc1++)
		{
			if (itProc1->second.size() == 1 && itProc1->second[0] == -1)
			{
				cout<<"Procedure "<<this->getProcName(itProc1->first)<<" is not recursively called by any procedures. " << endl;
				continue;
			}
			cout<<"Procedure "<<this->getProcName(itProc1->first)<<" is called recursively by procedure(s): ";
			proc2Vec = itProc1->second;
			for(unsigned int i = 0; i < proc2Vec.size(); i++)
				cout<<this->getProcName(proc2Vec[i])<<", ";
			cout<<endl;
		}
	}
}//end printCall_Table

//For Unit testing
/*
int main()
{
	ProcTable p;
	p.insertProc("dasf");
	p.insertProc("hi");
	p.insertProc("asfd");
	p.insertProc("x");
	p.insertProc("d");
	p.addCall(1,2); //procIndex1 call procIndex2
	p.addCall(1,3); //procIndex1 call procIndex3
	p.addCall(1,4); //procIndex1 call procIndex4
	vector<int> v = p.getCall(1);
	for(int i=0; i<v.size(); i++)
	{
		cout << v.at(i) << " ";
	}
	cout << endl;
	char s;
	cin >> s;
}*/