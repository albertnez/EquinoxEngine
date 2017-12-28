#include <GL/glew.h>
#include <MathGeoLib/include/Math/float3x3.h>

#include "Transform.h"
#include "GameObject.h"


Transform::Transform(GameObject* gameObject)
	: _gameObject(gameObject)
{
	assert(gameObject && "Attached Game Object can't be null");
}


const float4x4& Transform::GetTransformMatrix()
{
	if (_dirty)
	{
		recalculateTransform();
		_dirty = false;
	}

	return _transformMatrix;
}

void Transform::SetTransformMatrix(const float4x4& matrix)
{
	markChildrenDirty();
	_transformMatrix = matrix;
	recalculateLocalTransform();
	_dirty = false;
}

const float4x4& Transform::GetLocalTransformMatrix()
{
	return _localTransformMatrix;
}

void Transform::SetLocalTransformMatrix(const float4x4& matrix)
{
	markChildrenDirty();
	_localTransformMatrix = matrix;
	recalculateTransform();
	_dirty = false;
}

void Transform::Update(float dt)
{
	float4x4 transformMatrix = GetTransformMatrix().Transposed();
	glMultMatrixf(reinterpret_cast<GLfloat*>(&transformMatrix));
}

float3 Transform::GetPosition()
{
	if (_dirty)
	{
		_dirty = false;
		recalculateTransform();
	}

	return _transformMatrix.TranslatePart();
}

void Transform::SetPosition(const float3& position)
{
	if (!position.Equals(GetPosition()))
	{
		markChildrenDirty();
		_transformMatrix.SetTranslatePart(position);
		_dirty = false;
		recalculateLocalTransform();
	}
}

float3 Transform::GetLocalPosition() const
{
	return _localPosition;
}

void Transform::SetLocalPosition(const float3& position)
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

Quat Transform::GetRotation()
{
	if (_dirty)
	{
		recalculateTransform();
		_dirty = false;
	}

	return Quat(_transformMatrix.RotatePart());
}

void Transform::SetRotation(const Quat& rotation)
{
	if (!rotation.Equals(GetRotation()))
	{
		markChildrenDirty();
		_transformMatrix.SetRotatePart(rotation);
		_dirty = false;
		recalculateLocalTransform();
	}
}

Quat Transform::GetLocalRotation() const
{
	return _localRotation;
}

void Transform::SetLocalRotation(const Quat& rotation)
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

float3 Transform::GetLocalScale() const
{
	return _localScale;
}

void Transform::SetLocalScale(const float3& scale)
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

void Transform::markChildrenDirty()
{
	_dirty = true;

	for (GameObject* child : _gameObject->GetChilds())
		child->GetTransform().markChildrenDirty();
}

void Transform::regenerateLocalTransform()
{
	_localTransformMatrix = float4x4::FromTRS(_localPosition, _localRotation, _localScale);
}

void Transform::recalculateTransform()
{
	_transformMatrix = _localTransformMatrix;

	GameObject* parent = _gameObject->GetParent();
	if (parent)
		_transformMatrix = _localTransformMatrix * parent->GetTransform().GetTransformMatrix();
}

void Transform::recalculateLocalTransform()
{
	_localTransformMatrix = _transformMatrix;

	GameObject* parent = _gameObject->GetParent();
	if (parent)
		_localTransformMatrix = _transformMatrix * parent->GetTransform().GetTransformMatrix().Inverted();

	_localPosition = _localTransformMatrix.TranslatePart();
	_localRotation = _localTransformMatrix.RotatePart().ToQuat();
}
