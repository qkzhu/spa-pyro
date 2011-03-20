#include "TupleOperations.h"

using namespace std;

vector<vector<int> > TupleOperations::tupleJoinOneC(int same1Tuple1, int sameAtTuple2, vector<vector<int> > tuple1, vector<vector<int> > tuple2)
{
	vector<vector<int> > result;

	for(vector<vector<int> >::iterator i= tuple1.begin(); i<tuple1.end(); i++)
	{
		int common1Type = (*i).at(2*same1Tuple1-2);
		int common1 = (*i).at(2*same1Tuple1-1);
		for(vector<vector<int> >::iterator k = tuple2.begin(); k< tuple2.end(); k++)
		{
			int common2Type = (*k).at(sameAtTuple2*2);
			int common2 = (*k).at(sameAtTuple2*2+1);
			if(common1 == common2){
				vector<int> tmp = *i;
				if(sameAtTuple2 == 0)
					tmp.insert(tmp.end(), (*k).begin()+2, (*k).end());
				else tmp.insert(tmp.end(), (*k).begin(), (*k).begin() +2);
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
		int common1 = (*i).at(same1Tuple1*2-1);
		int common2 = (*i).at(same2Tuple1*2-1);
		for(vector<vector<int> >::iterator k = tuple2.begin(); k< tuple2.end(); k++)
		{
			int common1_tuple2_type = (*k).at(0);
			int common1_tuple2 = (*k).at(1);
			int common2_tuple2_type = (*k).at(2);
			int common2_tuple2 = (*k).at(3);
			if(common1 == common1_tuple2 && common2 == common2_tuple2){
				vector<int> tmp = *i;
				result.push_back(tmp);
			}
		}
	}
	return result;


}//fun: tupleJoinTwoC END