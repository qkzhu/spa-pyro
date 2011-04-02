#ifndef QUERYTABLE_H
#define QUERYTABLE_H


#include <iostream>
#include <utility>
#include "DataStucture.h"
#include "dataBase.h"


using namespace std;


class QueryTable{

public: 
	//QueryTable();
	void addClause(int type, vector<int> content); // add query to the data structure

	int selectSize(); // return the number of result variables
	void selectAt(vector<int> &tupleTemp, int index); //take an index and return the result variable at index

	int suchThatSize(); //return the number of such that conditions in such that clause
	void suchThatAt(vector<int> &relCondTemp, int index); //take an indx and return the such that condition in index

	int withSize(); //return the number of with conditions in the with clause
	void withAt(vector<int> &attrCondTemp,int index); //take an indx and return the with condition in index.

	int patternSize(); //return the number of pattern expressions in the pattern clause
	void patternAt(vector<int> &expression, int index); ////take an indx and return the pattern expression in index.



	// for internal usage
	vector<int> getQuery();
	vector < Select > getSelectClause();
	vector < SuchThat > getSuchThatClause();
	vector < With > getWithClause();
	vector < Pattern_PQL > getPattern_PQLClause();

	void showTable();


private:
	//for whole query
	vector<int> queryVector;

	//"select-clause"  
	vector < Select > selectClause;	
	vector <int> selectVector;

	//"such that-clause"
	vector < SuchThat > suchThatClause;
	vector <int> suchThatVector;

	//"with-clause"
	vector < With > withClause;
	vector <int> withVector;

	//"pattern-clause"
	vector < Pattern_PQL > patternClause;
	vector <int> patternVector;


	/*
	//"declaration - clause"
	vector<Declaration> declarationClause;
	vector <int> declarationVector;
	*/
	
};
#endif //QUERYTABLE_H