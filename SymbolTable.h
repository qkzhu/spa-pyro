#include <map>
#include <string>
#include <utility>
#include <set>
using namespace std;


//Delclarations
typedef map<string,int> MapType; //for implementation use

class SymbolTable
{
public:
	void InsertSymbol(string);
	string GetSymbolName(int);
	int GetSymbolIndex(string);
	int GetSymbolMapSize();
	bool ContainsSymbol(string);
	set<string> GetAllSymbol();
private:
	map<string, int> mSymbolMap;
};