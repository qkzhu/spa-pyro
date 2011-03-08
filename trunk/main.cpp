#include "QueryEvaluator.h"
#include "PqlPreprocessor.h"
#include "PKB.h"
#include "Parser.h"
#include <string>
using namespace std;

int main(int argc, char* argv[]){
	try
   {
	   string simple;
	   cout << "Please enter the simple file location: " << endl;
	   simple = "simple.txt";
	   //cin >> simple;
	   PKB pkb;
	   Parser p (pkb, simple);
	   p.parseProgram();

	   cout<<"******************** AST Testing ********************"<<endl;
	   cout<<endl;

	   cout<<"Child of statment: ";
	   pkb.printVectorInt(pkb.ast_GetChild(6));

	   cout<<"Previous statement of line 1 is "<<pkb.ast_GetPreviousStatementNum(1)<<endl;

	   cout<<"Following statement of line 25 is "<<pkb.ast_GetFollowingStatementNum(25)<<endl;

	   cout<<"Parent statement of line 1 is "<<pkb.ast_GetPreviousStatementNum(1)<<endl;

	   cout<<"Max number of  statement is "<<pkb.ast_getMaxStmtNum()<<endl;

	   cout<<"Get all assign: ";
	   pkb.printVectorInt(pkb.ast_GetAllAssign());

	   cout<<"Get all whil: ";
	   pkb.printVectorInt(pkb.ast_GetAllWhile());

	   cout<<"Get all if: ";
	   pkb.printVectorInt(pkb.ast_GetAllIf());

	   cout<<"Get all calls: ";
	   pkb.printVectorInt(pkb.ast_GetAllCall());	   

	   cout<<endl;
	   cout<<"****************** End AST Testing ******************"<<endl;


	   string query_file;
	   cout << "Please enter the query file location: " << endl;
	   query_file = "query.txt";
	   //cin >> query_file;
	   ifstream qrFile (query_file, ifstream::in);

	   string query;
	   while (!qrFile.eof()){
		   string tmp;
		   getline(qrFile, tmp);
		   query.append(tmp);
	   }
	   PqlPreprocessor pql (query);

	   QueryEvaluator bqe (&pkb, &pql);
	   bqe.evaluate();
	   bqe.printResult();

	}catch(string *s){
		cout << "Exception message: ";
		cout << *s << endl;
	}


	system("pause");
	return 0;
}
