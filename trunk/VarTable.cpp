#include "VarTable.h"
//#include <iostream>
using namespace std;


void VarTable::insertVar(string var)
{
	this->insertSymbol(var);
}// end InsertVar

string VarTable::getVarName(int index)
{
	return this->getSymbolName(index);
}// end GetVarName

int VarTable::getVarIndex(string var)
{
	return this->getSymbolIndex(var);
}// end GetVarIndex

int VarTable::getSize()
{
	return this->getSymbolMapSize();
} //end GetVarTableSize

bool VarTable::containsVar(string var)
{
	return this->containsSymbol(var);
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