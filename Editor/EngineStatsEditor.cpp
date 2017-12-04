#include "IMGUI/imgui.h"

#include "EditorSubmodule.h"
#include "Engine.h"
#include "ModuleWindow.h"
#include "EditorUtils.h"

namespace
{
	float ListGetter(void* data, int id)
	{
		auto it = reinterpret_cast<std::list<float>*>(data)->begin();
		advance(it, id);
		return *it;
	}
}

class EngineStatsEditor : public EditorSubmodule
{
public:
	void Init() override;
	void Update() override;

private:
	std::list<float> _fpsValues;

	std::shared_ptr<ModuleWindow> _moduleWindow;
};

REGISTER_EDITOR_SUBMODULE(EngineStatsEditor)

void EngineStatsEditor::Init()
{
	_moduleWindow = App->GetModule<ModuleWindow>();
}

void EngineStatsEditor::Update()
{
	int w, h;
	_moduleWindow->GetWindowSize(w, h);

	ImVec2 windowPosition(0, h - 100);
	ImGui::SetNextWindowSize(ImVec2(300, 100), ImGuiSetCond_Always);
	ImGui::SetNextWindowPos(windowPosition, ImGuiSetCond_Always);
	if (ImGui::Begin("Engine Stats", nullptr, ImGuiWindowFlags_AlwaysUseWindowPadding))
	{
		float framerate = ImGui::GetIO().Framerate;

		_fpsValues.push_back(framerate);
		if (_fpsValues.size() > 30)
			_fpsValues.pop_front();

		if (ImGui::BeginChild("Histogram", ImVec2(0, 0), true))
		{
			ImGui::Text("FPS: %f", framerate);
			ImGui::PlotHistogram("Framerate", &ListGetter, &_fpsValues, _fpsValues.size(), 0, nullptr, 0, 120);
		}

		ImGui::EndChild();
	}
	ImGui::End();
}
