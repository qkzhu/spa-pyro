#include "PQLparser.h"

   	string PQLpreparser::qu;

	map<string, string> syntaxValidator::DeclarationMap;	//The map between variable and its type, first string is variable

	int syntaxValidator::stmtNumber; //The statement numbers
	string syntaxValidator::stmtNumberVariable; //Keep the variable for statement number

	string PQLpreparser:: QueryReader()
		{
			string str1;
			string str2;
			string str3;

			string URL;
			
			
			cout << "Please enter the input query file location:"<<endl;
			cin >> URL;
			ifstream fin (URL);
			getline (fin,str1);
			getline (fin,str2);
			if(fin.eof())
				str3 = "";
			else getline(fin, str3);

			qu=str1+" "+str2+" "+str3;
			cout << '\n';
			cout<< "The input query is: "<<endl;
			cout << str1 << endl;
			cout << str2 << endl;
			cout << str3 << endl;
			cout << "Parsing in process...." << endl;
		return qu;
		}
	
	void syntaxValidator::SelectG(string selectquery,QueryTree* tree)
	{    
		tree->AddChild((tree->GetTRoot()),"select","SELECT");
		tree->GoDown(1);
		//cout << DeclarationMap[selectquery] <<"test select by feifei"<< endl;
		tree->AddChild(tree->GetCurrent(),selectquery,DeclarationMap[selectquery]);
	}
	void syntaxValidator::SuchthatG(string suchthatquery,QueryTree* tree)
	{       
			tree->AddChild(tree->GetTRoot(),"such that","SUCHTHAT");
			size_t pos1=suchthatquery.find_first_of('(');
			size_t pos2=suchthatquery.find_first_of(',');
			size_t pos3=suchthatquery.find_last_of(')');
	        string key1=suchthatquery.substr(0,pos1);
			string key2=suchthatquery.substr(pos1+1,pos2-pos1-1);
			string key3=suchthatquery.substr(pos2+1,pos3-pos2-1);
			string type;

			if (key1.compare("follows")==0)
			{
				tree->GoDown(2);
				tree->AddChild((tree->GetCurrent()),"follows","FOLLOWS");
				tree->GoDown(1); 
				//cout << "---------"<<DeclarationMap[key2] << "test follows by feifei"<<endl;
				//cout << DeclarationMap[key3] << "test follows by fiefei"<<endl;
				QNode* c2 = tree->AddChild(tree->GetCurrent(),key2,DeclarationMap[key2]);  //By HQ
				QNode* c3 = tree->AddChild(tree->GetCurrent(),key3,DeclarationMap[key3]);  //By HQ
			}
			else if (key1.compare("follows*")==0)
			{
				tree->GoDown(2);
				tree->AddChild((tree->GetCurrent()),"follows*","FOLLOWS*");
				tree->GoDown(1); 
				//cout << "---------"<<DeclarationMap[key2] << "test follows by feifei"<<endl;
				//cout << DeclarationMap[key3] << "test follows by fiefei"<<endl;
				QNode* c2 = tree->AddChild(tree->GetCurrent(),key2,DeclarationMap[key2]);  //By HQ
				QNode* c3 = tree->AddChild(tree->GetCurrent(),key3,DeclarationMap[key3]);
			}
			else
			{
				cout<<"oops,error in such that"<<endl;
			}
	}

	int syntaxValidator::getStmtNValue(){  //By HQ
		return stmtNumber;
	}
	string syntaxValidator::getStmtNVar(){  //By HQ
		return stmtNumberVariable;
	}

	void  syntaxValidator::Parse(string str,QueryTree* tree)
	{   
		string sub; 
		stmtNumberVariable = "NOTSET";  //By HQ
		istringstream iss(str);
		iss >> sub;
		while(iss)	
		{   //queryGramm subquery;
			      
			
			if (sub == "stmt" || sub == "stmt#" || sub == "assign"){  //By HQ
				string s2;
				iss >> s2;
				DeclarationMap.insert(pair<string, string>(s2, sub));


			}else if (sub=="select"){
				iss>>sub;
				SelectG(sub,tree);


			}else if (sub=="such"){
					iss>>sub;
					if(sub != "that") throw "The such that clause not correct!";
					iss >> sub;
					SuchthatG(sub,tree);

			//if sub is a variable and it's type is stmt#
			}else if(DeclarationMap.find(sub) != DeclarationMap.end() && DeclarationMap[sub] == "stmt#"){ //By HQ
				stmtNumberVariable = sub;
				//cout << stmtNumberVariable<<endl; //test
				string s ;
				iss >> sub;
				if(sub != "=") throw "Format Error: = needed";
				iss >> sub;
				stmtNumber = atoi(sub.c_str());
				//cout << stmtNumber << "--------HUANGQi-----"<<endl;
			}else if(sub == ";"){  // By HQ

				//do nothing

			}else cout<<"parsing finished"<<endl;
			iss >> sub;
		} 
	}
	