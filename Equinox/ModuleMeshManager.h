#pragma once
#include "Module.h"

#include <unordered_map>

struct Mesh;

class ModuleMeshManager :
	public Module
{
public:
	ModuleMeshManager();
	~ModuleMeshManager();

	bool CleanUp() override;

	Mesh* CreateMesh();
	Mesh* GetMeshById(int id) const;

private:
	int _lastId = 0;

	std::unordered_map<int, Mesh*> _meshContainer;
};

