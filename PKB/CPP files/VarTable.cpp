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

int VarTable::getAllVar()
{
	return this->getAllSymbol();
}// end GetAllVar

/**********Driver program************
int main()
{	
	cout << "Starting .. .. .. \n\n" << endl;
	VarTable t;
	t.insertVar("first");
	t.insertVar("second");
	cout << "Does mVarMap contains \"second\" ? " << t.isVarNameExist("second") << endl;
	string readin;
	for(int i=0; i< t.getSize() + 1; i++)
	{
		cout << "mVarMap[" << i << "] = " << t.getVarName(i) << endl;
		cout << "Index at \"" << t.getVarName(i) << "\" is " << t.getVarIndex(t.getVarName(i)) << endl;
	}
	cout << t.getAllVar() << endl;
	cin >> readin;
	return 0;
}//end main */

//For debugging
void VarTable::printVarTable()
{
	this->printSymbolTable("Variable");
}//end printVarTable