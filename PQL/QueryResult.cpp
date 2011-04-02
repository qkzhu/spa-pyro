#include "QueryResult.h"

using namespace std;


QueryResult::QueryResult()
{	
	mBoolSet = false;
	nullResult = false;
}


QueryResult::QueryResult(bool b)
{
	mBoolSet = true;
	mBoolResult = b;
	nullResult = false;
}

bool QueryResult::isEmptyResult(){
	return nullResult;
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
	if(type == -1)
		nullResult = true;
	mTupleType.push_back(type);
}

void QueryResult::addInTuple(vector<int> concat)
{
	cout << "Inserting " << mTupleResult.size() << endl;;
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



void QueryResult::resultSort(){
	if(mTupleResult.empty() || (int)mTupleResult.size() == 1) return;

	sort(mTupleResult.begin(), mTupleResult.end(), myCompare);

	//Eliminate duplicates
	vector<vector<int> > tmp_store;
	vector<int> equilities; //keeps equal tuples' indexes
	equilities.push_back(0);
	for(int i = 1; i < (int)mTupleResult.size(); i++){
		if(equalityCompare(mTupleResult[i-1], mTupleResult[i])){
			if(i == mTupleResult.size() -1) 
				tmp_store.push_back(mTupleResult[i]);
			equilities.push_back(i);
		}else{
			if(i == mTupleResult.size() -1) 
				tmp_store.push_back(mTupleResult[i]);
			tmp_store.push_back(mTupleResult[equilities[0]]);
			equilities.clear();
			equilities.push_back(i);
		}
	}
	mTupleResult = tmp_store;
}

bool QueryResult::myCompare(vector<int> v1, vector<int> v2){
	if(v1.empty() || v2.empty()) throw new string("QueryResult::myCompare, the comparator should not take empty vector!");
	return v1[0] < v2[0];
}

bool QueryResult::equalityCompare(vector<int> v1, vector<int> v2){
	if(v1.size() != v2.size()) throw new string("QueryResult::equalityCompare, the query result is not well formed!");
	for(int i = 0; i < (int)v1.size(); i++){
		if(v1[i] != v2[i])
			return false;
	}
	return true;
}