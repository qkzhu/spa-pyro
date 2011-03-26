#include <iostream>
#include "QueryTable.h"


//////////////////////// add Clause --- Start ///////////////////////////////
void QueryTable::addClause(int type, vector<int> content){

	if(type == 1) // select
	{
		if(selectVector.empty())
		{
			
			selectVector.push_back(1);
		}

		if(content.front() == 60) // content is boolean
		{
			Select selectTemp;
			selectTemp.tuple = content;
			selectClause.push_back(selectTemp);
			selectVector.push_back(content.front());
		}
		else  //content is tuple
		{   
			Select selectTemp;
			selectTemp.tuple = content;
			selectClause.push_back(selectTemp);
			
			//for display
			if(selectVector.size()>1)
			{
				selectVector.push_back(-1);
			}
			for(int i= 0; i<int(content.size());i++)
			{
				selectVector.push_back(content.at(i));
			}
	
		}
		
	}
	else if(type == 2) // such that, no length parameter
	{
		SuchThat SuchThatTemp;
		SuchThat validTemp;

		SuchThatTemp.relCond = content;
		SuchThatTemp.argumentNoCorrect = false;
		if((int)SuchThatTemp.relCond.size() == 5)
		{
			SuchThatTemp.argumentNoCorrect = true;
		}
		else if(SuchThatTemp.relCond.size() == 4)
		{
			
			if((SuchThatTemp.relCond[1] == 157 && SuchThatTemp.relCond[2]!=157) || (SuchThatTemp.relCond[1] != 157 && SuchThatTemp.relCond[3] ==157))
			{
				SuchThatTemp.argumentNoCorrect = true;
			}
		}
		else if(SuchThatTemp.relCond.size() == 3)
		{
			if(SuchThatTemp.relCond[1] == 157 && SuchThatTemp.relCond[2]==157)
			{
				SuchThatTemp.argumentNoCorrect = true;
			}
		}
		suchThatClause.push_back(SuchThatTemp);

		//add the content to the withVector for display
		for(int i = 0; i<(int) content.size();i++)
		{

			//cout<<content[i]<<" ///////////////BEFORE CHANGE"<<endl;
			if(content[i] == 8) // make "follow*" to be "follow"
			{
				validTemp.relCond.push_back(7);
			}
			else if(content[i] == 6) // make "parent*" to be "parent"
			{
				validTemp.relCond.push_back(5);
			}
			else if(content[i] == 12) // make "calls*" to be "calls"
			{
				validTemp.relCond.push_back(11);
			}
			else if(content[i] == 53 || content[i] == 54 || content[i] == 55) // make "assign", "while", "if" to be "stmt" 
			{
				//cout<<" I am inside this 53 54 55!"<<endl;
				if(content.at(0) == 15 || content.at(0) == 16)
					validTemp.relCond.push_back(content[i]);
				else
					validTemp.relCond.push_back(51);
			}
			else
			{
				validTemp.relCond.push_back(content[i]);
			}
			//cout<<validTemp.relCond[i]<<" ###########AFTER CHANGE"<<endl;
			
		}
		validTemp.argumentNoCorrect = SuchThatTemp.argumentNoCorrect;
		suchThatClauseValid.push_back(validTemp);


		/*
		cout<<SuchThatTemp.relCond[0]<<" ";
		cout<<SuchThatTemp.relCond[1]<<" ";
		cout<<SuchThatTemp.relCond[2]<<" ";
		cout<<SuchThatTemp.relCond[3]<<" ";
		cout<<SuchThatTemp.relCond[4]<<" ";
		*/
		//cout<<SuchThatTemp.argumentNoCorrect<<" ";
		//cout<<endl;
		

		//add the content to the withVector for display
		if(suchThatVector.empty() == true)
		{
			suchThatVector.push_back(2);
		}
		else 
		{
			suchThatVector.push_back(-1);
		}
		vector<int>::iterator it;
		for (it = content.begin(); it < content.end(); it++)
		{
			suchThatVector.push_back(*it);
		}
		
	}
	else if(type == 3) // with
	{
		With withTemp;
		With validTemp;
		if((int) content.size() == 7 || (int) content.size() == 9)
		{
			withTemp.argumentNoCorrect = true;
		}
		else
		{
			withTemp.argumentNoCorrect = false;
		}
		withTemp.attrCond = content;
		withClause.push_back(withTemp);

		//add data for validation
		for(int i = 0; i< (int) content.size();i++)
		{

			//cout<<content[i]<<" ///////////////BEFORE CHANGE"<<endl;
			if(content[i] == 53 || content[i] == 54 || content[i] == 55) // make "assign", "while", "if" to be "stmt" 
			{
				//cout<<" I am inside this 53 54 55!"<<endl;
				validTemp.attrCond.push_back(51);
			}
			else
			{
				validTemp.attrCond.push_back(content[i]);
			}
			//cout<<validTemp.attrCond[i]<<" ###########AFTER CHANGE"<<endl;
			
		}
		validTemp.argumentNoCorrect = withTemp.argumentNoCorrect;
		withClauseValid.push_back(validTemp);
		
		//add the content to the withVector for display
		if(withVector.empty() == true)
		{
			withVector.push_back(WITH);
		}
		else 
		{
			withVector.push_back(-1);
		}
		vector<int>::iterator it;
		for (it = content.begin(); it < content.end(); it++)
		{
			withVector.push_back(*it);
		}
		
	
	}
	else if(type == PATTERN) // pattern type
	{
		Pattern_PQL patternTemp;

		if(int(content.size()) == 0) // nothing inside the content
		{
			throw new string ("the content is empty! -- throw by QueryTable::addClause");
		}
		else if(content.at(0) == ASSIGN) // assign pattern
		{
			if( !(int(content.size()) == 5 || int(content.size()) == 6 || int(content.size()) == 7) ) // size is 4 when first argument is _,  size is 5 when first argument is patternOfSimple
			{
				patternTemp.argumentNoCorrect = false;
			}
			else
			{
				patternTemp.argumentNoCorrect = true;
			}	
		}
		else if(content.at(0) == WHILE) // while pattern 
		{
			if( !(int(content.size()) == 5 || int(content.size()) == 6) ) 
			{
				patternTemp.argumentNoCorrect = false;
			}
			else
			{
				patternTemp.argumentNoCorrect = true;
			}	
		
		}
		else if(content.at(0) == IF) // while pattern
		{
			if( !(int(content.size()) == 7 || int(content.size()) == 8) ) 
			{
				patternTemp.argumentNoCorrect = false;
			}
			else
			{
				patternTemp.argumentNoCorrect = true;
			}	
		
		}

		patternTemp.expression = content;
		patternClause.push_back(patternTemp);

		//add the content to the withVector for display
		if(patternVector.empty() == true)
		{
			patternVector.push_back(PATTERN);
		}
		else 
		{
			patternVector.push_back(-1);
		}
		vector<int>::iterator it;
		for (it = content.begin(); it < content.end(); it++)
		{
			patternVector.push_back(*it);
		}
		

	
	}
	else // other value
	{
		throw  new string ("Wrong type of content! -- throw by QueryTable::addClause()");
		
	}


}
//////////////////////// add Clause --- End ///////////////////////////////

//////////////////////// getQuery --- Start ///////////////////////////////
vector<int> QueryTable::getQuery(){
	vector <int> queryHolder;
	//insert select part
	if(selectVector.empty() == false)
	{
		queryHolder.insert(queryHolder.end(), selectVector.begin(), selectVector.end()); 
		queryHolder.push_back(-2); // add a tail to select part	
	}

	//insert such that part
	if(suchThatVector.empty() == false)
	{
		queryHolder.insert(queryHolder.end(), suchThatVector.begin(), suchThatVector.end());
		queryHolder.push_back(-2); // add a tail to such that part
	}

	//insert with part
	if(withVector.empty() == false)
	{
		queryHolder.insert(queryHolder.end(), withVector.begin(), withVector.end());
		queryHolder.push_back(-2); // add a tail to with part
	}

	//insert pattern part
	if(patternVector.empty() == false)
	{
		queryHolder.insert(queryHolder.end(), patternVector.begin(), patternVector.end());
		queryHolder.push_back(-2); // add a tail to with part
	}
	return queryHolder;

}
//////////////////////// getQuery --- End ///////////////////////////////


//////////////////////// Clause Size  --- Start ///////////////////////////////
int QueryTable::selectSize(){
		
	return (int) selectClause.size();
}

int QueryTable::suchThatSize(){

	return (int) suchThatClause.size();
}
int QueryTable::withSize(){

	return (int) withClause.size();
}
int QueryTable::patternSize(){

	return (int) patternClause.size();
}
//////////////////////// Clause Size  --- End ///////////////////////////////

//////////////////////// selectAt + suchThatAT + withAT + patternAt--- Start ///////////////////////////////
void QueryTable::selectAt(vector<int> &tupleTemp, int index){
	
	
	//check whether index is out of range (need to throw exception, make it as function)
	if(index >= (int) selectClause.size())
	{
		throw  new string ("Index Out of Range inside the Select Clause! -- throw by QueryTable::selectAt()");
	}
	else
	{
		tupleTemp = selectClause.at(index).tuple;
	}

}

void QueryTable::suchThatAt(vector<int> &relCondTemp, int index){

	

	if(index >= (int) suchThatClause.size())
	{
		throw  new string("Index Out of Range inside the Such That Clause! -- throw by QueryTable::suchThatAt()");
	}
	else
	{
		relCondTemp = suchThatClause.at(index).relCond;
	}

}

void QueryTable::withAt(vector<int> &attrCondTemp,int index){

	
	if(index >= (int) withClause.size())
	{
		throw new string ("Index Out of Range inside the With Clause! --  throw by QueryTable::withAt");
	}
	else
	{
		attrCondTemp = withClause.at(index).attrCond;
	}

	

}

void QueryTable::patternAt(vector<int> &expression, int index){


	if(index >= (int) patternClause.size())
	{
		throw new string ("Index Out of Range inside the Pattern_PQL Clause! --  throw by QueryTable::patternAt");
	}
	else
	{
		expression = patternClause.at(index).expression;
	}

}
//////////////////////// selectAt  + suchThatAT + withAT --- End ///////////////////////////////


/////////////////////////Get Clause --- Start ///////////////////////////
vector < Select > QueryTable::getSelectClause(){
	return selectClause;
}

vector < SuchThat > QueryTable::getSuchThatClause(){
	return suchThatClause;
}

vector < With > QueryTable::getWithClause(){
	return withClause;
}

vector < Pattern_PQL > QueryTable::getPattern_PQLClause(){
	return patternClause;
}
/////////////////////////Get Clause --- Start ///////////////////////////

//////////////////////// Get Validation Clause --- Start/////////////////
vector < SuchThat >  QueryTable::getSuchThatClauseV(){

	return suchThatClauseValid;
}

vector < With >  QueryTable::getWithClauseV(){
	return withClauseValid;
}
//////////////////////// Get Validation Clause --- End/////////////////

/////////////////////////Get Clause --- End ///////////////////////////

//////////////////////// Show Table  --- Start ////////////////////////
void QueryTable::showTable()
{
	vector<int> temHolder = getQuery();
	for(int i=0;i<(int) temHolder.size();i++)
	{
		cout<<temHolder[i]<<" ";
	}
	cout<<endl;

}
//////////////////////// Show Table  --- End ////////////////////////