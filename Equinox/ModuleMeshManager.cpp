#include "ModuleMeshManager.h"
#include "MeshComponent.h"

#include <cassert>
#include "Cube.h"

ModuleMeshManager::ModuleMeshManager()
{
}


ModuleMeshManager::~ModuleMeshManager()
{
}

bool ModuleMeshManager::Init()
{
	Cube::GenerateMesh(CreateMesh("Cube"));

	return true;
}

bool ModuleMeshManager::CleanUp()
{
	LOG("Cleaning meshes and MeshManager");
	LOG("%i meshes were loaded", _meshContainer.size());

	for (auto meshPair : _meshContainer)
	{
		meshPair.second.reset();
	}

	_meshContainer.clear();

	return true;
}

std::shared_ptr<Mesh> ModuleMeshManager::CreateMesh(const std::string& name)
{
	auto it = _meshContainer.find(name);
	assert(_meshContainer.end() == it && "Mesh already created!");
	if (_meshContainer.end() == it)
	{
		std::shared_ptr<Mesh> mesh(new Mesh);
		mesh->id = _lastId++;
		mesh->name = name;
		_meshContainer[name] = mesh;
		return mesh;
	}
	return it->second;
}

std::shared_ptr<Mesh> ModuleMeshManager::GetMesh(const std::string& name) const
{
	const auto it = _meshContainer.find(name);

	if (it == _meshContainer.end())
	{
		return nullptr;
	}

	return it->second;
}
