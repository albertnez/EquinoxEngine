#include "EditorUtils.h"
#include <unordered_map>

class EditorSubmoduleFactoryDictionary : public IEditorSubmoduleFactoryDictionary
{
public:
	void Insert(EditorSubmoduleFactoryBase* factory)
	{
		_factoryMap[factory->GetClassName()] = factory;
	}

	std::vector<EditorSubmoduleFactoryBase*> GetAllFactories() const
	{
		std::vector<EditorSubmoduleFactoryBase*> ret;
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

private:
	std::unordered_map<std::string, EditorSubmoduleFactoryBase*> _factoryMap;
};

IEditorSubmoduleFactoryDictionary* GetEditorSubmoduleFactoryDictionary()
{
	static EditorSubmoduleFactoryDictionary dictionary;
	return &dictionary;
}
