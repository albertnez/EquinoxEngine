#include "Engine.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleCollision.h"
#include "ModuleTimer.h"
#include "ComplexTimer.h"
#include "ModuleLighting.h"
#include "ModuleSettings.h"
#include "ModuleAnimation.h"
#include "ModuleStats.h"
#include "ModuleLevelManager.h"
#include "ModuleMaterialManager.h"
#include "ModuleMeshManager.h"
#include "ModuleCameraManager.h"
#include "ProgramManager.h"
#include "OpenGLGraphicsDevice.h"

using namespace std;

Engine* App;

Engine::Engine()
{
	state = State::CREATION;

	// Order matters: they will init/start/pre/update/post in this order
	AppendModule<ModuleInput>();
	AppendModule<ModuleWindow>();

	AppendModule<ModuleCameraManager>();

	AppendModule<ModuleRender>();
	AppendModule<OpenGLGraphicsDevice>();
	AppendModule<ProgramManager>();
	AppendModule<ModuleTextures>();
	AppendModule<ModuleLighting>();
	AppendModule<ModuleAudio>();
	AppendModule<ModuleSettings>();
	AppendModule<ModuleAnimation>();
	_statsModule = AppendModule<ModuleStats>();

	AppendModule<ModuleMaterialManager>();
	AppendModule<ModuleMeshManager>();

	// Game modules
	AppendModule<ModuleLevelManager>();

	// Modules to draw on top of game logic
	AppendModule<ModuleCollision>();
	AppendModule<ModuleTimer>();

	App = this;
}

Engine::~Engine()
{
	for (auto it = _modules.begin(); it != _modules.end(); ++it)
	{
		(*it).reset();
		assert((*it).use_count() == 0 && "Module is still references somewhere");
	}
}

Engine::UpdateState Engine::GetUpdateState() const
{
	return _updateState;
}

void Engine::SetUpdateState(const UpdateState state)
{
	_updateState = state;
}

bool Engine::IsPaused() const
{
	return _isPaused;
}

void Engine::SetPaused(bool paused)
{
	_isPaused = paused;
}

int Engine::Loop()
{
	int ret = EXIT_FAILURE;
	while (state != EXIT)
	{
		switch (state)
		{
			case CREATION:
			{
				state = START;
				break;
			}
			case START:
			{
				LOG("Engine Init --------------");
				if (App->Init() == false)
				{
					LOG("Engine Init exits with error -----");
					state = EXIT;
				}
				else
				{
					state = UPDATE;
					LOG("Engine Update --------------");
				}

				break;
			}

			case UPDATE:
			{
				int update_return = App->Update();

				if (update_return == UPDATE_ERROR)
				{
					LOG("Engine Update exits with error -----");
					state = EXIT;
				}

				if (update_return == UPDATE_STOP)
					state = FINISH;

				break;
			}

			case FINISH:
			{
				LOG("Engine CleanUp --------------");

				_updateState = UpdateState::Stopped;
				App->Update(); // Run a last update cycle before shutting down

				if (App->CleanUp() == false)
				{
					LOG("Engine CleanUp exits with error -----");
				}
				else
					ret = EXIT_SUCCESS;

				state = EXIT;

				break;
			}
		}
	}

	return ret;
}

bool Engine::Init()
{
	bool ret = true;

	for(auto it = _modules.begin(); it != _modules.end() && ret; ++it)
		ret = (*it)->Init(); // we init everything, even if not enabled

	for(auto it = _modules.begin(); it != _modules.end() && ret; ++it)
	{
		if((*it)->IsEnabled() == true)
			ret = (*it)->Start();
	}

	// Start the first scene --

	return ret;
}

update_status Engine::Update()
{
	update_status ret = UPDATE_CONTINUE;

	float dt = _isPaused ? 0 : DeltaTime;

	for(auto it = _modules.begin(); it != _modules.end() && ret == UPDATE_CONTINUE; ++it)
		if((*it)->IsEnabled() == true) 
			ret = (*it)->PreUpdate(dt);

	for(auto it = _modules.begin(); it != _modules.end() && ret == UPDATE_CONTINUE; ++it)
		if((*it)->IsEnabled() == true) 
			ret = (*it)->Update(dt);

	for(auto it = _modules.begin(); it != _modules.end() && ret == UPDATE_CONTINUE; ++it)
		if((*it)->IsEnabled() == true) 
			ret = (*it)->PostUpdate(dt);

	++_statsModule->_total_frames;

	float currentFrameTime = float(_statsModule->_total_simple_time.Read() / 1E3);
	DeltaTime = float(currentFrameTime - _timeFromLastFrame);
	
	_statsModule->_current_fps = 1 / DeltaTime;

	_statsModule->_current_avg = _statsModule->_current_avg ? (_statsModule->_current_avg + _statsModule->_current_fps) / 2 : _statsModule->_current_fps;

	_timeFromLastFrame = currentFrameTime;

	return ret;
}

bool Engine::CleanUp()
{
	bool ret = true;

	LOG("Total Time: %f microseconds", _statsModule->_total_complex_time.Stop());
	LOG("Total Time: %i miliseconds", _statsModule->_total_simple_time.Stop());
	LOG("Total Frames: %f", _statsModule->_total_frames);
	LOG("Average FPS: %f", _statsModule->_current_avg);

	for(auto it = _modules.rbegin(); it != _modules.rend() && ret; ++it)
		if((*it)->IsEnabled() == true) 
			ret = (*it)->CleanUp();
	return ret;
}
