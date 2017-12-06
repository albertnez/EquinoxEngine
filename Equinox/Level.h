#ifndef __LEVEL_H__
#define __LEVEL_H__

#include "Primitive.h"
#include "GameObject.h"
#include "Quadtree.h"

class Level
{
public:
	Level();
	~Level();

	void PreUpdate(float dt);
	void Update(float dt);
	void PostUpdate(float dt);
	bool CleanUp();

	void RegenerateQuadtree() const;

	GameObject* GetRootNode() { return _root; }
	const GameObject* GetRootNode() const { return _root; }

	GameObject* FindGameObject(const char* name);
	void LinkGameObject(GameObject* node, GameObject* destination);

	void AddToScene(GameObject* go);

	const Quadtree& GetQuadtree() const;

private:
	void cleanUpNodes(GameObject* node);

	Quadtree* _quadtree = nullptr;
	GameObject* _root = nullptr;
};

#endif // __LEVEL_H__