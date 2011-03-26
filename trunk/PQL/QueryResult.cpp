#include "QueryResult.h"

using namespace std;


QueryResult::QueryResult()
{	
	mBoolSet = false;
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

vector<vector<int> > QueryResult::getTuple()
{
	return mTupleResult;
}
vector<int> QueryResult::getTypes()
{
	return mTupleType;
}

int QueryResult::getTypeAt(int indx){
	return mTupleType.at(indx);
}

void QueryResult::setBoolValue(bool b)
{
	mBoolSet = true;
	mBoolResult = b;
}

bool QueryResult::isTupleInserted(vector<int> v){
	if((int)mTupleResult.size() == 0) return false;
	for(int i = 0; i < (int)mTupleResult.size(); i++){
		vector<int> tmp = mTupleResult[i];
		if(v.size() != tmp.size()) throw new string("QueryResult, trying to insert invalid tuple, error element number!");
		bool same = true;
		for(int j = 0; j < (int) tmp.size(); j++){
			if(tmp[j] != v[j])
				same = false;
		}
		if(same) return true;
	}
	return false;
}

void QueryResult::addInType(int type)
{
	mTupleType.push_back(type);
}

void QueryResult::addInTuple(vector<int> concat)
{
	cout << "Inserting " << mTupleResult.size() << endl;;
	if(!isTupleInserted(concat))
		mTupleResult.push_back(concat);
}

void QueryResult::print(){
	cout << "QueryResult Looks Like..."<< endl;
	cout << "types : " ;
	for(int i = 0; i < (int)mTupleType.size(); i++){
		cout << mTupleType.at(i) << " ";
	}
	cout << endl;
	cout << "tuples: " << endl;
	for(int i = 0; i < (int)mTupleResult.size(); i++){
		for(int j = 0; j < (int)mTupleResult[i].size(); j++){
			cout << mTupleResult[i][j] << " ";
		}
		cout << endl;
	}

	cout << "OK, that's all!" << endl;
}