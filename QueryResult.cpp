#include "QueryResult.h"

using namespace std;


QueryResult::QueryResult()
{	
	mBoolSet = false;
	mTupleResult = vector<int>();
}


QueryResult::QueryResult(bool b)
{
	mBoolSet = true;
	mBoolResult = b;
}

bool QueryResult::isBoolSet()
{
	return mBoolSet;
}

bool QueryResult::getBoolValue()
{
	return mBoolResult;	
}

vector<int> QueryResult::getTuple()
{
	return mTupleResult;
}

void QueryResult::setBoolValue(bool b)
{
	mBoolSet = true;
	mBoolResult = b;
}

void QueryResult::addInTuple(int value)
{
	mTupleResult.push_back(value);
}

void QueryResult::addInTuple(vector<int> concat)
{
	int s = (concat).size();
	int i =0;
	while(i < s){
		mTupleResult.push_back((concat).at(i));
	}
}

int QueryResult::getValAtTuple(int indx)
{
	return mTupleResult.at(indx);	
}

void QueryResult::deleteValAtTuple(int indx)
{
	mTupleResult.erase(mTupleResult.begin()+indx);
}