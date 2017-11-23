#include "BaseComponentEditor.h"
#include "MeshComponent.h"

#include "IMGUI/imgui.h"

DEFINE_COMPONENT_EDITOR(MeshComponent)
{
	BASE_COMPONENT_EDITOR(MeshComponent)

	void DrawUI(BaseComponent* component) override;
};

void MeshComponentEditor::DrawUI(BaseComponent* component)
{
	MeshComponent* meshComponent = static_cast<MeshComponent*>(component);
	assert(nullptr != meshComponent && "Component is not of type mesh component");

	ImGui::Checkbox("Enabled", &meshComponent->Enabled); /*ImGui::SameLine();
											  ImGui::PushStyleColor(ImGuiCol_Button, ImColor(255, 0, 0));
											  if (ImGui::Button("Delete Component"))
											  Parent->DeleteComponent(this);
											  ImGui::PopStyleColor();*/

	ImGui::PushStyleColor(ImGuiCol_Text, ImColor(240, 230, 140));

	ImGui::LabelText("", "%i mesh(es)", meshComponent->Meshes.size());
	int vertex, indices;
	vertex = indices = 0;
	int i = 0;
	for (Mesh* mesh : meshComponent->Meshes)
	{
		vertex += mesh->num_vertices;
		indices += mesh->num_indices;
		ImGui::LabelText("", "Mesh %i: %i triangles (%i indices, %i vertices)", i, mesh->num_indices / 3, mesh->num_indices,
		                 mesh->num_vertices);
		++i;
	}

	ImGui::LabelText("", "Total: %i triangles (%i indices, %i vertices)", indices / 3, indices, vertex);

	ImGui::PopStyleColor();
}

REGISTER_COMPONENT_EDITOR(MeshComponentEditor)
