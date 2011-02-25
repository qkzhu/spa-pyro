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

void Validator::displayTable(){
	/* //////////////////testing getIndex()/////////////////
	vector<int> index;
	index = getIndex(suchThatTable, 10);

	for(int j = 0;j<index.size();j++)
	{
		cout << index[j]<<" ";
		
	}
	cout<<endl;
	*/
	for(int i=0;i<(suchThatTable.size());i++)
	{
		for(int j=0;j<suchThatTable[i].size();j++)
		{
			cout<<suchThatTable[i][j]<<" ";
		}
		
		//cout<<suchThatTable[i][0];
		cout<<endl;
	}

	
	//cout<<suchThatTable.size()<<endl;
	//cout<<suchThatTable[0].size()<<endl;

}

vector<int> Validator::getIndex(vector<vector<int>> table, int name){

	vector<int> index;
	//cout<<suchThatTable[77][0]<<" getIndex"<<endl;
	for(int i=0;i<suchThatTable.size();i++)
	{
		if(suchThatTable[i][0] == name)
		{
			index.push_back(i);
		}
		
	}

	return index;
}
bool Validator::checkSuchThat(QueryTable table){
	
	bool noError = false;
	int size = table.suchThatSize();
	vector<int> suchThat;
	vector<int> index;

	//suchThat =table.suchThatAt(0);
	//cout<<suchThat[0];
	//index = getIndex(suchThatTable, suchThat[0]);

	
	for(int i=0; i<size; i++)
	{
		suchThat =table.suchThatAt(i);	
		index = getIndex(suchThatTable, suchThat[0]);
		
		vector<int>::iterator it;
		for(it = index.begin();it<index.end();it++)
		{
			if(suchThat.size() == 5)
			{
				if((suchThat.at(1) == suchThatTable[*it][1]) && (suchThat.at(3) == suchThatTable[*it][2]))
				{
					noError = true;
				}
			}
			else if(suchThat.size() == 4)
			{
				if((suchThat.at(1) == suchThatTable[*it][1] && (suchThat.at(2) == suchThatTable[*it][2])) || (suchThat.at(1) == suchThatTable[*it][1]) && (suchThat.at(3) == suchThatTable[*it][2]))
				{
					noError = true;
				}
			}
			else if(suchThat.size() == 3)
			{
				if((suchThat.at(1) == 157) && (suchThat.at(2) == 157))
				{
					noError = true;
				}
			}
		
		}
		
	}
	

	cout<<size<<endl;;

	return noError;


}