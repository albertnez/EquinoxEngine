#pragma once
#include "Module.h"
#include "GameObject.h"

#include <list>


class EditorSubmodule;
class DataImporter;

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

	DataImporter* GetDataImporter() const;

private:
	bool _drawHierachy = false;
	std::list<float> _fpsValues;
	std::vector<EditorSubmodule*> _submodules;
	DataImporter* _dataImporter = nullptr;

	std::shared_ptr<class ModuleWindow> _moduleWindow;
};
