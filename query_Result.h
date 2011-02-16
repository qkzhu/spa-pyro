#include <iostream>
#include <vector>

class query_Result{
private:
	bool boolResult;
	std::vector<int> tupleResult;


public:
	query_Result();
	query_Result(bool);

	bool getBoolValue();
	std::vector<int>* getTuple(); 

	void setBoolValue(bool);   //set bool value
	void addInTuple(int);  //add a value at index indx in tuple
	void addInTuple(std::vector<int>*);
	int getValAtTuple(int);  //get the value at index indx in tuple
	void deleteValAtTuple(int); //delete the value at index indx in tuple, return true upon success

};

