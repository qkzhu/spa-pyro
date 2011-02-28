#include "QueryEvaluator.h"
#include <string>
using namespace std;

int main(int argc, char* argv[]){
	try
   {
	   string simple;
	   cout << "Please enter the simple file location: " << endl;
	   cin >> simple;
	   PKB pkb;
	   Parser p (pkb, simple);
	   p.parseProgram();
		

	   string query_file;
	   cout << "Please enter the query file location: " << endl;
	   cin >> query_file;
	   ifstream *qrFile = new ifstream(query_file, ifstream::in);

	   string query;
	   while (qrFile->good())
		   query.append(qrFile->getline());

	   PQLParser q(query);

	   QueryEvaluator bqe (&pkb, &q);
	   bqe.evaluate();
	   bqe.printResult();

	}catch(string s){
		cout << "Exception message: ";
		cout << s << endl;
	}


	system("pause");
	return 0;
}
