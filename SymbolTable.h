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
	void insertSymbol(string);
	string getSymbolName(int);
	int getSymbolIndex(string);
	int getSymbolMapSize();
	bool containsSymbol(string);
	set<string> getAllSymbol();
private:
	map<string, int> mSymbolMap;
};