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
