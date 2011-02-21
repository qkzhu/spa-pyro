#include "VarTable.h"
//#include <iostream>
using namespace std;


void VarTable::InsertVar(string s)
{
	this->InsertSymbol(s);
}// end InsertVar

string VarTable::GetVarName(int index)
{
	return this->GetSymbolName(index);
}// end GetVarName

int VarTable::GetVarIndex(string s)
{
	return this->GetSymbolIndex(s);
}// end GetVarIndex

int VarTable::GetSize()
{
	return this->GetSymbolMapSize();
} //end GetVarTableSize

bool VarTable::ContainsVar(string s)
{
	return this->ContainsSymbol(s);
}// end ContainsVar

set<string> VarTable::GetAllVar()
{
	return this->GetAllSymbol();
}// end GetAllVar

/**********Driver program************
int main()
{	
	cout << "Starting .. .. .. \n\n" << endl;
	VarTable t;
	t.InsertVar("first");
	t.InsertVar("second");
	cout << "Does mVarMap contains \"second\" ? " << t.ContainsVar("second") << endl;
	string readin;
	for(int i=0; i< t.GetSize() + 1; i++)
	{
		cout << "mVarMap[" << i << "] = " << t.GetVarName(i) << endl;
		cout << "Index at \"" << t.GetVarName(i) << "\" is " << t.GetVarIndex(t.GetVarName(i)) << endl;
	}
	cin >> readin;
	return 0;
}//end main */