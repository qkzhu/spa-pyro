#ifndef QUERYTABLE_H
#define QUERYTABLE_H


#include <iostream>
#include <utility>
#include <set>
#include <algorithm>
#include <hash_map>
#include "DataStucture.h"
#include "dataBase.h"



using namespace std;


class QueryTable{

public: 
	//constructor
	QueryTable();
	
	void addClause(int type, vector<int> content); // add query to the data structure

	int selectSize(); // return the number of result variables
	void selectAt(vector<int> &tupleTemp, int index); //take an index and return the result variable at index

	int suchThatSize(); //return the number of such that conditions in such that clause
	void suchThatAt(vector<int> &relCondTemp, int index); //take an indx and return the such that condition in index

	int withSize(); //return the number of with conditions in the with clause
	void withAt(vector<int> &attrCondTemp,int index); //take an indx and return the with condition in index.

	int patternSize(); //return the number of pattern expressions in the pattern clause
	void patternAt(vector<int> &expression, int index); ////take an indx and return the pattern expression in index.

	// newly added
	int affectsStarSize(); //return the number of affects* clauses in the whole query
	int affectsSize(); //return the number of affects clauses in the whole query
	

	int withUnrelatedSize();  
	void withUnrelatedAt(vector<int> &unrelatedW, int index);

	int patternUnrelatedSize();
	void patternUnrelatedAt(vector<int> &unrelatedPat, int index);

	int suchThatUnrelatedSize();
	void suchThatUnrelatedAt(vector<int> &unrelatedST, int index);

	int relatedRelationSize();
	void relatedRelationAt(vector<int> &relations, int index); 



	// for internal usage
	vector<int> getQuery();
	vector < Select > getSelectClause();
	vector < SuchThat > getSuchThatClause();
	vector < With > getWithClause();
	vector < Pattern_PQL > getPattern_PQLClause();

	void showTable();

	void displayAll();
	//*************** For Optimizer Start ****************//
	
	void findPartition(); // a function that calls all function needed to work out the partition
	
	void displayNodeList();
	void findPartitionOne();

	void findPartitionTwo(); // find all related partitions
	
	void findPartitionFour(); // find all unrelated partitions



	void calculatedPartitionMarks();
	void putIntoPartitionGroups();

	static bool myComparePartition(Partitions par1, Partitions par2);
	static bool myCompareRelation(QueryNode node1, QueryNode node2);
	void sortPartitions(vector<Partitions> &partition);

	void displayPartitions();


	void sortRelationWithinPartition(vector<Partitions> &partition);
	void sortRelations(vector<QueryNode> &queryNodes);
	// recursive function to find all the related nodes
    //	sourceExistNode is the set of Nodes for comparision
	void findPartitionThree(set<int> &sourceExistNode, vector<QueryNode> &sourceQueryNode, vector<QueryNode> &nodeListContainer);
	
	
	void addToUnrelatedTable();
	void addToRelatedTable();
	//*************** For Optimizer End ****************//


	void tryHashMap();





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

	//"related Nodes"
	vector<vector<int>> relatedRelations;

	//"related With"
	vector<vector<int>> relatedWith;

	//"related Such That"
	vector<vector<int>> relatedSuchThat;

	//"related Pattern"
	vector<vector<int>> relatedPattern;

	//"unrelated With"
	vector<vector<int>> unRelatedWith;

	//"unrelated Such That"
	vector<vector<int>> unRelatedSuchThat;

	//"unrelated pattern"
	vector<vector<int>> unRelatedPattern;
	
	//counters
	int withCounter;
	int patternCounter;
	int affectsStarCounter;
	int affectsCounter;

	//*************** For Optimizer Start ****************//
	vector<QueryNode> queryNodeList;
	vector<SelectNode> selectNodeList;

	vector<vector<QueryNode>> relatedNodes;

	vector<vector<QueryNode>> unrelatedNodes;

	vector<Partitions> unrelatedGroup;


	set<int> existNodeValue; //contains all the nodes that have alread find it. it is used to compare the exist value to the value in queryNodeList.
	

	typedef pair<int, vector<int>> keyContentPair;

	hash_map<int, vector<int>> withRelationMap;
	hash_map<int, vector<int>> patternRelationMap;

	//*************** For Optimizer End ****************//

	/*
	//"declaration - clause"
	vector<Declaration> declarationClause;
	vector <int> declarationVector;
	*/
	
};
#endif //QUERYTABLE_H