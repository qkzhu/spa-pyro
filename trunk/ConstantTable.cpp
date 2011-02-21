#include "ConstantTable.h";

void ConstantTable::addConstant(int c)
{
	cSet.insert(c);
}

set<int> ConstantTable::getAllConstants()
{
	return cSet;
}

bool ConstantTable::isConstantExist(int c)
{
	set<int>::iterator it = cSet.find(c);
	if(it != cSet.end()) 
		return true;
	else 
		return false;
}

int ConstantTable::getNumOfConstant()
{
	return cSet.size();
}