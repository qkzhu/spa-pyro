#include <iostream>
#include <utility>
#include "DataStucture.h"


using namespace std;


class QueryTable{

public: 
	//QueryTable();
	void addClause(int type, vector<int> content);
	vector<int> getQuery();

	vector < Select > getSelectClause();
	vector < SuchThat > getSuchThatClause();
	vector<With> getWithClause();

	int selectSize(); // return the number of result variables
	vector<int> selectAt(int index); //take an index and return the result variable at index

	int suchThatSize(); //return the number of such that conditions in such that clause
	vector<int> suchThatAt(int index); //take an indx and return the such that condition in index

	int withSize(); //return the number of with conditions in the with clause
	vector<int> withAt(int index); //take an indx and return the with condition in index.

	int getSize();


private:
	//for whole query
	vector<int> queryVector;

	//"select-clause"  
	vector < Select > selectClause;
	vector <int> selectVector;
	//"such that-clause"
	vector<SuchThat> suchThatClause;
	vector <int> suchThatVector;
	//"with-clause"
	vector<With> withClause;
	vector <int> withVector;
	
	//"declaration - clause"
	vector<Declaration> declarationClause;
	vector <int> declarationVector;
	
	
};
