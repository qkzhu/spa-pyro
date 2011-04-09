#include"PqlParser.h"

TokenList *qu;
int static userdefine=PqlParser::USERDEFINED;//The index of symbol that declared in the declaration will starting from this;
bool selectBool;//to indicate there is only one select clause;
/*
given table and string, parse the string s and put the parse result into the table
*/
void PqlParser::parser(string s,QueryTable *table){
	userdefine=PqlParser::USERDEFINED;
	selectBool=true;	
	qu=new TokenList(s);
	//qu->showTokenList();

	Convertor::update();
  
	int i=0;//this is the safty triger once the loop go into infinite loop, stop the loop;
	int count=0;
	string pch="";
	int type;//the clause type when add into the table;
	int index;
	string declar,temp2;
	pch=(*qu).getNextToken();
	bool semi=false;//indicate whether end of semi is there;

	while(pch!="NULL"&&i<1000)
	{       
		i++;
        if(i==999) throw new string("infinite loop !");

     
      	if(Convertor::isDeclared(pch))
			{   
				declar=pch;//temporally store the Content of shortcut;
				pch=declarParser(declar,qu,table);
			}
			else 
			{
				
				index=Convertor::getIndex(pch);
			
				switch(index)
				{
					case SELECT:
					

					type=SELECT;
			
					pch=selectParser(type,qu,table);
				  

					break;
					
					case SUCH_THAT:
			
					
					type=SUCH_THAT;
					pch=suchThatParser(type,qu,table);
				    break;
				case WITH:
					type=WITH;
					pch=withParser(type,qu,table);
					break;
				case PATTERN:
					type=PATTERN;
					
					pch=PqlParser::patternParser(type,qu,table);


					break;

				case S_COLON:
					
					pch=(*qu).getNextToken();
					if(pch!="NULL") throw new string("illeagle using of S_COLON\n");
					else
					{
						semi=true;
						cout<<"PqlParsing Finished"<<endl;
					}
					break;
				default:
					throw new string("there are unpredicted format error in the query body "+pch+" \n");
					break;


		   }
    

		}

	}
	if (semi==false) throw new string("missing S_COLON in the end \n");
	if(selectBool==true) throw new string("missing select clause\n");


	return;
}


/*
given the string, we get the prefix of this string accordingly
*/
int PqlParser:: prefix(string token)
{
	int index,prefix;
	 
	index=Convertor::getIndex(token);

	
	if(index>=PqlParser::USERDEFINED)
		{
			string content=Convertor::getContent(token);
		    if(content!="NULL")
			prefix=Convertor::getIndex(content);
		    if(atoi(token.c_str())!=0)
				prefix=201;
;
	   }
	   else
	   {
		   if(Convertor::isPureNum(token))
              prefix=201;
		   else
	          prefix=0;
	   }
	
	return prefix;
}

/*
parse the select clause
*/

string PqlParser::selectParser(int type,TokenList *quPointer,QueryTable *tablePointer) 
{
	if(selectBool==false)throw new string("more than one select appear inside the querystring\n");
	selectBool=false;
	string pch;
	int index;
	int prefix;
	vector<int> content;
	QueryTable *table;
	  table=tablePointer;
	 TokenList *qu;
	  qu=quPointer;
	   
	  
	   pch=qu->getNextToken();
	   index=Convertor::getIndex(pch);
	  if(index==L_ARROW)
	  {
				do
				 {
					
	
				     pch=qu->getNextToken();
					 index=Convertor::getIndex(pch);
					 prefix=PqlParser::prefix(pch);
				
					 if (index>USERDEFINED&&index<=Convertor::QUATEDSTRING||index==BOOLEAN)
					 {   
						
						if(prefix!=0)
						content.push_back(prefix);
						
						content.push_back(index);

						pch=qu->getNextToken();

						index=Convertor::getIndex(pch);

						if(index==DOT)
						{
							content.push_back(index);
							pch=qu->getNextToken();
						    index=Convertor::getIndex(pch);
							if(index==-1) throw new string("the symbol"+pch+"should not after DOT");
							content.push_back(index);
						    pch=qu->getNextToken();
					        index=Convertor::getIndex(pch);
						}

						table->addClause(SELECT,content);
						content.clear();
					}
					else if(index==-1)
					{
						throw new string("undefined symbol in select\n");
					}
					else
					{
						throw new string(pch+"should not apear in the select or select should not empty\n");
					}

				 }while(index==COMMA);
				 if(index!=R_ARROW) { cout<<index; throw new string("missing R_ARROW");}
				 pch=qu->getNextToken();
	             index=Convertor::getIndex(pch);
	  }
	  else
	  {
		  		 prefix=PqlParser::prefix(pch);
				
					 if (index>USERDEFINED&&index<=Convertor::QUATEDSTRING||index==BOOLEAN)
					{   
					
						if(prefix!=0)
						content.push_back(prefix);
						
						content.push_back(index);

					    pch=qu->getNextToken();
						index=Convertor::getIndex(pch);
						if(index==DOT)
						{
							
							content.push_back(index);
							pch=qu->getNextToken();
						    
							index=Convertor::getIndex(pch);
							
							if(index==-1) throw new string(pch+"should not follow the DoT in select clause");
							    content.push_back(index);
					
								pch=qu->getNextToken();
						}
						
						
							

						
						table->addClause(SELECT,content);
						content.clear();
					}
					else if(index==-1)
					{
						throw new string("undefined symbol in select\n");
					}
					else
					{
						cout<<"this is the wrong index and should not appear here"<<index<<endl;
						throw new string(pch+"should not apear in the select or select should not empty\n");
					}
					
	  }



	  return pch;
}

/*
parse the pattern clause
*/
string PqlParser::patternParser(int type,TokenList *quPointer,QueryTable *tablePointer)
{
	string pch;
	int index;
	int prefix;
	int patterntype;
	vector<int> content;
	QueryTable *table;
	  table=tablePointer;
     TokenList *qu;
	  qu=quPointer;

//cout<<"the pattern is "<<type<<endl;
					do
					{
					pch=qu->getNextToken();
					index=Convertor::getIndex(pch);
			        

					if(index<USERDEFINED||index>USERDEFINED+STEP) throw new string("not valid symbol "+ pch+ " after the Pattern \n");
					prefix=PqlParser::prefix(pch);
					

					patterntype=prefix; //store the type of pattern
					
					if(prefix<50||prefix>60) throw new string(pch+" is an invalid type \n");
					content.push_back(prefix);
					content.push_back(index);

					pch=qu->getNextToken();
					index=Convertor::getIndex(pch);
					
					if(index!=L_QUOT) throw new string("miss L_QUOT in the Pattern clause");
					switch (patterntype)
					{
					case WHILE: case ASSIGN:
						 pch=qu->getNextToken();
					     index=Convertor::getIndex(pch);
						 prefix=PqlParser::prefix(pch);
						 if(prefix!=0)
						 content.push_back(prefix);//assign or while;
						 content.push_back(index);//shortcut;

						
						 pch=qu->getNextToken();
					     index=Convertor::getIndex(pch);
						 if(index!=COMMA) throw new string("miss COMMA after the "+pch+" in Pattern while /n ");
						 content.push_back(index);

						 pch=qu->getNextToken();
					     index=Convertor::getIndex(pch);
						 prefix=PqlParser::prefix(pch);
						 if(prefix!=0)
						 content.push_back(prefix);
						 content.push_back(index);

						 pch=qu->getNextToken();
					     index=Convertor::getIndex(pch);
					
					     if(index!=R_QUOT) throw new string("miss R_QUOT in the Pattern clause");


						break;					
					case IF:

						 pch=qu->getNextToken();
					     index=Convertor::getIndex(pch);
						 prefix=PqlParser::prefix(pch);
						 if(prefix!=0)
						 content.push_back(prefix);
						 content.push_back(index);

						 pch=qu->getNextToken();
					     index=Convertor::getIndex(pch);
						 if(index!=COMMA) throw new string("miss COMMA after the "+pch+" in Pattern while /n ");
						 content.push_back(index);

						 pch=qu->getNextToken();
					     index=Convertor::getIndex(pch);
						 prefix=PqlParser::prefix(pch);
						 if(prefix!=0)
						 content.push_back(prefix);
						 content.push_back(index);

						 pch=qu->getNextToken();
					     index=Convertor::getIndex(pch);
						 if(index!=COMMA) throw new string("miss COMMA after the "+pch+" in Pattern while /n ");
						 content.push_back(index);

						 pch=qu->getNextToken();
					     index=Convertor::getIndex(pch);
						 prefix=PqlParser::prefix(pch);
						 if(prefix!=0)
						 content.push_back(prefix);
						 content.push_back(index);




						 pch=qu->getNextToken();
					     index=Convertor::getIndex(pch);
					
					     if(index!=R_QUOT) throw new string("miss R_QUOT in the Pattern clause");


						break;

					}

				    pch=qu->getNextToken();
					index=Convertor::getIndex(pch);
					table->addClause(type,content);
					
				    content.clear();
					if(index!=0)
				       break;
					}while(true);

					return pch;
	}
	/*
	parse the suchthat clause
	*/
string PqlParser::suchThatParser(int type,TokenList *quPointer,QueryTable *tablePointer)
	{

	string pch;
	int index;
	int prefix;
	int suchthatclause;
	vector<int> content;
	QueryTable *table;
	table=tablePointer;
	TokenList *qu;
	qu=quPointer;




		pch=qu->getNextToken();

				if(Convertor::stringToLower(pch)!="that") throw new string("miss that after such\n");
				pch=qu->getNextToken();
				do{
					// this do loop is for the clause inside the suchthat;

				    suchthatclause=Convertor::getIndex(pch);
		            if(suchthatclause==-1||suchthatclause<=4||suchthatclause>=20)
						throw new string(pch+" is undefined querycall in suchthatclause\n");
					else
						content.push_back(suchthatclause);
					
						 
					     pch=qu->getNextToken();

						 index=Convertor::getIndex(pch);
						 if (index!=L_QUOT) throw new string("missing L_QUOT in suchthat after "+ pch+" \n");
						 else
						 {
							 pch=qu->getNextToken();
                             index=Convertor::getIndex(pch);
						 }
						 if (index>Convertor::QUATEDSTRING)
							{
								switch(suchthatclause)
								{
				                case CALLS: case CALLS_T:

								Convertor::insertShortcut(pch,"procOfSimpl"); break;
								default:

                                 Convertor::insertShortcut(pch,"procOfSimpl");
								}	
					    
						    }
							prefix=PqlParser::prefix(pch);
						
						 if(index>=USERDEFINED||prefix==INT||index==UNDERSCORE)
                        {   

							 if(prefix!=0)
							 content.push_back(prefix);
						   	 content.push_back(index);
							 pch=qu->getNextToken();
							
							 index=Convertor::getIndex(pch);
							 if(index!=COMMA) throw new string("should have COMMA after "+pch+" \n"); 
						    
							 pch=qu->getNextToken();
							 index=Convertor::getIndex(pch);  


				       if (index>Convertor::QUATEDSTRING)
							{
								switch(suchthatclause)
								{
				                case CALLS: case CALLS_T:
								Convertor::insertShortcut(pch,"procOfSimpl"); break;
								default:
								
								 Convertor::insertShortcut(pch,"varOfSimpl"); break;
						        }
					       }
					   
					         prefix=PqlParser::prefix(pch);
					   	     if(prefix!=0)
							 content.push_back(prefix);
						   	 content.push_back(index);
							 
							 pch=qu->getNextToken();
							 index=Convertor::getIndex(pch);
							 if(index!=R_QUOT) throw new string("missing R_QUOT after " +pch+" \n");

							
				   }
				table->addClause(type,content);
				content.clear();
				 pch=qu->getNextToken();
				 index=Convertor::getIndex(pch);
				 if(index!=0)
					 break;
				 else
					 pch=qu->getNextToken();
				}while(true);
	return pch;
	}
	/*
	parse the with clause
	*/
string PqlParser::withParser(int type,TokenList *quPointer,QueryTable *tablePointer)
	 {

		 	string pch;
	int index;
	int prefix;
	int indicator=VARNAME;

	vector<int> content;
	QueryTable *table;
	table=tablePointer;
	TokenList *qu;
	qu=quPointer;




		  do{
					 pch=qu->getNextToken();
					 index=Convertor::getIndex(pch);
					 prefix=PqlParser::prefix(pch);
					 if(index==-1) throw new string("undefined symbol1 in with after "+pch+" \n" );
					 if(prefix!=0)
					 content.push_back(prefix);
					 content.push_back(index);
					 pch=qu->getNextToken();
					 index=Convertor::getIndex(pch);
					 if(index==DOT)
					 {
						 content.push_back(index);
						 pch=qu->getNextToken();
						 index=Convertor::getIndex(pch);
						 if (index!=-1&&index!=EQUAL)
                         {
							  if(index==PROCNAME||index==VARNAME)
								 indicator=index;
							 content.push_back(index);
						 }
						 else
						 {
							 throw new string ("undentified symbol "+pch+" after DOT \n");
						 }
						 pch=qu->getNextToken();
						 index=Convertor::getIndex(pch);
					 }
					 if(index!=EQUAL)
					 {
						 throw new string("missing EQUAL after " +pch+" \n");
					 }
					 else
					 {
						 content.push_back(index);//push equal
						 pch=qu->getNextToken();
						 index=Convertor::getIndex(pch);
						 prefix=PqlParser::prefix(pch);
						 if (index>Convertor::QUATEDSTRING&&prefix!=INT)
						 {   if(indicator==VARNAME)
							 Convertor::insertShortcut(pch,"varofsimpl");
							else if(indicator==PROCNAME)
                             Convertor::insertShortcut(pch,"procofsimpl");
						 }
							 prefix=PqlParser::prefix(pch);


						 if(index!=-1||prefix==INT)
						 {
						     if(prefix!=0)
						 content.push_back(prefix);
                         content.push_back(index);//push first token after equal;
						 pch=qu->getNextToken();
						 index=Convertor::getIndex(pch);
						 }
						 else
						 {
							 throw new string("undefined symbol2 after "+pch+" \n");
						 }
						 if(index==DOT)
					     {
						 content.push_back(index);
						 pch=qu->getNextToken();
						 index=Convertor::getIndex(pch);
						 if (index!=-1)
						 {
                         content.push_back(index);
						 pch=qu->getNextToken();
						 index=Convertor::getIndex(pch);
						 }
						 else
						 {
							 throw new string("undefined symbol after DOT in Withclause\n");
						 }
						 }
					 }
				     table->addClause(type,content);
				     content.clear();
					 if(index!=0)
						 break;
					 }while(true);
		 return pch;
	 }
    /*
	parse the declar clause
	*/
string PqlParser::declarParser(string type,TokenList *quPointer,QueryTable *tablePointer)
{
	string pch, temp2;
	
	int index;
	vector<int> content;

	QueryTable *table;
	table=tablePointer;
	TokenList *qu;
	qu=quPointer;

	string declar=type;


		        pch=qu->getNextToken();
				index=Convertor::getIndex(pch);
	         
				if(pch!="NULL"&&index==-1)
				{
					temp2=pch;
		
					do
					{
						if(index==COMMA)
						{
						pch=qu->getNextToken();
						index=Convertor::getIndex(pch);
						if(index!=-1)
							throw new string("should have a undeclared symbol after the COMMA\n");
						temp2=pch;
						}
						Convertor::insertShortcut(temp2,declar);
						Convertor::insertIndex(++userdefine,temp2);
						pch=qu->getNextToken();
						index=Convertor::getIndex(pch);
					//cout<<"feifei"<<pch<<index<<endl;
						if (index==S_COLON)
						{
							pch=qu->getNextToken();
							break;
						}
						else if(index==-1)
						{
							 throw new string("unidentified symbol after the decalred symbol\n");
						}
					}while(index==COMMA);
			  }
			  else
				{

					throw new string("the first token after the declaration"+type+" should be undeclared symbol\n");
				}
				
				return pch;
}