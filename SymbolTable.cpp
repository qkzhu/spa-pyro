#include "SymbolTable.h"
using namespace std;


void SymbolTable::InsertSymbol(string s)
{
	if(mSymbolMap.find(s) == mSymbolMap.end())
	{
		mSymbolMap.insert(make_pair(s, SymbolTable::GetSymbolMapSize()));
	}	
}// end InsertSymbol

string SymbolTable::GetSymbolName(int sIndex)
{
	if(sIndex >= mSymbolMap.size())
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

int SymbolTable::GetSymbolIndex(string s)
{
	MapType::const_iterator iter = mSymbolMap.begin();
	iter = mSymbolMap.find(s);
	if(iter != mSymbolMap.end())
	{
		return iter->second;
	}
	return -1; // return an invalid index
}

int SymbolTable::GetSymbolMapSize()
{
	return mSymbolMap.size();
}

bool SymbolTable::ContainsSymbol(string s)
{
	MapType::const_iterator iter = mSymbolMap.begin();
	iter = mSymbolMap.find(s);
	if(iter != mSymbolMap.end())
	{
		return true;
	}
	return false;
}

set<string> SymbolTable::GetAllSymbol()
{
	set<string> symbols;
	MapType::const_iterator end = mSymbolMap.end();
	for(MapType::const_iterator it = mSymbolMap.begin(); it != end; it++)
	{
		symbols.insert(it->first);
	}
	return symbols;
}