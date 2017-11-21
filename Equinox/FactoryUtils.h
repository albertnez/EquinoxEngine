#pragma once

#include <vector>

template<class FactoryBaseClass>
class IFactoryDictionary
{
public:
	virtual ~IFactoryDictionary() = default;

	virtual void Insert(FactoryBaseClass* factory) = 0;
	virtual std::vector<FactoryBaseClass*> GetAllFactories() const = 0;
	virtual size_t Size() const = 0;
};

template<class BaseClass>
class AbstractFactoryBase
{
public:
	virtual ~AbstractFactoryBase() = default;

	virtual std::string GetClassName() = 0;
	virtual BaseClass* Instantiate() = 0;
};

template<class FactoryBaseClass>
IFactoryDictionary<FactoryBaseClass>* GetFactoryDictionary();

template<class ElementClass, class BaseClass>
class Factory : AbstractFactoryBase<BaseClass>
{
	static_assert(std::is_base_of<BaseClass, ElementClass>::value, "The specified element does not inherit from base class");
public:
	Factory()
	{
		GetFactoryDictionary<AbstractFactoryBase<BaseClass>>()->Insert(this);
	}

	std::string GetClassName() override
	{
		return typeid(ElementClass).name();
	}

	ElementClass* Instantiate() override
	{
		return new ElementClass;
	}
};

#define CREATE_STATIC_FACTORY(BaseType) \
	class BaseType##; \
	using BaseType##FactoryBase = AbstractFactoryBase<BaseType>; \
	template<class ElementType> using BaseType##Factory = Factory<ElementType, BaseType>; \
	using I##BaseType##FactoryDictionary = IFactoryDictionary<BaseType##FactoryBase>; \
	extern I##BaseType##FactoryDictionary* Get##BaseType##FactoryDictionary();

#define IMPLEMENT_STATIC_FACTORY(BaseType) \
	template<> I##BaseType##FactoryDictionary* GetFactoryDictionary() \
	{ \
		static FactoryDictionary<BaseType##FactoryBase> dictionary; \
		return &dictionary; \
	} \
	I##BaseType##FactoryDictionary* Get##BaseType##FactoryDictionary() { return GetFactoryDictionary<BaseType##FactoryBase>(); }
