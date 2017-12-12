#pragma once
#include <MathGeoLib/include/Geometry/AABB.h>

struct Material;

class Mesh
{
	friend class ModuleMeshManager;
	friend class DataImporter;

public:
	~Mesh() = default;

	int GetId() const;

	const std::string& GetName() const;

	Material* GetMaterial() const;
	void SetMaterial(Material* material);

	uint32_t GetVertexId() const;

	bool HasVertices() const;
	const std::vector<float3>& GetVertices() const;
	const float3* GetVerticesArray() const;
	void SetVertices(const float3* vertices, int numVertices);
	uint32_t GetNumVertices() const;

	uint32_t GetNormalId() const;

	bool HasNormals() const;
	const std::vector<float3>& GetNormals() const;
	const float3* GetNormalsArray() const;
	void SetNormals(const float3* normals, int numNormals);

	uint32_t GetTextureCoordsId() const;

	bool HasTextureCoords() const;
	const std::vector<float3>& GetTextureCoords() const;
	const float3* GetTextureCoordsArray() const;
	void SetTextureCoords(const float3* textureCoords, int numCoords);

	uint32_t GetIndexesId() const;

	const std::vector<uint32_t>& GetIndexes() const;
	const uint32_t* GetIndexesArray() const;
	void SetIndexes(const uint32_t* indexes, int numIndexes);
	uint32_t GetNumIndices() const;

	const AABB& GetBoundingBox() const;

private:
	Mesh() = default;

	int id = -1;
	std::string name;

	uint32_t vertexID = 0;
	std::vector<float3> _vertices;

	uint32_t normalID = 0;
	std::vector<float3> _normals;

	uint32_t textureCoordsID = 0;
	std::vector<float3> _textureCoords;

	uint32_t indexesID = 0;
	std::vector<uint32_t> _indexes;

	Material* _material = nullptr;
	uint32_t num_vertices = 0;
	uint32_t num_indices = 0;
	AABB boundingBox;

};

