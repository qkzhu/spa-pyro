#include <vector>

#include"PQLpreprocessor.h"

//PQLpreprocessor::tree=new QueryTree("root")
QueryTree* PQLpreprocessor::preprocess()
{
     QueryTree* tree=new QueryTree("root");
	 string qu=PQLpreparser::QueryReader();
	 TestControl(qu);
	 cout << "this query is "<<qu;
	 syntaxValidator::Parse(qu,tree);
	 return tree;
}
void PQLpreprocessor::TestControl(string qu)
{  
	
	string sub;
	int i=0;
	istringstream iss(qu);
	iss>>sub;
	//if (sub.empty()==0) a=0; else a=0;  
	while(iss)
	{    
		i++;
	
       
		cout << i<<sub;
		 iss >> sub;
	};
}

