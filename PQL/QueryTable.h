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

	int selectSize(); //return the number of result variables
	void selectAt(vector<int> &tupleTemp, int index); //take an index and return the result variable at index index

	int suchThatSize(); //return the number of such that conditions in related partitions
	void suchThatAt(vector<int> &relCondTemp, int index); //take an index and return the related such that condition 

	int withSize(); //return the number of with conditions in the related partition
	void withAt(vector<int> &attrCondTemp,int index);//take an index and return the related with condition 

	int patternSize(); //return the number of pattern expressions in the related partition
	void patternAt(vector<int> &expression, int index); //take an indx and return the related pattern expression
	
	int suchThatUnrelatedSize(); //return the number of such that conditions in Unrelated partitions
	void suchThatUnrelatedAt(vector<int> &relCondTemp, int index); //take an index and return the Unrelated such that condition

	int withUnrelatedSize();  //return the number of with conditions in the Unrelated partition
	void withUnrelatedAt(vector<int> &attrCondTemp, int index); //take an index and return the Unrelated with condition 

	int patternUnrelatedSize(); //return the number of pattern expressions in the Unrelated partition
	void patternUnrelatedAt(vector<int> &expression, int index); //take an indx and return the Unrelated pattern expression

	int affectsStarSize(); //return the number of affects* clauses in the whole query
	int affectsSize(); //return the number of affects clauses in the whole query
	
	// for internal usage
	vector<int> getQuery();
	vector < Select > getSelectClause();
	vector < SuchThat > getSuchThatClause();
	vector < With > getWithClause();
	vector < Pattern_PQL > getPattern_PQLClause();

	void showTable(); // show the query 

	
	//*************** For Optimizer Start ****************//	
	void displayAll();	// display all the tables
	void displayNodeList(); // display all the node
	void displayPartitions(); // display all the partitions

	void findPartition(); // a function that calls all function needed to work out the partition
	void findPartitionOne(); // find all the directed related nodes
	void findPartitionTwo(); // find all related partitions
	void findPartitionFour(); // find all unrelated partitions

	void calculatedPartitionMarks(); // calculate the partition mark according to certain criteria

	void calculatePartitionMark(const vector<vector<QueryNode>> &nodeList, vector<Partitions> &group);

	void putIntoPartitionGroups();	// put the partitions with total mark into the groups 

	static bool myComparePartition(Partitions par1, Partitions par2); // compare function used to sort partitions in partition group
	static bool myCompareRelation(QueryNode node1, QueryNode node2); // compare function used to sort relations within a partition

	void sortPartitions(vector<Partitions> &partition); // sort partition in partition group
	void sortRelations(vector<QueryNode> &queryNodes); // sort relations for a single partition 
	void sortRelationsPartitions(vector<Partitions> &partition); // sort relations for all partition
	
	// recursive function to find all the related nodes
    //	sourceExistNode is the set of Nodes for comparision
	void findPartitionThree(set<int> &sourceExistNode, vector<QueryNode> &sourceQueryNode, vector<QueryNode> &nodeListContainer);

	void addToTable(vector<Partitions> &group,vector<vector<int>> &withTable, vector<vector<int>> &patternTable, vector<vector<int>> &suchThatTable);	

	void addToUnrelatedTable(); // add sorted unrelated nodes to the unrelated table
	void addToRelatedTable(); // add sorted related nodes to the related table
	//*************** For Optimizer End ****************//


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
	// store queryNode
	vector<QueryNode> queryNodeList;
	// store selectNode
	vector<SelectNode> selectNodeList;
	// store related query Node
	vector<vector<QueryNode>> relatedNodes;
	// store unrealted query Node
	vector<vector<QueryNode>> unrelatedNodes;
	// store unrelated partitions
	vector<Partitions> unrelatedGroup;
	// store related partitions
	vector<Partitions> relatedGroup;
	// contains all the nodes that have alread find it. it is used to compare the exist value to the value in queryNodeList.
	set<int> existNodeValue; 	
	// define the pair for hash_map
	typedef pair<int, vector<int>> keyContentPair;
	// hash_map stores with relation
	hash_map<int, vector<int>> withRelationMap;
	// hash_map stores pattern relation
	hash_map<int, vector<int>> patternRelationMap;

	//*************** For Optimizer End ****************//

};
#endif //QUERYTABLE_H