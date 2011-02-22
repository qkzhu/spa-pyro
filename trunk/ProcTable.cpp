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
