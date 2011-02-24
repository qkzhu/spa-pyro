#include <iostream>
#include <vector>
#include <map>

//This class defines every operation on the relation result
static class TupleOperations
{
public:
	//when the two tuples have only one attribute in common
	static std::vector<std::vector<int> >  tupleJoinOneC(int, std::vector<std::vector<int> >, std::vector<std::vector<int> >);

	//have two attrs in common
	static std::vector<std::vector<int> >  tupleJoinTwoC(int,int,std::vector<std::vector<int> >, std::vector<std::vector<int> >);

};