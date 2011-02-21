#include "ProcTable.h"
using namespace std;


void ProcTable::InsertProc(string s)
{
	this->InsertSymbol(s);
}// end InsertProc

string ProcTable::GetProcName(int index)
{
	return this->GetSymbolName(index);
}// end GetProcName

int ProcTable::GetProcIndex(string s)
{
	return this->GetSymbolIndex(s);
}// end GetProcIndex

int ProcTable::GetSize()
{
	return this->GetSymbolMapSize();
}// end getProcTableSize

bool ProcTable::ContainsProc(string s)
{
	return this->ContainsSymbol(s);
}// end ContainsProc

set<string> ProcTable::GetAllProc()
{
	return this->GetAllSymbol();
}// end GetAllProc
