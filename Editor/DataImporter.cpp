#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "DataImporter.h"
#include "Globals.h"
#include "MeshComponent.h"
#include "Level.h"
#include "Transform.h"
#include "ModuleTextures.h"
#include "ModuleMaterialManager.h"
#include "ModuleMeshManager.h"
#include "ModuleAnimation.h"

std::shared_ptr<Level> DataImporter::ImportLevel(const char* path, const char* file) const
{
	LOG("Importing level %s", file);
	char filePath[256];
	sprintf_s(filePath, "%s%s", path, file);

	const aiScene* scene = aiImportFile(filePath, aiProcessPreset_TargetRealtime_MaxQuality);

	aiNode* node = scene->mRootNode;

	std::vector<std::shared_ptr<Mesh>> meshes;
	std::vector<Material*> materials;
	meshes.reserve(scene->mNumMeshes);
	//ImportMeshes(scene, path, meshes);
	ImportMaterials(scene, path, materials);

	std::shared_ptr<Level> level = std::make_shared<Level>();

	LoadNodes(scene, node, level->GetRootNode(), materials);
	level->RegenerateQuadtree(); // TODO: Improve quadtree generation

	aiReleaseImport(scene);

	return level;
}

std::shared_ptr<Animation> DataImporter::ImportAnimation(const char* name, const char* filePath) const
{
	LOG("Loading animation %s", filePath);

	const aiScene* scene = aiImportFile(filePath, aiProcessPreset_TargetRealtime_MaxQuality);

	aiAnimation** animations = scene->mAnimations;

	std::shared_ptr<Animation> animation = App->GetModule<ModuleAnimation>()->CreateAnimation(name);
	animation->Duration = animations[0]->mDuration;
	animation->Channels = std::vector<Channel*>(animations[0]->mNumChannels);

	for (unsigned int i = 0; i < animations[0]->mNumChannels; ++i)
	{
		aiNodeAnim* aiNodeAnim = animations[0]->mChannels[i];
		
		animation->Channels[i] = new Channel();
		animation->Channels[i]->NodeName = aiNodeAnim->mNodeName.C_Str();

		for(unsigned int j = 0; j < aiNodeAnim->mNumPositionKeys; ++j)
		{
			aiVector3D position = aiNodeAnim->mPositionKeys[j].mValue;
			animation->Channels[i]->Positions.push_back(&float3(position.x, position.y, position.z));
		}

		for (unsigned int j = 0; j < aiNodeAnim->mNumRotationKeys; ++j)
		{
			aiQuaternion rotation = aiNodeAnim->mRotationKeys[j].mValue;
			animation->Channels[i]->Rotations.push_back(&Quat(rotation.x, rotation.y, rotation.z, rotation.w));
		}
	}

	aiReleaseImport(scene);

	return animation;
}

void DataImporter::ImportMeshes(const aiScene* scene, const char* path, std::vector<std::shared_ptr<Mesh>>& meshes)
{
	std::shared_ptr<ModuleTextures> moduleTextures = App->GetModule<ModuleTextures>();
	std::shared_ptr<ModuleMaterialManager> materialManager = App->GetModule<ModuleMaterialManager>();
	std::vector<Material*> materials;
	for (size_t i = 0; i < scene->mNumMaterials; ++i)
	{
		aiMaterial* aiMat = scene->mMaterials[i];
		Material* material = materialManager->CreateMaterial();

		aiColor4D ai_property;
		float shininess;

		if (aiMat->Get(AI_MATKEY_COLOR_AMBIENT, ai_property) == AI_SUCCESS)
			material->ambient = float4(&ai_property[0]);
		if (aiMat->Get(AI_MATKEY_COLOR_DIFFUSE, ai_property) == AI_SUCCESS)
			material->diffuse = float4(&ai_property[0]);
		if (aiMat->Get(AI_MATKEY_COLOR_SPECULAR, ai_property) == AI_SUCCESS)
			material->specular = float4(&ai_property[0]);
		if (aiMat->Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS)
			material->shininess = shininess;

		int numTexturesByMaterial = scene->mMaterials[i]->GetTextureCount(aiTextureType_DIFFUSE);
		if (numTexturesByMaterial > 0)
		{
			aiMaterial* aMaterial = scene->mMaterials[i];

			aiString fileName;
			aMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &fileName);
			sprintf_s(material->FilePath, "%s%s", path, fileName.C_Str());

			material->texture = moduleTextures->Load(material->FilePath);
		}

		materials.push_back(material);
	}

	std::shared_ptr<ModuleMeshManager> meshManager = App->GetModule<ModuleMeshManager>();
	for (size_t i = 0; i < scene->mNumMeshes; ++i)
	{
		aiMesh* aMesh = scene->mMeshes[i];
		std::shared_ptr<Mesh> mesh = meshManager->CreateMesh(aMesh->mName.C_Str());

		mesh->num_vertices = aMesh->mNumVertices;
		mesh->num_indices = aMesh->mNumFaces * 3;

		mesh->_material = materials[aMesh->mMaterialIndex];

		GLuint* indexes = new uint32_t[aMesh->mNumFaces * 3];

		for (unsigned iFace = 0; iFace < aMesh->mNumFaces; ++iFace)
		{
			aiFace* face = &aMesh->mFaces[iFace];

			indexes[(iFace * 3)] = face->mIndices[0];
			indexes[(iFace * 3) + 1] = face->mIndices[1];
			indexes[(iFace * 3) + 2] = face->mIndices[2];
		}

		if (aMesh->mVertices != nullptr)
		{
			mesh->SetVertices(reinterpret_cast<float3*>(&aMesh->mVertices), aMesh->mNumVertices);
			/*glGenBuffers(1, &mesh->vertexID);
			glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexID);
			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * mesh->num_vertices * 3, &aMesh->mVertices[0], GL_STATIC_DRAW);*/
		}

		if (aMesh->mNormals != nullptr)
		{
			glGenBuffers(1, &mesh->normalID);
			glBindBuffer(GL_ARRAY_BUFFER, mesh->normalID);
			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * mesh->num_vertices * 3, &aMesh->mNormals[0], GL_STATIC_DRAW);
		}

		if (aMesh->mTextureCoords[0] != nullptr)
		{
			glGenBuffers(1, &mesh->textureCoordsID);
			glBindBuffer(GL_ARRAY_BUFFER, mesh->textureCoordsID);
			glBufferData(GL_ARRAY_BUFFER, sizeof(aiVector3D) * mesh->num_vertices, &aMesh->mTextureCoords[0][0], GL_STATIC_DRAW);
		}

		glGenBuffers(1, &mesh->indexesID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexesID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(aiVector3D) * aMesh->mNumFaces, indexes, GL_STATIC_DRAW);

		meshes.push_back(mesh);

		mesh->boundingBox.SetNegativeInfinity();
		mesh->boundingBox.Enclose(reinterpret_cast<float3*>(&aMesh->mVertices[0]), mesh->num_vertices);

		RELEASE_ARRAY(indexes);
	}
}

void DataImporter::ImportMaterials(const aiScene* scene, const char* path, std::vector<Material*>& materials)
{
	std::shared_ptr<ModuleTextures> moduleTextures = App->GetModule<ModuleTextures>();
	std::shared_ptr<ModuleMaterialManager> materialManager = App->GetModule<ModuleMaterialManager>();
	materials.reserve(scene->mNumMaterials);

	for (size_t i = 0; i < scene->mNumMaterials; ++i)
	{
		aiMaterial* aiMat = scene->mMaterials[i];
		Material* material = materialManager->CreateMaterial();

		aiColor4D ai_property;
		float shininess;

		if (aiMat->Get(AI_MATKEY_COLOR_AMBIENT, ai_property) == AI_SUCCESS)
			material->ambient = float4(&ai_property[0]);
		if (aiMat->Get(AI_MATKEY_COLOR_DIFFUSE, ai_property) == AI_SUCCESS)
			material->diffuse = float4(&ai_property[0]);
		if (aiMat->Get(AI_MATKEY_COLOR_SPECULAR, ai_property) == AI_SUCCESS)
			material->specular = float4(&ai_property[0]);
		if (aiMat->Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS)
			material->shininess = shininess;

		int numTexturesByMaterial = scene->mMaterials[i]->GetTextureCount(aiTextureType_DIFFUSE);
		if (numTexturesByMaterial > 0)
		{
			aiMaterial* aMaterial = scene->mMaterials[i];

			aiString fileName;
			aMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &fileName);
			sprintf_s(material->FilePath, "%s%s", path, fileName.C_Str());

			material->texture = moduleTextures->Load(material->FilePath);
		}

		materials.push_back(material);
	}
}

void DataImporter::ImportMeshes(const aiNode* node, const aiScene* scene, const std::vector<Material*>& materials, std::vector<std::shared_ptr<Mesh>>& meshes)
{
	std::shared_ptr<ModuleMeshManager> meshManager = App->GetModule<ModuleMeshManager>();
	meshes.reserve(node->mNumMeshes);

	for (size_t i = 0; i < node->mNumMeshes; ++i)
	{
		std::string name = node->mName.C_Str();
		if (node->mNumMeshes > 1)
		{
			name += "_" + std::to_string(i);
		}
		aiMesh* aMesh = scene->mMeshes[node->mMeshes[i]];
		std::shared_ptr<Mesh> mesh = meshManager->CreateMesh(name);

		mesh->num_indices = aMesh->mNumFaces * 3;

		mesh->SetMaterial(materials[aMesh->mMaterialIndex]);

		GLuint* indexes = new uint32_t[aMesh->mNumFaces * 3];

		for (unsigned iFace = 0; iFace < aMesh->mNumFaces; ++iFace)
		{
			aiFace* face = &aMesh->mFaces[iFace];

			indexes[(iFace * 3)] = face->mIndices[0];
			indexes[(iFace * 3) + 1] = face->mIndices[1];
			indexes[(iFace * 3) + 2] = face->mIndices[2];
		}

		if (aMesh->mVertices != nullptr)
		{
			mesh->SetVertices(reinterpret_cast<float3*>(aMesh->mVertices), aMesh->mNumVertices);
		}

		if (aMesh->mNormals != nullptr)
		{
			mesh->SetNormals(reinterpret_cast<float3*>(aMesh->mNormals), aMesh->mNumVertices);
		}

		if (aMesh->mTextureCoords[0] != nullptr)
		{
			mesh->SetTextureCoords(reinterpret_cast<float3*>(aMesh->mTextureCoords[0]), aMesh->mNumVertices);
		}

		mesh->SetIndexes(indexes, aMesh->mNumFaces * 3);

		RELEASE_ARRAY(indexes);

		meshes.push_back(mesh);
	}
}

void DataImporter::LoadNodes(const aiScene* scene, aiNode* originalNode, GameObject* node, const std::vector<Material*>& materials)
{
	if (originalNode == nullptr)
		return;

	GameObject* children = new GameObject;

	children->Name = originalNode->mName.C_Str();
	children->SetParent(node);

	aiVector3D position;
	aiVector3D scale;
	aiQuaternion rotation;

	originalNode->mTransformation.Decompose(scale, rotation, position);
	Transform& transform = children->GetTransform();
	
	transform.SetLocalTransformMatrix(float4x4::FromTRS(
		float3(position.x, position.y, position.z),
		Quat(rotation.x, rotation.y, rotation.z, rotation.w),
		float3(scale.x, scale.y, scale.z)
	));

	std::vector<vec> vertex_boundingbox;

	if (originalNode->mMeshes != nullptr)
	{
		vertex_boundingbox.resize(originalNode->mNumMeshes * 8);
		MeshComponent* meshComponent = new MeshComponent;
		children->AddComponent(meshComponent);

		MaterialComponent* materialComponent = new MaterialComponent;
		children->AddComponent(materialComponent);

		meshComponent->MaterialComponent = materialComponent;
		std::shared_ptr<ModuleMaterialManager> materialManager = App->GetModule<ModuleMaterialManager>();
		std::vector<std::shared_ptr<Mesh>> meshes;
		ImportMeshes(originalNode, scene, materials, meshes);

		for (size_t i = 0; i < meshes.size(); ++i)
		{
			std::shared_ptr<Mesh> mesh = meshes[i];

			meshComponent->Meshes.push_back(mesh);

			mesh->boundingBox.GetCornerPoints(&vertex_boundingbox[i * 8]);
		}
	}

	children->BoundingBox.SetNegativeInfinity();
	if (!vertex_boundingbox.empty())
		children->BoundingBox.Enclose(reinterpret_cast<float3*>(&vertex_boundingbox[0]), originalNode->mNumMeshes * 8);

	for (size_t i = 0; i < originalNode->mNumChildren; ++i)
	{
		LoadNodes(scene, originalNode->mChildren[i], children, materials);
	}
}
