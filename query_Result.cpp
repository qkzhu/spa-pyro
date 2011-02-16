#include "query_Result.h"

using namespace std;


query_Result::query_Result()
{	tupleResult = vector<int>();
}


query_Result::query_Result(bool b)
{
	boolResult = b;
}


bool query_Result::getBoolValue(){
	return boolResult;	
}

vector<int>* query_Result::getTuple(){
	return &tupleResult;
}

void query_Result::setBoolValue(bool b){
		boolResult = b;
}

void query_Result::addInTuple(int value){
	tupleResult.push_back(value);
}

void query_Result::addInTuple(vector<int> *concat){
	int s = (*concat).size();
	int i =0;
	while(i < s){
		tupleResult.push_back((*concat).at(i));
	}
}

int query_Result::getValAtTuple(int indx){
	return tupleResult.at(indx);	
}

void query_Result::deleteValAtTuple(int indx){
	tupleResult.erase(tupleResult.begin()+indx);
}