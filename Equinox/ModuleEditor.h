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
	ModuleEditor() = default;
	~ModuleEditor();

	bool Init() override;
	bool Start() override;
	update_status PreUpdate(float DeltaTime) override;
	update_status Update(float DeltaTime) override;
	update_status PostUpdate(float DeltaTime) override;
	bool CleanUp() override;

	bool GetDrawHierachy() const;
	void SetDrawHierachy(bool drawHierachy);

	bool GetDrawQuadtree() const;
	void SetDrawQuadtree(bool drawQuadtree);

	DataImporter* GetDataImporter() const;

	GameObject* SelectedGameObject = nullptr;

private:
	void drawLevelHierachy();
	void drawLevelHierachy(GameObject* node);

	bool _drawHierachy = false;
	bool _drawQuadtree = false;
	std::list<float> _fpsValues;
	std::vector<EditorSubmodule*> _submodules;
	DataImporter* _dataImporter = nullptr;
};

