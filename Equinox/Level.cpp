#include "Level.h"
#include "ModuleTextures.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "ModuleWindow.h"
#include "ModuleEditor.h"
#include "Quadtree.h"
#include "ModuleEditorCamera.h"

#include "IMGUI/imgui.h"

#include <stack>

Level::Level()
{
	root = new GameObject;

	vec minPoint = vec(-10000, -100, -10000);
	vec maxPoint = vec(10000, 100, 10000);

	AABB limits = AABB(minPoint, maxPoint);

	quadtree = new Quadtree(limits);
}

Level::~Level()
{
}

bool Level::CleanUp()
{
	cleanUpNodes(root);

	quadtree->Clear();

	RELEASE(quadtree);

	RELEASE(root);

	return true;
}

void Level::Update(float dt)
{
	std::vector<GameObject*> visibleObjects;
	quadtree->CollectIntersections(visibleObjects, App->editorCamera->GetCamera()->GetFrustumAABB());

	for (GameObject* go : visibleObjects)
		go->VisibleOnCamera = true;

	root->Update(dt);

	if (App->editor->DrawHierachy)
		root->DrawHierachy();

	for (GameObject* go : visibleObjects)
		go->VisibleOnCamera = false;

	if(App->editor->DrawQuadtree)
		quadtree->DrawQuadtree();
}

void Level::DrawUI()
{
	int w, h;
	SDL_GetWindowSize(App->window->window, &w, &h);

	ImGui::SetNextWindowSize(ImVec2(300, h), ImGuiSetCond_Once);
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiSetCond_Always);

	if (ImGui::Begin("Hierachy", nullptr, ImGuiWindowFlags_AlwaysUseWindowPadding))
	{
		for (GameObject* node : root->GetChilds())
			drawHierachy(node);
	}
	ImGui::End();
}

void Level::RegenerateQuadtree() const
{
	std::stack<GameObject*> gameObjects;
	gameObjects.push(root);

	while (!gameObjects.empty())
	{
		GameObject* current = gameObjects.top();
		gameObjects.pop();
		quadtree->Insert(current);

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
		LinkGameObject(go, root);
	}
}

void Level::drawHierachy(GameObject* node)
{
	int flags = ImGuiTreeNodeFlags_DefaultOpen;
	if (node->GetChilds().size() == 0)
		flags |= ImGuiTreeNodeFlags_Leaf;

	if (App->editor->SelectedGameObject == node)
		flags |= ImGuiTreeNodeFlags_Selected;

	if (ImGui::TreeNodeEx(node->Name.c_str(), flags))
	{
		if (ImGui::IsItemClicked(0))
		{
			App->editor->SelectedGameObject = node;
		}

		for (GameObject* child : node->GetChilds())
		{
			drawHierachy(child);
		}
		ImGui::TreePop();
	}
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
