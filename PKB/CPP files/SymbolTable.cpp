#include "SymbolTable.h"
using namespace std;


void SymbolTable::insertSymbol(string s)
{
	if(mSymbolMap.find(s) == mSymbolMap.end())
	{
		mSymbolMap.insert(make_pair(s, SymbolTable::getSymbolMapSize()));
	}	
}// end InsertSymbol

string SymbolTable::getSymbolName(int sIndex)
{
	/*if((int)sIndex >= (int)mSymbolMap.size())
	{
		return ""; // return an empty string
	}
	else*/ 
	//get symbol name
	{
		MapType::const_iterator end = mSymbolMap.end();
		for(MapType::const_iterator it = mSymbolMap.begin(); it != end; it++)
		{
			if(it->second == sIndex)
			{
				return it->first;
			}
		}
	}
	return "Program should not reach here";	
}

int SymbolTable::getSymbolIndex(string s)
{
	MapType::const_iterator iter = mSymbolMap.begin();
	iter = mSymbolMap.find(s);

	if(iter != mSymbolMap.end()) return iter->second;
	return -1; // return an invalid index
}

int SymbolTable::getSymbolMapSize()
{
	return mSymbolMap.size();
}

bool SymbolTable::isNameExist(string s)
{
	return mSymbolMap.find(s) != mSymbolMap.end();
	//MapType::const_iterator iter = mSymbolMap.begin();
	//iter = mSymbolMap.find(s);

	//if(iter != mSymbolMap.end()) return true;
	//else return false;
}

bool SymbolTable::isIndexExist(int varIndex)
{
	string name = getSymbolName(varIndex);
	MapType::const_iterator iter = mSymbolMap.begin();
	iter = mSymbolMap.find(name);

	if(iter != mSymbolMap.end()) return true;
	else return false;
}


int SymbolTable::getAllSymbol() // return only the largest index of symbol
{
	/*
	set<string> symbols;
	MapType::const_iterator end = mSymbolMap.end();
	for(MapType::const_iterator it = mSymbolMap.begin(); it != end; it++)
	{
		symbols.insert(it->first);
	}
	return symbols;
	*/
	return getSymbolMapSize();
}


//For debugging
void SymbolTable::printSymbolTable(string name){
	for(hash_map<string, int>::iterator it = mSymbolMap.begin(); it != mSymbolMap.end(); it++)
	{
		cout<<name<<" name "<<it->first<<" is index# "<<it->second<<endl;;
	}
}//end printSymbolTable