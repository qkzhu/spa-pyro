#include "SymbolTable.h"
//#include <iostream>
using namespace std;


void VarTable::insertVar(string s)
{
	this->insertSymbol(s);
}// end InsertVar

string VarTable::getVarName(int index)
{
	return this->getSymbolName(index);
}// end GetVarName

int VarTable::getVarIndex(string s)
{
	return this->getSymbolIndex(s);
}// end GetVarIndex

int VarTable::getSize()
{
	return this->getSymbolMapSize();
} //end GetVarTableSize

bool VarTable::containsVar(string s)
{
	return this->containsSymbol(s);
}// end ContainsVar

set<string> VarTable::getAllVar()
{
	return this->getAllSymbol();
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