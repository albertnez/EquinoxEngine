#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "DataImporter.h"
#include "Globals.h"
#include "MeshComponent.h"
#include "Level.h"
#include "TransformComponent.h"
#include "ModuleTextures.h"
#include "ModuleMaterialManager.h"
#include "ModuleMeshManager.h"

namespace
{
	void ImportMeshes(const aiScene* scene, const char* path, std::vector<Mesh*>& meshes)
	{
		std::vector<Material*> materials;
		for (size_t i = 0; i < scene->mNumMaterials; ++i)
		{
			aiMaterial* aiMat = scene->mMaterials[i];
			Material* material = App->materialManager->CreateMaterial();

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

				material->texture = App->textures->Load(material->FilePath);
			}

			materials.push_back(material);
		}

		for (size_t i = 0; i < scene->mNumMeshes; ++i)
		{
			Mesh* mesh = App->meshManager->CreateMesh();
			aiMesh* aMesh = scene->mMeshes[i];

			mesh->num_vertices = aMesh->mNumVertices;
			mesh->num_indices = aMesh->mNumFaces * 3;

			mesh->material = materials[aMesh->mMaterialIndex]->id;

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
				glGenBuffers(1, &mesh->vertexID);
				glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexID);
				glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * mesh->num_vertices * 3, &aMesh->mVertices[0], GL_STATIC_DRAW);
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

	void LoadNodes(aiNode* originalNode, GameObject* node, const std::vector<Mesh*>& meshes)
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
		TransformComponent* transform = new TransformComponent;
		transform->Position = float3(position.x, position.y, position.z);
		transform->Scale = float3(scale.x, scale.y, scale.z);
		transform->Rotation = Quat(rotation.x, rotation.y, rotation.z, rotation.w);

		children->AddComponent(transform);

		std::vector<vec> vertex_boundingbox;

		if (originalNode->mMeshes != nullptr)
		{
			vertex_boundingbox.resize(originalNode->mNumMeshes * 8);
			MeshComponent* meshComponent = new MeshComponent;
			children->AddComponent(meshComponent);

			MaterialComponent* materialComponent = new MaterialComponent;
			children->AddComponent(materialComponent);

			meshComponent->MaterialComponent = materialComponent;

			for (size_t i = 0; i < originalNode->mNumMeshes; ++i)
			{
				Mesh* mesh = meshes[originalNode->mMeshes[i]];

				mesh->materialInComponent = materialComponent->AddMaterial(App->materialManager->GetMaterial(mesh->material));

				meshComponent->Meshes.push_back(mesh);

				mesh->boundingBox.GetCornerPoints(&vertex_boundingbox[i * 8]);
			}
		}

		children->BoundingBox.SetNegativeInfinity();
		if (!vertex_boundingbox.empty())
			children->BoundingBox.Enclose(reinterpret_cast<float3*>(&vertex_boundingbox[0]), originalNode->mNumMeshes * 8);

		for (size_t i = 0; i < originalNode->mNumChildren; ++i)
		{
			LoadNodes(originalNode->mChildren[i], children, meshes);
		}
	}
}

std::shared_ptr<Level> DataImporter::ImportLevel(const char* path, const char* file) const
{
	LOG("Importing level %s", file);
	char filePath[256];
	sprintf_s(filePath, "%s%s", path, file);

	const aiScene* scene = aiImportFile(filePath, aiProcessPreset_TargetRealtime_MaxQuality);

	aiNode* node = scene->mRootNode;

	std::vector<Mesh*> meshes;
	meshes.reserve(scene->mNumMeshes);
	ImportMeshes(scene, path, meshes);

	std::shared_ptr<Level> level = std::make_shared<Level>();

	LoadNodes(node, level->GetRootNode(), meshes);
	level->RegenerateQuadtree(); // TODO: Improve quadtree generation

	aiReleaseImport(scene);

	return level;
}
