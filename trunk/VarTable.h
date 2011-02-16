#include <string>
#include <map>
#include <set>

using namespace std;

class VarTable
{
public:
	void insertVar(string var_name, int statNum); // if v not in VarTable, records that v appears in statement number n. else no change. 
	bool containsVar(string var); // return index of variable
	int getVarSize(); // returns total no. of variables in VarTable (given a var, find all stmts that var appear on)
	int getStmtSize(); //returns total no. of Stmt in StmtTable (given a stmt, find all variables that appear on the stmt)
	set<int> getAllStmt();
	set<int> getAllStmt(string var_name); //gets all statements that has variable var_name
	set<string> getAllVar(int statNum); //gets all variables that appear in this statement.

	VarTable();
private:
	map<string, set<int>> var_map;
	map<int, set<string>> stat_map;
};