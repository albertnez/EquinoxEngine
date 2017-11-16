#ifndef __COMPONENT_MATERIAL_H__
#define __COMPONENT_MATERIAL_H__
#include "BaseComponent.h"
#include "MathGeoLib/include/Math/float4.h"
#include <list>

struct Material
{
	int id = -1;
	float4 ambient = float4(1.0f, 1.0f, 1.0f, 1.0f);
	float4 diffuse = float4(1.0f, 1.0f, 1.0f, 1.0f);
	float4 specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float shininess = 0.0f;
	unsigned texture = 0;
	char FilePath[256] = { 0 };
};

class MaterialComponent :
	public BaseComponent
{
	DEFAULT_COMPONENT_IMPLEMENTATION;
public:
	std::vector<Material*> Materials;
	
public:
	MaterialComponent();
	~MaterialComponent();

	unsigned AddMaterial(Material* material);

	void DrawUI() override;
};

#endif
