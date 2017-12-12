#pragma once

#include <memory>
#include <vector>

struct Material;
struct Animation;
class GameObject;
class Mesh;
class Level;
struct aiNode;
struct aiScene;

class DataImporter
{
public:
	DataImporter() = default;
	~DataImporter() = default;

	std::shared_ptr<Level> ImportLevel(const char* path, const char* file) const;
	std::shared_ptr<Animation> ImportAnimation(const char* path, const char* file) const;

private:
	static void ImportMeshes(const aiScene* scene, const char* path, std::vector<std::shared_ptr<Mesh>>& meshes);
	static void ImportMaterials(const aiScene* scene, const char* path, std::vector<Material*>& materials);
	static void ImportMeshes(const aiNode* node, const aiScene* scene, const std::vector<Material*>& materials, std::vector<std::shared_ptr<Mesh>>&
	                         meshes);
	static void LoadNodes(const aiScene* scene, aiNode* originalNode, GameObject* node, const std::vector<Material*>& materials);
};
