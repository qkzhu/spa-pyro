#include "SymbolTable.h"
using namespace std;


void VarTable::insertVar(string s)
{
	this->insertSymbol(s);
}// end InsertVar

string VarTable::getVarName(int index)
{
	if(isVarIndexExist(index)) return this->getSymbolName(index);
	else return "";
}// end GetVarName

int VarTable::getVarIndex(string s)
{
	if(isVarNameExist(s)) return this->getSymbolIndex(s);
	else return -1;
}// end GetVarIndex

int VarTable::getSize()
{
	return this->getSymbolMapSize();
} //end GetVarTableSize

bool VarTable::isVarNameExist(string varName) {
	return this->isNameExist(varName);
}// end ContainsVar

bool VarTable::isVarIndexExist(int varIndex) {
	return this->isIndexExist(varIndex);
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