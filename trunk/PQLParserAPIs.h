#include <iostream>
#include <vector>


class QueryParser { 
public:
	QueryParser(std::string); //constructor, with the query string being passed

	int selectSize(); //return the number of result variables
	int selectAt(int indx); //take an index and return the result variable at index indx.
	int suchThatSize(); //return the number of such that conditions in such that clause
	std::vector<int> suchThatAt(int indx); //take an indx and return the such that condition in index indx
	int withSize(); //return the number of with conditions in the with clause
	std::vector<int> withAt(int indx); //take an indx and return the with condition in index indx

	int varNum(); //return the number of all query variables
	int varType(int var); //take an integer form of a query variable and return its type in integer

	//For keyword decoding
	int procCode(std::string key);  //take a simple procedure name and return the PQL code
	int varCode(std::string key);  //take a simple variable name and return the PQL code
	std::string varDecode(int i);  //take a code of simple variable and return the variable string
	std::string procDecode(int i);  //take the code of simple variable and return the procedure name string
		
private:
	std::string mQuery;
};