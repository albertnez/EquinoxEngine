#include "EditorSubmodule.h"
#include "EditorUtils.h"
#include "IMGUI/imgui.h"
#include "ModuleLighting.h"
#include "Engine.h"
#include "ModuleWindow.h"

class LightingEditor : public EditorSubmodule
{
public:
	void Update() override;

};

REGISTER_EDITOR_SUBMODULE(LightingEditor);

void LightingEditor::Update()
{
	int w, h;
	App->window->GetWindowSize(w, h);
	ImVec2 windowPosition(w - 400, h - 400);
	ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiSetCond_Always);
	ImGui::SetNextWindowPos(windowPosition, ImGuiSetCond_Always);
	if (ImGui::Begin("Lighting", nullptr, ImGuiWindowFlags_HorizontalScrollbar))
	{
		ModuleLighting* lighting = App->lighting;

		if (ImGui::CollapsingHeader("Global Ambient Light", ImGuiTreeNodeFlags_AllowOverlapMode))
		{
			ImGui::Checkbox("Enabled", &lighting->EnableAmbientLight);

			if (lighting->EnableAmbientLight)
			{
				ImGui::InputFloat4("Ambient", &lighting->AmbientLight[0], -1, ImGuiInputTextFlags_CharsDecimal);
			}
		}

		int i = 0;
		for (Light* light : lighting->Lights)
		{
			std::string name = "Light-" + std::to_string(i);
			if (ImGui::CollapsingHeader(name.c_str(), ImGuiTreeNodeFlags_AllowOverlapMode))
			{
				ImGui::Checkbox("Enabled", &light->IsEnabled);

				if (light->IsEnabled)
				{
					ImGui::InputFloat4("Ambient", &light->Ambient[0], -1, ImGuiInputTextFlags_CharsDecimal);
					ImGui::InputFloat4("Diffuse", &light->Diffuse[0], -1, ImGuiInputTextFlags_CharsDecimal);
					ImGui::InputFloat4("Specular", &light->Specular[0], -1, ImGuiInputTextFlags_CharsDecimal);

					ImGui::NewLine();

					if (light->Type == L_POINT || light->Type == L_DIRECTIONAL || light->Type == L_SPOTLIGHT)
					{
						ImGui::InputFloat3("Position", &light->Position[0], -1, ImGuiInputTextFlags_CharsDecimal);
					}
					if (light->Type == L_SPOTLIGHT)
					{
						ImGui::InputFloat("CutOff", &light->CutOff, -1, ImGuiInputTextFlags_CharsDecimal);
						ImGui::InputFloat3("Direction", &light->Direction[0], -1, ImGuiInputTextFlags_CharsDecimal);
					}

					int current_type = lighting->GetLabelByType(light->Type);
					ImGui::Combo("Type", &current_type, "Point\0Directional\0Spotlight\0Default\0");
					if (lighting->GetTypeByLabel(current_type) != light->Type)
						lighting->SetLightType(light, lighting->GetTypeByLabel(current_type));
				}
			}
			i++;
		}

	}
	ImGui::End();
}
