#pragma once
#include "Module.h"

#include <unordered_map>

class Mesh;

class ModuleMeshManager :
	public Module
{
public:
	ModuleMeshManager();
	~ModuleMeshManager();

	bool Init() override;
	bool CleanUp() override;

	std::shared_ptr<Mesh> CreateMesh(const std::string& name);
	std::shared_ptr<Mesh> GetMesh(const std::string& name) const;

private:
	int _lastId = 0;

	std::unordered_map<std::string, std::shared_ptr<Mesh>> _meshContainer;
};

