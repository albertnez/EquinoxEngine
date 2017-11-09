#include "ModuleMeshManager.h"
#include "MeshComponent.h"

#include <cassert>

ModuleMeshManager::ModuleMeshManager()
{
}


ModuleMeshManager::~ModuleMeshManager()
{
}

bool ModuleMeshManager::CleanUp()
{
	LOG("Cleaning meshes and MeshManager");
	LOG("%i meshes were loaded", _meshContainer.size());

	for (auto meshPair : _meshContainer)
	{
		RELEASE(meshPair.second);
	}

	_meshContainer.clear();

	return true;
}

Mesh* ModuleMeshManager::CreateMesh()
{
	Mesh* mesh = new Mesh;
	mesh->id = _lastId++;
	_meshContainer[mesh->id] = mesh;
	return mesh;
}

Mesh* ModuleMeshManager::GetMeshById(int id) const
{
	const auto it = _meshContainer.find(id);

	if (it == _meshContainer.end())
	{
		return nullptr;
	}

	return it->second;
}
