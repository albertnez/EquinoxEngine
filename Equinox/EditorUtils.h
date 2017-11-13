#pragma once
#include <vector>

class EditorSubmodule;

class EditorSubmoduleFactoryBase
{
public:
	virtual std::string GetClassName() = 0;
	virtual EditorSubmodule* Instantiate() = 0;
};

class IEditorSubmoduleFactoryDictionary
{
public:
	virtual void Insert(EditorSubmoduleFactoryBase* factory) = 0;
	virtual std::vector<EditorSubmoduleFactoryBase*> GetAllFactories() const = 0;
	virtual size_t Size() const = 0;
};

IEditorSubmoduleFactoryDictionary* GetEditorSubmoduleFactoryDictionary();

template<class Module>
class EditorSubmoduleFactory : EditorSubmoduleFactoryBase
{
	static_assert(std::is_base_of<EditorSubmodule, Module>::value, "The specified module does not inherit from EditorSubmodule");

public:
	EditorSubmoduleFactory()
	{
		GetEditorSubmoduleFactoryDictionary()->Insert(this);
	}

	std::string GetClassName() override
	{
		return typeid(Module).name();
	}

	EditorSubmodule* Instantiate() override
	{
		return new Module;
	}
};

#define REGISTER_EDITOR_SUBMODULE(ClassName) \
	static EditorSubmoduleFactory<ClassName> Factory_##ClassName;

