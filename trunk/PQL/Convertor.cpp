#include"Convertor.h"


 map<int,string> Convertor::indexToKeyword;
 map<int,string>::iterator it;
 map<string,int> Convertor::keywordToIndex;
 map<int,string> Convertor::indexToDeclaration;
 map<string,int> Convertor::declarationToIndex;
 map<string, string> Convertor::shortcutToContent;//s->stmt;
 int u=Convertor::QUATEDSTRING;

int Convertor::update()
{
	//empty the maps and initialise the varable;
	indexToDeclaration.clear();
	declarationToIndex.clear();
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
	  
		p=strtok(cstr," ");
	  
        if(p !=NULL)
		{
			string keyword=p;
	
			 p=strtok(NULL," ");
		 
		if (p!=NULL)
		{
			int index=atoi(p);
		
			Convertor::indexToKeyword[index]=keyword;
			Convertor::keywordToIndex[keyword]=index;
	
		}
		
	  }
	}
    grammerfile.close();
  }

  else cout << "Unable to open file"; 

  return 0;
}


void Convertor::getAllIndex(vector<int> &indexlist)
{
	int index;
	for ( it=indexToDeclaration.begin() ; it != indexToDeclaration.end(); it++ )
	{
		index=(*it).first;
		if(index>300)
		indexlist.push_back(index);
		else
		throw new string("all the index inside the indexToDeclaration should be lager than 300");
    cout << (*it).first << " => " << (*it).second << endl;
	}
}
 bool Convertor::isDeclared(string token)
 {
	 bool flag;
	 int i;
	 
	 i=getIndex(token);
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
			num=atoi(token.c_str());
		    int a=ToIndexCount(token);
			if (a==1)
				index=ToIndexFind(token);
			else if(isPureNum(token))
			{
				index=num;
			}  
			else if(isPatterned(token))
			{
			
				
				insertIndex(++u ,token);
				insertShortcut(token,"patternOfSimpl");
				index=declarationToIndex.find(token)->second;
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
	else if(indexToDeclaration.count(index)==1)
		keyword=indexToDeclaration.find(index)->second;
	else
				keyword="NULL";
			return keyword;
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
	indexToDeclaration[i]=s;
	declarationToIndex[s]=i;
}
void Convertor::insertShortcut(string s,string content)
{
	shortcutToContent[s]=content;
}
void Convertor::showIndexTable()
{

	cout<<indexToKeyword.size()<<"compared with"<<indexToKeyword.max_size()<<endl;
}


	int Convertor:: ToIndexCount(string token)
	{
		int a,b;
		a=keywordToIndex.count(stringToLower(token));
		b=declarationToIndex.count(token);
		if(a==1||b==1)
			return 1;
		else
			return a;
	}

	int Convertor::ToIndexFind(string token)
	{
		int a,b;
		if(keywordToIndex.count(stringToLower(token))==1)
		{
			a=keywordToIndex.find(stringToLower(token))->second;
		    return a;
		}
		else if(declarationToIndex.count(token)==1)
		{
			b=declarationToIndex.find(token)->second;
			return b;
		}
		else
        throw new string("Convertor::find(): "+token+" should have been predefined ");
		
	}


string Convertor::stringToLower(string strToConvert)
{//change each element of the string to lower case
	string a= string();
   for(unsigned int i=0;i<strToConvert.length();i++)
   {
	   char b=strToConvert[i];
	  
	   a.push_back(tolower(b));
   }

   return a;//return the converted string
}
