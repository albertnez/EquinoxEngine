#pragma once
#include "Module.h"
#include "CameraComponent.h"
#include <unordered_map>

class ModuleCameraManager :
	public Module
{
public:
	ModuleCameraManager();
	~ModuleCameraManager();

	int AddCamera(CameraComponent* camera);
	void RemoveCamera(const CameraComponent* camera);
	void RemoveCamera(int camera);
	CameraComponent* GetCamera(int camera) const;

	CameraComponent* GetMainCamera() const;
	void SetMainCamera(int camera);
	void SetMainCamera(CameraComponent* camera);

private:
	int _lastId = 0;

	CameraComponent* _mainCamera = nullptr;
	std::unordered_map<int, CameraComponent*> _cameras;
};

