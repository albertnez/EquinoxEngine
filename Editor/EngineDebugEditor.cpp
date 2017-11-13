#pragma once

#include "EditorSubmodule.h"
#include "IMGUI/imgui.h"
#include "Engine.h"
#include "ModuleWindow.h"
#include "EditorUtils.h"

class EngineDebugEditor : public EditorSubmodule
{
public:
	void Update() override;

private:
	bool _wireframe = false;
};

REGISTER_EDITOR_SUBMODULE(EngineDebugEditor);


void EngineDebugEditor::Update()
{
	int w, h;
	App->window->GetWindowSize(w, h);

	ImVec2 windowPosition(301, h - 100);
	ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_Always);
	ImGui::SetNextWindowPos(windowPosition, ImGuiSetCond_Always);
	if (ImGui::Begin("Engine Debug", nullptr, ImGuiWindowFlags_AlwaysUseWindowPadding))
	{

		bool wireframe = _wireframe;
		ImGui::Checkbox("Wireframe mode", &wireframe);

		if (wireframe != _wireframe)
		{
			_wireframe = wireframe;
			wireframe ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		bool drawHierachy = App->editor->GetDrawHierachy();
		if (ImGui::Checkbox("Draw hierachy", &drawHierachy))
		{
			App->editor->SetDrawHierachy(drawHierachy);
		}

		bool drawQuadtree = App->editor->GetDrawQuadtree();
		if (ImGui::Checkbox("Draw quadtree", &drawQuadtree))
		{
			App->editor->SetDrawQuadtree(drawQuadtree);
		}
	}
	ImGui::End();
}
