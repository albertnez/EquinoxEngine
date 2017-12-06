﻿#ifndef __COMPONENT_TRANSFORM_H__
#define __COMPONENT_TRANSFORM_H__
#include "BaseComponent.h"
#include <MathGeoLib/include/Math/float3.h>
#include <MathGeoLib/include/Math/Quat.h>

class TransformComponent :
	public BaseComponent
{
	DEFINE_COMPONENT(TransformComponent);

public:
	TransformComponent();
	~TransformComponent();

	float4x4 GetTransformMatrix() const;

	void Update(float dt) override;
	void EditorUpdate(float dt) override;

	float3 Position = float3(0, 0, 0);
	Quat Rotation = Quat(0, 0, 0, 1);
	float3 Scale = float3(1.f, 1.f, 1.f);
};

#endif
