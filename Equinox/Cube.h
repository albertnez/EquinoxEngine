#ifndef __CUBE_H__
#define __CUBE_H__

#include "Primitive.h"

class Mesh;

class Cube :
	public Primitive
{
public:
	Cube();
	Cube(const float3& position, const Quat& rotation);
	Cube(const float3& position, const Quat& rotation, int textureId);
	Cube(const float3& position, const Quat& rotation, const float3& color);
	~Cube();

	static void GenerateMesh(std::shared_ptr<Mesh> mesh);

	void Draw() override;

private:
	int _textureId = 0;
};

#endif // __CUBE_H__
