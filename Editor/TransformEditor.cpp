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

	float3 position = transformComponent->GetLocalPosition();
	if (ImGui::InputFloat3("Position", &position[0], -1, ImGuiInputTextFlags_CharsDecimal))
	{
		transformComponent->SetLocalPosition(position);
	}
	float3 rot = RadToDeg(transformComponent->GetLocalRotation().ToEulerXYZ());
	if (ImGui::SliderFloat3("Rotation", &rot[0], 0, 360, "%.2f deg"))
	{
		rot = DegToRad(rot);
		transformComponent->SetLocalRotation(Quat::FromEulerXYZ(rot.x, rot.y, rot.z));
	}
	float3 scale = transformComponent->GetLocalScale();
	if (ImGui::InputFloat3("Scale", &scale[0], -1, ImGuiInputTextFlags_CharsDecimal))
	{
		transformComponent->SetLocalScale(scale);
	}
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
