#include "PqlParser.h"
//int prefix(string token);
  int main(void)
  {
	  string s;
//	  bool result1;
	  string result2;
	  
	  Validator check;
	 
	 ifstream test("testcase 2.txt");
	 if(test.is_open())
	 {
	 while(test.good())
	 {
		getline(test,s);
	
	    if(s!="")
	    {
	      try
	      {
	       

	     
	  QueryTable *table=new QueryTable();

	  
	  PqlParser::parser(s,table);
	  table->showTable();
	  
	 // cout<<"showtable here:"<<table.selectSize()<<endl;
//	  check.populateTable();
	//  QueryTable table1;
      check.checkResults(*table);
	  
	  //getline(test,result2);

//	  if(result1)//&&result2=="true"||!result1&&result2=="false")
		//  cout<<1<<endl;
	//  else
		  ///cout<<0<<endl;
	 
	  }
	  catch(string *s)
	  {
		  cout<< *s;
	  }
	 }
	}

	 }
	 else
	 {
		 cout<<"can not open file testcase1 \n";
	 }
	  
	  //cout<<"++"<<Convertor::getKeyword(154)<<Convertor::getIndex(")")<<"this is testing";
	  //tokens=a.getNextToken();
	// QueryTable qt = PqlParser::parser(s);
	// qt.showTable();

	 test.close();
	  //system("pause");
	  return 0;
  }