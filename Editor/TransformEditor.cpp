#include "BaseComponentEditor.h"
#include "TransformComponent.h"

#include "IMGUI/imgui.h"

DEFINE_COMPONENT_EDITOR(TransformComponent)
{
	BASE_COMPONENT_EDITOR(TransformComponent)

public:

	void DrawUI(BaseComponent* component) override;
	bool IsRemovable() const override;
	bool CanBeDisabled() const override;
};

void TransformComponentEditor::DrawUI(BaseComponent* component)
{
	TransformComponent* transformComponent = static_cast<TransformComponent*>(component);
	assert(nullptr != transformComponent && "Component is not of type transform component");

	ImGui::InputFloat3("Position", &transformComponent->Position[0], -1, ImGuiInputTextFlags_CharsDecimal);
	float3 rot = transformComponent->Rotation.ToEulerXYZ() * RadToDeg(transformComponent->Rotation.Angle());
	ImGui::SliderFloat3("Rotation", &rot[0], -360, 360, "%.2f deg");
	ImGui::InputFloat3("Scale", &transformComponent->Scale[0], -1, ImGuiInputTextFlags_CharsDecimal);
}

bool TransformComponentEditor::IsRemovable() const
{
	return false;
}

bool TransformComponentEditor::CanBeDisabled() const
{
	return false;
}

REGISTER_COMPONENT_EDITOR(TransformComponentEditor)
