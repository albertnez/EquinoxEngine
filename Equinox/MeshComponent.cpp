#include "MeshComponent.h"
#include "GameObject.h"

#include <GL/glew.h>
#include "IMGUI/imgui.h"


MeshComponent::MeshComponent()
{
	_shaderTexture = App->programManager->GetProgramByName("StandardTextureDiffuse");
	_shaderColor = App->programManager->GetProgramByName("StandardColorDiffuse");
}

MeshComponent::~MeshComponent()
{
}

void MeshComponent::Update(float dt)
{
	if (Parent->VisibleOnCamera)
	{
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);

		for (Mesh* mesh : Meshes)
		{
			glColor3f(1.f, 1.f, 1.f);
			Material* mat = MaterialComponent->Materials[mesh->materialInComponent];

			glMaterialfv(GL_FRONT, GL_AMBIENT, &mat->ambient.x);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, reinterpret_cast<GLfloat*>(&mat->diffuse));
			glMaterialfv(GL_FRONT, GL_SPECULAR, reinterpret_cast<GLfloat*>(&mat->specular));
			glMaterialf(GL_FRONT, GL_SHININESS, mat->shininess);

			glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexID);
			glVertexPointer(3, GL_FLOAT, 0, nullptr);

			if (mesh->normalID != 0)
			{
				glBindBuffer(GL_ARRAY_BUFFER, mesh->normalID);
				glNormalPointer(GL_FLOAT, 0, nullptr);
			}

			GLint diffuse_id = 0;

			ProgramManager* pManager = App->programManager;

			if (mesh->textureCoordsID)
			{
				pManager->UseProgram(_shaderTexture);
				diffuse_id = glGetUniformLocation(_shaderTexture->id, "diffuse");

				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glBindBuffer(GL_ARRAY_BUFFER, mesh->textureCoordsID);
				glTexCoordPointer(3, GL_FLOAT, 0, nullptr);
			}
			else
			{
				pManager->UseProgram(_shaderColor);
				diffuse_id = glGetUniformLocation(_shaderColor->id, "diffuse");
			}

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, mat->texture);
			glUniform1i(diffuse_id, 0);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexesID);
			glDrawElements(GL_TRIANGLES, mesh->num_indices, GL_UNSIGNED_INT, nullptr);

			glMaterialfv(GL_FRONT, GL_AMBIENT, DEFAULT_GL_AMBIENT);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, DEFAULT_GL_DIFFUSE);
			glMaterialfv(GL_FRONT, GL_SPECULAR, DEFAULT_GL_SPECULAR);
			glMaterialf(GL_FRONT, GL_SHININESS, DEFAULT_GL_SHININESS);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}

		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		glDisable(GL_COLOR_MATERIAL);
		glDisable(GL_LIGHTING);
	}
}

void MeshComponent::EditorUpdate(float dt)
{
	Update(dt);
}
