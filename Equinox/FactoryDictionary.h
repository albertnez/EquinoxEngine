#pragma once

// This file is intended to be included before the IMPLEMENT_STATIC_FACTORY macro

#include <unordered_map>
#include "FactoryUtils.h"

template<class FactoryBaseClass>
class FactoryDictionary : public IFactoryDictionary<FactoryBaseClass>
{
public:
	void Insert(FactoryBaseClass* factory)
	{
		_factoryMap[factory->GetClassName()] = factory;
	}

	std::vector<FactoryBaseClass*> GetAllFactories() const override
	{
		std::vector<FactoryBaseClass*> ret;
		ret.reserve(_factoryMap.size());

		for (const auto& pair : _factoryMap)
		{
			ret.push_back(pair.second);
		}

		return ret;
	}

	size_t Size() const
	{
		return _factoryMap.size();
	}

	void Clear()
	{
		_factoryMap.clear();
	}

private:
	std::unordered_map<std::string, FactoryBaseClass*> _factoryMap;
};
