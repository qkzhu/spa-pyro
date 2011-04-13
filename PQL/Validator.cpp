#include "Validator.h"
#include "dataBase.h"
#include "Tokenizer.h"

///////////////////////////////////////////// Constructor Start///////////////////////////////////////////////////////////
Validator::Validator(){
	validTable.populateAllTables();
}
///////////////////////////////////////////// Constructor End ///////////////////////////////////////////////////////////


///////////////////////////////////////////// Check Select Start////////////////////////////////////////////////////
void Validator::checkSelect(QueryTable &table){

	int size = int (table.getSelectClause().size());
	Select tempSelect;
	
	if (table.getSelectClause().size() == 0)
	{
		throw new string("no attributes inside select clause -- throw by Validator::checkSelect ");
	}	
	// if BOOLEAN involved, there can only be one child in select clause
	if(table.getSelectClause().at(0).tuple.at(0) == BOOLEAN)
	{
		if(int(table.getSelectClause().size())>1)
		{
			throw new string("there can only be one child if BOOLEAN inside select clause -- throw by Validator::checkSelect ");		
		}
	}

	for(int i =0; i<size;i++)
	{		
		//cout<<"table.getSelectClause().at(i).tuple.at(1)" <<table.getSelectClause().at(i).tuple.at(1)<<endl;
		//cout<<"table.getSelectClause().at(i).tuple.at(2)" <<table.getSelectClause().at(i).tuple.at(2)<<endl;
		//cout<<"table.getSelectClause().at(i).tuple.at(3)" <<table.getSelectClause().at(i).tuple.at(3)<<endl;


		if(int(table.getSelectClause().at(i).tuple.size()) == 4)
		{
			//check whether the third digit is DOT or not
			if( table.getSelectClause().at(i).tuple.at(2)!= DOT)
			{
				throw new string ("short of DOT ! -- throw by Validator::checkSelect -- size = 4");
			}
			//stmt, assign, while, if -- attrName: stmt#
			if (table.getSelectClause().at(i).tuple.at(0) == STMT || table.getSelectClause().at(i).tuple.at(0) == ASSIGN || table.getSelectClause().at(i).tuple.at(0) == WHILE || table.getSelectClause().at(i).tuple.at(0) == IF )
			{
				if(table.getSelectClause().at(i).tuple.at(3)!= STMT_NUM)
				{
					throw new string ("attribute error in select clause - stmt, call, assign, while, if ! -- throw by Validator::checkSelect -- size = 4");
				}	

			}
			//call -- attrName: stmt# and procName
			else if(table.getSelectClause().at(i).tuple.at(0) == CALL )
			{

				if(table.getSelectClause().at(i).tuple.at(3)!= STMT_NUM  && table.getSelectClause().at(i).tuple.at(3)!= PROCNAME)
				{
					throw new string ("attribute error in select clause - call  -- throw by Validator::checkSelect -- size = 4");
				}	
			
			}
			// constant c -- attrName: value
			else if(table.getSelectClause().at(i).tuple.at(0) == CONSTANT)
			{
				if(table.getSelectClause().at(i).tuple.at(3) != VALUE)
				{
					throw new string ("attribute error in select clause - constant !  -- throw by Validator::checkSelect  -- size = 4");
				}
			}
			//prog_line -- attrName: prog_line#
			else if(table.getSelectClause().at(i).tuple.at(0) == PROG_L)
			{
				if(table.getSelectClause().at(i).tuple.at(3) != PROG_LINE_NUM)
				{
					throw new string ("attribute error in select clause - prog_line ! -- throw by Validator::checkSelect  -- size = 4");
				}
			}
			//variable  -- attrName: varName
			else if (table.getSelectClause().at(i).tuple.at(0) == VARIABLE)
			{
				if(table.getSelectClause().at(i).tuple.at(3)!= VARNAME)
				{
					throw new string ("attribute error in select clause - variable ! -- throw by Validator::checkSelect  -- size = 4 ");
				}
		
			}
			//procedure  --- attrName: ProcName 
			else if (table.getSelectClause().at(i).tuple.at(0) == PROCEDURE)
			{
				if(table.getSelectClause().at(i).tuple.at(3)!=PROCNAME)
				{
					throw new string ("attribute error in select clause - procedure ! -- throw by Validator::checkSelect  -- size = 4");
				}
			}
			else
			{
				throw new string ("first prefix undefined in select clause! -- throw by Validator::checkSelect  -- size = 4");
			
			}

		} // end if the select tuple size is 4
		else if(int(table.getSelectClause().at(i).tuple.size()) == 2)
		{
			if( !(table.getSelectClause().at(i).tuple.at(0) >=51 && table.getSelectClause().at(i).tuple.at(0) <=60))
			{
				throw new string ("first prefix undefined in select clause! -- throw by Validator::checkSelect -- size = 2");
			}
		} // end if the select tuple size is 2
		else if(int(table.getSelectClause().at(i).tuple.size()) == 1)
		{
			if( table.getSelectClause().at(i).tuple.at(0) != BOOLEAN)
			{
				throw new string ("the prefix has to be BOOLEAN in select clause! -- throw by Validator::checkSelect -- size = 1");
			}
		}// end if the select tuple size is 1
		else
		{
		
			throw new string ("size error in select clause! -- throw by Validator::checkSelect ");
		}
	} // end for loop : for(int i =0; i<size;i++)	
}
///////////////////////////////////////////// Check Select End////////////////////////////////////////////////////



///////////////////////////////////////////// Check Such That Start////////////////////////////////////////////////////

void  Validator::getValidTable(int relationType, vector<vector<int>> &validationTable){
	switch (relationType){

	case PARENT:
	case PARENT_T:
		validTable.getParentTable(validationTable);
		break;
	case FOLLOWS:
	case FOLLOWS_T:
		validTable.getFollowsTable(validationTable);
		break;
	case CALLS:
	case CALLS_T:
		validTable.getCallsTable(validationTable);
		break;
	case NEXT:
	case NEXT_T:
		validTable.getNextTable(validationTable);
		break;
	case AFFECTS:
	case AFFECTS_T:
		validTable.getAffectsTable(validationTable);
		break;
	case USES:
		validTable.getUseTable(validationTable);
		break;
	case MODIFIES:
		validTable.getModifyTable(validationTable);
		break;
	case WITH:
		validTable.getWithTable(validationTable);
		break;
	default:
		throw new string ("undefined relation or with type! -- throw by Validator::getValidTable ");
	}

}

void Validator::getKeyWord(int type, string &relationType){

	switch (type){

	case PARENT:
		relationType = "Parent ";
		break;
	case PARENT_T:
		relationType = "Parent* ";
		break;
	case FOLLOWS:
		relationType = "Follows ";
		break;
	case FOLLOWS_T:
		relationType = "Follows* ";
		break;
	case CALLS:
		relationType = "Calls ";
		break;
	case CALLS_T:
		relationType = "Calls* ";
		break;
	case NEXT:
		relationType = "Next ";
		break;
	case NEXT_T:
		relationType = "Next* ";
		break;
	case AFFECTS:
		relationType = "Affects ";
		break;
	case AFFECTS_T:
		relationType = "Affects* ";
		break;
	case USES:
		relationType = "Uses ";
		break;
	case MODIFIES:
		relationType = "Modifies ";
		break;		
	default:
		throw new string ("undefined relation type! -- throw by Validator::getKeyWord ");
	}

}

void Validator::checkSuchThat(QueryTable &table){
	
	int size = (int) table.getSuchThatClause().size();
	
	for(int i=0; i<size; i++)
	{
		//to check against table, if it has a entry, it will not throw error
		bool noError = false;
		vector<int> suchThat;
		suchThat = table.getSuchThatClause().at(i).relCond;
		string relationType;
		getKeyWord(suchThat[0], relationType);
		string functionName = " -- throw by Validator::checkSuchThat";
		
		if(table.getSuchThatClause().at(i).argumentNoCorrect == false)
		{
			throw new string("Argument number is wrong in relation " + relationType + functionName);
		}


		//check for empty string for varOfSimple and procOfSimple
		for(int j=0;j<int(suchThat.size());j++)
		{
			if(suchThat.at(j) == VAROFSIMPLE ||suchThat.at(j) == PROCOFSIMPLE)
			{
				if((j+1)<int(suchThat.size()))
				{
					string tempString = Convertor::getKeyword(suchThat.at(j+1));
					//cout<<"tempString "<<tempString<<endl;

					if(int(tempString.size())<2)
					{
						if(suchThat.at(j) == VAROFSIMPLE)
						{
							throw new string("varOfSimple size is wrong in relation " + relationType + functionName);
						}
						else if(suchThat.at(j) == PROCOFSIMPLE)
						{
							throw new string("procOfSimple size is wrong in relation" + relationType + functionName);
						}
					}
					if(tempString == "\"\"")
					{
						throw new string("nothing inside the double quotes in relation "+ relationType + functionName);
					}
					else
					{
						if(tempString.at(0) != '\"' || tempString.at(int(tempString.size())-1) !='\"')
						{
							throw new string("format error in first argument in relation "+ relationType + functionName);
						}
					}
					
				} // end if (j+1) is not out of range
				else
				{
					throw new string("no parameter after prefix in relation "+ relationType + functionName);
				}
			
			} // end if(suchThat.at(j) == VAROFSIMPLE ||suchThat.at(j) == PROCOFSIMPLE)
		
		} //end for(int j=0;j<int(suchThat.size());j++)
		
		//fill up the validation according to the relation type
		vector<vector<int>> validationTables;
		getValidTable(suchThat[0],validationTables);
		//cout<<"validationTable size "<<validationTables.size()<<endl;
		//cout<<"suchThat size "<<suchThat.size()<<endl;
		
		for(int k = 0;k<int(validationTables.size());k++)
		{
			if(suchThat.size() == 5)
			{
				//cout<<validationTables[k][0]<<endl;

				if( (suchThat.at(1) == validationTables[k][0]) && (suchThat.at(3) == validationTables[k][1]) )
				{
					
					noError = true;
				}
			}
			else if(suchThat.size() == 4)
			{
				if(  (suchThat.at(1) == validationTables[k][0] && (suchThat.at(2) == validationTables[k][1])) || (suchThat.at(1) == validationTables[k][0]) && (suchThat.at(3) == validationTables[k][1]) ) 
				{
					
					noError = true;
				}
			}
			else if(suchThat.size() == 3)
			{
				//relation USES and MODIFIES do not have such case
				if(suchThat.at(0) == USES || suchThat.at(0) == MODIFIES)
				{
					
					continue;
				}
				else if((suchThat.at(1) == UNDERSCORE) && (suchThat.at(2) == UNDERSCORE))
				{	
					
					noError = true;
				}
			}
		
		} // end for loop: for(int k = 0;k<int(validationTables.size());k++)
		//cout<<"relationType: " <<relationType<<endl;
		if(!noError)
		{
			throw new string("Error Occurs at relation: "+ relationType + functionName);
		}
	} // end outer for loop : for(int i=0; i<size; i++)
}
///////////////////////////////////////////// Check Such That End////////////////////////////////////////////////////

///////////////////////////////////////////// Check With Start//////////////////////////////////////////////////////
void Validator::checkWith(QueryTable &table){

	int size = (int) table.getWithClause().size();
	vector<vector<int>> validationTables;
	getValidTable(WITH,validationTables);
	string functionName =" -- throw by Validator::checkWith";
	//cout<<"with size is " <<size<<endl;

	for(int i=0; i<size;i++)
	{
		bool noError = false;
		vector<int> with;
		string number;  
		std::stringstream out;
		out << i+1;
		number = out.str();

		//cout<<"argumentNoCorrect "<<table.getWithClause().at(i).argumentNoCorrect<<endl;
		// to check with the argument is correct of not
		if(!table.getWithClause().at(i).argumentNoCorrect)
		{
			throw new string("format (length) error in with condition No. "+ number + functionName);
		}
		
		with =  table.getWithClause().at(i).attrCond;

		//check the whether there are invalid procOfSimple or varOfSimple
		for(int j=0;j<int(with.size());j++)
		{
			if(with.at(j) == VAROFSIMPLE ||with.at(j) == PROCOFSIMPLE)
			{
				if((j+1)<int(with.size()))
				{
					string tempString = Convertor::getKeyword(with.at(j+1));
					//cout<<"tempString "<<tempString<<endl;
					if(int(tempString.size())<2)
					{
						if(with.at(j) == VAROFSIMPLE)
						{
							throw new string("varOfSimple size is wrong in with condition No. "+ number + functionName);
						}
						else if(with.at(j) == PROCOFSIMPLE)
						{
							throw new string("procOfSimple size is wrong in with condition No. "+ number + functionName);
						}
					}
					if(tempString == "\"\"")
					{
						throw new string("nothing inside the double quotes in with condition No. "+ number + functionName);
					}
					else
					{
						if(tempString.at(0) != '\"' || tempString.at(int(tempString.size())-1) !='\"')
						{
							throw new string("format error in first argument in with condition No. "+ number + functionName);
						}
					}
					
				}
				else
				{
					throw new string("no parameter after prefix! in with condition No. "+ number + functionName);
				}
			
			}
		
		} // end innter for loop: for(int j=0;j<int(with.size());j++)
		
		//Check against Table
		for(int k=0; k<int(validationTables.size());k++)
		{
			if(with.size() == 7)
			{
				//cout<<"with size is "<<with.size()<<endl;
				if((with[0] == validationTables[k][0]) && (with[3] == validationTables[k][1]) && (with[5] == validationTables[k][2]))
				{
					noError = true;
				}
				
				// to check if any other type is in this size
				if(with[5]>=51 && with[5]<=60)
				{
					noError = false;
				}
				

			}
			else if(with.size() == 9)
			{
				if((with[0] == validationTables[k][0]) && (with[3] == validationTables[k][1]) && (with[5] == validationTables[k][2]) && (with[8] == validationTables[k][3]))
				{
					noError = true;
				}
							
			}
		
		} // end innter for loop :for(int k=0; k<int(validationTables.size());k++)

		//cout<<tempNoError<<" inside checkWith "<<endl;
		if (!noError)
		{
			throw new string("format error in with condition No. "+ number + functionName);
		}
		
	} // end outer for loop : for(int i=0; i<size;i++)

}
///////////////////////////////////////////// Check With Start////////////////////////////////////////////////////


///////////////////////////////////////////// Check Pattern_PQL Start //////////////////////////////////////////////////
void Validator::checkPattern_PQL(QueryTable &table){

	int size =  (int) table.getPattern_PQLClause().size();


	for(int i=0; i<size;i++)
	{
		bool tempNoError = false;

		if(table.getPattern_PQLClause().at(i).argumentNoCorrect == false)
		{
			throw new string("argument error inside pattern clause! -- throw by Validator::checkPattern_PQL - argumentNoCorrect is false");
			
		}
		vector<int> pattern;
		pattern = table.getPattern_PQLClause().at(i).expression;

		if(pattern.at(0) == ASSIGN)
		{
			if(int(pattern.size()) == 5) // pattern a(_,_); ASSIGN 301 UNDERSCORE COMMA UNDERSCORE
			{
				//cout<< pattern.at(2)<<" "<<pattern.at(3)<<" "<<pattern.at(4)<<endl;
				if( pattern.at(2) != UNDERSCORE || pattern.at(3) != COMMA || pattern.at(4) != UNDERSCORE )
				{
					throw new string("argument error inside pattern clause! -- throw by Validator::checkPattern_PQL - pattern size 5");
				}
			}
			else if(int(pattern.size()) == 6) 
			{
				//cout<<pattern.at(2)<<endl;
				if( pattern.at(2) == UNDERSCORE ) // pattern a(_, _"x+y"_ ) ASSIGN 301 UNDERSCORE COMMA PATTERNOFSIMPLE 302
				{
					if( pattern.at(3) != COMMA || pattern.at(4) != PATTERNOFSIMPLE )
						throw new string("argument error inside pattern clause! -- throw by Validator::checkPattern_PQL - pattern size 6");
				}
				else if(pattern.at(2) == PATTERNOFSIMPLE || pattern.at(2) == VAROFSIMPLE || pattern.at(2) == VARIABLE) // pattern("x", _) ASSIGN 301 PATTERNOFSIMPLE(or VARIABLE) 301 COMMA UNDERCORE
				{
					if( pattern.at(4) != COMMA || pattern.at(5) != UNDERSCORE )
						throw new string("argument error inside pattern clause! -- throw by Validator::checkPattern_PQL - pattern size 6");
				}
				else
				{
					throw new string("argument error inside pattern clause! -- throw by Validator::checkPattern_PQL - pattern size 6");
				}
					
			} // end if(int(pattern.size()) == 6) 
			else if(int(pattern.size()) == 7) // assign("x", "x+y") ->  ASSIGN 301 PATTERNOFSIMPLE(or VARIABLE) 301 COMMA PATTERNOFSIMPLE 302
			{
				//cout<<pattern.at(2)<<endl;
				if( !(pattern.at(2) == PATTERNOFSIMPLE || pattern.at(2) == VAROFSIMPLE || pattern.at(2) == VARIABLE) || pattern.at(4) != COMMA || pattern.at(5) != PATTERNOFSIMPLE) 
				{
					throw new string("argument error inside pattern clause! -- throw by Validator::checkPattern_PQL - pattern size 7");
				}
			}
			else
			{
				throw new string("pattern size error -- throw by Validator::checkPattern_PQL - pattern size 5");
			}
		}// end if(pattern.at(0) == ASSIGN)
		else if(pattern.at(0) == WHILE)
		{

			if(pattern.at(2) == UNDERSCORE)
			{
				if( pattern.at(3) != COMMA || pattern.at(4) != UNDERSCORE ) // while(_,_) WHILE 301 UNDERSCORE COMMA UNDERSCORE
					throw new string("argument error inside pattern clause! -- throw by Validator::checkPattern_PQL -  WHILE - size 5");
			}
			else if(pattern.at(2) == PATTERNOFSIMPLE || pattern.at(2) == VAROFSIMPLE || pattern.at(2) == VARIABLE)
			{
				if( pattern.at(4) != COMMA || pattern.at(5) != UNDERSCORE ) // while("x",_) WHILE 301 PATTERNOFSIMPLE (or VARIABLE) 301 COMMA UNDERSCORE
					throw new string("argument error inside pattern clause! -- throw by Validator::checkPattern_PQL -  WHILE - size 6");
			}
			else
			{
				throw new string("size error! -- throw by Validator::checkPattern_PQL -  WHILE ");
			}
		} // end if(pattern.at(0) == WHILE)
		else if(pattern.at(0) == IF)
		{
			if(pattern.at(2) == UNDERSCORE)
			{
				if( pattern.at(3) != COMMA || pattern.at(4) != UNDERSCORE || pattern.at(5) != COMMA || pattern.at(6) != UNDERSCORE ) // if(_,_,_) IF UNDERSCORE COMMA UNDERSCORE COMMA UNDERSCORE
					throw new string("argument error inside pattern clause! -- throw by Validator::checkPattern_PQL -  IF - size 6 ");
			}
			else if(pattern.at(2) == PATTERNOFSIMPLE || pattern.at(2) == VAROFSIMPLE || pattern.at(2) == VARIABLE)
			{
				if( pattern.at(4) != COMMA || pattern.at(5) != UNDERSCORE || pattern.at(6) != COMMA || pattern.at(7) != UNDERSCORE ) // if("x",_,_) WHILE PATTERNOFSIMPLE (or VARIABLE) 301 COMMA UNDERSCORE COMMA UNDERSCORE
					throw new string("argument error inside pattern clause! -- throw by Validator::checkPattern_PQL -  IF - size 7");
			}
			else
			{
				throw new string("size error -- throw by Validator::checkPattern_PQL -  IF ");
			}
		}// end if(pattern.at(0) == IF)

		checkPattern_PQLThree(pattern);
	} // end outer for loop: for(int i=0; i<size;i++)
}
void Validator::checkPattern_PQLOne(const string &str)
{
	vector<int> expression;
	vector<int> separatorPostion;
	vector<vector<int>> varOfSimple;
	
	fillAssignVector(str, expression, separatorPostion, varOfSimple);

	for(int i = 0; i< int(varOfSimple.size());i++)
	{
		
		checkFisrstCharacter(varOfSimple,i);

		//check all other possibilities
		for(int j = 0; j<int(varOfSimple.at(i).size());j++)
		{

			if( !( (varOfSimple.at(i).at(j) >= 65 && varOfSimple.at(i).at(j) <= 97) || (varOfSimple.at(i).at(j) >= 97 && varOfSimple.at(i).at(j) <= 122) || (varOfSimple.at(i).at(j) >= 48 && varOfSimple.at(i).at(j) <= 57) || (varOfSimple.at(i).at(j) == 32) ) )
			{
				throw new string("invalid symbol at pattern expression -- throw by Validator::checkPattern_PQLOne");			
			}
		}
	}
}

void Validator::checkPattern_PQLTwo(const string &str){

	int qouteNo = 0;
	int quoteStart = 0;
	int quoteEnd = 0;
	int underscoreNo = 0;
	string tempString;

	if(str.empty())
	{
		throw new string("expression is empty, after _ must be \" -- throw by Validator::checkPattern_PQLTwo");
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
		throw new string("quotation Number is wrong-- throw by Validator::checkPattern_PQLTwo");
	}

	if(underscoreNo == 0) // no underscore involved
	{
		//cout<<str.at(str.length()-1)<<endl;
		//cout <<str.find_first_of("\"")<<" 0"<<endl;
		//cout <<int(str.find_last_of("\""))<<" "<<int((str.length()-1))<<endl;

		if( int(str.find_first_of("\"") ) != 0 || int(str.find_last_of("\"")) != int((str.length()-1)))
		{
			throw new string("quotation position is wrong -- throw by Validator::checkPattern_PQLTwo");
		}
	}// end if(underscoreNo == 0)
	else if(underscoreNo == 1)
	{
		//cout<<int(str.at(str.length()-2))<<endl;
		
		if(int(str.at(0)) == 95) // check _"
		{
			if( int(str.at(1)) != 34 ) // second character is "
			{
				throw new string("quotation error in the front -- throw by Validator::checkPattern_PQLTwo");
			}
		}
		else if( int(str.at(str.length()-1)) == 95 )
		{
			
			if( int(int(str.at(str.length()-2))) != 34 ) // check "_
			{
				throw new string("quotation error in the end -- throw by Validator::checkPattern_PQLTwo");
			}

		}
		else
		{
			throw new string("underscore error, underscoreNo == 1 -- throw by Validator::checkPattern_PQLTwo");
			
		}
	}//end if(underscoreNo == 1)
	else if(underscoreNo == 2) // _" "_
	{


		if( int(str.find_first_of("_")) != 0 || int(str.find_first_of("\"")) != 1 || int(str.find_last_of("_")) != (int(str.length())-1) || int(str.find_last_of("\"")) != (int(str.length())-2))
		{
			throw new string("undersocore and quotation wrong, underscoreNo == 2  -- throw by Validator::checkPattern_PQLTwo");
		}
	
	}// else if(underscoreNo == 2)
	else
	{
		throw new string("undersocreNo is wrong -- throw by Validator::checkPattern_PQLTwo");
	}
	
	tempString = str.substr(quoteStart+1, (quoteEnd-quoteStart-1));

	checkPattern_PQLOne(tempString);
}


void Validator::checkPattern_PQLThree(vector<int> &patternExpression){

	if(patternExpression.at(2) == PATTERNOFSIMPLE)
	{
		string tempString = Convertor::getKeyword(patternExpression.at(3));

		if(int(tempString.size())<2)
		{
			throw new string("first argument size is wrong -- throw by Validator::checkPattern_PQLThree");
		}
		if(tempString == "\"\"")
		{
			throw new string("nothing inside the double quotes -- throw by Validator::checkPattern_PQLThree");
		}
		else
		{
			if(tempString.at(0) != '\"' || tempString.at(int(tempString.size())-1) !='\"')
			{
				throw new string("format error in first argument -- throw by Validator::checkPattern_PQLThree");
			}
		}
	} // end if if(patternExpression.at(2) == PATTERNOFSIMPLE)
	
	for(int i=0; i < int(patternExpression.size());i++)
	{
		//cout<<patternExpression.at(i)<< " "; 
		
		if(patternExpression.at(i) == PATTERNOFSIMPLE)
		{		
			if((i+1) > int(patternExpression.size()))
			{
				throw new string("index out of range -- throw by Validator::checkPattern_PQLThree");
			}
			
			//cout<<getString(patternExpression.at(i+1))<<"----------------------get string----------------- "<<endl;
			
			//cout<<"haha"<<endl;
			//cout<<"get keyword: "<<Convertor::getKeyword(patternExpression.at(i+1))<<endl;
			
			if(Convertor::getKeyword(patternExpression.at(i+1)) == "\"\"")
			{
				throw new string("nothing inside the double quotes -- throw by Validator::checkPattern_PQLThree");
			}
			else
			{
				checkPattern_PQLTwo(Convertor::getKeyword(patternExpression.at(i+1)));
			}
			
		}
		
	}
	//cout<<endl;
	
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
	

	if(str.empty())
	{
		throw new string("nothing inside the doube quote -- throw by Validator::fillAssignVector");
	}


	if(int(str.front()) == 42 || int(str.front()) == 43 || int(str.front()) == 45 || int(str.back()) == 42 || int(str.back()) == 43 || int(str.back()) == 45)
	{
		throw new string("the operator(+,-,*) cannot appear in front or back -- throw by Validator::fillAssignVector");
	}

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
///////////////////////////////////////////// Check Pattern_PQL End//////////////////////////////////////////////////

///////////////////////////////////////////// Check ALL Start////////////////////////////////////////////////////
void Validator::checkResults(QueryTable &table){
	
	checkSelect(table);
	checkSuchThat(table);
	checkWith(table);
	checkPattern_PQL(table);
}
///////////////////////////////////////////// Check ALL End////////////////////////////////////////////////////

/******************************************** FOR DEGUGGING START ********************************************/
void Validator::displayTable(vector<vector<int>> table){

	cout<<"No of Rows "<< table.size()<<endl;

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

	return NULL;
}
/******************************************** FOR DEGUGGING END ********************************************/

