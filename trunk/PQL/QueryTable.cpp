#include <iostream>
#include "QueryTable.h"

QueryTable::QueryTable(){
	withCounter = 0;
	patternCounter = 0;	
	affectsStarCounter = 0;
	affectsCounter = 0;
}


//////////////////////// add Clause --- Start ///////////////////////////////
void QueryTable::addClause(int type, vector<int> content){

	if(type == 1) // select
	{
		//for display -- debugging******
		if(selectVector.empty())
		{
			selectVector.push_back(1);
		}

		Select selectTemp;
		SelectNode selectNode;
		if(content.front() == 60) // content is boolean
		{
		
			selectTemp.tuple = content;

			selectClause.push_back(selectTemp);

			selectNode.prefix = BOOLEAN;
			selectNode.argument = BOOLEAN;

			//for display -- debugging******
			selectVector.push_back(content.front());

			//for Optimizer ****************
			selectNodeList.push_back(selectNode);

		}
		else  //content is tuple
		{   
		
			selectTemp.tuple = content;
			selectClause.push_back(selectTemp);
			
			//for Optimizer ****************
			selectNode.prefix = content.at(0);
			selectNode.argument = content.at(1);
			selectNodeList.push_back(selectNode);

			//for display -- debugging******
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
		QueryNode queryNode;


		SuchThatTemp.relCond = content;
		SuchThatTemp.argumentNoCorrect = false;

		if((int)SuchThatTemp.relCond.size() == 5)
		{
			SuchThatTemp.argumentNoCorrect = true;
			
			//for Optimizer ****************
			queryNode.type = SuchThatTemp.relCond.at(0);
			queryNode.prefix1 = SuchThatTemp.relCond.at(1);
			queryNode.argument1 = SuchThatTemp.relCond.at(2);
			queryNode.prefix2 = SuchThatTemp.relCond.at(3);
			queryNode.argument2 = SuchThatTemp.relCond.at(4);
			queryNode.id = -1;
			queryNode.ranking = 0;

		}
		else if(SuchThatTemp.relCond.size() == 4)
		{
			
			if( (SuchThatTemp.relCond[1] == UNDERSCORE && SuchThatTemp.relCond[2]!= UNDERSCORE) )
			{
				SuchThatTemp.argumentNoCorrect = true;

				//for Optimizer ****************
				queryNode.type = SuchThatTemp.relCond.at(0);
				queryNode.prefix1 = SuchThatTemp.relCond.at(1);
				queryNode.argument1 = SuchThatTemp.relCond.at(1);
				queryNode.prefix2 = SuchThatTemp.relCond.at(2);
				queryNode.argument2 = SuchThatTemp.relCond.at(3);
				queryNode.id = -1;
				queryNode.ranking = 0;
			}
			if((SuchThatTemp.relCond[1] != UNDERSCORE && SuchThatTemp.relCond[3] == UNDERSCORE) )
			{
				SuchThatTemp.argumentNoCorrect = true;

				//for Optimizer ****************
				queryNode.type = SuchThatTemp.relCond.at(0);
				queryNode.prefix1 = SuchThatTemp.relCond.at(1);
				queryNode.argument1 = SuchThatTemp.relCond.at(2);
				queryNode.prefix2 = SuchThatTemp.relCond.at(3);
				queryNode.argument2 = SuchThatTemp.relCond.at(3);
				queryNode.id = -1;
				queryNode.ranking = 0;
			}
		}
		else if(SuchThatTemp.relCond.size() == 3)
		{
			if(SuchThatTemp.relCond[1] == UNDERSCORE && SuchThatTemp.relCond[2] == UNDERSCORE)
			{
				SuchThatTemp.argumentNoCorrect = true;
					
				//for Optimizer ****************
				queryNode.type = SuchThatTemp.relCond.at(0);
				queryNode.prefix1 = SuchThatTemp.relCond.at(1);
				queryNode.argument1 = SuchThatTemp.relCond.at(1);
				queryNode.prefix2 = SuchThatTemp.relCond.at(2);
				queryNode.argument2 = SuchThatTemp.relCond.at(2);
				queryNode.id = -1;
				queryNode.ranking = 0;
			}
		}

		if(SuchThatTemp.relCond.at(0) == AFFECTS)
		{
			affectsCounter++;
		}
		else if(SuchThatTemp.relCond.at(0) == AFFECTS_T)
		{
			affectsStarCounter++;
		}
		
		suchThatClause.push_back(SuchThatTemp);

		//for Optimizer **************** add to the queryNodeList
		queryNodeList.push_back(queryNode);

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
		QueryNode queryNode;
		int id;
		withTemp.attrCond = content;
		withCounter = withCounter + 1;
		id = withCounter+ WITHCOUNTER;

		//cout<<"counter: "<<withCounter<<endl;

		if((int) withTemp.attrCond.size() == 7 || (int) withTemp.attrCond.size() == 9)
		{
			withTemp.argumentNoCorrect = true;
	
			// for Optimizer ***************
			queryNode.type = WITH;
			queryNode.prefix1 = withTemp.attrCond.at(0);
			queryNode.argument1 = withTemp.attrCond.at(1);
			queryNode.prefix2 = withTemp.attrCond.at(5);
			queryNode.argument2 = withTemp.attrCond.at(6);
			queryNode.id = id;
			queryNode.ranking = 0;
			//push it into map
			withRelationMap.insert(keyContentPair(queryNode.id,withTemp.attrCond));



		}
		else
		{
			withTemp.argumentNoCorrect = false;
		}
		
		withClause.push_back(withTemp);
		
		//for Optimizer *******************
		queryNodeList.push_back(queryNode);
		
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
		QueryNode queryNode;
		patternCounter = patternCounter + 1;
		int id = patternCounter +  PATTERNCOUNTER;

		if(int(content.size()) == 0) // nothing inside the content
		{
			throw new string ("the content is empty! -- throw by QueryTable::addClause");
		}
		else if(content.at(0) == ASSIGN) // assign pattern
		{
			if(int(content.size()) == 5)
			{
				patternTemp.argumentNoCorrect = true;
				
				queryNode.prefix1 = content.at(0);
				queryNode.argument1 = content.at(1);
				queryNode.prefix2 = id;
				queryNode.argument2 = id;
				queryNode.id = id;
				queryNode.type = PATTERN;
				queryNode.ranking = -10;
				patternRelationMap.insert(keyContentPair(queryNode.id,content));
			}
			else if(int(content.size()) == 6)
			{
				patternTemp.argumentNoCorrect = true;				
				queryNode.prefix1 = content.at(0);
				queryNode.argument1 = content.at(1);
				queryNode.id = id;
				queryNode.type = PATTERN;
				queryNode.ranking = -10;

				if(content.at(2)!=UNDERSCORE)
				{
					queryNode.prefix2 = content.at(2);
					queryNode.argument2 = content.at(3);		
				}
				else
				{
					queryNode.prefix2 = id;
					queryNode.argument2 = id;	
				}

				patternRelationMap.insert(keyContentPair(queryNode.id,content));
			
			}
			else if(int(content.size()) == 7)
			{				
				patternTemp.argumentNoCorrect = true;
		
				queryNode.prefix1 = content.at(0);
				queryNode.argument1 = content.at(1);
				queryNode.prefix2 = content.at(2);
				queryNode.argument2 = content.at(2);
				queryNode.id = id;
				queryNode.type = PATTERN;
				queryNode.ranking = -10;
				patternRelationMap.insert(keyContentPair(queryNode.id,content));
			}
			else
			{
				patternTemp.argumentNoCorrect = false;
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
				queryNode.prefix1 = content.at(0);
				queryNode.argument1 = content.at(1);
				queryNode.prefix2 = id;
				queryNode.argument2 = id;
				queryNode.id = id;
				queryNode.type = PATTERN;
				queryNode.ranking = -10;
				patternRelationMap.insert(keyContentPair(queryNode.id,content));

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
				queryNode.prefix1 = content.at(0);
				queryNode.argument1 = content.at(1);
				queryNode.prefix2 = id;
				queryNode.argument2 = id;
				queryNode.id = id;
				queryNode.type = PATTERN;
				queryNode.ranking = -10;
				patternRelationMap.insert(keyContentPair(queryNode.id,content));
			}	
		
		}
		else
		{
			throw new string ("the content is empty! -- throw by QueryTable::addClause");
			
		}

	

		patternTemp.expression = content;
		patternClause.push_back(patternTemp);

		//add to queryNodeList
		queryNodeList.push_back(queryNode);

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
	return int(selectClause.size());
}


int QueryTable::suchThatSize(){

	return int(relatedSuchThat.size());
}
int QueryTable::suchThatUnrelatedSize(){

	return int(unRelatedSuchThat.size());
}


int QueryTable::withSize(){
	
	return int(relatedWith.size());
}
int QueryTable::withUnrelatedSize(){
	
	return int(unRelatedWith.size());
}


int QueryTable::patternSize(){

	return int(relatedPattern.size());

}
int QueryTable::patternUnrelatedSize(){
	
	return int(unRelatedPattern.size());
}





//////////////////////// Clause Size  --- End ///////////////////////////////////////////

//////////////////////// selectAt + suchThatAT  + withAT + patternAt ( + unrelated ) --- Start ///////////////////////////////

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

	
	if(index >= int(relatedSuchThat.size()))
	{
		throw  new string("Index Out of Range inside the Such That Clause! -- throw by QueryTable::suchThatAt()");
	}
	else
	{
		relCondTemp = relatedSuchThat.at(index);
	}

}
void QueryTable::suchThatUnrelatedAt(vector<int> &relCondTemp, int index){
	
	if(index >= int(unRelatedSuchThat.size()))
	{
		throw  new string("Index Out of Range inside the Such That Unrelated Clause! -- throw by QueryTable::suchThatUnrelatedAt()");
	}
	else
	{
		relCondTemp = unRelatedSuchThat.at(index);
	}

}


void QueryTable::withAt(vector<int> &attrCondTemp,int index){

	cout<<"withAT size "<<relatedWith.size()<<endl;
	cout<<"index "<<index<<endl;
	if(index >= int(relatedWith.size()))
	{
		throw new string ("Index Out of Range inside the With Clause! --  throw by QueryTable::withAt");
	}
	else
	{
		attrCondTemp = relatedWith.at(index);
	}

}

void QueryTable::withUnrelatedAt(vector<int> &attrCondTemp, int index){

	if(index >= int(unRelatedWith.size()))
	{
		throw new string ("Index Out of Range inside the With Unrelated Clause! --  throw by QueryTable::withUnrelatedAt");
	}
	else
	{
		attrCondTemp = unRelatedWith.at(index);
	}
	
}

void QueryTable::patternAt(vector<int> &expression, int index){


	if(index >= int(relatedPattern.size()))
	{
		throw new string ("Index Out of Range inside the Pattern_PQL Clause! --  throw by QueryTable::patternAt");
	}
	else
	{
		expression = relatedPattern.at(index);
	}

}

void QueryTable::patternUnrelatedAt(vector<int> &expression, int index){

	if(index >= int(unRelatedPattern.size()))
	{
		throw new string ("Index Out of Range inside the Pattern_PQL Clause! --  throw by QueryTable::patternUnrelatedAt");
	}
	else
	{
		expression =  unRelatedPattern.at(index);
	}
}

//////////////////////// selectAt  + suchThatAT + withAT ( + unrelated ) --- End ///////////////////////////////


/////////////////////////////////// Affects size and Affects* Start ////////////////////////////////////////
int QueryTable::affectsSize(){

	return affectsCounter;
}
int QueryTable::affectsStarSize(){

	return affectsStarCounter;
}


/////////////////////////////////// Affects size and Affects* Start End ////////////////////////////////////


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


/////////////////////////////// OPTIMIZER START /////////////////////////////
void QueryTable::displayNodeList(){

	cout<<"SelectNodeList: "<<endl;
	for(int i =0;i<int(selectNodeList.size());i++)
	{
		cout<<selectNodeList.at(i).prefix<<" "<<selectNodeList.at(i).argument<<endl;
	}

	cout<<"QueryNodeList: "<<endl;

	for(int i =0;i<int(queryNodeList.size());i++)
	{
		cout<<queryNodeList.at(i).type<<" "<<queryNodeList.at(i).prefix1<<" "<<queryNodeList.at(i).argument1<<" "<<queryNodeList.at(i).prefix2<<" "<<queryNodeList.at(i).argument2<<endl;
	}
	cout<<"relatedNodes: "<<endl;
	for(int j = 0;j<int(relatedNodes.size());j++)
	{
		for(int i =0;i<int(relatedNodes.at(j).size());i++)
		{
			cout<<relatedNodes.at(j).at(i).type<<" "<<relatedNodes.at(j).at(i).prefix1<<" "<<relatedNodes.at(j).at(i).argument1<<" "<<relatedNodes.at(j).at(i).prefix2<<" "<<relatedNodes.at(j).at(i).argument2<<endl;
		}
		cout<<"================================="<<endl;
	}

	cout<<"UNrelatedNodes: "<<endl;
	for(int j = 0;j<int(unrelatedNodes.size());j++)
	{
		for(int i =0;i<int(unrelatedNodes.at(j).size());i++)
		{
			cout<<unrelatedNodes.at(j).at(i).type<<" "<<unrelatedNodes.at(j).at(i).prefix1<<" "<<unrelatedNodes.at(j).at(i).argument1<<" "<<unrelatedNodes.at(j).at(i).prefix2<<" "<<unrelatedNodes.at(j).at(i).argument2<<endl;
		}
		cout<<"================================="<<endl;
	}

}
void QueryTable::displayPartitions(){

	for(int i = 0; i<int(unrelatedGroup.size());i++)
	{
		cout<<"Partition No. "<<i <<"'s Total Mark      is:"<<unrelatedGroup.at(i).totalMarks<<endl;
		//cout<<"Partition No. "<<i <<"'s Pattern No.     is:"<<unrelatedGroup.at(i).noOfPatterns<<endl;
		//cout<<"Partition No. "<<i <<"'s Constant No.    is:"<<unrelatedGroup.at(i).noOfConstants<<endl;
		//cout<<"Partition No. "<<i <<"'s Underscore No.  is:"<<unrelatedGroup.at(i).noOfUnderscores<<endl;
		//cout<<"Partition No. "<<i <<"'s Relation No.    is:"<<unrelatedGroup.at(i).noOfRelations<<endl;
		//cout<<"Partition No. "<<i <<"'s Star No.        is:"<<unrelatedGroup.at(i).noOfStars<<endl;

		for(int j=0;j<int(unrelatedGroup.at(i).partition.size());j++)
		{
			cout<<unrelatedGroup.at(i).partition.at(j).type<<" "<<unrelatedGroup.at(i).partition.at(j).prefix1<<" "<<unrelatedGroup.at(i).partition.at(j).argument1<<" "<<unrelatedGroup.at(i).partition.at(j).prefix2<<" "<<unrelatedGroup.at(i).partition.at(j).argument2<<endl;
		}
		cout<<endl;
	}

}

void QueryTable::findPartitionOne(){	

	vector<QueryNode> tempDirectRelated;
	//Loop 1 to find the direct related QueryNodes
	for(int i=0;i<int(selectNodeList.size());i++)
	{
		int nodeValue = selectNodeList.at(i).argument;
		vector<int> removedPosition;

		for(int j = 0; j<int(queryNodeList.size());j++)
		{
			QueryNode tempNode;

			if(queryNodeList.at(j).prefix1 != INT && queryNodeList.at(j).prefix1 != PROCOFSIMPLE && queryNodeList.at(j).prefix1 != VAROFSIMPLE)
			{
				//cout<<"queryNodeList.at(j).argument1: " <<queryNodeList.at(j).argument1<<endl;
				if(queryNodeList.at(j).argument1 == nodeValue )
				{
					tempNode = queryNodeList.at(j);
					tempDirectRelated.push_back(tempNode);
					removedPosition.push_back(j);

					//if it is not constant, we insert it into the set
					if(tempNode.prefix1 != INT && tempNode.prefix1 != PROCOFSIMPLE && tempNode.prefix1 != VAROFSIMPLE)
					{
						existNodeValue.insert(tempNode.argument1);
					}
					if(tempNode.prefix2 != INT && tempNode.prefix2 != PROCOFSIMPLE && tempNode.prefix2 != VAROFSIMPLE)	
					{	
						existNodeValue.insert(tempNode.argument2);
					}
					continue;							
				}
			
			}
			if(queryNodeList.at(j).prefix2 != INT && queryNodeList.at(j).prefix2 != PROCOFSIMPLE && queryNodeList.at(j).prefix2 != VAROFSIMPLE)
			{

				//cout<<"queryNodeList.at(j).argument2: " <<queryNodeList.at(j).argument2<<endl;
				if(queryNodeList.at(j).argument2 == nodeValue )
				{
					tempNode = queryNodeList.at(j);
					tempDirectRelated.push_back(tempNode);
					removedPosition.push_back(j);

					//if it is not constant, we insert it into the set
					if(tempNode.prefix1 != INT && tempNode.prefix1 != PROCOFSIMPLE && tempNode.prefix1 != VAROFSIMPLE)
					{
						existNodeValue.insert(tempNode.argument1);
					}
					if(tempNode.prefix2 != INT && tempNode.prefix2 != PROCOFSIMPLE && tempNode.prefix2 != VAROFSIMPLE)	
					{	
						existNodeValue.insert(tempNode.argument2);
					}
				}
			
			}
		
		}
		if(!tempDirectRelated.empty())
		{
			relatedNodes.push_back(tempDirectRelated);
		}

		//clear all the contents inside the vector
		tempDirectRelated.clear();
		
		//to remove all the value that has been pushed into related table
		for(int k =0; k<int(removedPosition.size());k++)
		{
			//cout<<removedPosition.at(k)<<" ";
			
			queryNodeList.erase(queryNodeList.begin()+removedPosition.at(k));
			for(int l = k+1;l<int(removedPosition.size());l++)
			{
				removedPosition.at(l)--;
			}

		}
		cout<<endl;
	
	}
	
	findPartitionTwo();
}

void QueryTable::findPartitionTwo(){

	
	for(int i=0; i<int(relatedNodes.size());i++)
	{
		findPartitionThree(existNodeValue,queryNodeList,relatedNodes.at(i));
	}	

}

void QueryTable::findPartitionThree(set<int> &sourceExistNode, vector<QueryNode> &sourceQueryNode, vector<QueryNode> &nodeListContainer){	
	bool found = false;

	for(int i=0;i<int(sourceQueryNode.size());i++)
	{
		set<int>::iterator itArgument1;
		set<int>::iterator itArgument2;

		vector<int> removePosition;
		itArgument1 = sourceExistNode.find(sourceQueryNode.at(i).argument1);
		itArgument2 = sourceExistNode.find(sourceQueryNode.at(i).argument2);


		//if found any value in queryNodeList,we store it into the 
		if(itArgument1 != sourceExistNode.end() || itArgument2 != sourceExistNode.end())
		{
		
			found = true;
			removePosition.push_back(i);

		}	

		if(found)
		{
			//add the node to related nodelist
			nodeListContainer.push_back(sourceQueryNode.at(i));

			//if it is not constant, we insert it into the set
			if(sourceQueryNode.at(i).prefix1 != INT && sourceQueryNode.at(i).prefix1 != PROCOFSIMPLE && sourceQueryNode.at(i).prefix1 != VAROFSIMPLE)
			{
				sourceExistNode.insert(sourceQueryNode.at(i).argument1);
			}
			if(sourceQueryNode.at(i).prefix2 != INT && sourceQueryNode.at(i).prefix2 != PROCOFSIMPLE && sourceQueryNode.at(i).prefix2 != VAROFSIMPLE)	
			{	
				sourceExistNode.insert(sourceQueryNode.at(i).argument2);
			}

			//remove the FOUNDED related node from the source node list
			for(int k =0; k<int(removePosition.size());k++)
			{
				//cout<<removedPosition.at(k)<<" ";
				sourceQueryNode.erase(sourceQueryNode.begin()+removePosition.at(k));
			}
			break;	
		}
	}

	if(found)
	{
		findPartitionThree(sourceExistNode, sourceQueryNode, nodeListContainer);
	}
	else
	{
		// this is base case, there is no related nodes for current node
		return;
	}
}




void QueryTable::findPartitionFour(){

	if(queryNodeList.empty())
	{
		return;
	}
	else
	{
		QueryNode tempNode;
		set<int> existUnrelatedNode;
		vector<QueryNode> tempUnrealtedNodeList;
		//assign first node as the root
		tempNode = queryNodeList.at(0);
		queryNodeList.erase(queryNodeList.begin());
		

		tempUnrealtedNodeList.push_back(tempNode);
		//if it is not constant, we insert it into the set
		if(tempNode.prefix1 != INT && tempNode.prefix1 != PROCOFSIMPLE && tempNode.prefix1 != VAROFSIMPLE)
		{
			existUnrelatedNode.insert(tempNode.argument1);
		}
		if(tempNode.prefix2 != INT && tempNode.prefix2 != PROCOFSIMPLE && tempNode.prefix2 != VAROFSIMPLE)	
		{	
			existUnrelatedNode.insert(tempNode.argument2);
		}
		
		//call recursive function to find the partition
		findPartitionThree(existUnrelatedNode, queryNodeList,tempUnrealtedNodeList);
		
		//add the Partition to unrelatedNodes Table
		unrelatedNodes.push_back(tempUnrealtedNodeList);

		//call itself for next root 
		findPartitionFour();
	}

}


void QueryTable::calculatePartitionMark(const  vector<vector<QueryNode>> &nodeList, vector<Partitions> &group){
	
	for(int i=0;i<int(nodeList.size());i++)
	{
		Partitions tempPartition;

		tempPartition.noOfConstants = 0;
		tempPartition.noOfRelations = int(nodeList.at(i).size());
		tempPartition.noOfStars = 0;
		tempPartition.noOfUnderscores = 0;
		tempPartition.noOfPatterns = 0;
		tempPartition.totalMarks = 0;
		tempPartition.partition = nodeList.at(i);

		for(int j=0; j< int(nodeList.at(i).size());j++)
		{
			//see if there is any constants
			if(nodeList.at(i).at(j).prefix1 == INT || nodeList.at(i).at(j).prefix1 == PROCOFSIMPLE || nodeList.at(i).at(j).prefix1 == VAROFSIMPLE)
			{
				tempPartition.noOfConstants++;
			}
			if(nodeList.at(i).at(j).prefix2 == INT || nodeList.at(i).at(j).prefix2 == PROCOFSIMPLE || nodeList.at(i).at(j).prefix2 == VAROFSIMPLE)
			{
				tempPartition.noOfConstants++;
				
			}
			//see if there is any underscore
			if(nodeList.at(i).at(j).prefix1 == UNDERSCORE)
			{
				tempPartition.noOfUnderscores++;
			}
			if(nodeList.at(i).at(j).prefix2 == UNDERSCORE)
			{
				tempPartition.noOfUnderscores++;
			}
			//see if there is any star relations
			if(nodeList.at(i).at(j).type == AFFECTS_T || nodeList.at(i).at(j).type == NEXT_T || nodeList.at(i).at(j).type == PARENT_T || nodeList.at(i).at(j).type == FOLLOWS_T || nodeList.at(i).at(j).type == CALLS_T)
			{
				tempPartition.noOfStars++;
			
			}
			//see if there is any pattern
			if(nodeList.at(i).at(j).type  == PATTERN)
			{
				tempPartition.noOfPatterns++;
			}
			
		}
		//calculate total markes;
		tempPartition.totalMarks = tempPartition.noOfConstants*(5) + tempPartition.noOfUnderscores*(1) + tempPartition.noOfStars*(-3) + tempPartition.noOfRelations*(-1)+ tempPartition.noOfPatterns*(4);
		group.push_back(tempPartition);
	}

}

void QueryTable::sortPartitions(vector<Partitions> &partition){ 

	sort(partition.begin(),partition.end(),myComparePartition);
	
	
}

bool QueryTable::myComparePartition(Partitions par1, Partitions par2){

	if(par1.totalMarks > par2.totalMarks)
		return true;
	else if(par1.totalMarks < par2.totalMarks)
		return false;

	return false;
}

void QueryTable::sortRelationsPartitions(vector<Partitions> &partition){

	for(int i = 0; i<int(partition.size());i++)
	{
		sortRelations(partition.at(i).partition);
	}

}


void QueryTable::sortRelations(vector<QueryNode> &queryNodes){
	
	vector<QueryNode> tempWithNodeList;
	
	
	for(int i=0;i<int(queryNodes.size());i++)
	{
		queryNodes.at(i).ranking = 0;
	}

	

	for(int i=0;i<int(queryNodes.size());i++)
	{
		//first argument is constant
		if(queryNodes.at(i).prefix1 == INT || queryNodes.at(i).prefix1 == PROCOFSIMPLE || queryNodes.at(i).prefix1 == VAROFSIMPLE)
		{
			if(queryNodes.at(i).prefix2 == INT || queryNodes.at(i).prefix2 == PROCOFSIMPLE || queryNodes.at(i).prefix2 == VAROFSIMPLE)
			{
				queryNodes.at(i).ranking = 6; // both of the argument are constants.
			
			}
			else
			{
				//only first argument is constant
				queryNodes.at(i).ranking = 3;
			}
		}
		//second argument is constant
		else if(queryNodes.at(i).prefix2 == INT || queryNodes.at(i).prefix2 == PROCOFSIMPLE || queryNodes.at(i).prefix2 == VAROFSIMPLE)
		{
			if(queryNodes.at(i).type == WITH)
			{
				// WITH type of relation and its right side is constant. it has highest ranking
				queryNodes.at(i).ranking = 10;
				tempWithNodeList.push_back(queryNodes.at(i));			
			}
			else
			{
				// only second argument is constant and it is not WITH type
				queryNodes.at(i).ranking = 3;
			}
		
		}
		// both arguments are not constant
		else 
		{
			if(queryNodes.at(i).type == WITH)
			{
				// WITH type of relation and its left and right side are not constant, it has second highest ranking 
				queryNodes.at(i).ranking = 9;
				tempWithNodeList.push_back(queryNodes.at(i));
			}
			else
			{
				//relations that has both argument are constant
				queryNodes.at(i).ranking = 1;
			}
		
		}
	}
	
	
	for(int i=0;i<int(queryNodes.size());i++)
	{
		if(queryNodes.at(i).type != WITH && queryNodes.at(i).ranking != 6)
		{
			//check any agrgument related to the with type relation 	
			for(int j=0; j<int(tempWithNodeList.size());j++)
			{
				if(tempWithNodeList.at(j).ranking == 10)
				{
					if(queryNodes.at(i).argument1 == tempWithNodeList.at(j).argument1 || queryNodes.at(i).argument1 == tempWithNodeList.at(j).argument2)
					{
						if(queryNodes.at(i).ranking == 3) // single argument is constant
						{
							queryNodes.at(i).ranking = 5;	
						}
						else //both arguments are not constant
						{
							queryNodes.at(i).ranking = 4;
						}
					}
					else if(queryNodes.at(i).argument2 == tempWithNodeList.at(j).argument1 || queryNodes.at(i).argument2 == tempWithNodeList.at(j).argument2)
					{
						if(queryNodes.at(i).ranking == 3) // single argument is constant
						{
							queryNodes.at(i).ranking = 5;	
						}
						else //both arguments are not constant
						{
							queryNodes.at(i).ranking = 4;
						}

					}
				}
				else if(tempWithNodeList.at(j).ranking == 9)
				{
					if(queryNodes.at(i).argument1 == tempWithNodeList.at(j).argument1 || queryNodes.at(i).argument1 == tempWithNodeList.at(j).argument2)
					{
						if(queryNodes.at(i).ranking == 3) // single argument is constant
						{
							queryNodes.at(i).ranking = 5;	
						}
						else //both arguments are not constant
						{
							queryNodes.at(i).ranking = 4;
						}
					}
					else if(queryNodes.at(i).argument2 == tempWithNodeList.at(j).argument1 || queryNodes.at(i).argument2 == tempWithNodeList.at(j).argument2)
					{
						if(queryNodes.at(i).ranking == 3) // single argument is constant
						{
							queryNodes.at(i).ranking = 5;	
						}
						else //both arguments are not constant
						{
							queryNodes.at(i).ranking = 4;
						}					
					
					}
				}
			}
	
		}
	
	}

	/*
	for(int i=0;i<int(queryNodes.size());i++)
	{
		cout<<"/////////////////////////////////////////////////////////////////////////"<<endl;
		cout<<queryNodes.at(i).type<<" "<<queryNodes.at(i).prefix1<<" "<<queryNodes.at(i).argument1<<" "<<queryNodes.at(i).prefix2<<" "<<queryNodes.at(i).argument2<<endl;
		cout<<"Ranking is "<<queryNodes.at(i).ranking<<endl;
	}
	*/
	//TODO 
	
	sort(queryNodes.begin(),queryNodes.end(),myCompareRelation);
}


bool QueryTable::myCompareRelation(QueryNode node1, QueryNode node2){
	if(node1.ranking > node2.ranking)
		return true;
	else if(node1.ranking < node2.ranking)
		return false;

	return false;
}

void QueryTable::findPartition(){
	
	// find all the related nodes and put them into the related group
	findPartitionOne();
	
	// find all the unrelated nodes and put them into unrealted group;
	findPartitionFour();

	// calculated the mark for each partition.
	//calculatedPartitionMarks();
	calculatePartitionMark(unrelatedNodes,unrelatedGroup);
	calculatePartitionMark(relatedNodes,relatedGroup);

	// sort the partitions according to the total marks.
	sortPartitions(unrelatedGroup);
	sortPartitions(relatedGroup);
	
	// sort unrelated relation Within the Partition
	sortRelationsPartitions(unrelatedGroup);
	sortRelationsPartitions(relatedGroup);

	//add to the unrelated table
	//addToUnrelatedTable();
	addToTable(unrelatedGroup, unRelatedWith, unRelatedPattern, unRelatedSuchThat);
	addToTable(relatedGroup, relatedWith, relatedPattern, relatedSuchThat);


	//add to teh related table
	//addToRelatedTable();

	//cout<<endl<<endl;
	

	displayAll();
	
	//display the node list
	//displayNodeList();

	//display the partition
	displayPartitions();

}

void QueryTable::addToUnrelatedTable(){

	vector<int> tempRelations;
	vector<int> partitionDivider;
	vector<int> emptyIndicator;
	
	partitionDivider.push_back(-1);
	emptyIndicator.push_back(-2);

	for(int i = 0; i<int(unrelatedGroup.size());i++)
	{
		bool hasWith  = false;
		bool hasPattern = false;
		bool hasSuchThat = false;
		hash_map<int, vector<int>>::iterator it;


		for(int j=0;j<int(unrelatedGroup.at(i).partition.size());j++)
		{
			/*
			cout<<"unrelatedGroup.at(i).partition.at(j).type: "<<unrelatedGroup.at(i).partition.at(j).type<<endl;
			cout<<"unrelatedGroup.at(i).partition.at(j).prefix1 " <<unrelatedGroup.at(i).partition.at(j).prefix1<<endl;
			cout<<"unrelatedGroup.at(i).partition.at(j).argument1 " <<unrelatedGroup.at(i).partition.at(j).argument1<<endl;
			cout<<"unrelatedGroup.at(i).partition.at(j).prefix2 " <<unrelatedGroup.at(i).partition.at(j).prefix2<<endl;
			cout<<"unrelatedGroup.at(i).partition.at(j).argument2 " <<unrelatedGroup.at(i).partition.at(j).argument2<<endl;*/
			
			//the relation is with type
			if(unrelatedGroup.at(i).partition.at(j).type == WITH)
			{
				hasWith = true;
				//get it from with hash Map
				

				it = withRelationMap.find(unrelatedGroup.at(i).partition.at(j).id);

				tempRelations = it->second;		
				unRelatedWith.push_back(tempRelations);
				
			}
			//the relation is pattern 
			else if(unrelatedGroup.at(i).partition.at(j).type == PATTERN)
			{
				hasPattern = true;

				//hash_map<int, vector<int>>::iterator it

				it = patternRelationMap.find(unrelatedGroup.at(i).partition.at(j).id);

				tempRelations = it->second;		
				unRelatedPattern.push_back(tempRelations);
			}
			//the relation is such that relation
			else if(unrelatedGroup.at(i).partition.at(j).type >=5 && unrelatedGroup.at(i).partition.at(j).type <=16)
			{
				hasSuchThat = true;
				//first argument is UNDERSCORE and the second is not UNDERSCORE
				if( unrelatedGroup.at(i).partition.at(j).prefix1 == UNDERSCORE && unrelatedGroup.at(i).partition.at(j).prefix2 != UNDERSCORE)
				{
					tempRelations.push_back( unrelatedGroup.at(i).partition.at(j).type);
					tempRelations.push_back( unrelatedGroup.at(i).partition.at(j).argument1);
					tempRelations.push_back( unrelatedGroup.at(i).partition.at(j).prefix2);
					tempRelations.push_back( unrelatedGroup.at(i).partition.at(j).argument2);

					unRelatedSuchThat.push_back(tempRelations);
					
				}
				//first argument is not UNDERSCORE and the second is UNDERSCORE
				else if( unrelatedGroup.at(i).partition.at(j).prefix1 != UNDERSCORE && unrelatedGroup.at(i).partition.at(j).prefix2 == UNDERSCORE)
				{
					tempRelations.push_back( unrelatedGroup.at(i).partition.at(j).type);
					tempRelations.push_back( unrelatedGroup.at(i).partition.at(j).prefix1);
					tempRelations.push_back( unrelatedGroup.at(i).partition.at(j).argument1);
					tempRelations.push_back( unrelatedGroup.at(i).partition.at(j).argument2);
					unRelatedSuchThat.push_back(tempRelations);
					
				}
				// both arguments are not UNDRESCORE
				else if(unrelatedGroup.at(i).partition.at(j).prefix1 != UNDERSCORE && unrelatedGroup.at(i).partition.at(j).prefix2 != UNDERSCORE)
				{
					tempRelations.push_back( unrelatedGroup.at(i).partition.at(j).type);
					tempRelations.push_back( unrelatedGroup.at(i).partition.at(j).prefix1);
					tempRelations.push_back( unrelatedGroup.at(i).partition.at(j).argument1);
					tempRelations.push_back( unrelatedGroup.at(i).partition.at(j).prefix2);
					tempRelations.push_back( unrelatedGroup.at(i).partition.at(j).argument2);
					unRelatedSuchThat.push_back(tempRelations);
					
				}
				else
				{
				
					throw new string("no such type can be added into the unRelatedSuchThat Table --  throw by QueryTable::addToUnrelatedTable!");
				}
				
			
			}
			else
			{
				throw new string("The type is undefined in unRelatedGroup -- throw by QueryTable::addToUnrelatedTable!");
			
			}
			tempRelations.clear();
		}
		
		if(hasWith)
		{
			// add partitionDivider to unRelatedWith
			unRelatedWith.push_back(partitionDivider);
		}
		else
		{
			//add emptyIndicator to unRelatedWith
			unRelatedWith.push_back(partitionDivider);
			unRelatedWith.push_back(emptyIndicator);
		}

		if(hasPattern)
		{
			// add partitionDivider to unRelatedWith
			unRelatedPattern.push_back(partitionDivider);
		}
		else
		{
			//add emptyIndicator to unRelatedWith
			unRelatedPattern.push_back(partitionDivider);
			unRelatedPattern.push_back(emptyIndicator);
		}

		if(hasSuchThat)
		{
			// add partitionDivider to unRelatedWith
			unRelatedSuchThat.push_back(partitionDivider);
		}
		else
		{
			//add emptyIndicator to unRelatedWith
			unRelatedSuchThat.push_back(partitionDivider);
			unRelatedSuchThat.push_back(emptyIndicator);
		}

	}

}
void  QueryTable::addToTable(vector<Partitions> &group,vector<vector<int>> &withTable, vector<vector<int>> &patternTable, vector<vector<int>> &suchThatTable){

	vector<int> tempRelations;
	vector<int> partitionDivider;
	vector<int> emptyIndicator;
	
	partitionDivider.push_back(-1);
	emptyIndicator.push_back(-2);

	for(int i = 0; i<int(group.size());i++)
	{
		bool hasWith  = false;
		bool hasPattern = false;
		bool hasSuchThat = false;
		hash_map<int, vector<int>>::iterator it;


		for(int j=0;j<int(group.at(i).partition.size());j++)
		{
			/*
			cout<<"group.at(i).partition.at(j).type: "<<group.at(i).partition.at(j).type<<endl;
			cout<<"group.at(i).partition.at(j).prefix1 " <<group.at(i).partition.at(j).prefix1<<endl;
			cout<<"group.at(i).partition.at(j).argument1 " <<group.at(i).partition.at(j).argument1<<endl;
			cout<<"group.at(i).partition.at(j).prefix2 " <<group.at(i).partition.at(j).prefix2<<endl;
			cout<<"group.at(i).partition.at(j).argument2 " <<group.at(i).partition.at(j).argument2<<endl;
			*/
			
			//the relation is with type
			if(group.at(i).partition.at(j).type == WITH)
			{
				hasWith = true;
				//get it from with hash Map
				

				it = withRelationMap.find(group.at(i).partition.at(j).id);

				tempRelations = it->second;		
				withTable.push_back(tempRelations);
				
			}
			//the relation is pattern 
			else if(group.at(i).partition.at(j).type == PATTERN)
			{
				hasPattern = true;

				//hash_map<int, vector<int>>::iterator it

				it = patternRelationMap.find(group.at(i).partition.at(j).id);

				tempRelations = it->second;		
				patternTable.push_back(tempRelations);
			}
			//the relation is such that relation
			else if(group.at(i).partition.at(j).type >=5 && group.at(i).partition.at(j).type <=16)
			{
				hasSuchThat = true;
				//first argument is UNDERSCORE and the second is not UNDERSCORE
				if( group.at(i).partition.at(j).prefix1 == UNDERSCORE && group.at(i).partition.at(j).prefix2 != UNDERSCORE)
				{
					tempRelations.push_back( group.at(i).partition.at(j).type);
					tempRelations.push_back( group.at(i).partition.at(j).argument1);
					tempRelations.push_back( group.at(i).partition.at(j).prefix2);
					tempRelations.push_back( group.at(i).partition.at(j).argument2);

					suchThatTable.push_back(tempRelations);
					
				}
				//first argument is not UNDERSCORE and the second is UNDERSCORE
				else if( group.at(i).partition.at(j).prefix1 != UNDERSCORE && group.at(i).partition.at(j).prefix2 == UNDERSCORE)
				{
					tempRelations.push_back( group.at(i).partition.at(j).type);
					tempRelations.push_back( group.at(i).partition.at(j).prefix1);
					tempRelations.push_back( group.at(i).partition.at(j).argument1);
					tempRelations.push_back( group.at(i).partition.at(j).argument2);
					suchThatTable.push_back(tempRelations);
					
				}
				// both arguments are not UNDRESCORE
				else if(group.at(i).partition.at(j).prefix1 != UNDERSCORE && group.at(i).partition.at(j).prefix2 != UNDERSCORE)
				{
					tempRelations.push_back( group.at(i).partition.at(j).type);
					tempRelations.push_back( group.at(i).partition.at(j).prefix1);
					tempRelations.push_back( group.at(i).partition.at(j).argument1);
					tempRelations.push_back( group.at(i).partition.at(j).prefix2);
					tempRelations.push_back( group.at(i).partition.at(j).argument2);
					suchThatTable.push_back(tempRelations);
					
				}
				else
				{
				
					throw new string("no such type can be added into the suchThatTable Table --  throw by QueryTable::addToUnrelatedTable!");
				}
				
			
			}
			else
			{
				throw new string("The type is undefined in group -- throw by QueryTable::addToUnrelatedTable!");
			
			}
			tempRelations.clear();
		}
		
		if(hasWith)
		{
			// add partitionDivider to withTable
			withTable.push_back(partitionDivider);
		}
		else
		{
			//add emptyIndicator to withTable
			withTable.push_back(partitionDivider);
			withTable.push_back(emptyIndicator);
		}

		if(hasPattern)
		{
			// add partitionDivider to patternTable
			patternTable.push_back(partitionDivider);
		}
		else
		{
			//add emptyIndicator to patternTable
			patternTable.push_back(partitionDivider);
			patternTable.push_back(emptyIndicator);
		}

		if(hasSuchThat)
		{
			// add partitionDivider to suchThatTable
			suchThatTable.push_back(partitionDivider);
		}
		else
		{
			//add emptyIndicator to suchThatTable
			suchThatTable.push_back(partitionDivider);
			suchThatTable.push_back(emptyIndicator);
		}

	}

}

void QueryTable::addToRelatedTable(){
	
	vector<int> tempRelations;	
	for(int i = 0; i<int(relatedNodes.size());i++)
	{
		
		hash_map<int, vector<int>>::iterator it;

		for(int j=0;j<int(relatedNodes.at(i).size());j++)
		{
			if(relatedNodes.at(i).at(j).type == WITH)
			{
				
				//get it from with hash Map
				it = withRelationMap.find(relatedNodes.at(i).at(j).id);

				tempRelations = it->second;		
				relatedWith.push_back(tempRelations);
				
			}
			//the relation is pattern 
			else if(relatedNodes.at(i).at(j).type == PATTERN)
			{
				it = patternRelationMap.find(relatedNodes.at(i).at(j).id);

				tempRelations = it->second;		
				relatedPattern.push_back(tempRelations);
			}
			//the relation is such that relation
			else if(relatedNodes.at(i).at(j).type >=5 && relatedNodes.at(i).at(j).type <=16)
			{
				//first argument is UNDERSCORE and the second is not UNDERSCORE
				if( relatedNodes.at(i).at(j).prefix1 == UNDERSCORE && relatedNodes.at(i).at(j).prefix1 != UNDERSCORE)
				{
					tempRelations.push_back( relatedNodes.at(i).at(j).type);
					tempRelations.push_back( relatedNodes.at(i).at(j).argument1);
					tempRelations.push_back( relatedNodes.at(i).at(j).prefix2);
					tempRelations.push_back( relatedNodes.at(i).at(j).argument2);

					relatedSuchThat.push_back(tempRelations);
					
				}
				//first argument is not UNDERSCORE and the second is UNDERSCORE
				else if( relatedNodes.at(i).at(j).prefix1 != UNDERSCORE && relatedNodes.at(i).at(j).prefix2 == UNDERSCORE)
				{
					tempRelations.push_back( relatedNodes.at(i).at(j).type);
					tempRelations.push_back( relatedNodes.at(i).at(j).prefix1);
					tempRelations.push_back( relatedNodes.at(i).at(j).argument1);
					tempRelations.push_back( relatedNodes.at(i).at(j).argument2);

					relatedSuchThat.push_back(tempRelations);
					
				}
				// both arguments are not UNDRESCORE
				else if(relatedNodes.at(i).at(j).prefix1 != UNDERSCORE && relatedNodes.at(i).at(j).prefix2 != UNDERSCORE)
				{
					tempRelations.push_back( relatedNodes.at(i).at(j).type);
					tempRelations.push_back( relatedNodes.at(i).at(j).prefix1);
					tempRelations.push_back( relatedNodes.at(i).at(j).argument1);
					tempRelations.push_back( relatedNodes.at(i).at(j).prefix2);
					tempRelations.push_back( relatedNodes.at(i).at(j).argument2);

					relatedSuchThat.push_back(tempRelations);
					
				}
				else
				{
				
					throw new string("no such type can be added into the unRelatedSuchThat Table --  throw by QueryTable::addToUnrelatedTable!");
				}
				
			
			}
			else
			{
				throw new string("The type is undefined in relatedGroup -- throw by QueryTable::addToUnrelatedTable!");
			
			}
			tempRelations.clear();
		
		}	
	}
}
/////////////////////////////// OPTIMIZER END ///////////////////////////////


void QueryTable::displayAll(){

	cout<<"unRelatedWith: "<<endl;
	for(int i = 0;i<int(unRelatedWith.size());i++)
	{
		for(int j = 0; j<int(unRelatedWith.at(i).size());j++)
		{
		
			cout<<unRelatedWith.at(i).at(j)<<" ";
		}
		cout<<endl;
	}
	cout<<endl<<endl;

	cout<<"unRelatedPattern: "<<endl;
	for(int i = 0;i<int(unRelatedPattern.size());i++)
	{
		for(int j = 0; j<int(unRelatedPattern.at(i).size());j++)
		{
		
			cout<<unRelatedPattern.at(i).at(j)<<" ";
		}
		cout<<endl;
	}
	cout<<endl<<endl;

	
	cout<<"unRelatedSuchThat: "<<endl;
	for(int i = 0;i<int(unRelatedSuchThat.size());i++)
	{
		for(int j = 0; j<int(unRelatedSuchThat.at(i).size());j++)
		{
		
			cout<<unRelatedSuchThat.at(i).at(j)<<" ";
		}
		cout<<endl;
	}
	cout<<endl<<endl;

	
	cout<<"relatedWith: "<<endl;
	for(int i = 0;i<int(relatedWith.size());i++)
	{
		for(int j = 0; j<int(relatedWith.at(i).size());j++)
		{
		
			cout<<relatedWith.at(i).at(j)<<" ";
		}
		cout<<endl;
	}
	cout<<endl<<endl;


	cout<<"relatedPattern: "<<endl;
	for(int i = 0;i<int(relatedPattern.size());i++)
	{
		for(int j = 0; j<int(relatedPattern.at(i).size());j++)
		{
		
			cout<<relatedPattern.at(i).at(j)<<" ";
		}
		cout<<endl;
	}
	cout<<endl<<endl;


	cout<<"relatedSuchThat: "<<endl;
	for(int i = 0;i<int(relatedSuchThat.size());i++)
	{
		for(int j = 0; j<int(relatedSuchThat.at(i).size());j++)
		{
		
			cout<<relatedSuchThat.at(i).at(j)<<" ";
		}
		cout<<endl;
	}
	cout<<endl<<endl;

	//cout<<"display HASH_MAP With"<<endl;

	


}