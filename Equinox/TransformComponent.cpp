#include <GL/glew.h>
#include <MathGeoLib/include/Math/float3x3.h>

#include "TransformComponent.h"
#include "GameObject.h"


TransformComponent::TransformComponent()
{
}

TransformComponent::~TransformComponent()
{
}

const float4x4& TransformComponent::GetTransformMatrix()
{
	if (_dirty)
	{
		recalculateTransform();
		_dirty = false;
	}

	return _transformMatrix;
}

void TransformComponent::SetTransformMatrix(const float4x4& matrix)
{
	markChildrenDirty();
	_transformMatrix = matrix;
	recalculateLocalTransform();
	_dirty = false;
}

const float4x4& TransformComponent::GetLocalTransformMatrix()
{
	return _localTransformMatrix;
}

void TransformComponent::SetLocalTransformMatrix(const float4x4& matrix)
{
	markChildrenDirty();
	_localTransformMatrix = matrix;
	recalculateTransform();
	_dirty = false;
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

float3 TransformComponent::GetPosition()
{
	if (_dirty)
	{
		_dirty = false;
		recalculateTransform();
	}

	return _transformMatrix.TranslatePart();
}

void TransformComponent::SetPosition(const float3& position)
{
	if (!position.Equals(GetPosition()))
	{
		markChildrenDirty();
		_transformMatrix.SetTranslatePart(position);
		_dirty = false;
		recalculateLocalTransform();
	}
}

float3 TransformComponent::GetLocalPosition() const
{
	return _localPosition;
}

void TransformComponent::SetLocalPosition(const float3& position)
{
	if (!position.Equals(GetLocalPosition()))
	{
		markChildrenDirty();
		_localPosition = position;
		regenerateLocalTransform();
		_dirty = false;
		recalculateTransform();
	}
}

Quat TransformComponent::GetRotation()
{
	if (_dirty)
	{
		recalculateTransform();
		_dirty = false;
	}

	return Quat(_transformMatrix.RotatePart());
}

void TransformComponent::SetRotation(const Quat& rotation)
{
	if (!rotation.Equals(GetRotation()))
	{
		markChildrenDirty();
		_transformMatrix.SetRotatePart(rotation);
		_dirty = false;
		recalculateLocalTransform();
	}
}

Quat TransformComponent::GetLocalRotation() const
{
	return _localRotation;
}

void TransformComponent::SetLocalRotation(const Quat& rotation)
{
	if (!rotation.Equals(GetLocalRotation()))
	{
		markChildrenDirty();
		_localRotation = rotation;
		regenerateLocalTransform();
		_dirty = false;
		recalculateTransform();
	}
}

float3 TransformComponent::GetLocalScale() const
{
	return _localScale;
}

void TransformComponent::SetLocalScale(const float3& scale)
{
	if (!scale.Equals(GetLocalScale()))
	{
		markChildrenDirty();
		_localScale = scale;
		regenerateLocalTransform();
		_dirty = false;
		recalculateTransform();
	}
}

void TransformComponent::markChildrenDirty()
{
	_dirty = true;
	if (Parent)
	{
		for (GameObject* child : Parent->GetChilds())
			child->GetTransform()->markChildrenDirty();
	}
}

void TransformComponent::regenerateLocalTransform()
{
	_localTransformMatrix = float4x4::FromTRS(_localPosition, _localRotation, _localScale);
}

void TransformComponent::recalculateTransform()
{
	_transformMatrix = _localTransformMatrix;

	GameObject* parent = Parent->GetParent();
	if (parent && parent->GetTransform())
		_transformMatrix = _localTransformMatrix * parent->GetTransform()->GetTransformMatrix();
}

void TransformComponent::recalculateLocalTransform()
{
	_localTransformMatrix = _transformMatrix;

	GameObject* parent = Parent->GetParent();
	if (parent && parent->GetTransform())
		_localTransformMatrix = _transformMatrix * parent->GetTransform()->GetTransformMatrix().Inverted();

	_localPosition = _localTransformMatrix.TranslatePart();
	_localRotation = _localTransformMatrix.RotatePart().ToQuat();
}
