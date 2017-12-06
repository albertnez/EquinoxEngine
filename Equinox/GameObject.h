#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__
#include <vector>
#include <string>
#include <list>
#include <MathGeoLib/include/Geometry/AABB.h>
#include "Engine.h"

class BaseComponent;
class TransformComponent;

class GameObject
{

public:
	GameObject();
	~GameObject();

	void SetParent(GameObject* new_parent);
	GameObject* GetParent() const;

	const std::vector<GameObject*>& GetChilds() const;
	void AddChild(GameObject* child);
	void RemoveChild(const GameObject* child);

	const std::list<BaseComponent*>& GetComponents() const;
	void AddComponent(BaseComponent* component);
	BaseComponent* GetComponentByName(const std::string& name) const;
	void DeleteComponentByName(const std::string& name);
	void DeleteComponent(BaseComponent* component);

	TransformComponent* GetTransform() const;

	void DrawBoundingBox();
	void DrawHierachy() const;
	void DrawHierachy(const float4x4& transformMatrix) const;
	
	void Update(float dt);
	bool CleanUp();

	std::string Name = "GameObject";
	bool Enabled = true;
	AABB BoundingBox;
	bool VisibleOnCamera = false;

private:
	GameObject* _parent = nullptr;
	TransformComponent* _transform = nullptr;
	std::vector<GameObject*> _childs;
	std::list<BaseComponent*> _componentsToRemove;
	std::list<BaseComponent*> _components;
	Engine::UpdateState _playState = Engine::UpdateState::Stopped;

};

#endif
