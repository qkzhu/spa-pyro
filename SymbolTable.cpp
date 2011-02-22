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
	if((int)sIndex >= (int)mSymbolMap.size())
	{
		return "There is no nothing stored at the given index";
	}
	else
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
	if(iter != mSymbolMap.end())
	{
		return iter->second;
	}
	return -1; // return an invalid index
}

int SymbolTable::getSymbolMapSize()
{
	return mSymbolMap.size();
}

bool SymbolTable::containsSymbol(string s)
{
	MapType::const_iterator iter = mSymbolMap.begin();
	iter = mSymbolMap.find(s);
	if(iter != mSymbolMap.end())
	{
		return true;
	}
	return false;
}

set<string> SymbolTable::getAllSymbol()
{
	set<string> symbols;
	MapType::const_iterator end = mSymbolMap.end();
	for(MapType::const_iterator it = mSymbolMap.begin(); it != end; it++)
	{
		symbols.insert(it->first);
	}
	return symbols;
}