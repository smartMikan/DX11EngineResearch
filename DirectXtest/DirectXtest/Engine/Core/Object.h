#pragma once

//struct TreeObj
//{
//	int objID;
//	int numInCurrentLevel;
//	int ChildrenNum;
//	TreeObj* lastObj;
//	TreeObj* nextObj;
//	TreeObj* parentObj;
//	TreeObj* firstChild;
//	TreeObj* lastChild;
//
//	TreeObj* findChildByNum(int num) 
//	{
//		if (num > ChildrenNum || num < 0) {
//			return nullptr;
//		}
//		
//	}
//
//
//	void AddParent(TreeObj* parent)
//	{
//		parentObj = parent;
//		numInCurrentLevel = parent->lastChild->numInCurrentLevel + 1;
//		parent->lastChild = this;
//	}
//
//	void RemoveParent() {
//		
//	}
//};



class Object
{
public:
	Object();
	~Object();





	virtual bool Update();
	virtual bool FixUpdate();
	virtual bool LateUpdate();


};

