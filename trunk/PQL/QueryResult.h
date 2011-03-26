#ifndef QUERY_RESULT_H
#define QUERY_RESULT_H

#include <iostream>
#include <vector>
#include <algorithm>

class QueryResult{
private:
	bool mBoolSet;
	bool mBoolResult;
	std::vector<std::vector<int> > mTupleResult;
	std::vector<int> mTupleType;

public:
	QueryResult();
	QueryResult(bool);

	bool isTupleInserted(std::vector<int> v); //whether the v is inserted inside Tuple result already
	bool isBoolSet();
	bool getBoolValue();
	std::vector<std::vector<int> > getTuple();  //
	std::vector<int> getTypes();
	int getTypeAt(int indx);

	void setBoolValue(bool);   //set bool value

	void addInType(int);
	void addInTuple(std::vector<int>);  //add a value at index indx in tuple, and check the duplication


	void resultSort(); //Do sorting and condense the result to eliminate duplicates
	static bool myCompare(std::vector<int> v1, std::vector<int> v2);
	bool equalityCompare(std::vector<int>, std::vector<int>);

	void print();
};

#endif