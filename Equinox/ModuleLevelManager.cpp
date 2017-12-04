#include "Level.h"
#include "ParticleEmitter.h"
#include "ModuleAnimation.h"
#include "TransformComponent.h"
#include "ModuleLevelManager.h"

ModuleLevelManager::ModuleLevelManager()
{
}


ModuleLevelManager::~ModuleLevelManager()
{
}

bool ModuleLevelManager::Init()
{
	_currentLevel = std::make_shared<Level>();

	return true;
}

bool ModuleLevelManager::Start()
{
	

	return true;
}

update_status ModuleLevelManager::PreUpdate(float DeltaTime)
{
	return UPDATE_CONTINUE;
}

update_status ModuleLevelManager::Update(float DeltaTime)
{
	_currentLevel->Update(DeltaTime);
	return UPDATE_CONTINUE;
}

update_status ModuleLevelManager::PostUpdate(float DeltaTime)
{
	return UPDATE_CONTINUE;
}

bool ModuleLevelManager::CleanUp()
{
	_currentLevel->CleanUp();
	_currentLevel.reset();

	assert(_currentLevel.use_count() == 0 && "Level is still referenced somewere");

	return true;
}

void ModuleLevelManager::ChangeLevel(const std::shared_ptr<Level> level)
{
	_currentLevel->CleanUp();
	_currentLevel.reset();

	_currentLevel = level;
}

Level& ModuleLevelManager::GetCurrentLevel()
{
	return *_currentLevel;
}

const Level& ModuleLevelManager::GetCurrentLevel() const
{
	return *_currentLevel;
}
