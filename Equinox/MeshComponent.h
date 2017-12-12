#ifndef __COMPONENT_MESH_H__
#define __COMPONENT_MESH_H__
#include "BaseComponent.h"
#include <GL/glew.h>
#include <list>
#include "MaterialComponent.h"
#include "Mesh.h"


struct ShaderProgram;

class MeshComponent :
	public BaseComponent
{
	DEFINE_COMPONENT(MeshComponent);
public:
	MeshComponent();
	~MeshComponent();

	void Update(float dt) override;
	void EditorUpdate(float dt) override;

	const GLfloat DEFAULT_GL_AMBIENT[4] = { 0.2f, 0.2f, 0.2f, 1.f };
	const GLfloat DEFAULT_GL_DIFFUSE[4] = { 0.8f, 0.8f, 0.8f, 1.f };
	const GLfloat DEFAULT_GL_SPECULAR[4] = { 0.f, 0.f, 0.f, 1.f };
	const GLfloat DEFAULT_GL_SHININESS = 0.f;	

	std::list<std::shared_ptr<Mesh>> Meshes;
	MaterialComponent* MaterialComponent;

private:
	std::shared_ptr<class ProgramManager> _programManager;

	std::shared_ptr<ShaderProgram> _shaderUnlit;
};

#endif
