#include "BaseComponentEditor.h"
#include "ParticleEmitter.h"

DEFINE_COMPONENT_EDITOR(ParticleEmitter)
{
	BASE_COMPONENT_EDITOR(ParticleEmitter)

	void DrawUI(BaseComponent* component) override;
};

void ParticleEmitterEditor::DrawUI(BaseComponent* component)
{
	ParticleEmitter* emitter = static_cast<ParticleEmitter*>(component);
	assert(emitter != nullptr && "Component is not of type particle emitter component");
	ImGui::Checkbox("Simulate On Editor", &emitter->_editorSimulation);

	ImGui::SameLine();

	if (ImGui::Button("Restart"))
	{
		emitter->CleanUp();
	}

	ImGui::InputFloat2("Emit Area", &emitter->EmitArea[0], -1, ImGuiInputTextFlags_CharsDecimal);
	ImGui::InputInt("Max Particles", &emitter->MaxParticles, -1);
	ImGui::InputFloat("Fall Height", &emitter->FallHeight, -1, ImGuiInputTextFlags_CharsDecimal);
	ImGui::InputFloat("Fall Speed", &emitter->FallSpeed, -1, ImGuiInputTextFlags_CharsDecimal);
	ImGui::InputFloat("Particle's LifeTime", &emitter->LifeTime, -1, ImGuiInputTextFlags_CharsDecimal);
}

REGISTER_COMPONENT_EDITOR(ParticleEmitterEditor)
