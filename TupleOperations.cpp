#include "TupleOperations.h"

using namespace std;

vector<vector<int> > TupleOperations::tupleJoinOneC(int same1Tuple1, vector<vector<int> > tuple1, vector<vector<int> > tuple2)
{
	vector<vector<int> > result;

	for(vector<vector<int> >::iterator i= tuple1.begin(); i<tuple1.end(); i++)
	{
		int common1 = (*i).at(same1Tuple1);
		for(vector<vector<int> >::iterator k = tuple2.begin(); k< tuple2.end(); k++)
		{
			int common2 = (*k).at(0);
			if(common1 == common2){
				vector<int> tmp = *i;
				tmp.insert(tmp.end(), (*k).begin(), (*k).end());
				result.push_back(tmp);
			}
		}
	}
	return result;
}//fun: tupleJoinOneC END


vector<vector<int> >  TupleOperations::tupleJoinTwoC(int same1Tuple1,int same2Tuple1, vector<vector<int> > tuple1, vector<vector<int> > tuple2)
{
	vector<vector<int> > result;

	for(vector<vector<int> >::iterator i= tuple1.begin(); i<tuple1.end(); i++)
	{
		int common1 = (*i).at(same1Tuple1);
		int common2 = (*i).at(same2Tuple1);
		for(vector<vector<int> >::iterator k = tuple2.begin(); k< tuple2.end(); k++)
		{
			int common1_tuple2 = (*k).at(0);
			int common2_tuple2 = (*k).at(1);
			if(common1 == common1_tuple2 && common2 == common2_tuple2){
				vector<int> tmp = *i;
				tmp.insert(tmp.end(), (*k).begin(), (*k).end());
				result.push_back(tmp);
			}
		}
	}
	return result;


}//fun: tupleJoinTwoC END