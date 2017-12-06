﻿#ifndef __COMPONENT_MESH_H__
#define __COMPONENT_MESH_H__
#include "BaseComponent.h"
#include <GL/glew.h>
#include <list>
#include "MaterialComponent.h"

struct Mesh
{
	int id = -1;
	int material = 0;
	GLuint vertexID = 0;
	GLuint normalID = 0;
	GLuint textureCoordsID = 0;
	int materialInComponent = 0;
	float3** textureCoords;
	GLuint indexesID = 0;
	unsigned num_vertices = 0;
	unsigned num_indices = 0;
	AABB boundingBox;
};

class ShaderProgram;

class MeshComponent :
	public BaseComponent
{
	DEFINE_COMPONENT(MeshComponent);
public:
	std::list<Mesh*> Meshes;
	MaterialComponent* MaterialComponent;

public:
	MeshComponent();
	~MeshComponent();

	void Update(float dt) override;
	void EditorUpdate(float dt) override;

	const GLfloat DEFAULT_GL_AMBIENT[4] = { 0.2f, 0.2f, 0.2f, 1.f };
	const GLfloat DEFAULT_GL_DIFFUSE[4] = { 0.8f, 0.8f, 0.8f, 1.f };
	const GLfloat DEFAULT_GL_SPECULAR[4] = { 0.f, 0.f, 0.f, 1.f };
	const GLfloat DEFAULT_GL_SHININESS = 0.f;	

private:
	ShaderProgram* _shaderTexture;
	ShaderProgram* _shaderColor;
};

#endif
