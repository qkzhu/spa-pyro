#include "Validator.h"
#include "Tokenizer.h"

/////////////////////////////////////////////Populate Checking Table Start////////////////////////////////////////////////////
void Validator::fillTable(vector <vector<int>> &table, string fileName){
	Tokenizer str;

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

void Validator::populateTable(){
	fillTable(suchThatTable,"DesignModel.txt");
	fillTable(withTable,"withTable.txt");
}
///////////////////////////////////////////// Populate Checking Table End////////////////////////////////////////////////////

///////////////////////////////////////////// Check Select Start////////////////////////////////////////////////////
void Validator::checkSelect(QueryTable &table){

	int size = int (table.getSelectClause().size());
	Select tempSelect;
	
	if (table.getSelectClause().size() == 0)
	{
		throw new string("no attributes inside select clause -- throw by Validator::checkSelect ");
	}	
	// if BOOLEAN involved, there can only be one child in select clause
	if(table.getSelectClause().at(0).tuple.at(0) == 60)
	{
		if(int(table.getSelectClause().size())>1)
		{
			throw new string("there can only be one child if BOOLEAN inside select clause -- throw by Validator::checkSelect ");		
		}
	}

	for(int i =0; i<size;i++)
	{
		if(int(table.getSelectClause().at(i).tuple.size()) == 4)
		{
			//check whether the third digit is DOT or not
			if( table.getSelectClause().at(i).tuple.at(2)!= 156)
			{
				throw new string ("short of DOT ! -- throw by Validator::checkSelect -- size = 4");
			}
			//stmt, call, assign, while, if -- attrName: ‘stmt#’
			if (table.getSelectClause().at(i).tuple.at(0) == 51 || table.getSelectClause().at(i).tuple.at(0) == 53 || table.getSelectClause().at(i).tuple.at(0) == 54 || table.getSelectClause().at(i).tuple.at(0) == 55 || table.getSelectClause().at(i).tuple.at(0) == 59)
			{
				if(table.getSelectClause().at(i).tuple.at(3)!=104)
				{
					throw new string ("attribute error in select clause! -- throw by Validator::checkSelect -- size = 4");
				}	

			}
			//prog_line, constant c -- attrName: ‘value’
			else if(table.getSelectClause().at(i).tuple.at(0) == 52 || table.getSelectClause().at(i).tuple.at(0) == 56)
			{
				if(table.getSelectClause().at(i).tuple.at(3)!=101)
				{
					throw new string ("attribute error in select clause! -- throw by Validator::checkSelect  -- size = 4");
				}
			}
			//variable  -- attrName: ‘varName’
			else if (table.getSelectClause().at(i).tuple.at(0) == 57)
			{
				if(table.getSelectClause().at(i).tuple.at(3)!=102)
				{
					throw new string ("attribute error in select clause! -- throw by Validator::checkSelect  -- size = 4 ");
				}
		
			}
			//procedure  --- attrName: ‘procName’
			else if (table.getSelectClause().at(i).tuple.at(0) == 58)
			{
				if(table.getSelectClause().at(i).tuple.at(3)!=103)
				{
					throw new string ("attribute error in select clause! -- throw by Validator::checkSelect");
				}
			}
			else
			{
				throw new string ("first prefix undefined in select clause! -- throw by Validator::checkSelect  -- size = 4");
			
			}

		}
		else if(int(table.getSelectClause().at(i).tuple.size()) == 1)
		{
			if( !(table.getSelectClause().at(i).tuple.at(0) >=51 && table.getSelectClause().at(i).tuple.at(0) <=60))
			{
				throw new string ("first prefix undefined in select clause! -- throw by Validator::checkSelect -- size = 1");
			}
		}
		else
		{
		
			throw new string ("size error in select clause! -- throw by Validator::checkSelect -- size = 1");
		}
	}


	
}
///////////////////////////////////////////// Check Select End////////////////////////////////////////////////////

///////////////////////////////////////////// Check Such That Start////////////////////////////////////////////////////
vector<int> Validator::getIndex(vector<vector<int>> table, int name){

	vector<int> index;
	if((int)table.size() == 0)
	{
		throw "Query Validation Table is empty!  -- throw by Validator::getIndex()";
	}
	//cout<<suchThatTable[77][0]<<" getIndex"<<endl;
	for(int i=0;i<(int) table.size();i++)
	{
		if(table[i][0] == name)
		{
			index.push_back(i);
		}
		
	}
	return index;
}

void Validator::checkSuchThat(QueryTable &table){
	
	bool noError = true;
	int size = (int) table.getSuchThatClauseV().size();
	//cout<<"hahahh"<<size<<endl;

	//suchThat =table.suchThatAt(0);
	//cout<<suchThat[0];
	//index = getIndex(suchThatTable, suchThat[0]);

	
	for(int i=0; i<size; i++)
	{
		bool tempNoError = false;
		//cout<<"====="<<table.getSuchThatClauseV().at(i).argumentNoCorrect<<endl;
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
		/* for debug***
		for(int i=0;i<suchThat.size();i++)
		{
			cout<<suchThat.at(i)<<" "<<endl;
		}
		cout<<endl;
		*/
		vector<int>::iterator it;
		for(it = index.begin();it<index.end();it++)
		{
			if(suchThat.size() == 5)
			{
				//cout<<"index 1 in query  "<<suchThat.at(1)<<"   index  1 in table"<<suchThatTable[*it][1]<<endl;
				//cout<<"index 3 in query  "<<suchThat.at(3)<<"   index  2 in table"<<suchThatTable[*it][2]<<endl;
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

	if (!noError)
	{
		throw new string("argument error inside SuchThat clause! -- throw by Validator::checkSuchThat ");
	}
}
///////////////////////////////////////////// Check Such That End////////////////////////////////////////////////////

///////////////////////////////////////////// Check With Start////////////////////////////////////////////////////
void Validator::checkWith(QueryTable &table){
	bool noError = true;
	int size = (int) table.getWithClauseV().size();

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
		for(int it = 0;it<(int) index.size();it++)
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
	if (!noError)
	{
		throw new string("argument error inside With clause! -- throw by Validator::checkWith ");
	}
}
///////////////////////////////////////////// Check With Start////////////////////////////////////////////////////


///////////////////////////////////////////// Check Pattern Start //////////////////////////////////////////////////
void Validator::checkPattern(QueryTable &table){

	int size =  (int) table.getPatternClause().size();

	for(int i=0; i<size;i++)
	{
		bool tempNoError = false;

		if(table.getPatternClause().at(i).argumentNoCorrect == false)
		{
			throw new string("argument error inside pattern clause! -- throw by Validator::checkPattern - argumentNoCorrect is false");
			
		}
		vector<int> pattern;
		pattern = table.getPatternClause().at(i).expression;

		if(pattern.at(0) == ASSIGN)
		{
			if(int(pattern.size()) == 5) // pattern a(_,_); ASSIGN 301 UNDERSCORE COMMA UNDERSCORE
			{
				//cout<< pattern.at(2)<<" "<<pattern.at(3)<<" "<<pattern.at(4)<<endl;
				if( pattern.at(2) != UNDERSCORE || pattern.at(3) != COMMA || pattern.at(4) != UNDERSCORE )
				{
					throw new string("argument error inside pattern clause! -- throw by Validator::checkPattern - pattern size 5");
				}
			}
			else if(int(pattern.size()) == 6) 
			{
				if( pattern.at(2) == UNDERSCORE ) // pattern a(_, _"x+y"_ ) ASSIGN 301 UNDERSCORE COMMA PATTERNOFSIMPLE 302
				{
					if( pattern.at(3) != COMMA || pattern.at(4) != PATTERNOFSIMPLE )
						throw new string("argument error inside pattern clause! -- throw by Validator::checkPattern - pattern size 6");
				}
				else if(pattern.at(2) == PATTERNOFSIMPLE) // pattern("x", _) ASSIGN 301 PATTERNOFSIMPLE 301 COMMA UNDERCORE
				{
					if( pattern.at(4) != COMMA || pattern.at(5) != UNDERSCORE )
						throw new string("argument error inside pattern clause! -- throw by Validator::checkPattern - pattern size 6");
				}
				else
				{
					throw new string("argument error inside pattern clause! -- throw by Validator::checkPattern - pattern size 6");
				}
					
			}
			else if(int(pattern.size()) == 7)
			{
				
					if( pattern.at(2) != PATTERNOFSIMPLE || pattern.at(4) != COMMA || pattern.at(5) != PATTERNOFSIMPLE) // assign("x", "x+y") ->  ASSIGN 301 PATTERNOFSIMPLE 301 COMMA PATTERNOFSIMPLE 302
					{
						throw new string("argument error inside pattern clause! -- throw by Validator::checkPattern - pattern size 7");
					}
			}
			else
			{
				throw new string("pattern size error -- throw by Validator::checkPattern - pattern size 5");
			}
		}
		else if(pattern.at(0) == WHILE)
		{

			if(pattern.at(2) == UNDERSCORE)
			{
				if( pattern.at(3) != COMMA || pattern.at(4) != UNDERSCORE ) // while(_,_) WHILE 301 UNDERSCORE COMMA UNDERSCORE
					throw new string("argument error inside pattern clause! -- throw by Validator::checkPattern -  WHILE - size 5");
			}
			else if(pattern.at(2) == PATTERNOFSIMPLE)
			{
				if( pattern.at(4) != COMMA || pattern.at(5) != UNDERSCORE ) // while("x",_) WHILE 301 PATTERNOFSIMPLE 301 COMMA UNDERSCORE
					throw new string("argument error inside pattern clause! -- throw by Validator::checkPattern -  WHILE - size 6");
			}
			else
			{
				throw new string("size error! -- throw by Validator::checkPattern -  WHILE ");
			}
		}
		else if(pattern.at(0) == IF)
		{
			if(pattern.at(2) == UNDERSCORE)
			{
				if( pattern.at(3) != COMMA || pattern.at(4) != UNDERSCORE || pattern.at(5) != COMMA || pattern.at(6) != UNDERSCORE ) // if(_,_,_) IF UNDERSCORE COMMA UNDERSCORE COMMA UNDERSCORE
					throw new string("argument error inside pattern clause! -- throw by Validator::checkPattern -  IF - size 6 ");
			}
			else if(pattern.at(2) == PATTERNOFSIMPLE)
			{
				if( pattern.at(4) != COMMA || pattern.at(5) != UNDERSCORE || pattern.at(6) != COMMA || pattern.at(7) != UNDERSCORE ) // if("x",_,_) WHILE PATTERNOFSIMPLE 301 COMMA UNDERSCORE COMMA UNDERSCORE
					throw new string("argument error inside pattern clause! -- throw by Validator::checkPattern -  IF - size 7");
			}
			else
			{
				throw new string("size error -- throw by Validator::checkPattern -  IF ");
			}
		}

		checkPatternThree(pattern);
	}
}
void Validator::checkPatternOne(const string &str)
{
	vector<int> expression;
	vector<int> separatorPostion;
	vector<vector<int>> varOfSimple;
	
	fillAssignVector(str, expression, separatorPostion, varOfSimple);

	
	//validation
	//cout<<int(varOfSimple.size())<<endl;
	for(int i = 0; i< int(varOfSimple.size());i++)
	{
		checkFisrstCharacter(varOfSimple,i);
		//check all other possibilities
		for(int j = 0; j<int(varOfSimple.at(i).size());j++)
		{
			//cout<<varOfSimple.at(i).at(j)<<" ";
			if( !( (varOfSimple.at(i).at(j) >= 65 && varOfSimple.at(i).at(j) <= 97) || (varOfSimple.at(i).at(j) >= 97 && varOfSimple.at(i).at(j) <= 122) || (varOfSimple.at(i).at(j) >= 48 && varOfSimple.at(i).at(j) <= 57) || (varOfSimple.at(i).at(j) == 32) ) )
			{
				throw new string("invalid symbol at pattern expression -- throw by Validator::checkAssignPattern");			
			}
		}
	}
}

void Validator::checkPatternTwo(const string &str){

	int qouteNo = 0;
	int quoteStart = 0;
	int quoteEnd = 0;
	int underscoreNo = 0;
	string tempString;
	
	if(str.empty())
	{
		throw new string("expression is empty, after _ must be \" -- throw by Validator::preCheckAssign");
	}
	
	for(int i=0;i<int(str.length());i++)
	{
		if( int(str.at(i)) == 34 ) // quote number
		{
			qouteNo++;
			if(qouteNo == 1)
			{
				quoteStart = i;
			}
			else if(qouteNo == 2)
			{
				quoteEnd = i;
			}
		}
		if(int(str.at(i)) == 95)  // underscore number
		{
			underscoreNo++;
		}
	
	}
	

	//cout<<"underscoreNo = "<<underscoreNo<<" qouteNo = "<<qouteNo<<" quoteStart = "<<quoteStart<<" quoteEnd = "<<quoteEnd<<endl;

	
	if(qouteNo!=2)
	{
		throw new string("quotation Number is wrong-- throw by Validator::preCheckAssign");
	}
	if(underscoreNo == 0) // no underscore involved
	{
		//cout<<str.at(str.length()-1)<<endl;
		//cout <<str.find_first_of("\"")<<" 0"<<endl;
		//cout <<int(str.find_last_of("\""))<<" "<<int((str.length()-1))<<endl;

		if( int(str.find_first_of("\"") ) != 0 || int(str.find_last_of("\"")) != int((str.length()-1)))
		{
			throw new string("quotation position is wrong -- throw by Validator::preCheckAssign");
		}
	}
	
	else if(underscoreNo == 1)
	{
		//cout<<int(str.at(str.length()-2))<<endl;
		
		if(int(str.at(0)) == 95) // check _"
		{
			if( int(str.at(1)) != 34 ) // second character is "
			{
				throw new string("quotation error in the front -- throw by Validator::preCheckAssign");
			}
		}
		else if( int(str.at(str.length()-1)) == 95 )
		{
			
			if( int(int(str.at(str.length()-2))) != 34 ) // check "_
			{
				throw new string("quotation error in the end -- throw by Validator::preCheckAssign");
			}

		}
		else
		{
			throw new string("underscore error, underscoreNo == 1 -- throw by Validator::preCheckAssign");
			
		}
	}
	else if(underscoreNo == 2) // _" "_
	{


		if( int(str.find_first_of("_")) != 0 || int(str.find_first_of("\"")) != 1 || int(str.find_last_of("_")) != (int(str.length())-1) || int(str.find_last_of("\"")) != (int(str.length())-2))
		{
			throw new string("undersocore and quotation wrong, underscoreNo == 2  -- throw by Validator::preCheckAssign");
		}
	
	}
	else
	{
		throw new string("undersocreNo is wrong -- throw by Validator::preCheckAssign");
	}
	
	tempString = str.substr(quoteStart+1, (quoteEnd-quoteStart-1));

	//cout<<tempString<<endl;
	checkPatternOne(tempString);
}


void Validator::checkPatternThree(vector<int> &patternExpression){

	//cout<<patternExpression.at(0)<<endl;
	//cout<<patternExpression.size()<<endl;

	for(int i=0; i < int(patternExpression.size());i++)
	{
		//cout<<patternExpression.at(i)<< " "; 
		
		if(patternExpression.at(i) == PATTERNOFSIMPLE)
		{
			
			
			if((i+1) > int(patternExpression.size()))
			{
				throw new string("index out of range -- throw by Validator::checkPatternString");
			}
			
			//cout<<getString(patternExpression.at(i+1))<<"----------------------get string----------------- "<<endl;
			
			//checkPatternTwo(getString(patternExpression.at(i+1)));
			checkAssignPattern(Convertor::getKeyword(i+1));
			
		}
		
	}
	cout<<endl;
	
}

void Validator::checkFisrstCharacter(const vector<vector<int>> &varOfSimple, int i){	

	if( !( (varOfSimple.at(i).at(0) >= 65 && varOfSimple.at(i).at(0) <= 97) || (varOfSimple.at(i).at(0) >= 97 && varOfSimple.at(i).at(0) <= 122) ) )
	{
		//if it is first letter is a digit
		if( (varOfSimple.at(i).at(0) >= 48 && varOfSimple.at(i).at(0) <= 57) )
		{
			//if the rest of varOfSimple is digits, it is valid. other than that, it is not valid
			for(int j = 0; j < int(varOfSimple.at(i).size());j++)
			{
				if(!(varOfSimple.at(i).at(j) >= 48 && varOfSimple.at(i).at(j) <= 57))
				{
					throw new string("invalid symbol at pattern expression,  first character can only be letter -- throw by Validator::checkFirstCharacter");
				}
			}
		}
	}
}



void Validator::fillAssignVector(const string &str, vector<int> &expression, vector<int> &separatorPostion, vector<vector<int>> &varOfSimple){

	bool flag = false;
	vector<int> tempHolder;

	for(int i=0;i<int(str.length());i++)
	{
		expression.push_back(int(str.at(i)));

		if(int(str.at(i)) == 42 || int(str.at(i)) == 43 || int(str.at(i)) == 45) //take down separator's (+,-,*) position
		{
			separatorPostion.push_back(i);
			flag = true;
		}
		if(!flag)
		{
			tempHolder.push_back(int(str.at(i)));
		}
		else
		{
			if(!tempHolder.empty())
			{
				varOfSimple.push_back(tempHolder);
				tempHolder.clear();
			}
			flag = false;
		}
		
	}

	//add last varOfSimple to varOfSimple vector
	if(int(tempHolder.size())!=0)
	{
		varOfSimple.push_back(tempHolder);
	}

}
///////////////////////////////////////////// Check Pattern End//////////////////////////////////////////////////

///////////////////////////////////////////// Check ALL Start////////////////////////////////////////////////////
void Validator::checkResults(QueryTable &table){
	populateTable();
	checkSelect(table);
	checkSuchThat(table);
	checkWith(table);
	checkPattern(table);
}
///////////////////////////////////////////// Check ALL End////////////////////////////////////////////////////

/******************************************** FOR DEGUGGING START ********************************************/
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
	for(int i=0;i<((int) table.size());i++)
	{
		for(int j=0;j<(int) table[i].size();j++)
		{
			cout<<table[i][j]<<" ";
		}
		
		//cout<<table[i][0];
		cout<<endl;
	}

	
	//cout<<table.size()<<endl;
	//cout<<table[0].size()<<endl;

}

string Validator::getString(int index){
	string temp1 = "\"x\""; // 302
	string temp2 = "_\"x+y+z\"_"; // 303


	if(index == 302)
	{
		return temp1;
	}
	
	if(index == 303)
	{
		return temp2;
	}

}
/******************************************** FOR DEGUGGING END ********************************************/

/******************************************** ACCESSOR START ************************************************/
vector<vector<int>> Validator::getsuchThatTable(){

	return suchThatTable;

}
vector<vector<int>>  Validator::getwithTable(){

	return withTable;
}
/******************************************** ACCESSOR END ************************************************/
