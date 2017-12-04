#ifndef __APPLICATION_CPP__
#define __APPLICATION_CPP__

#include "Globals.h"
#include "Module.h"

#include <list>
#include <map>
#include <typeindex>

class ModuleEditorCamera;

// Game modules ---

class Engine
{
public:

	enum State
	{
		CREATION,
		START,
		UPDATE,
		FINISH,
		EXIT
	};

	enum class UpdateState
	{
		Playing,
		Stopped
	};

	Engine();
	~Engine();

	UpdateState GetUpdateState() const;
	void SetUpdateState(const UpdateState state);
	bool IsPaused() const;
	void SetPaused(bool paused);

	int Loop();

	template<typename Type>
	std::shared_ptr<Type> GetModule() const
	{
		static_assert(std::is_base_of<Module, Type>::value, "The specified type does not inherit from module");
		auto it = _moduleMap.find(typeid(Type));
		return it != _moduleMap.end() ? std::static_pointer_cast<Type>(it->second) : nullptr;
	}

	template<typename ModuleType>
	std::shared_ptr<ModuleType> AppendModule()
	{
		static_assert(std::is_base_of<Module, ModuleType>::value, "The specified type does not inherit from module");
		std::shared_ptr<ModuleType> mod(new ModuleType);
		_moduleMap[typeid(ModuleType)] = mod;
		_modules.push_back(mod);

		if (state >= START)
		{
			mod->Init();
			if (true == mod->IsEnabled())
			{
				mod->Start();
			}
		}
		return mod;
	}

	bool Init();
	update_status Update();
	bool CleanUp();

public:
	std::shared_ptr<ModuleEditorCamera> editorCamera;

	float DeltaTime;

private:
	State state = CREATION;
	UpdateState _updateState = UpdateState::Playing;
	bool _isPaused = false;

	std::shared_ptr<class ModuleStats> _statsModule;

	std::map<std::type_index, std::shared_ptr<Module>> _moduleMap;
	std::list<std::shared_ptr<Module>> _modules;

	float _timeFromLastFrame = 0;
};

extern Engine* App;

#endif // __APPLICATION_CPP__