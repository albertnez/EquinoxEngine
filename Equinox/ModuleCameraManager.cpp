#include "ModuleCameraManager.h"
#include "Engine.h"
#include "ModuleWindow.h"


ModuleCameraManager::ModuleCameraManager()
{
}

ModuleCameraManager::~ModuleCameraManager()
{
}

int ModuleCameraManager::AddCamera(CameraComponent* camera)
{
	assert(camera != nullptr && "Camera is null");
	assert(_cameras.find(camera->GetCameraId()) != _cameras.end() && "Camera already inserted");

	_cameras[_lastId] = camera;
	camera->_cameraId = _lastId;

	++_lastId;
	return camera->GetCameraId();
}

void ModuleCameraManager::RemoveCamera(const CameraComponent* camera)
{
	_cameras.erase(camera->GetCameraId());
}

void ModuleCameraManager::RemoveCamera(int cameraId)
{
	_cameras.erase(cameraId);
}

CameraComponent* ModuleCameraManager::GetCamera(int camera) const
{
	auto it = _cameras.find(camera);
	return it != _cameras.end() ? it->second : nullptr;
}

CameraComponent* ModuleCameraManager::GetMainCamera() const
{
	return _mainCamera;
}

void ModuleCameraManager::SetMainCamera(int cameraId)
{
	CameraComponent* camera = GetCamera(cameraId);

	SetMainCamera(camera);
}

void ModuleCameraManager::SetMainCamera(CameraComponent* camera)
{
	if (camera != nullptr)
	{
		int w, h;
		App->GetModule<ModuleWindow>()->GetWindowSize(w, h);
		camera->SetAspectRatio(float(w) / float(h));
		_mainCamera = camera;
	}
}
