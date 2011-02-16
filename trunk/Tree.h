#include <string>
#include <sstream>
#include <iostream>
#include <list>
using namespace std;

class QNode
{
public:
	string Name;
	string Type;
	int Value;   //Only applicable for stmt#
    list<QNode*> ChildList;
	list<QNode*>::iterator ChildIndex;

	QNode* parent;
	QNode();

	QNode(string name,string Type);
	void AddChild(QNode* child);
	list<QNode*> GetChildList();
	bool IsEmptyChild();
	int GetChildSize();
	QNode* GetChild(int indx);


	void SetNodeValue(int);

	string GetNodeName();
	string GetNodeType();
	int GetNodeValue();
	
	QNode* GetParent();
	void SetParent(QNode* parent);
    void ShowNode();

};

class  QueryTree
{
private:
	QNode *root;
	QNode *Pointer;
public:
	QueryTree();
	QueryTree(string rootname);
	QNode* GetTRoot();
	QNode* GetCurrent();
	void GetUp();
	void GoDown(int Position);
	
    QNode* AddChild(QNode* parent, string name,string type);
};
