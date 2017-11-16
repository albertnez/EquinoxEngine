#include "EditorSubmodule.h"
#include "EditorUtils.h"
#include "Engine.h"
#include "ModuleWindow.h"
#include "BaseComponent.h"
#include "ModuleLevelManager.h"
#include "Level.h"

#include <cctype>
#include "IMGUI/imgui.h"

namespace
{
	std::string GenerateNiceNameForComponent(const std::string& componentName)
	{
		std::string ret = componentName;
		
		for (size_t i = 1; i < ret.length(); ++i)
		{
			if (islower(ret[i - 1]) && isupper(ret[i]))
			{
				ret.insert(i, 1, ' ');  
			}
		}

		return ret;
	}
}

class LevelEditor : public EditorSubmodule
{
public:
	void Update() override;

private:
	void drawProperties();
	void drawLevelHierachy();
	void drawLevelHierachy(GameObject* node);
};

REGISTER_EDITOR_SUBMODULE(LevelEditor);

void LevelEditor::Update()
{
	drawProperties();
	drawLevelHierachy();
}

void LevelEditor::drawProperties()
{
	int w, h;
	App->window->GetWindowSize(w, h);
	ImVec2 windowPosition(w - 400, 0);
	ImGui::SetNextWindowSize(ImVec2(400, h - 400), ImGuiSetCond_Always);
	ImGui::SetNextWindowPos(windowPosition, ImGuiSetCond_Always);
	if (ImGui::Begin("Properties", nullptr, ImGuiWindowFlags_HorizontalScrollbar))
	{
		if (App->editor->SelectedGameObject)
		{
			for (BaseComponent* component : App->editor->SelectedGameObject->GetComponents())
			{
				std::string componentName = GenerateNiceNameForComponent(component->GetName());
				if (ImGui::CollapsingHeader(componentName.c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowOverlapMode))
					component->DrawUI();
			}
		}
	}
	ImGui::End();
}

void LevelEditor::drawLevelHierachy()
{
	int w, h;
	App->window->GetWindowSize(w, h);

	ImGui::SetNextWindowSize(ImVec2(300, h), ImGuiSetCond_Once);
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiSetCond_Always);

	if (ImGui::Begin("Hierachy", nullptr, ImGuiWindowFlags_AlwaysUseWindowPadding))
	{
		for (GameObject* node : App->level_manager->GetCurrentLevel()->GetRootNode()->GetChilds())
			drawLevelHierachy(node);
	}
	ImGui::End();
}

void LevelEditor::drawLevelHierachy(GameObject* node)
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
			drawLevelHierachy(child);
		}
		ImGui::TreePop();
	}
}
