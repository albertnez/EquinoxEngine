#include "Level.h"
#include "ModuleTextures.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "Quadtree.h"
#include "ModuleEditorCamera.h"

#include "IMGUI/imgui.h"

#include <stack>

Level::Level()
{
	_root = new GameObject;

	vec minPoint = vec(-10000, -100, -10000);
	vec maxPoint = vec(10000, 100, 10000);

	AABB limits = AABB(minPoint, maxPoint);

	_quadtree = new Quadtree(limits);
}

Level::~Level()
{
}

bool Level::CleanUp()
{
	cleanUpNodes(_root);

	_quadtree->Clear();

	RELEASE(_quadtree);

	RELEASE(_root);

	return true;
}

void Level::PreUpdate(float dt)
{
	
}

void Level::Update(float dt)
{
	std::vector<GameObject*> visibleObjects;
	_quadtree->CollectIntersections(visibleObjects, App->editorCamera->GetCamera()->GetFrustumAABB());

	for (GameObject* go : visibleObjects)
		go->VisibleOnCamera = true;

	_root->Update(dt);

	for (GameObject* go : visibleObjects)
		go->VisibleOnCamera = false;
}

void Level::PostUpdate(float dt)
{
}

void Level::RegenerateQuadtree() const
{
	std::stack<GameObject*> gameObjects;
	gameObjects.push(_root);

	while (!gameObjects.empty())
	{
		GameObject* current = gameObjects.top();
		gameObjects.pop();
		_quadtree->Insert(current);

		for (GameObject* child : current->GetChilds())
		{
			gameObjects.push(child);
		}
	}
}

GameObject* Level::FindGameObject(const char* name)
{
	return nullptr;
}

void Level::LinkGameObject(GameObject* node, GameObject* destination)
{
	destination->AddChild(node);
}

void Level::AddToScene(GameObject* go)
{
	if (go != nullptr)
	{
		LinkGameObject(go, _root);
	}
}

const Quadtree& Level::GetQuadtree() const
{
	return *_quadtree;
}

void Level::cleanUpNodes(GameObject* node)
{
	for(GameObject* child : node->GetChilds())
	{
		child->CleanUp();
		cleanUpNodes(child);
		RELEASE(child);
	}
}
