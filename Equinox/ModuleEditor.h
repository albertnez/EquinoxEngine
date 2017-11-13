#pragma once
#include "Module.h"
#include "GameObject.h"

class EditorSubmodule;
#include <list>

class DataImporter;
class EditorSubmoduleFactoryBase;

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
	void drawLevelHierachy();
	void drawLevelHierachy(GameObject* node);

	bool _wireframe = false;
	bool _isPlaying = false;
	bool _isPaused = false;
	std::list<float> _fpsValues;
	std::vector<EditorSubmodule*> _submodules;
	DataImporter* _dataImporter = nullptr;
};

