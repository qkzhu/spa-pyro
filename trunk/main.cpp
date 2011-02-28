#include "QueryEvaluator.h"
#include <string>
using namespace std;

void TestRelEva(PKB * pkb, QueryParser *query1){
	QueryEvaluator qe1 (pkb, query1);
		cout << "check eval function :" << endl;
		cout << "follows(2, a)" << endl;
		vector<vector<int> > tmp1 = qe1.getRel(201,53, 2, 302, 7);
		for(vector<vector<int> >::iterator i=tmp1.begin(); i<tmp1.end(); i++){
			for(vector<int>::iterator k = (*i).begin(); k < (*i).end(); k++){
				cout << (*k) << " ";
			}
			cout << endl;
		}
		cout << "follows*(2, a)" << endl;
		vector<vector<int> > tmp2 = qe1.getRel(201,53, 2, 302, 8);
		for(vector<vector<int> >::iterator i=tmp2.begin(); i<tmp2.end(); i++){
			for(vector<int>::iterator k = (*i).begin(); k < (*i).end(); k++){
				cout << (*k) << " ";
			}
			cout << endl;
		}
		cout << "parents(s, a)" << endl;
		vector<vector<int> > tmp3 = qe1.getRel(51, 53, 301,  302, 5);
		for(vector<vector<int> >::iterator i=tmp3.begin(); i<tmp3.end(); i++){
			for(vector<int>::iterator k = (*i).begin(); k < (*i).end(); k++){
				cout << (*k) << " ";
			}
			cout << endl;
		}
		cout << "parents*(s, a)" << endl;
		vector<vector<int> > tmp4 = qe1.getRel(51, 53, 301, 302, 6);
		for(vector<vector<int> >::iterator i=tmp4.begin(); i<tmp4.end(); i++){
			for(vector<int>::iterator k = (*i).begin(); k < (*i).end(); k++){
				cout << (*k) << " ";
			}
			cout << endl;
		}
		cout << "modifies(2, 'a')" << endl;
		vector<vector<int> > tmp5 = qe1.getRel(201, 202, 2, 0, 10);
		for(vector<vector<int> >::iterator i=tmp5.begin(); i<tmp5.end(); i++){
			for(vector<int>::iterator k = (*i).begin(); k < (*i).end(); k++){
				cout << (*k) << " ";
			}
			cout << endl;
		}
		cout << "uses(w, 'a')" << endl;
		vector<vector<int> > tmp6 = qe1.getRel(54, 202, 304, 0, 9);
		for(vector<vector<int> >::iterator i=tmp6.begin(); i<tmp6.end(); i++){
			for(vector<int>::iterator k = (*i).begin(); k < (*i).end(); k++){
				cout << (*k) << " ";
			}
			cout << endl;
		}
		cout << "calls(p, 'd')" << endl;
		vector<vector<int> > tmp7 = qe1.getRel(58, 203, 302, 1, 11);
		for(vector<vector<int> >::iterator i=tmp7.begin(); i<tmp7.end(); i++){
			for(vector<int>::iterator k = (*i).begin(); k < (*i).end(); k++){
				cout << (*k) << " ";
			}
			cout << endl;
		}
		cout << "modifies(s, 'a')" << endl;
		vector<vector<int> > tmp8 = qe1.getRel(51, 202, 301, 0, 10);
		for(vector<vector<int> >::iterator i=tmp8.begin(); i<tmp8.end(); i++){
			for(vector<int>::iterator k = (*i).begin(); k < (*i).end(); k++){
				cout << (*k) << " ";
			}
			cout << endl;
		}
}

void TestEvaluation(PKB *pkb){
	QueryParser *query1 = new QueryParser(0);
	QueryEvaluator qe1 (pkb, query1);
	qe1.evaluate();
	qe1.printResult();

	QueryParser *query2 = new QueryParser(1);
	QueryEvaluator qe2 (pkb, query2);
	qe2.evaluate();
	qe2.printResult();

	QueryParser *query3 = new QueryParser(2);
	QueryEvaluator qe3 (pkb, query3);
	qe3.evaluate();
	qe3.printResult();

	QueryParser *query4 = new QueryParser(3);
	QueryEvaluator qe4 (pkb, query4);
	qe4.evaluate();
	qe4.printResult();

	QueryParser *query5 = new QueryParser(4);
	QueryEvaluator qe5 (pkb, query5);
	qe5.evaluate();
	qe5.printResult();

}

int main(int argc, char* argv[]){
	try
   {

	   //Relation evaluation TEST
	    PKB *pkb = new PKB();
		
		//TestRelEva(pkb, query1);
		TestEvaluation(pkb);
		
	}catch(string s){
		cout << "Exception message: ";
		cout << s << endl;
	}


	system("pause");
	return 0;
}
