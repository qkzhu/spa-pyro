#include <string>
#include <map>
#include <set>
#include "VarTable.h"

using namespace std;

VarTable::VarTable() {}

//Accessors
int VarTable::getVarSize()
{
	return var_map.size();
}

set<int> VarTable::getAllStmt()
{
	set<int> output;

	for (map<string, set<int>>::iterator it = var_map.begin(); it != var_map.end(); it++)
		for (set<int>::iterator it1 = it->second.begin(); it1 != it->second.end(); it1++)
			output.insert(*it1);

	return output;
}

set<int> VarTable::getAllStmt(string v)
{
	return var_map[v];
}

bool VarTable::containsVar(string v)
{
	return var_map.find(v) != var_map.end();
}
//
////Mutators
void VarTable::insertVar(string var_name, int statNum)
{
	//updates var_name -> statNum mapping
	if (var_map.find(var_name) == var_map.end())
	{
		set<int> store;
		store.insert(statNum);
		var_map[var_name] = store;
	}
	else
		var_map[var_name].insert(statNum);

	//updates statNum -> var_name
	if (stat_map.find(statNum) == stat_map.end())
	{
		set<string> store;
		store.insert(var_name);
		stat_map[statNum] = store;
	}
	else
		stat_map[statNum].insert(var_name);
}

set<string> VarTable::getAllVar(int statNum)
{
	return stat_map[statNum];
}