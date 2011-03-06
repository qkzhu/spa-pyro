#include "ConstantTable.h"
#include <iostream>

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

// For debugging
void ConstantTable::printConstantTable(){
	if(cSet.size() == 0) cout<<"The Constant Table is empty!"<<endl;
	else {
		int i = 0;
		cout<<"***** Constant Table *****"<<endl;
		for(set<int>::iterator it = cSet.begin(); it != cSet.end(); it++)
		{
			cout<<"Index: "<<(i++)<<",  Constant Value: "<<*it<<endl;
		}
		cout<<"***** End Constant Table *****"<<endl;
	}
}
