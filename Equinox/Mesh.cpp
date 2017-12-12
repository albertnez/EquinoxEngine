#include "Mesh.h"
#include "Engine.h"
#include "OpenGLGraphicsDevice.h"

int Mesh::GetId() const
{
	return id;
}

const std::string& Mesh::GetName() const
{
	return name;
}

Material* Mesh::GetMaterial() const
{
	return _material;
}

void Mesh::SetMaterial(Material* material)
{
	_material = material;
}

uint32_t Mesh::GetVertexId() const
{
	return vertexID;
}

bool Mesh::HasVertices() const
{
	return !_vertices.empty();
}

const std::vector<float3>& Mesh::GetVertices() const
{
	return _vertices;
}

const float3* Mesh::GetVerticesArray() const
{
	return &_vertices[0];
}

void Mesh::SetVertices(const float3* vertices, int numVertices)
{
	_vertices.clear();
	_vertices.resize(numVertices);
	boundingBox.SetNegativeInfinity();
	for (int i = 0; i < numVertices; ++i)
	{
		_vertices[i] = float3(*(vertices + i));
		boundingBox.Enclose(_vertices[i]);
	}

	num_vertices = numVertices;

	if (vertexID == 0)
	{
		std::shared_ptr<OpenGLGraphicsDevice> graphicsDevice = App->GetModule<OpenGLGraphicsDevice>();

		vertexID = graphicsDevice->GenerateBuffer();
		graphicsDevice->BindBuffer(GL_ARRAY_BUFFER, vertexID);
		graphicsDevice->LoadDataToBuffer(GL_ARRAY_BUFFER, sizeof(float) * numVertices * 3, GetVerticesArray(), GL_STATIC_DRAW);
		graphicsDevice->UnbindBuffer(GL_ARRAY_BUFFER);
	}
}

uint32_t Mesh::GetNumVertices() const
{
	return _vertices.size();
}

uint32_t Mesh::GetNormalId() const
{
	return normalID;
}

bool Mesh::HasNormals() const
{
	return !_normals.empty();
}

const std::vector<float3>& Mesh::GetNormals() const
{
	return _normals;
}

const float3* Mesh::GetNormalsArray() const
{
	return &_normals[0];
}

void Mesh::SetNormals(const float3* normals, int numNormals)
{
	_normals.clear();
	_normals.resize(numNormals);
	for (int i = 0; i < numNormals; ++i)
	{
		_normals[i] = float3(*(normals + i));
	}

	if (normalID == 0)
	{
		std::shared_ptr<OpenGLGraphicsDevice> graphicsDevice = App->GetModule<OpenGLGraphicsDevice>();

		normalID = graphicsDevice->GenerateBuffer();
		graphicsDevice->BindBuffer(GL_ARRAY_BUFFER, normalID);
		graphicsDevice->LoadDataToBuffer(GL_ARRAY_BUFFER, sizeof(float) * numNormals * 3, GetNormalsArray(), GL_STATIC_DRAW);
		graphicsDevice->UnbindBuffer(GL_ARRAY_BUFFER);
	}
}

uint32_t Mesh::GetTextureCoordsId() const
{
	return textureCoordsID;
}

bool Mesh::HasTextureCoords() const
{
	return !_textureCoords.empty();
}

const std::vector<float3>& Mesh::GetTextureCoords() const
{
	return _textureCoords;
}

const float3* Mesh::GetTextureCoordsArray() const
{
	return &_textureCoords[0];
}

void Mesh::SetTextureCoords(const float3* textureCoords, int numCoords)
{
	_textureCoords.clear();
	_textureCoords.resize(numCoords);
	for (int i = 0; i < numCoords; ++i)
	{
		_textureCoords[i] = float3(*(textureCoords + i));
	}

	if (textureCoordsID == 0)
	{
		std::shared_ptr<OpenGLGraphicsDevice> graphicsDevice = App->GetModule<OpenGLGraphicsDevice>();

		textureCoordsID = graphicsDevice->GenerateBuffer();
		graphicsDevice->BindBuffer(GL_ARRAY_BUFFER, textureCoordsID);
		graphicsDevice->LoadDataToBuffer(GL_ARRAY_BUFFER, sizeof(float) * numCoords * 3, GetTextureCoordsArray(), GL_STATIC_DRAW);
		graphicsDevice->UnbindBuffer(GL_ARRAY_BUFFER);
	}
}

uint32_t Mesh::GetIndexesId() const
{
	return indexesID;
}

const std::vector<uint32_t>& Mesh::GetIndexes() const
{
	return _indexes;
}

const uint32_t* Mesh::GetIndexesArray() const
{
	return &_indexes[0];
}

void Mesh::SetIndexes(const uint32_t* indexes, int numIndexes)
{
	_indexes.clear();
	_indexes.resize(numIndexes);
	for (int i = 0; i < numIndexes; ++i)
	{
		_indexes[i] = *(indexes + i);
	}

	if (indexesID == 0)
	{
		std::shared_ptr<OpenGLGraphicsDevice> graphicsDevice = App->GetModule<OpenGLGraphicsDevice>();

		indexesID = graphicsDevice->GenerateBuffer();
		graphicsDevice->BindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexesID);
		graphicsDevice->LoadDataToBuffer(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * numIndexes, GetIndexesArray(), GL_STATIC_DRAW);
		graphicsDevice->UnbindBuffer(GL_ELEMENT_ARRAY_BUFFER);
	}
}

uint32_t Mesh::GetNumIndexes() const
{
	return _indexes.size();
}

const AABB& Mesh::GetBoundingBox() const
{
	return boundingBox;
}
