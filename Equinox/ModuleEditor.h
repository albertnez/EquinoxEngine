#pragma once
#include "Module.h"
#include <list>
#include "GameObject.h"

class DataImporter;
class ModuleEditor :
	public Module
{
public:
	ModuleEditor();
	~ModuleEditor();

	bool Init() override;
	bool Start() override;
	update_status PreUpdate(float DeltaTime) override;
	update_status Update(float DeltaTime) override;
	update_status PostUpdate(float DeltaTime) override;
	bool CleanUp() override;

	bool IsPlaying() const { return _isPlaying; }
	bool IsPaused() const { return _isPaused; }

	DataImporter* GetDataImporter() const;

	GameObject* SelectedGameObject = nullptr;

	bool DrawHierachy = false;
	bool DrawQuadtree = false;

private:
	bool _wireframe = false;
	bool _isPlaying = false;
	bool _isPaused = false;
	std::list<float> _fpsValues;
	DataImporter* _dataImporter = nullptr;

	static float ListGetter(void* data, int id);
};

