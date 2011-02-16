#include "Tree.h"

QNode::QNode()//svn is done
{
	Name="root1";
	Type="ROOT";
	ChildList=list<QNode*>();
	ChildIndex=ChildList.begin();
}

	QNode::QNode(string name,string type)
	{
	     Name=name;
		 Type=type;
	}
	string QNode::GetNodeName()
	{
		return Name;
	}
	string QNode::GetNodeType()
	{
		return Type;
	}
	int QNode::GetNodeValue(){
		return Value;
	}

	void QNode::SetNodeValue(int v){
		if(Type != "stmt#") throw "You are not allowed to use this!";
		Value = v;
	}

	bool QNode:: IsEmptyChild()
	{
		if(ChildList.empty()==0)
			return true;
		else
			return false;
	}
	//void AddChild(QNode* child, string postion);
	void QNode::AddChild(QNode* child)
	{
		ChildList.push_back(child);
	}
	QNode* QNode::GetChild(int Position)
	{
		try
		{
		if (IsEmptyChild()!=0&&Position<=GetChildSize()&&Position>=0)
		{
			ChildIndex=ChildList.begin();
			for(int i=1;i<Position;i++)
			{
				ChildIndex++;
			}
			return *ChildIndex;
		}
		}
		catch(...)
		{
			cout<<"eorr by feifeifei";
		}
	}
	int QNode:: GetChildSize()
	{
		return ChildList.size();
	}
		
		
	QNode* QNode::GetParent()
	{
		return parent;
	}
	void QNode::SetParent(QNode* pa)
	{
		parent=pa;
	}
	void QNode::ShowNode()
	{   cout<<"--------------------";
		cout<<GetNodeName()<<endl;
		cout<<GetNodeType()<<endl;
		//cout<<(GetParent()->GetNodeName())<<endl;
		cout<<GetChildSize()<<endl;
		cout<<"--------------------";
	}


	QueryTree::QueryTree()
	{
		root=new QNode();
		Pointer=root;
	}
	QueryTree::QueryTree(string rootname)
	{
		root=new QNode(rootname,"ROOT");
		Pointer=root;
	}
	QNode* QueryTree::GetTRoot()
	{
		Pointer=root;
		return root;
	}
	QNode* QueryTree::GetCurrent()
	{
		return Pointer;
	}
	void QueryTree::GoDown(int Position)
	{
		if (Position<=(Pointer->GetChildSize())&&Position>=0)
		{
				Pointer=Pointer->GetChild(Position);			
		}
		else
		{
			cout<<"no child at such position";
	   }
	}
	void QueryTree::GetUp()
	{
		Pointer=Pointer->GetParent();
	}
	QNode* QueryTree::AddChild(QNode* parent, string name,string type )
	{
		QNode* temp=new QNode(name,type);
		temp->SetParent(parent);
		parent->AddChild(temp);
		return temp;
	}
	
	