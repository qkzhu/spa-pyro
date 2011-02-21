#include "ProcTable.h"
using namespace std;


void ProcTable::insertProc(string procName)
{
	this->insertSymbol(procName);
}// end InsertProc

string ProcTable::getProcName(int index)
{
	return this->getSymbolName(index);
}// end GetProcName

int ProcTable::getProcIndex(string procName)
{
	return this->getSymbolIndex(procName);
}// end GetProcIndex

int ProcTable::getSize()
{
	return this->getSymbolMapSize();
}// end getProcTableSize

bool ProcTable::containsProc(string procName)
{
	return this->containsSymbol(procName);
}// end ContainsProc

set<string> ProcTable::getAllProc()
{
	return this->getAllSymbol();
}// end GetAllProc
