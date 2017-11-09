#include "ModuleMaterialManager.h"
#include "MaterialComponent.h"


ModuleMaterialManager::ModuleMaterialManager()
{
}


ModuleMaterialManager::~ModuleMaterialManager()
{
}

bool ModuleMaterialManager::CleanUp()
{
	LOG("Cleaning materials and MaterialManager");
	LOG("%i materials were loaded", _materialContainer.size());

	for (auto materialPair : _materialContainer)
	{
		RELEASE(materialPair.second);
	}

	_materialContainer.clear();
	return true;
}

Material* ModuleMaterialManager::CreateMaterial()
{
	Material* mat = new Material;
	mat->id = _lastId++;
	_materialContainer[mat->id] = mat;
	return mat;
}

Material* ModuleMaterialManager::GetMaterial(int material)
{
	const auto it = _materialContainer.find(material);
	
	if (it == _materialContainer.end())
	{
		return nullptr;
	}

	return it->second;
}
