#include "BaseComponentEditor.h"
#include "MaterialComponent.h"

#include "IMGUI/imgui.h"

DEFINE_COMPONENT_EDITOR(MaterialComponent)
{
	BASE_COMPONENT_EDITOR(MaterialComponent)

	void DrawUI(BaseComponent* component) override;
};

void MaterialComponentEditor::DrawUI(BaseComponent* component)
{
	MaterialComponent* materialComponent = static_cast<MaterialComponent*>(component);
	assert(nullptr != materialComponent && "Component is not of type material component");
	for (size_t i = 0; i < materialComponent->Materials.size(); ++i)
	{
		char name[20];
		sprintf_s(name, "Material %i", i);

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowOverlapMode;
		if (ImGui::TreeNodeEx(name, flags))
		{
			Material* mat = materialComponent->Materials[i];

			char* path = mat->FilePath;
			if (strcmp(path, "") == 0)
				path = "(None)";

			ImGui::PushStyleColor(ImGuiCol_Text, ImColor(240, 230, 140));
			ImGui::LabelText("", "Texture: %s", path);
			ImGui::PopStyleColor();

			ImGui::SliderFloat4("Ambient", &mat->ambient.x, 0.f, 1.f);
			ImGui::SliderFloat4("Diffuse", &mat->diffuse.x, 0.f, 1.f);
			ImGui::SliderFloat4("Specular", &mat->specular.x, 0.f, 1.f);
			ImGui::SliderFloat("Shininess", &mat->shininess, 0.f, 128.f);

			ImGui::TreePop();
		}
	}
}

REGISTER_COMPONENT_EDITOR(MaterialComponentEditor)
