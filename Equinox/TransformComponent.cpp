﻿#include "TransformComponent.h"
#include <GL/glew.h>
#include <MathGeoLib/include/Math/float4x4.h>
#include "IMGUI/imgui.h"

TransformComponent::TransformComponent()
{
}

TransformComponent::~TransformComponent()
{
}

float4x4 TransformComponent::GetTransformMatrix() const
{
	return float4x4::FromTRS(Position, Rotation, Scale);
}

void TransformComponent::Update(float dt)
{	
	float4x4 transformMatrix = GetTransformMatrix().Transposed();
	glMultMatrixf(reinterpret_cast<GLfloat*>(&transformMatrix));
}

void TransformComponent::EditorUpdate(float dt)
{
	Update(dt);
}

void TransformComponent::DrawUI()
{
	ImGui::InputFloat3("Position", &Position[0], -1, ImGuiInputTextFlags_CharsDecimal);
	float3 rot = Rotation.ToEulerXYZ() * RadToDeg(Rotation.Angle());
	ImGui::SliderFloat3("Rotation", &rot[0], -360, 360, "%.2f deg");
	ImGui::InputFloat3("Scale", &Scale[0], -1, ImGuiInputTextFlags_CharsDecimal);
}
