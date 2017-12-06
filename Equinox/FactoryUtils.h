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
	virtual void Clear() = 0;
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

template<class ElementClass, typename BaseClass>
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

	BaseClass* Instantiate() override
	{
		return new ElementClass;
	}
};

#define CREATE_NAMED_STATIC_FACTORY_BASE(BaseType, FactoryName) \
	class BaseType##; \
	using FactoryName##FactoryBase = AbstractFactoryBase<BaseType>; \
	template<class ElementType> using FactoryName##Factory = Factory<ElementType, BaseType>; \
	using I##FactoryName##FactoryDictionary = IFactoryDictionary<FactoryName##FactoryBase>;

#define IMPLEMENT_NAMED_STATIC_FACTORY_ACCESSOR(BaseType, FactoryName) \
	I##FactoryName##FactoryDictionary* Get##FactoryName##FactoryDictionary() { return GetFactoryDictionary<FactoryName##FactoryBase>(); }

#define CREATE_NAMED_STATIC_FACTORY_NOEXPORT(BaseType, FactoryName) \
	CREATE_NAMED_STATIC_FACTORY_BASE(BaseType, FactoryName) \
	IMPLEMENT_NAMED_STATIC_FACTORY_ACCESSOR(BaseType, FactoryName)

#define CREATE_NAMED_STATIC_FACTORY(BaseType, FactoryName) \
	CREATE_NAMED_STATIC_FACTORY_BASE(BaseType, FactoryName) \
	extern I##FactoryName##FactoryDictionary* Get##FactoryName##FactoryDictionary();

#define IMPLEMENT_NAMED_STATIC_FACTORY(BaseType, FactoryName) \
	template<> I##FactoryName##FactoryDictionary* GetFactoryDictionary() \
	{ \
		static FactoryDictionary<FactoryName##FactoryBase> dictionary; \
		return &dictionary; \
	} \
	IMPLEMENT_NAMED_STATIC_FACTORY_ACCESSOR(BaseType, FactoryName)

#define CREATE_STATIC_FACTORY_NOEXPORT(BaseType) CREATE_NAMED_STATIC_FACTORY_NOEXPORT(BaseType, BaseType)

#define CREATE_STATIC_FACTORY(BaseType)  CREATE_NAMED_STATIC_FACTORY(BaseType, BaseType)

#define IMPLEMENT_STATIC_FACTORY(BaseType) IMPLEMENT_NAMED_STATIC_FACTORY(BaseType, BaseType)
