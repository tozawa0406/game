//-----------------------------------------------------------------------------
//
//	マネージャークラスの基底[BaseManager.h]
//	Auther : 戸澤翔太
//																	2018/08/18
//-----------------------------------------------------------------------------
#ifndef _BASE_MANAGER_H_
#define _BASE_MANAGER_H_

#include "../Define/Define.h"

class Systems;
class Interface
{
public:
	Interface(Systems* systems) { systems_ = systems; }
	virtual ~Interface(void) {}

	Systems* GetSystems(void) { return systems_; }

	virtual HRESULT Init(void) = 0;
	virtual void    Uninit(void) {}
	virtual void    Update(void) {}
	virtual void    Draw(void)   {}

protected:
	Systems* systems_;

};

template<class Child>
class BaseManager : public Interface
{
public:
	BaseManager(Systems* systems) : Interface(systems) {}

	virtual ~BaseManager(void) { obj_.clear(); }

protected:
	virtual void Add(Child* obj)
	{
		obj_.emplace_back(obj);
	}

	void RemovePtr(Child* obj)
	{
		RemoveVectorPtr(obj_, obj);
	}

	void Remove(Child* obj)
	{
		RemoveVector(obj_, obj);
	}

	std::vector<Child*> obj_;
};

#endif // _BASE_MANAGER_H_