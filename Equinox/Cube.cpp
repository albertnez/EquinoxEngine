#include <GL/glew.h>
#include <MathGeoLib/include/Math/MathFunc.h>

#include "Cube.h"
#include "Mesh.h"


Cube::Cube() : 
	Primitive() {}

Cube::Cube(const float3& position, const Quat& rotation) :
	Primitive(position, rotation) {}

Cube::Cube(const float3& position, const Quat& rotation, int textureId) : Primitive(position, rotation), _textureId(textureId)
{
}

Cube::Cube(const float3& position, const Quat& rotation, const float3& color) :
	Primitive(position, rotation, color) {}

Cube::~Cube() {}

void Cube::GenerateMesh(std::shared_ptr<Mesh> cube)
{
	static float3 vertices[] = {
		float3(0.5f, 0.0f, -0.5f),
		float3(-0.5f, 0.0f, -0.5f),
		float3(-0.5f, 1.0f, -0.5f),
		float3(0.5f, 1.0f, -0.5f),

		float3(-0.5f, 0.0f, -0.5f),
		float3(-0.5f, 0.0f, 0.5f),
		float3(-0.5f, 1.0f, 0.5f),
		float3(-0.5f, 1.0f, -0.5f),

		float3(-0.5f, 0.0f, 0.5f),
		float3(0.5f, 0.0f, 0.5f),
		float3(0.5f, 1.0f, 0.5f),
		float3(-0.5f, 1.0f, 0.5f),

		float3(0.5f, 0.0f, 0.5f),
		float3(0.5f, 0.0f, -0.5f),
		float3(0.5f, 1.0f, -0.5f),
		float3(0.5f, 1.0f, 0.5f),

		float3(0.5f, 1.0f, -0.5f),
		float3(-0.5f, 1.0f, -0.5f),
		float3(-0.5f, 1.0f, 0.5f),
		float3(0.5f, 1.0f, 0.5f),

		float3(-0.5f, 0.0f, -0.5f),
		float3(0.5f, 0.0f, -0.5f),
		float3(0.5f, 0.0f, 0.5f),
		float3(-0.5f, 0.0f, 0.5f)
	};

	static float3 textureCoords[] = {
		float3(0.0f, 0.0f, 0.0f),
		float3(1.0f, 0.0f, 0.0f),
		float3(1.0f, 1.0f, 0.0f),
		float3(0.0f, 1.0f, 0.0f),

		float3(0.0f, 0.0f, 0.0f),
		float3(1.0f, 0.0f, 0.0f),
		float3(1.0f, 1.0f, 0.0f),
		float3(0.0f, 1.0f, 0.0f),

		float3(0.0f, 0.0f, 0.0f),
		float3(1.0f, 0.0f, 0.0f),
		float3(1.0f, 1.0f, 0.0f),
		float3(0.0f, 1.0f, 0.0f),

		float3(0.0f, 0.0f, 0.0f),
		float3(1.0f, 0.0f, 0.0f),
		float3(1.0f, 1.0f, 0.0f),
		float3(0.0f, 1.0f, 0.0f),

		float3(0.0f, 0.0f, 0.0f),
		float3(1.0f, 0.0f, 0.0f),
		float3(1.0f, 1.0f, 0.0f),
		float3(0.0f, 1.0f, 0.0f),

		float3(0.0f, 0.0f, 0.0f),
		float3(1.0f, 0.0f, 0.0f),
		float3(1.0f, 1.0f, 0.0f),
		float3(0.0f, 1.0f, 0.0f)
	};

	static uint32_t indexes[] = {
		0, 1, 2,
		2, 3, 0,
		
		4, 5, 6,
		6, 7, 4,
		
		8, 9, 10,
		10, 11, 8,

		12, 13, 14,
		14, 15, 12,

		16, 17, 18,
		18, 19, 16,

		20, 21, 22,
		22, 23, 20
	};

	cube->SetVertices(vertices, ARRAY_LENGTH(vertices));
	cube->SetNormals(vertices, ARRAY_LENGTH(vertices));
	cube->SetTextureCoords(textureCoords, ARRAY_LENGTH(textureCoords));
	cube->SetIndexes(indexes, ARRAY_LENGTH(indexes));
}

void Cube::Draw()
{
	glPushMatrix();

	glBindTexture(GL_TEXTURE_2D, _textureId);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glTranslatef(Position.x, Position.y, Position.z);
	float3 axis = Rotation.Axis();
	glRotatef(RadToDeg(Rotation.Angle()), axis.x, axis.y, axis.z);

	glColor3f(Color.x, Color.y, Color.z);

	glBegin(GL_TRIANGLES);

	// a, b, c
	glTexCoord2f(1.f, 1.f);
	glVertex3f(1.f, 1.f, 0);
	glTexCoord2f(0.f, 1.f);
	glVertex3f(0.f, 1.f, 0.f);
	glTexCoord2f(0.f, 0.f);
	glVertex3f(0.f, 0.f, 0.f);

	// c, d, a
	glTexCoord2f(0.f, 0.f);
	glVertex3f(0.f, 0.f, 0);
	glTexCoord2f(1.f, 0.f);
	glVertex3f(1.f, 0.f, 0.f);
	glTexCoord2f(1.f, 1.f);
	glVertex3f(1.f, 1.f, 0.f);

	// a, d, e
	glTexCoord2f(0.f, 1.f);
	glVertex3f(1.f, 1.f, 0.f);
	glTexCoord2f(0.f, 0.f);
	glVertex3f(1.f, 0.f, 0.f);
	glTexCoord2f(1.f, 0.f);
	glVertex3f(1.f, 0.f, -1.f);

	// e, f, a
	glTexCoord2f(1.f, 0.f);
	glVertex3f(1.f, 0.f, -1.f);
	glTexCoord2f(1.f, 1.f);
	glVertex3f(1.f, 1.f, -1.f);
	glTexCoord2f(0.f, 1.f);
	glVertex3f(1.f, 1.f, 0.f);

	// a, f, g
	glTexCoord2f(1.f, 0.f);
	glVertex3f(1.f, 1.f, 0.f);
	glTexCoord2f(1.f, 1.f);
	glVertex3f(1.f, 1.f, -1.f);
	glTexCoord2f(0.f, 1.f);
	glVertex3f(0.f, 1.f, -1.f);

	// g, b, a
	glTexCoord2f(0.f, 1.f);
	glVertex3f(0.f, 1.f, -1.f);
	glTexCoord2f(0.f, 0.f);
	glVertex3f(0.f, 1.f, 0.f);
	glTexCoord2f(1.f, 0.f);
	glVertex3f(1.f, 1.f, 0.f);

	// g, f, e
	glTexCoord2f(1.f, 1.f);
	glVertex3f(0.f, 1.f, -1.f);
	glTexCoord2f(0.f, 1.f);
	glVertex3f(1.f, 1.f, -1.f);
	glTexCoord2f(0.f, 0.f);
	glVertex3f(1.f, 0.f, -1.f);

	// e, h, g
	glTexCoord2f(0.f, 0.f);
	glVertex3f(1.f, 0.f, -1.f);
	glTexCoord2f(1.f, 0.f);
	glVertex3f(0.f, 0.f, -1.f);
	glTexCoord2f(1.f, 1.f);
	glVertex3f(0.f, 1.f, -1.f);

	// g, h, c
	glTexCoord2f(1.f, 1.f);
	glVertex3f(0.f, 1.f, -1.f);
	glTexCoord2f(1.f, 0.f);
	glVertex3f(0.f, 0.f, -1.f);
	glTexCoord2f(0.f, 0.f);
	glVertex3f(0.f, 0.f, 0.f);

	// c, b, g
	glTexCoord2f(0.f, 0.f);
	glVertex3f(0.f, 0.f, 0.f);
	glTexCoord2f(0.f, 1.f);
	glVertex3f(0.f, 1.f, 0.f);
	glTexCoord2f(1.f, 1.f);
	glVertex3f(0.f, 1.f, -1.f);

	// h, e, d
	glTexCoord2f(1.f, 1.f);
	glVertex3f(0.f, 0.f, -1.f);
	glTexCoord2f(0.f, 1.f);
	glVertex3f(1.f, 0.f, -1.f);
	glTexCoord2f(0.f, 0.f);
	glVertex3f(1.f, 0.f, 0.f);

	// d, c, h
	glTexCoord2f(0.f, 0.f);
	glVertex3f(1.f, 0.f, 0.f);
	glTexCoord2f(1.f, 0.f);
	glVertex3f(0.f, 0.f, 0.f);
	glTexCoord2f(1.f, 1.f);
	glVertex3f(0.f, 0.f, -1.f);

	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); // Always restore Texture Environment to GL_MODULATE

	glPopMatrix();
}
