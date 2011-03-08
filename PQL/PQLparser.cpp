#include"PqlParser.h"


QueryTable PqlParser::parser(string s) {
	 TokenList qu=TokenList(s);
	// qu.showTokenList();
	enum KEYWORD
  {
	AND=0,
	SELECT=1,
	SUCH_THAT=2,
	WITH=3,
	PATTERN=4,
	USES=9,
    MODIFIES=10,
	CALLS=11,
	CALLS_T=12,
	//VALUE=101,
	INT=201,
	
	
	COMMA=151,
	S_COLON	=152,
	L_QUOT	=153,
	R_QUOT	=154,
	EQUAL	=155,
	DOT		=156,
	UNDERSCORE =157,
	QUOTE	=158,
	BOOLEAN	=60,
  };
  Convertor::update();
  
  int u=PqlParser::USERDEFINED;
  int g=PqlParser::GRAMMERDEFINED;
  QueryTable table;
  int i=0;
  int count=0;
  string pch="";
 // int p;//swither
   int type;//the clause type;
   int index;
   int prefix;
   int suchthatclause;
   string temp1,temp2;
   pch=qu.getNextToken();
  // if(pch=="NULL") throw new string("the first token is NULL \n");
   vector<int> content;
   bool semi=false;
   //temp store the clause content;
  while(pch!="NULL"&&i<100)
  {       i++;
        

        //  cout<<pch<<"++++++++++++++++++";
	      // pch=qu.getNextToken();
      	if(Convertor::isDeclar(pch))
			{   
				temp1=pch;//temporally store the Content of shortcut;
				pch=qu.getNextToken();
				index=Convertor::getIndex(pch);
			//	cout<<"the first is :"<<index<<endl;
				if(pch!="NULL"&&index==-1)
				{
					temp2=pch;
				//	cout<<"stoppping here";
				 do
				 {
					if(index==COMMA)
					{
                      pch=qu.getNextToken();
					  index=Convertor::getIndex(pch);
					  if(index!=-1)
						  throw new string("should have a undeclared symbol after the COMMA\n");
					  temp2=pch;
					}
					Convertor::insertShortcut(temp2,temp1);
				    Convertor::insertIndex(++u,temp2);
					pch=qu.getNextToken();
					index=Convertor::getIndex(pch);
					//cout<<"feifei"<<pch<<index<<endl;
					if (index==S_COLON)
					{
						pch=qu.getNextToken();
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
					throw new string("the first token after the declaration should be undeclared symbol\n");
				}
			}
			else 
			{
				
			 index=Convertor::getIndex(pch);
			 //cout<<pch<<"--------------";
			 
			 switch(index)
			 {
				case SELECT:
					

				type=SELECT;
				pch=qu.getNextToken();
				 
				do
				 {
					
	
				
					 index=Convertor::getIndex(pch);
					 prefix=PqlParser::prefix(pch);
				
					 if (index>USERDEFINED&&index<=Convertor::QUATEDSTRING||index==BOOLEAN)
					{   
						//prefix=PqlParser::prefix(pch);
						//cout<<"prefix"<<PqlParser::prefix(pch);
						if(prefix!=0)
						content.push_back(prefix);
						
						content.push_back(index);

						table.addClause(SELECT,content);
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
					pch=qu.getNextToken();
					index=Convertor::getIndex(pch);
				  }while(index==COMMA);

				  //table.addClause(SELECT,content);

				  break;
					
				case SUCH_THAT:
				//cout<<type<<"shuaigevs lianghongge";
					
				type=SUCH_THAT;
				//content.clear();
				pch=qu.getNextToken();
				if(pch!="that") throw new string("miss that after such\n");
				pch=qu.getNextToken();
				//cout<<pch<<endl;
				do{// this do loop is for the and clause inside the suchthat;

				    suchthatclause=Convertor::getIndex(pch);
		            if(suchthatclause==-1||suchthatclause<=4||suchthatclause>=20)
						throw new string(pch+" is undefined querycall in suchthatclause\n");
					else
					content.push_back(suchthatclause);
					
						 
					     pch=qu.getNextToken();

						 index=Convertor::getIndex(pch);
						 if (index!=L_QUOT) throw new string("missing L_QUOT in suchthat after "+ pch+" \n");
						 else
						 {
							 pch=qu.getNextToken();
                             index=Convertor::getIndex(pch);
						 }
						 if (index>Convertor::QUATEDSTRING)
							{
								switch(suchthatclause)
								{
				                case CALLS: case CALLS_T:

								Convertor::insertShortcut(pch,"procOfSimiple"); break;
								default:

                                 Convertor::insertShortcut(pch,"procOfSimiple");
								}	
					     //cout<<"temp:"<<temp<<"prefix"<<prefix;
						    }
							prefix=PqlParser::prefix(pch);
						 //cout<<"temp:"<<temp<<"prefix"<<prefix;
						// int paraPosition=0;//identify the position of the parameters; 
						 if(index>=USERDEFINED||prefix==INT||index==UNDERSCORE)
                        {   

							 if(prefix!=0)
							 content.push_back(prefix);
						   	 content.push_back(index);
							 pch=qu.getNextToken();
							
							 index=Convertor::getIndex(pch);
							if(index!=COMMA) throw new string("should have COMMA after "+pch+" \n"); 
						    
							 pch=qu.getNextToken();
							 index=Convertor::getIndex(pch);  


				       if (index>Convertor::QUATEDSTRING)
							{
								switch(suchthatclause)
								{
				                case CALLS: case CALLS_T:
								Convertor::insertShortcut(pch,"procOfSimiple"); break;
								default:
								
								 Convertor::insertShortcut(pch,"varOfSimple"); break;
						        }
					       }
					   
					         prefix=PqlParser::prefix(pch);
					   	     if(prefix!=0)
							 content.push_back(prefix);
						   	 content.push_back(index);
							 
							 pch=qu.getNextToken();
							 index=Convertor::getIndex(pch);
							 if(index!=R_QUOT) throw new string("missing R_QUOT after " +pch+" \n");

							
				   }
				table.addClause(type,content);
				content.clear();
				 pch=qu.getNextToken();
				 index=Convertor::getIndex(pch);
				 if(index!=0)
					 break;
				 else
					 pch=qu.getNextToken();
				}while(true);
				        break;
				case WITH:
					 type=WITH;
					 do{
					 pch=qu.getNextToken();
					 index=Convertor::getIndex(pch);
					 prefix=PqlParser::prefix(pch);
					 if(index==-1) throw new string("undefined symbol in with after "+pch+" \n" );
					 if(prefix!=0)
					 content.push_back(prefix);
					 content.push_back(index);
					 pch=qu.getNextToken();
					 index=Convertor::getIndex(pch);
					 if(index==DOT)
					 {
						 content.push_back(index);
						 pch=qu.getNextToken();
						 index=Convertor::getIndex(pch);
						 if (index!=-1&&index!=EQUAL)
                         content.push_back(index);
						 else
						 {
							 throw new string ("undentified symbol "+pch+" after DOT \n");
						 }
						 pch=qu.getNextToken();
						 index=Convertor::getIndex(pch);
					 }
					 if(index!=EQUAL)
					 {
						 throw new string("missing EQUAL after " +pch+" \n");
					 }
					 else
					 {
						 content.push_back(index);//push equal
						 pch=qu.getNextToken();
						 index=Convertor::getIndex(pch);
						 if (index>Convertor::QUATEDSTRING)
						 Convertor::insertShortcut(pch,"varOfSimple");
						 prefix=PqlParser::prefix(pch);
						 if(index!=-1)
						 {
							 if(prefix!=0)
						 content.push_back(prefix);
                         content.push_back(index);//push first token after equal;
						 pch=qu.getNextToken();
						 index=Convertor::getIndex(pch);
						 }
						 else
						 {
							 throw new string("undefined symbol after "+pch+" \n");
						 }
						 if(index==DOT)
					     {
						 content.push_back(index);
						 pch=qu.getNextToken();
						 index=Convertor::getIndex(pch);
						 if (index!=-1)
						 {
                         content.push_back(index);
						 pch=qu.getNextToken();
						 index=Convertor::getIndex(pch);
						 }
						 else
						 {
							 throw new string("undefined symbol after DOT in Withclause\n");
						 }
						 }
					 }
				     table.addClause(type,content);
				     content.clear();
					 if(index!=0)
						 break;
					 }while(true);

					break;
				case S_COLON:
					//cout<<"reaching"<<endl;
					pch=qu.getNextToken();
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
return table;
}













int PqlParser:: prefix(string token)
{
	int index,prefix;
	//vector<int> content;
	index=Convertor::getIndex(token);
	//cout<<"========================="<<index;
	
	if(index>=PqlParser::USERDEFINED)
		{
			prefix=Convertor::getIndex(Convertor::getContent(token));
			//content.push_back(prefix);
			//content.push_back(index);
	    }
	else 
	   {
		   if(atoi(token.c_str())!=0)
              prefix=201;
		   else
			   prefix=0;
		   //content.push_back(index);
	   }
	
	return prefix;
}
