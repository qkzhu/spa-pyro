#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include "tree.h"
using namespace std;


class PQLpreparser
{
private:
	static string qu;
	static bool flag;
public:
	static string QueryReader();
	//static string GetQuery();
};
class syntaxValidator
{
public:

static void Parse(string str,QueryTree* tree);
static int getStmtNValue();  //By HQ
static string getStmtNVar();  // By HQ

private:

//The fields for declaration and statement number information
static map<string, string> DeclarationMap;	//The map between variable and its type, first string is variable, By HQ

static int stmtNumber; //The statement numbers , By HQ
static string stmtNumberVariable; //Keep the variable for statement number, By HQ


static void SelectG(string query,QueryTree* tree);
static void SuchthatG(string query,QueryTree* tree);


private:
	static string TypeDecider(string s);

};

