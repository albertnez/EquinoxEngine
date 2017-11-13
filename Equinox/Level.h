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

	GameObject* root = nullptr;
	Quadtree* quadtree = nullptr;

	bool CleanUp();
	void Update(float dt);

	void RegenerateQuadtree() const;

	GameObject* GetRootNode() { return root; }
	const GameObject* GetRootNode() const { return root; }

	GameObject* FindGameObject(const char* name);
	void LinkGameObject(GameObject* node, GameObject* destination);

	void AddToScene(GameObject* go);

private:
	void cleanUpNodes(GameObject* node);
};

#endif // __LEVEL_H__