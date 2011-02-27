#include <iostream>
#include <vector>

class QueryResult{
private:
	bool mBoolSet;
	bool mBoolResult;
	std::vector<int> mTupleResult;
	std::vector<int> mTupleType;

public:
	QueryResult();
	QueryResult(bool);

	bool isBoolSet();
	bool getBoolValue();
	std::vector<int> getTuple();  //
	std::vector<int> getTypes();

	void setBoolValue(bool);   //set bool value

	void addInType(int);
	void addInTuple(int);  //add a value at index indx in tuple
	void addInTuple(std::vector<int>);
	int getValAtTuple(int);  //get the value at index indx in tuple
	void deleteValAtTuple(int); //delete the value at index indx in tuple, return true upon success

};

