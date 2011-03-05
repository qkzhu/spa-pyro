#include "Validator.h"
#include "Tokenizer.h"


void Validator::populateTable(){

	Tokenizer str;

	//pupulate suchThatTable
	ifstream suchThatFile;
	suchThatFile.open("DesignModel.txt");
	string line;
	string token;

    if (suchThatFile.is_open())
    {
		while (!suchThatFile.eof())
		{
			getline(suchThatFile,line);
			str.set(line);
			int tempHolder;
			vector<int> rows;
			while((token = str.next()) != "")
			{	

				tempHolder = atoi(token.c_str());
				rows.push_back(tempHolder);
				
			}
			if(!rows.empty())
				suchThatTable.push_back(rows);

		}
		suchThatFile.close();
	}
	
	// populate withTable ######this one can be refined as a function call -- fillTable(vector<vector<int>> table, string filename)#####
	ifstream withFile;
	withFile.open("withTable.txt");
	string line1;
	string token1;

    if (withFile.is_open())
    {
		while (!withFile.eof())
		{
			getline(withFile,line1);
			str.set(line1);
			int tempHolder1;
			vector<int> rows1;
			while((token1 = str.next()) != "")
			{	

				tempHolder1 = atoi(token1.c_str());
				rows1.push_back(tempHolder1);
				
			}
			if(!rows1.empty())
				withTable.push_back(rows1);

		}
		withFile.close();
	}
	
}

/*
void Validator::fillTable(vector <vector<int>> table, string fileName){
	
	Tokenizer str;

	//pupulate suchThatTable
	ifstream suchThatFile;
	suchThatFile.open(fileName);
	string line;
	string token;

    if (suchThatFile.is_open())
    {
		while (!suchThatFile.eof())
		{
			getline(suchThatFile,line);
			str.set(line);
			int tempHolder;
			vector<int> rows;
			while((token = str.next()) != "")
			{	

				tempHolder = atoi(token.c_str());
				rows.push_back(tempHolder);
				
			}
			if(!rows.empty())
				table.push_back(rows);

		}
		suchThatFile.close();
	}

}
*/


void Validator::displayTable(vector<vector<int>> table){
	/* //////////////////testing getIndex()/////////////////
	vector<int> index;
	index = getIndex(table, 10);

	for(int j = 0;j<index.size();j++)
	{
		cout << index[j]<<" ";
		
	}
	cout<<endl;
	*/
	for(int i=0;i<(table.size());i++)
	{
		for(int j=0;j<table[i].size();j++)
		{
			cout<<table[i][j]<<" ";
		}
		
		//cout<<table[i][0];
		cout<<endl;
	}

	
	//cout<<table.size()<<endl;
	//cout<<table[0].size()<<endl;

}

vector<int> Validator::getIndex(vector<vector<int>> table, int name){

	vector<int> index;
	//cout<<suchThatTable[77][0]<<" getIndex"<<endl;
	for(int i=0;i<table.size();i++)
	{
		if(table[i][0] == name)
		{
			index.push_back(i);
		}
		
	}

	return index;
}

bool Validator::checkSuchThat(QueryTable table){
	
	bool noError = true;
	int size = table.getSuchThatClauseV().size();
	
	

	//suchThat =table.suchThatAt(0);
	//cout<<suchThat[0];
	//index = getIndex(suchThatTable, suchThat[0]);

	
	for(int i=0; i<size; i++)
	{
		bool tempNoError = false;
		// to check with the argument is correct of not
		if(table.getSuchThatClauseV().at(i).argumentNoCorrect == false)
		{
			noError = tempNoError && noError;
			break;
		}

		vector<int> suchThat;
		vector<int> index;
		suchThat =table.getSuchThatClauseV().at(i).relCond;	
		index = getIndex(suchThatTable, suchThat[0]);
		
		vector<int>::iterator it;
		for(it = index.begin();it<index.end();it++)
		{
			if(suchThat.size() == 5)
			{
				if((suchThat.at(1) == suchThatTable[*it][1]) && (suchThat.at(3) == suchThatTable[*it][2]))
				{
					tempNoError = true;
				}
			}
			else if(suchThat.size() == 4)
			{
				if((suchThat.at(1) == suchThatTable[*it][1] && (suchThat.at(2) == suchThatTable[*it][2])) || (suchThat.at(1) == suchThatTable[*it][1]) && (suchThat.at(3) == suchThatTable[*it][2]))
				{
					tempNoError = true;
				}
			}
			else if(suchThat.size() == 3)
			{
				if((suchThat.at(1) == 157) && (suchThat.at(2) == 157))
				{
					tempNoError = true;
				}
			}
		
		}
		//cout<<tempNoError<<" inside checkSuchThat "<<endl;
		noError = tempNoError && noError;
	}
	//cout<<size<<endl;;

	return noError;
}

bool Validator::checkWith(QueryTable table){
	bool noError = true;
	int size = table.getWithClauseV().size();

	//cout<<"with size is " <<size<<endl;


	for(int i=0; i<size;i++)
	{
		bool tempNoError = false;
		//cout<<"argumentNoCorrect "<<table.getWithClause().at(i).argumentNoCorrect<<endl;
		// to check with the argument is correct of not
		if(!table.getWithClauseV().at(i).argumentNoCorrect)
		{
			noError = tempNoError && noError;
			break;
		}
		
		vector<int> with;
		vector<int> index;
	

		with =  table.getWithClauseV().at(i).attrCond;
		//cout<<"with[0] is "<<with[0]<<endl;
		index = getIndex(withTable, with[0]);

		//cout<<"index size is "<<index.size()<<endl; 
		for(int it = 0;it<index.size();it++)
		{
			//cout<<"with size is 1 "<<with.size()<<endl;
			if(with.size() == 7)
			{
				//cout<<"with size is "<<with.size()<<endl;
				if((with[0] == withTable[index[it]][0]) && (with[3] == withTable[index[it]][1]) && (with[5] == withTable[index[it]][2]))
				{
					tempNoError = true;
				}
			
			}
			if(with.size() == 9)
			{
				if((with[0] == withTable[index[it]][0]) && (with[3] == withTable[index[it]][1]) && (with[5] == withTable[index[it]][2]) && (with[8] == withTable[index[it]][3]))
				{
					tempNoError = true;
				}
							
			}

		}
		//cout<<tempNoError<<" inside checkWith "<<endl;
		noError = tempNoError && noError;
	}
	return noError;
}

vector<vector<int>> Validator::getsuchThatTable(){

	return suchThatTable;

}
vector<vector<int>>  Validator::getwithTable(){

	return withTable;
}
