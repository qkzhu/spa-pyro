#ifndef CONSTANTTABLE_H
#define CONSTANTTABLE_H

#include <set>
#include <iostream>	//For Debugging

using namespace std;

class ConstantTable
{
public:
	void addConstant(int c);
	set<int> getAllConstants();
	bool isConstantExist(int c);
	int getNumOfConstant();

	// For debugging use
	void printConstantTable();

private:
	set<int> cSet;
};

#endif