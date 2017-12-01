#include "IMGUI/imgui.h"
#include "IMGUI/imgui_impl_sdl_gl3.h"
#include <GL/glew.h>
#include <iterator>

#include "ModuleEditor.h"
#include "Engine.h"
#include "ModuleWindow.h"
#include "ModuleLighting.h"
#include "DataImporter.h"
#include "EditorSubmodule.h"
#include "EditorUtils.h"
#include "Level.h"
#include "ParticleEmitter.h"
#include "TransformComponent.h"
#include "ModuleAnimation.h"
#include "ModuleLevelManager.h"
#include "ModuleTextures.h"

ModuleEditor::~ModuleEditor()
{
}

bool ModuleEditor::Init()
{
	LOG("Initializing Editor module");
	_dataImporter = new DataImporter;

	IEditorSubmoduleFactoryDictionary* submoduleFactoryDictionary = GetEditorSubmoduleFactoryDictionary();
	_submodules.reserve(submoduleFactoryDictionary->Size());
	for (EditorSubmoduleFactoryBase* submoduleFactory : submoduleFactoryDictionary->GetAllFactories())
	{
		EditorSubmodule* submodule = submoduleFactory->Instantiate();
		submodule->Init();
		_submodules.push_back(submodule);
	}
	submoduleFactoryDictionary->Clear();
	LOG("Registered %d editor submodules", _submodules.size());

	return true;
}

bool ModuleEditor::Start()
{
	ImGui_ImplSdlGL3_Init(App->window->window);

	for (EditorSubmodule* submodule : _submodules)
	{
		submodule->Start();
	}

	App->SetUpdateState(Engine::UpdateState::Stopped);


	std::shared_ptr<Level> level = GetDataImporter()->ImportLevel("Models/street/", "Street.obj");

	App->animator->Load("Idle", "Models/ArmyPilot/Animations/ArmyPilot_Idle.fbx");

	////////////
	GameObject* goPS = new GameObject;
	TransformComponent* transform = new TransformComponent;
	ParticleEmitter* peComponent = new ParticleEmitter(200, float2(50.f, 50.f), 20.f, 1.2f, 15.f);
	unsigned rainTex = App->textures->Load("Models/rainSprite.tga");
	//unsigned snowTex = App->textures->Load("Models/simpleflake.tga");
	peComponent->SetTexture(rainTex);
	goPS->Name = "ParticleSystem";
	goPS->AddComponent(transform);
	goPS->AddComponent(peComponent);

	level->AddToScene(goPS);

	App->level_manager->ChangeLevel(level);

	return true;
}

update_status ModuleEditor::PreUpdate(float DeltaTime)
{
	ImGui_ImplSdlGL3_NewFrame(App->window->window);

	return UPDATE_CONTINUE;
}

update_status ModuleEditor::Update(float DeltaTime)
{
	int w, h;
	App->window->GetWindowSize(w, h);

	for (EditorSubmodule* submodule : _submodules)
	{
		submodule->Update();
	}

	ImGui::SetNextWindowSize(ImVec2(120, 10), ImGuiSetCond_Always);
	ImVec2 playPosition(w / 2 - 60, 10);
	ImGui::SetNextWindowPos(playPosition, ImGuiSetCond_Always);
	if (ImGui::Begin("Editor Status", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_Modal | ImGuiWindowFlags_NoResize | ImGuiAlign_Center))
	{
		std::string playStop = App->GetUpdateState() == Engine::UpdateState::Playing ? "Stop" : "Play";
		if (ImGui::Button(playStop.c_str()))
		{
			switch (App->GetUpdateState()) 
			{ 
				case Engine::UpdateState::Playing:
					App->SetUpdateState(Engine::UpdateState::Stopped);
					break;
				case Engine::UpdateState::Stopped: 
					App->SetUpdateState(Engine::UpdateState::Playing);
					break;
			}

			LOG("Engine state changed to %s", playStop.c_str());
		}

		ImGui::SameLine();

		bool isPaused = App->IsPaused();
		if (ImGui::Button(isPaused ? "Unpause" : "Pause"))
		{
			App->SetPaused(!isPaused);

			LOG("Engine play state changed to %s", isPaused ? "Playing" : "Paused");
		}
	}
	ImGui::End();

	ImGui::Render();

	return UPDATE_CONTINUE;
}

update_status ModuleEditor::PostUpdate(float DeltaTime)
{
	return UPDATE_CONTINUE;
}

bool ModuleEditor::CleanUp()
{
	LOG("Shutting down Engine module");

	ImGui_ImplSdlGL3_Shutdown();

	for (EditorSubmodule* submodule : _submodules)
	{
		submodule->CleanUp();

		RELEASE(submodule);
	}

	_submodules.clear();

	RELEASE(_dataImporter);
	return true;
}

bool ModuleEditor::GetDrawHierachy() const
{
	return _drawHierachy;
}

void ModuleEditor::SetDrawHierachy(bool drawHierachy)
{
	_drawHierachy = drawHierachy;
}

DataImporter* ModuleEditor::GetDataImporter() const
{
	return _dataImporter;
}
