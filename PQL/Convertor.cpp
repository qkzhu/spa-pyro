#include"Convertor.h"


 map<int,string> Convertor::indexToKeyword;
 map<string,int> Convertor::keywordToIndex;
 
 map<string, string> Convertor::shortcutToContent;//s->stmt;
 //Tokenizer keywordToken;
 int u=Convertor::QUATEDSTRING;
 int w=Convertor::PATTERNSTRING;
int Convertor::update()
{
	indexToKeyword.clear();
	keywordToIndex.clear();
	shortcutToContent.clear();
	u=Convertor::QUATEDSTRING;
  string line;
   char * cstr, *p;

  ifstream grammerfile ("pqlgrammer.txt");
  if (grammerfile.is_open())
  {
    while ( grammerfile.good() )
    {
      getline (grammerfile,line);
	 
	   
  cstr = new char [line.size()+1];
  strcpy (cstr, line.c_str());
	  //cout<<line<<endl;
	  p=strtok(cstr," ");
	  
        if(p !=NULL)
		{
		string keyword=p;
		//cout<<p<<endl;
		 p=strtok(NULL," ");
		 
		if (p!=NULL)
		{
		int index=atoi(p);//be careful about the error;
		//cout<<index<<"............"<<keyword<<endl;	
		Convertor::indexToKeyword[index]=keyword;
		Convertor::keywordToIndex[keyword]=index;
		//cout<<indexToKeyword[index]<<"-----"<<keywordToIndex[keyword]<<endl;
		//pch = strtok (NULL, " ;");
		}
		//pch=keywordToken.next();
	  }
	}
	//cout<<"the line is" <<i<<endl;
    grammerfile.close();
  }

  else cout << "Unable to open file"; 

  return 0;
}

 bool Convertor::isDeclared(string token)
 {
	 bool flag;
	 int i;
	 
	 i=getIndex(token);
	 //cout<<i<<"-------------"<<token<<"\n";
	 if (i>50&&i<150&&i!=60)
		 flag=true;
	 else
		 flag=false;
	 return flag;
 }


 bool Convertor::isQuated(string s)
 {      if(s.size()<2)
           return false;
     string a="\"";
	
    if((s.substr(0,1)==a )&& (s.substr(s.size()-1,1)==a) )
	{
		 
		 return true;
		 
	} 
	else
		 return false;

 }
 bool Convertor::isPatterned(string s)
 {
	 string a="_\"";
	 string b="\"_";


	 if (isQuated(s))
		 return true;
	 if(s.size()>=4)
	 {
        if(s.substr(0,2)==a||s.substr(s.size()-2,2)==b)
		   return true;
	    else
		   return false;
	 }
	 if(s.size()>=3)
	 {
		if(s.substr(0,2)==a||s.substr(s.size()-2,2)==b)
		   return true;
	    else
		   return false;
	 }
	 else
	 return false;
 }
bool Convertor::isPureNum(string token)
{
	int num;
	char buffer[33];
	if (token=="0")
		return true;
	else 
		{
			num=atoi(token.c_str());
			if(num!=0)
			{
				itoa(num,buffer,10);
				if(num>=0&&(buffer==token))
			        return true;
				else if(num<0&&(buffer==token))
				    throw new string("negative int\n"); 
				else if(num>=0&&buffer!=token)
                    throw new string("illegal defined symbol start with positive int\n");
				else
					throw new string("illegal defined symbol start with negative int\n");
			}
			else
				return false;
	    }
}
int Convertor::getIndex(string token)//given a token, looking up for the coressponding index, and return it, 
	                                   //if not in the maptable and the declarkeywordtable return -1;
		{
			//cout<<token<<endl;
			int index=-1;
			int num;
			char buffer[33];
			num=atoi(token.c_str());
		    int a=keywordToIndex.count(token);
			//cout<<a;
			if (a==1)
				index=keywordToIndex.find(token)->second;
			else if(isPureNum(token))
			{
				index=num;
			}  
			else if(isPatterned(token))
			{
			
				
				insertIndex(++u ,token);
				insertShortcut(token,"patternOfSimpl");
				index=keywordToIndex.find(token)->second;
			}
			else
			{
				
				index=-1;
			}
		//	cout<<token<<index<<endl;
			return index;
		}
string Convertor:: getKeyword(int index)//given a index, looking for the coressponding keyword, and return it,
	                                    //if not in the table,then return string "NULL";
{
	string keyword;
	if(indexToKeyword.count(index)==1)

				keyword=indexToKeyword.find(index)->second;
	else
				keyword="NULL";
			return keyword;
}
string Convertor::getShortCut(string content)
{
	string shortcut;
	if(shortcutToContent.count(content)==1)

				shortcut=shortcutToContent.find(content)->second;
			else
				shortcut="NULL";
	return shortcut;
}
string Convertor::getContent(string shortcut)
{
	string content;
	if(shortcutToContent.count(shortcut)==1)

				content=shortcutToContent.find(shortcut)->second;
			else
				content="NULL";

	return content;

}
void Convertor::insertIndex(int i,string s)
{
	indexToKeyword[i]=s;
	keywordToIndex[s]=i;
}
void Convertor::insertShortcut(string s,string content)
{
	shortcutToContent[s]=content;
}
void Convertor::showIndexTable()
{

	cout<<indexToKeyword.size()<<"compared with"<<indexToKeyword.max_size()<<endl;
}