#pragma once
#include "Module.h"
#include <unordered_map>

struct Material;

class ModuleMaterialManager :
	public Module
{
public:
	ModuleMaterialManager();
	~ModuleMaterialManager();

	bool CleanUp() override;

	Material* CreateMaterial();
	Material* GetMaterial(int material);

private:
	int _lastId = 0;

	std::unordered_map<int, Material*> _materialContainer;
};

