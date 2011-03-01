#include"Convertor.h"


 map<int,string> Convertor::indexToKeyword;
 map<string,int> Convertor::keywordToIndex;
 
 map<string, string> Convertor::shortcutToContent;//s->stmt;
 Tokenizer keywordToken;
 int u=Convertor::QUATEDSTRING;
int Convertor::update()
{
  string line;
  string pch;
  //int index;
  ifstream grammerfile ("pqlgrammer.txt");
  if (grammerfile.is_open())
  {
    while ( grammerfile.good() )
    {
      getline (grammerfile,line);
	  keywordToken.set(line," ;");
	  string pch=keywordToken.next();
	  while (pch != "")
		{
		string keyword=pch;
		string indexstring=keywordToken.next();
		if (indexstring!="")
		{
		int index=atoi(indexstring.c_str());//be careful about the error;
			//printf("%s-------------------%s\n",pch,keyword);
		//cout<<index<<endl;	
		Convertor::indexToKeyword[index]=keyword;
		Convertor::keywordToIndex[keyword]=index;
		//pch = strtok (NULL, " ;");
		}
		pch=keywordToken.next();
	  }
	}
    grammerfile.close();
  }

  else cout << "Unable to open file"; 

  return 0;
}

 bool Convertor::isDeclar(string token)
 {
	 bool flag;
	 int i;
	 
	 i=getIndex(token);
	 //cout<<i<<"-------------"<<token<<"\n";
	 if (i>50&&i<150)
		 flag=true;
	 else
		 flag=false;
	 return flag;
 }


 bool Convertor::isQuated(string s)
 {      
     string a="\"";
	
    if((s.substr(0,1)==a )&& (s.substr(s.size()-1,1)==a) )
		 return true;
	    else
		 return false;

 }

int Convertor::getIndex(string token)//given a token, looking up for the coressponding index, and return it, 
	                                   //if not in the maptable and the declarkeywordtable return -1;
		{
			int index;
			int num;

			num=atoi(token.c_str());
		
			if (keywordToIndex.count(token)==1)
				index=keywordToIndex.find(token)->second;
            else if(isQuated(token))
			{
				insertIndex(++u ,token);
				index=keywordToIndex.find(token)->second;
			}
			else if(num!=0)
				index=num;
			else
			{
				index=-1;
			}
			return index;
		}
string Convertor:: getKeyword(int index)//given a index, looking for the coressponding keyword, and return it,
	                                    //if not in the table,then return string "NULL";
{
	string keyword;
	if(index<100&&indexToKeyword.count(index)==1)

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
