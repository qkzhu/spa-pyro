#include "dataBase.h"

/////////////////////////////////////////// Populate Functions Start ///////////////////////////////////////
void dataBase::populateModifiesTables(){
	vector<int> table;
	table.push_back(PROCEDURE);		table.push_back(VARIABLE); modifyTable.push_back(table); table.clear();
	table.push_back(PROCOFSIMPLE);  table.push_back(VARIABLE); modifyTable.push_back(table); table.clear();
	table.push_back(STMT);			table.push_back(VARIABLE); modifyTable.push_back(table); table.clear();
	table.push_back(INT);			table.push_back(VARIABLE); modifyTable.push_back(table); table.clear();
	table.push_back(IF);			table.push_back(VARIABLE); modifyTable.push_back(table); table.clear();
	table.push_back(WHILE);			table.push_back(VARIABLE); modifyTable.push_back(table); table.clear();
	table.push_back(ASSIGN);		table.push_back(VARIABLE); modifyTable.push_back(table); table.clear();
	table.push_back(CALL);			table.push_back(VARIABLE); modifyTable.push_back(table); table.clear();
	table.push_back(PROG_L);		table.push_back(VARIABLE); modifyTable.push_back(table); table.clear();

	table.push_back(PROCEDURE);		table.push_back(VAROFSIMPLE); modifyTable.push_back(table); table.clear();
	table.push_back(PROCOFSIMPLE);  table.push_back(VAROFSIMPLE); modifyTable.push_back(table); table.clear();
	table.push_back(STMT);			table.push_back(VAROFSIMPLE); modifyTable.push_back(table); table.clear();
	table.push_back(INT);			table.push_back(VAROFSIMPLE); modifyTable.push_back(table); table.clear();
	table.push_back(IF);			table.push_back(VAROFSIMPLE); modifyTable.push_back(table); table.clear();
	table.push_back(WHILE);			table.push_back(VAROFSIMPLE); modifyTable.push_back(table); table.clear();
	table.push_back(ASSIGN);		table.push_back(VAROFSIMPLE); modifyTable.push_back(table); table.clear();
	table.push_back(CALL);			table.push_back(VAROFSIMPLE); modifyTable.push_back(table); table.clear();
	table.push_back(PROG_L);		table.push_back(VAROFSIMPLE); modifyTable.push_back(table); table.clear();

	table.push_back(PROCEDURE);		table.push_back(UNDERSCORE); modifyTable.push_back(table); table.clear();
	table.push_back(PROCOFSIMPLE);  table.push_back(UNDERSCORE); modifyTable.push_back(table); table.clear();
	table.push_back(STMT);			table.push_back(UNDERSCORE); modifyTable.push_back(table); table.clear();
	table.push_back(INT);			table.push_back(UNDERSCORE); modifyTable.push_back(table); table.clear();
	table.push_back(IF);			table.push_back(UNDERSCORE); modifyTable.push_back(table); table.clear();
	table.push_back(WHILE);			table.push_back(UNDERSCORE); modifyTable.push_back(table); table.clear();
	table.push_back(ASSIGN);		table.push_back(UNDERSCORE); modifyTable.push_back(table); table.clear();
	table.push_back(CALL);			table.push_back(UNDERSCORE); modifyTable.push_back(table); table.clear();
	table.push_back(PROG_L);		table.push_back(UNDERSCORE); modifyTable.push_back(table); table.clear();

}
void dataBase::populateCallsTables(){
	vector<int> table;
	table.push_back(PROCEDURE);		table.push_back(PROCEDURE); callsTable.push_back(table); table.clear();
	table.push_back(PROCOFSIMPLE);	table.push_back(PROCEDURE); callsTable.push_back(table); table.clear();
	table.push_back(UNDERSCORE);	table.push_back(PROCEDURE); callsTable.push_back(table); table.clear();

	table.push_back(PROCEDURE);		table.push_back(PROCOFSIMPLE); callsTable.push_back(table); table.clear();
	table.push_back(PROCOFSIMPLE);	table.push_back(PROCOFSIMPLE); callsTable.push_back(table); table.clear();
	table.push_back(UNDERSCORE);	table.push_back(PROCOFSIMPLE); callsTable.push_back(table); table.clear();

	table.push_back(PROCEDURE);		table.push_back(UNDERSCORE); callsTable.push_back(table); table.clear();
	table.push_back(PROCOFSIMPLE);	table.push_back(UNDERSCORE); callsTable.push_back(table); table.clear();
	table.push_back(UNDERSCORE);	table.push_back(UNDERSCORE); callsTable.push_back(table); table.clear();


	

}

void dataBase::populateUseTables(){
	vector<int> table;
	table.push_back(PROCEDURE);		table.push_back(VARIABLE); useTable.push_back(table); table.clear();
	table.push_back(PROCOFSIMPLE);  table.push_back(VARIABLE); useTable.push_back(table); table.clear();
	table.push_back(STMT);			table.push_back(VARIABLE); useTable.push_back(table); table.clear();
	table.push_back(INT);			table.push_back(VARIABLE); useTable.push_back(table); table.clear();
	table.push_back(IF);			table.push_back(VARIABLE); useTable.push_back(table); table.clear();
	table.push_back(WHILE);			table.push_back(VARIABLE); useTable.push_back(table); table.clear();
	table.push_back(ASSIGN);		table.push_back(VARIABLE); useTable.push_back(table); table.clear();
	table.push_back(CALL);			table.push_back(VARIABLE); useTable.push_back(table); table.clear();
	table.push_back(PROG_L);		table.push_back(VARIABLE); useTable.push_back(table); table.clear();

	table.push_back(PROCEDURE);		table.push_back(VAROFSIMPLE); useTable.push_back(table); table.clear();
	table.push_back(PROCOFSIMPLE);	table.push_back(VAROFSIMPLE); useTable.push_back(table); table.clear();
	table.push_back(STMT);			table.push_back(VAROFSIMPLE); useTable.push_back(table); table.clear();
	table.push_back(INT);			table.push_back(VAROFSIMPLE); useTable.push_back(table); table.clear();
	table.push_back(IF);			table.push_back(VAROFSIMPLE); useTable.push_back(table); table.clear();
	table.push_back(WHILE);			table.push_back(VAROFSIMPLE); useTable.push_back(table); table.clear();
	table.push_back(ASSIGN);		table.push_back(VAROFSIMPLE); useTable.push_back(table); table.clear();
	table.push_back(CALL);			table.push_back(VAROFSIMPLE); useTable.push_back(table); table.clear();
	table.push_back(PROG_L);		table.push_back(VAROFSIMPLE); useTable.push_back(table); table.clear();

	table.push_back(PROCEDURE);		table.push_back(UNDERSCORE); useTable.push_back(table); table.clear();
	table.push_back(PROCOFSIMPLE);	table.push_back(UNDERSCORE); useTable.push_back(table); table.clear();
	table.push_back(STMT);			table.push_back(UNDERSCORE); useTable.push_back(table); table.clear();
	table.push_back(INT);			table.push_back(UNDERSCORE); useTable.push_back(table); table.clear();
	table.push_back(IF);			table.push_back(UNDERSCORE); useTable.push_back(table); table.clear();
	table.push_back(WHILE);			table.push_back(UNDERSCORE); useTable.push_back(table); table.clear();
	table.push_back(ASSIGN);		table.push_back(UNDERSCORE); useTable.push_back(table); table.clear();
	table.push_back(CALL);			table.push_back(UNDERSCORE); useTable.push_back(table); table.clear();
	table.push_back(PROG_L);		table.push_back(UNDERSCORE); useTable.push_back(table); table.clear();
}

void dataBase::populateParentTables(){
	vector<int> table;
	table.push_back(STMT);			table.push_back(STMT); parentTable.push_back(table); table.clear();
	table.push_back(IF);			table.push_back(STMT); parentTable.push_back(table); table.clear();
	table.push_back(WHILE);			table.push_back(STMT); parentTable.push_back(table); table.clear();
	table.push_back(ASSIGN);		table.push_back(STMT); parentTable.push_back(table); table.clear();
	table.push_back(UNDERSCORE);	table.push_back(STMT); parentTable.push_back(table); table.clear();
	table.push_back(INT);			table.push_back(STMT); parentTable.push_back(table); table.clear();
	table.push_back(PROG_L);		table.push_back(STMT); parentTable.push_back(table); table.clear();


	table.push_back(STMT);			table.push_back(IF); parentTable.push_back(table); table.clear();
	table.push_back(IF);			table.push_back(IF); parentTable.push_back(table); table.clear();
	table.push_back(WHILE);			table.push_back(IF); parentTable.push_back(table); table.clear();
	table.push_back(ASSIGN);		table.push_back(IF); parentTable.push_back(table); table.clear();
	table.push_back(UNDERSCORE);	table.push_back(IF); parentTable.push_back(table); table.clear();
	table.push_back(INT);			table.push_back(IF); parentTable.push_back(table); table.clear();
	table.push_back(PROG_L);		table.push_back(IF); parentTable.push_back(table); table.clear();

	table.push_back(STMT);			table.push_back(WHILE); parentTable.push_back(table); table.clear();
	table.push_back(IF);			table.push_back(WHILE); parentTable.push_back(table); table.clear();
	table.push_back(WHILE);			table.push_back(WHILE); parentTable.push_back(table); table.clear();
	table.push_back(ASSIGN);		table.push_back(WHILE); parentTable.push_back(table); table.clear();
	table.push_back(UNDERSCORE);	table.push_back(WHILE); parentTable.push_back(table); table.clear();
	table.push_back(INT);			table.push_back(WHILE); parentTable.push_back(table); table.clear();
	table.push_back(PROG_L);		table.push_back(WHILE); parentTable.push_back(table); table.clear();

	table.push_back(STMT);			table.push_back(ASSIGN); parentTable.push_back(table); table.clear();
	table.push_back(IF);			table.push_back(ASSIGN); parentTable.push_back(table); table.clear();
	table.push_back(WHILE);			table.push_back(ASSIGN); parentTable.push_back(table); table.clear();
	table.push_back(ASSIGN);		table.push_back(ASSIGN); parentTable.push_back(table); table.clear();
	table.push_back(UNDERSCORE);	table.push_back(ASSIGN); parentTable.push_back(table); table.clear();
	table.push_back(INT);			table.push_back(ASSIGN); parentTable.push_back(table); table.clear();
	table.push_back(PROG_L);		table.push_back(ASSIGN); parentTable.push_back(table); table.clear();

	table.push_back(STMT);			table.push_back(UNDERSCORE); parentTable.push_back(table); table.clear();
	table.push_back(IF);			table.push_back(UNDERSCORE); parentTable.push_back(table); table.clear();
	table.push_back(WHILE);			table.push_back(UNDERSCORE); parentTable.push_back(table); table.clear();
	table.push_back(ASSIGN);		table.push_back(UNDERSCORE); parentTable.push_back(table); table.clear();
	table.push_back(UNDERSCORE);	table.push_back(UNDERSCORE); parentTable.push_back(table); table.clear();
	table.push_back(INT);			table.push_back(UNDERSCORE); parentTable.push_back(table); table.clear();
	table.push_back(PROG_L);		table.push_back(UNDERSCORE); parentTable.push_back(table); table.clear();

	table.push_back(STMT);			table.push_back(INT); parentTable.push_back(table); table.clear();
	table.push_back(IF);			table.push_back(INT); parentTable.push_back(table); table.clear();
	table.push_back(WHILE);			table.push_back(INT); parentTable.push_back(table); table.clear();
	table.push_back(ASSIGN);		table.push_back(INT); parentTable.push_back(table); table.clear();
	table.push_back(UNDERSCORE);	table.push_back(INT); parentTable.push_back(table); table.clear();
	table.push_back(INT);			table.push_back(INT); parentTable.push_back(table); table.clear();
	table.push_back(PROG_L);		table.push_back(INT); parentTable.push_back(table); table.clear();

	table.push_back(STMT);			table.push_back(PROG_L); parentTable.push_back(table); table.clear();
	table.push_back(IF);			table.push_back(PROG_L); parentTable.push_back(table); table.clear();
	table.push_back(WHILE);			table.push_back(PROG_L); parentTable.push_back(table); table.clear();
	table.push_back(ASSIGN);		table.push_back(PROG_L); parentTable.push_back(table); table.clear();
	table.push_back(UNDERSCORE);	table.push_back(PROG_L); parentTable.push_back(table); table.clear();
	table.push_back(INT);			table.push_back(PROG_L); parentTable.push_back(table); table.clear();
	table.push_back(PROG_L);		table.push_back(PROG_L); parentTable.push_back(table); table.clear();

	table.push_back(STMT);			table.push_back(CALL); parentTable.push_back(table); table.clear();
	table.push_back(IF);			table.push_back(CALL); parentTable.push_back(table); table.clear();
	table.push_back(WHILE);			table.push_back(CALL); parentTable.push_back(table); table.clear();
	table.push_back(ASSIGN);		table.push_back(CALL); parentTable.push_back(table); table.clear();
	table.push_back(UNDERSCORE);	table.push_back(CALL); parentTable.push_back(table); table.clear();
	table.push_back(INT);			table.push_back(CALL); parentTable.push_back(table); table.clear();
	table.push_back(PROG_L);		table.push_back(CALL); parentTable.push_back(table); table.clear();


}

void dataBase::populateFollowsTables(){
	vector<int> table;
	table.push_back(STMT);			table.push_back(STMT); followsTable.push_back(table); table.clear();
	table.push_back(IF);			table.push_back(STMT); followsTable.push_back(table); table.clear();
	table.push_back(WHILE);			table.push_back(STMT); followsTable.push_back(table); table.clear();
	table.push_back(ASSIGN);		table.push_back(STMT); followsTable.push_back(table); table.clear();
	table.push_back(UNDERSCORE);	table.push_back(STMT); followsTable.push_back(table); table.clear();
	table.push_back(INT);			table.push_back(STMT); followsTable.push_back(table); table.clear();
	table.push_back(CALL);			table.push_back(STMT); followsTable.push_back(table); table.clear();
	table.push_back(PROG_L);		table.push_back(STMT); followsTable.push_back(table); table.clear();

	table.push_back(STMT);			table.push_back(IF); followsTable.push_back(table); table.clear();
	table.push_back(IF);			table.push_back(IF); followsTable.push_back(table); table.clear();
	table.push_back(WHILE);			table.push_back(IF); followsTable.push_back(table); table.clear();
	table.push_back(ASSIGN);		table.push_back(IF); followsTable.push_back(table); table.clear();
	table.push_back(UNDERSCORE);	table.push_back(IF); followsTable.push_back(table); table.clear();
	table.push_back(INT);			table.push_back(IF); followsTable.push_back(table); table.clear();
	table.push_back(CALL);			table.push_back(IF); followsTable.push_back(table); table.clear();
	table.push_back(PROG_L);		table.push_back(IF); followsTable.push_back(table); table.clear();

	table.push_back(STMT);			table.push_back(WHILE); followsTable.push_back(table); table.clear();
	table.push_back(IF);			table.push_back(WHILE); followsTable.push_back(table); table.clear();
	table.push_back(WHILE);			table.push_back(WHILE); followsTable.push_back(table); table.clear();
	table.push_back(ASSIGN);		table.push_back(WHILE); followsTable.push_back(table); table.clear();
	table.push_back(UNDERSCORE);	table.push_back(WHILE); followsTable.push_back(table); table.clear();
	table.push_back(INT);			table.push_back(WHILE);	followsTable.push_back(table); table.clear();
	table.push_back(CALL);			table.push_back(WHILE); followsTable.push_back(table); table.clear();
	table.push_back(PROG_L);		table.push_back(WHILE); followsTable.push_back(table); table.clear();

	table.push_back(STMT);			table.push_back(ASSIGN); followsTable.push_back(table); table.clear();
	table.push_back(IF);			table.push_back(ASSIGN); followsTable.push_back(table); table.clear();
	table.push_back(WHILE);			table.push_back(ASSIGN); followsTable.push_back(table); table.clear();
	table.push_back(ASSIGN);		table.push_back(ASSIGN); followsTable.push_back(table); table.clear();
	table.push_back(UNDERSCORE);	table.push_back(ASSIGN); followsTable.push_back(table); table.clear();
	table.push_back(INT);			table.push_back(ASSIGN); followsTable.push_back(table); table.clear();
	table.push_back(CALL);			table.push_back(ASSIGN); followsTable.push_back(table); table.clear();
	table.push_back(PROG_L);		table.push_back(ASSIGN); followsTable.push_back(table); table.clear();

	table.push_back(STMT);			table.push_back(UNDERSCORE); followsTable.push_back(table); table.clear();
	table.push_back(IF);			table.push_back(UNDERSCORE); followsTable.push_back(table); table.clear();
	table.push_back(WHILE);			table.push_back(UNDERSCORE); followsTable.push_back(table); table.clear();
	table.push_back(ASSIGN);		table.push_back(UNDERSCORE); followsTable.push_back(table); table.clear();
	table.push_back(UNDERSCORE);	table.push_back(UNDERSCORE); followsTable.push_back(table); table.clear();
	table.push_back(INT);			table.push_back(UNDERSCORE); followsTable.push_back(table); table.clear();
	table.push_back(CALL);			table.push_back(UNDERSCORE); followsTable.push_back(table); table.clear();
	table.push_back(PROG_L);		table.push_back(UNDERSCORE); followsTable.push_back(table); table.clear();

	table.push_back(STMT);			table.push_back(INT); followsTable.push_back(table); table.clear();
	table.push_back(IF);			table.push_back(INT); followsTable.push_back(table); table.clear();
	table.push_back(WHILE);			table.push_back(INT); followsTable.push_back(table); table.clear();
	table.push_back(ASSIGN);		table.push_back(INT); followsTable.push_back(table); table.clear();
	table.push_back(UNDERSCORE);	table.push_back(INT); followsTable.push_back(table); table.clear();
	table.push_back(INT);			table.push_back(INT); followsTable.push_back(table); table.clear();
	table.push_back(CALL);			table.push_back(INT); followsTable.push_back(table); table.clear();
	table.push_back(PROG_L);		table.push_back(INT); followsTable.push_back(table); table.clear();

	table.push_back(STMT);			table.push_back(CALL); followsTable.push_back(table); table.clear();
	table.push_back(IF);			table.push_back(CALL); followsTable.push_back(table); table.clear();
	table.push_back(WHILE);			table.push_back(CALL); followsTable.push_back(table); table.clear();
	table.push_back(ASSIGN);		table.push_back(CALL); followsTable.push_back(table); table.clear();
	table.push_back(UNDERSCORE);	table.push_back(CALL); followsTable.push_back(table); table.clear();
	table.push_back(INT);			table.push_back(CALL); followsTable.push_back(table); table.clear();
	table.push_back(CALL);			table.push_back(CALL); followsTable.push_back(table); table.clear();
	table.push_back(PROG_L);		table.push_back(CALL); followsTable.push_back(table); table.clear();
	
	table.push_back(STMT);			table.push_back(PROG_L); followsTable.push_back(table); table.clear();
	table.push_back(IF);			table.push_back(PROG_L); followsTable.push_back(table); table.clear();
	table.push_back(WHILE);			table.push_back(PROG_L); followsTable.push_back(table); table.clear();
	table.push_back(ASSIGN);		table.push_back(PROG_L); followsTable.push_back(table); table.clear();
	table.push_back(UNDERSCORE);	table.push_back(PROG_L); followsTable.push_back(table); table.clear();
	table.push_back(INT);			table.push_back(PROG_L); followsTable.push_back(table); table.clear();
	table.push_back(CALL);			table.push_back(PROG_L); followsTable.push_back(table); table.clear();
	table.push_back(PROG_L);		table.push_back(PROG_L); followsTable.push_back(table); table.clear();
}

void dataBase::populateNextTables(){
	vector<int> table;
	table.push_back(PROG_L);		table.push_back(PROG_L); nextTable.push_back(table); table.clear();
	table.push_back(INT);			table.push_back(PROG_L); nextTable.push_back(table); table.clear();
	table.push_back(UNDERSCORE);	table.push_back(PROG_L); nextTable.push_back(table); table.clear();
	

	table.push_back(PROG_L);		table.push_back(INT); nextTable.push_back(table); table.clear();
	table.push_back(INT);			table.push_back(INT); nextTable.push_back(table); table.clear();
	table.push_back(UNDERSCORE);	table.push_back(INT); nextTable.push_back(table); table.clear();
	
	table.push_back(PROG_L);		table.push_back(UNDERSCORE); nextTable.push_back(table); table.clear();
	table.push_back(INT);			table.push_back(UNDERSCORE); nextTable.push_back(table); table.clear();
	table.push_back(UNDERSCORE);	table.push_back(UNDERSCORE); nextTable.push_back(table); table.clear();
	
}
void dataBase::populateAffectsTables(){
	vector<int> table;
	table.push_back(STMT);			table.push_back(STMT); affectsTable.push_back(table); table.clear();
	table.push_back(ASSIGN);		table.push_back(STMT); affectsTable.push_back(table); table.clear();
	table.push_back(UNDERSCORE);	table.push_back(STMT); affectsTable.push_back(table); table.clear();
	table.push_back(INT);			table.push_back(STMT); affectsTable.push_back(table); table.clear();
	table.push_back(PROG_L);		table.push_back(STMT); affectsTable.push_back(table); table.clear();

	table.push_back(STMT);			table.push_back(ASSIGN); affectsTable.push_back(table); table.clear();
	table.push_back(ASSIGN);		table.push_back(ASSIGN); affectsTable.push_back(table); table.clear();
	table.push_back(UNDERSCORE);	table.push_back(ASSIGN); affectsTable.push_back(table); table.clear();
	table.push_back(INT);			table.push_back(ASSIGN); affectsTable.push_back(table); table.clear();
	table.push_back(PROG_L);		table.push_back(ASSIGN); affectsTable.push_back(table); table.clear();

	table.push_back(STMT);			table.push_back(UNDERSCORE); affectsTable.push_back(table); table.clear();
	table.push_back(ASSIGN);		table.push_back(UNDERSCORE); affectsTable.push_back(table); table.clear();
	table.push_back(UNDERSCORE);	table.push_back(UNDERSCORE); affectsTable.push_back(table); table.clear();
	table.push_back(INT);			table.push_back(UNDERSCORE); affectsTable.push_back(table); table.clear();
	table.push_back(PROG_L);		table.push_back(UNDERSCORE); affectsTable.push_back(table); table.clear();

	table.push_back(STMT);			table.push_back(INT); affectsTable.push_back(table); table.clear();
	table.push_back(ASSIGN);		table.push_back(INT); affectsTable.push_back(table); table.clear();
	table.push_back(UNDERSCORE);	table.push_back(INT); affectsTable.push_back(table); table.clear();
	table.push_back(INT);			table.push_back(INT); affectsTable.push_back(table); table.clear();
	table.push_back(PROG_L);		table.push_back(INT); affectsTable.push_back(table); table.clear();

	table.push_back(STMT);			table.push_back(PROG_L); affectsTable.push_back(table); table.clear();
	table.push_back(ASSIGN);		table.push_back(PROG_L); affectsTable.push_back(table); table.clear();
	table.push_back(UNDERSCORE);	table.push_back(PROG_L); affectsTable.push_back(table); table.clear();
	table.push_back(INT);			table.push_back(PROG_L); affectsTable.push_back(table); table.clear();
	table.push_back(PROG_L);		table.push_back(PROG_L); affectsTable.push_back(table); table.clear();


}
void dataBase::populateWithTables(){
	vector<int> table;
	table.push_back(STMT);		table.push_back(STMT_NUM);		table.push_back(INT);			table.push_back(-1);				withTable.push_back(table); table.clear();
	table.push_back(ASSIGN);	table.push_back(STMT_NUM);		table.push_back(INT);			table.push_back(-1);				withTable.push_back(table); table.clear();
	table.push_back(WHILE);		table.push_back(STMT_NUM);		table.push_back(INT);			table.push_back(-1);				withTable.push_back(table); table.clear();
	table.push_back(IF);		table.push_back(STMT_NUM);		table.push_back(INT);			table.push_back(-1);				withTable.push_back(table); table.clear();
	table.push_back(CALL);		table.push_back(STMT_NUM);		table.push_back(INT);			table.push_back(-1);				withTable.push_back(table); table.clear();
	table.push_back(PROG_L);	table.push_back(PROG_LINE_NUM);	table.push_back(INT);			table.push_back(-1);				withTable.push_back(table); table.clear();
	table.push_back(CONSTANT);	table.push_back(VALUE);			table.push_back(INT);			table.push_back(-1);				withTable.push_back(table); table.clear();

	table.push_back(STMT);		table.push_back(STMT_NUM);		table.push_back(STMT);			table.push_back(STMT_NUM);				withTable.push_back(table); table.clear();
	table.push_back(ASSIGN);	table.push_back(STMT_NUM);		table.push_back(STMT);			table.push_back(STMT_NUM);				withTable.push_back(table); table.clear();
	table.push_back(WHILE);		table.push_back(STMT_NUM);		table.push_back(STMT);			table.push_back(STMT_NUM);				withTable.push_back(table); table.clear();
	table.push_back(IF);		table.push_back(STMT_NUM);		table.push_back(STMT);			table.push_back(STMT_NUM);				withTable.push_back(table); table.clear();
	table.push_back(CALL);		table.push_back(STMT_NUM);		table.push_back(STMT);			table.push_back(STMT_NUM);				withTable.push_back(table); table.clear();
	table.push_back(PROG_L);	table.push_back(PROG_LINE_NUM);	table.push_back(STMT);			table.push_back(STMT_NUM);				withTable.push_back(table); table.clear();
	table.push_back(CONSTANT);	table.push_back(VALUE);			table.push_back(STMT);			table.push_back(STMT_NUM);				withTable.push_back(table); table.clear();

	table.push_back(STMT);		table.push_back(STMT_NUM);		table.push_back(ASSIGN);		table.push_back(STMT_NUM);				withTable.push_back(table); table.clear();
	table.push_back(ASSIGN);	table.push_back(STMT_NUM);		table.push_back(ASSIGN);		table.push_back(STMT_NUM);				withTable.push_back(table); table.clear();
	table.push_back(WHILE);		table.push_back(STMT_NUM);		table.push_back(ASSIGN);		table.push_back(STMT_NUM);				withTable.push_back(table); table.clear();
	table.push_back(IF);		table.push_back(STMT_NUM);		table.push_back(ASSIGN);		table.push_back(STMT_NUM);				withTable.push_back(table); table.clear();
	table.push_back(CALL);		table.push_back(STMT_NUM);		table.push_back(ASSIGN);		table.push_back(STMT_NUM);				withTable.push_back(table); table.clear();
	table.push_back(PROG_L);	table.push_back(PROG_LINE_NUM);	table.push_back(ASSIGN);		table.push_back(STMT_NUM);				withTable.push_back(table); table.clear();
	table.push_back(CONSTANT);	table.push_back(VALUE);			table.push_back(ASSIGN);		table.push_back(STMT_NUM);				withTable.push_back(table); table.clear();

	table.push_back(STMT);		table.push_back(STMT_NUM);		table.push_back(WHILE);			table.push_back(STMT_NUM);				withTable.push_back(table); table.clear();
	table.push_back(ASSIGN);	table.push_back(STMT_NUM);		table.push_back(WHILE);			table.push_back(STMT_NUM);				withTable.push_back(table); table.clear();
	table.push_back(WHILE);		table.push_back(STMT_NUM);		table.push_back(WHILE);			table.push_back(STMT_NUM);				withTable.push_back(table); table.clear();
	table.push_back(IF);		table.push_back(STMT_NUM);		table.push_back(WHILE);			table.push_back(STMT_NUM);				withTable.push_back(table); table.clear();
	table.push_back(CALL);		table.push_back(STMT_NUM);		table.push_back(WHILE);			table.push_back(STMT_NUM);				withTable.push_back(table); table.clear();
	table.push_back(PROG_L);	table.push_back(PROG_LINE_NUM);	table.push_back(WHILE);			table.push_back(STMT_NUM);				withTable.push_back(table); table.clear();
	table.push_back(CONSTANT);	table.push_back(VALUE);			table.push_back(WHILE);			table.push_back(STMT_NUM);				withTable.push_back(table); table.clear();

	table.push_back(STMT);		table.push_back(STMT_NUM);		table.push_back(IF);			table.push_back(STMT_NUM);				withTable.push_back(table); table.clear();
	table.push_back(ASSIGN);	table.push_back(STMT_NUM);		table.push_back(IF);			table.push_back(STMT_NUM);				withTable.push_back(table); table.clear();
	table.push_back(WHILE);		table.push_back(STMT_NUM);		table.push_back(IF);			table.push_back(STMT_NUM);				withTable.push_back(table); table.clear();
	table.push_back(IF);		table.push_back(STMT_NUM);		table.push_back(IF);			table.push_back(STMT_NUM);				withTable.push_back(table); table.clear();
	table.push_back(CALL);		table.push_back(STMT_NUM);		table.push_back(IF);			table.push_back(STMT_NUM);				withTable.push_back(table); table.clear();
	table.push_back(PROG_L);	table.push_back(PROG_LINE_NUM);	table.push_back(IF);			table.push_back(STMT_NUM);				withTable.push_back(table); table.clear();
	table.push_back(CONSTANT);	table.push_back(VALUE);			table.push_back(IF);			table.push_back(STMT_NUM);				withTable.push_back(table); table.clear();

	table.push_back(STMT);		table.push_back(STMT_NUM);		table.push_back(CALL);			table.push_back(STMT_NUM);				withTable.push_back(table); table.clear();
	table.push_back(ASSIGN);	table.push_back(STMT_NUM);		table.push_back(CALL);			table.push_back(STMT_NUM);				withTable.push_back(table); table.clear();
	table.push_back(WHILE);		table.push_back(STMT_NUM);		table.push_back(CALL);			table.push_back(STMT_NUM);				withTable.push_back(table); table.clear();
	table.push_back(IF);		table.push_back(STMT_NUM);		table.push_back(CALL);			table.push_back(STMT_NUM);				withTable.push_back(table); table.clear();
	table.push_back(CALL);		table.push_back(STMT_NUM);		table.push_back(CALL);			table.push_back(STMT_NUM);				withTable.push_back(table); table.clear();
	table.push_back(PROG_L);	table.push_back(PROG_LINE_NUM);	table.push_back(CALL);			table.push_back(STMT_NUM);				withTable.push_back(table); table.clear();
	table.push_back(CONSTANT);	table.push_back(VALUE);			table.push_back(CALL);			table.push_back(STMT_NUM);				withTable.push_back(table); table.clear();

	table.push_back(STMT);		table.push_back(STMT_NUM);		table.push_back(CONSTANT);		table.push_back(VALUE);				withTable.push_back(table); table.clear();
	table.push_back(ASSIGN);	table.push_back(STMT_NUM);		table.push_back(CONSTANT);		table.push_back(VALUE);				withTable.push_back(table); table.clear();
	table.push_back(WHILE);		table.push_back(STMT_NUM);		table.push_back(CONSTANT);		table.push_back(VALUE);				withTable.push_back(table); table.clear();
	table.push_back(IF);		table.push_back(STMT_NUM);		table.push_back(CONSTANT);		table.push_back(VALUE);				withTable.push_back(table); table.clear();
	table.push_back(CALL);		table.push_back(STMT_NUM);		table.push_back(CONSTANT);		table.push_back(VALUE);				withTable.push_back(table); table.clear();
	table.push_back(PROG_L);	table.push_back(PROG_LINE_NUM);	table.push_back(CONSTANT);		table.push_back(VALUE);				withTable.push_back(table); table.clear();
	table.push_back(CONSTANT);	table.push_back(VALUE);			table.push_back(CONSTANT);		table.push_back(VALUE);				withTable.push_back(table); table.clear();

	table.push_back(STMT);		table.push_back(STMT_NUM);		table.push_back(PROG_L);		table.push_back(PROG_LINE_NUM);				withTable.push_back(table); table.clear();
	table.push_back(ASSIGN);	table.push_back(STMT_NUM);		table.push_back(PROG_L);		table.push_back(PROG_LINE_NUM);				withTable.push_back(table); table.clear();
	table.push_back(WHILE);		table.push_back(STMT_NUM);		table.push_back(PROG_L);		table.push_back(PROG_LINE_NUM);				withTable.push_back(table); table.clear();
	table.push_back(IF);		table.push_back(STMT_NUM);		table.push_back(PROG_L);		table.push_back(PROG_LINE_NUM);				withTable.push_back(table); table.clear();
	table.push_back(CALL);		table.push_back(STMT_NUM);		table.push_back(PROG_L);		table.push_back(PROG_LINE_NUM);				withTable.push_back(table); table.clear();
	table.push_back(PROG_L);	table.push_back(PROG_LINE_NUM);	table.push_back(PROG_L);		table.push_back(PROG_LINE_NUM);				withTable.push_back(table); table.clear();
	table.push_back(CONSTANT);	table.push_back(VALUE);			table.push_back(PROG_L);		table.push_back(PROG_LINE_NUM);				withTable.push_back(table); table.clear();

	table.push_back(PROCEDURE);	table.push_back(PROCNAME);		table.push_back(VAROFSIMPLE);	table.push_back(-1);			withTable.push_back(table); table.clear();
	table.push_back(PROCEDURE);	table.push_back(PROCNAME);		table.push_back(PROCOFSIMPLE);	table.push_back(-1);			withTable.push_back(table); table.clear();
	table.push_back(PROCEDURE);	table.push_back(PROCNAME);		table.push_back(PROCEDURE);		table.push_back(PROCNAME);		withTable.push_back(table); table.clear();
	table.push_back(PROCEDURE);	table.push_back(PROCNAME);		table.push_back(CALL);			table.push_back(PROCNAME);		withTable.push_back(table); table.clear();
	table.push_back(PROCEDURE);	table.push_back(PROCNAME);		table.push_back(VARIABLE);		table.push_back(VARNAME);		withTable.push_back(table); table.clear();
	
	table.push_back(VARIABLE);	table.push_back(VARNAME);		table.push_back(VAROFSIMPLE);	table.push_back(-1);			withTable.push_back(table); table.clear();
	table.push_back(VARIABLE);	table.push_back(VARNAME);		table.push_back(PROCOFSIMPLE);	table.push_back(-1);			withTable.push_back(table); table.clear();
	table.push_back(VARIABLE);	table.push_back(VARNAME);		table.push_back(PROCEDURE);		table.push_back(PROCNAME);		withTable.push_back(table); table.clear();
	table.push_back(VARIABLE);	table.push_back(VARNAME);		table.push_back(CALL);			table.push_back(PROCNAME);		withTable.push_back(table); table.clear();
	table.push_back(VARIABLE);	table.push_back(VARNAME);		table.push_back(VARIABLE);		table.push_back(VARNAME);		withTable.push_back(table); table.clear();

	table.push_back(CALL);		table.push_back(PROCNAME);		table.push_back(VAROFSIMPLE);	table.push_back(-1);			withTable.push_back(table); table.clear();
	table.push_back(CALL);		table.push_back(PROCNAME);		table.push_back(PROCOFSIMPLE);	table.push_back(-1);			withTable.push_back(table); table.clear();
	table.push_back(CALL);		table.push_back(PROCNAME);		table.push_back(PROCEDURE);		table.push_back(PROCNAME);		withTable.push_back(table); table.clear();
	table.push_back(CALL);		table.push_back(PROCNAME);		table.push_back(CALL);			table.push_back(PROCNAME);		withTable.push_back(table); table.clear();
	table.push_back(CALL);		table.push_back(PROCNAME);		table.push_back(VARIABLE);		table.push_back(VARNAME);		withTable.push_back(table); table.clear();

}

void dataBase::populateAllTables(){

	populateModifiesTables();
	populateCallsTables();
	populateUseTables();
	populateParentTables();
	populateFollowsTables();
	populateNextTables();
	populateAffectsTables();
	populateWithTables();

}
/////////////////////////////////////////// Populate Functions End ///////////////////////////////////////


/////////////////////////////////////////// Display Functions Start ///////////////////////////////////////
void dataBase::displayTable(vector<vector<int>> &table){
	cout<<"No of Rows:  "<<table.size()<<endl;
	for(int i=0;i< int(table.size());i++)
	{
		for(int j=0;j<int(table.at(i).size());j++)
		{
			cout<<table.at(i).at(j)<<" ";
		}
		cout<<endl;
	}
}
/////////////////////////////////////////// Display Functions End //////////////////////////////////////////


/////////////////////////////////////////// Accessor Functions Start ///////////////////////////////////////
void dataBase::getModifyTable(vector<vector<int>> &table){
	table = modifyTable;
}

void dataBase::getCallsTable(vector<vector<int>> &table){
	table = callsTable;
}

void dataBase::getUseTable(vector<vector<int>> &table){
	table = useTable;
}

void dataBase::getParentTable(vector<vector<int>> &table){
	table = parentTable;
}

void dataBase::getFollowsTable(vector<vector<int>> &table){
	table = followsTable;
}

void dataBase::getNextTable(vector<vector<int>> &table){
	table = nextTable;
}

void dataBase::getAffectsTable(vector<vector<int>> &table){
	table = affectsTable;
}

void dataBase::getWithTable(vector<vector<int>> &table){

	table = withTable;
}
/////////////////////////////////////////// Accessor Functions End ///////////////////////////////////////

