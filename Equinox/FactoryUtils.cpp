#include "FactoryUtils.h"
#include "FactoryDictionary.h"

template<class FactoryBaseClass>
IFactoryDictionary<FactoryBaseClass>* GetFactoryDictionary()
{
	static FactoryDictionary<FactoryBaseClass> dictionary;
	return &dictionary;
}
