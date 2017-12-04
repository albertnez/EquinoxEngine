#ifndef __MODULEEDITORCAMERA_H__
#define __MODULEEDITORCAMERA_H__

#include "Module.h"
#include <MathGeoLib/include/Geometry/Frustum.h>
#include "CameraComponent.h"
#include "ModuleInput.h"

class ModuleEditorCamera :
	public Module
{
public:
	ModuleEditorCamera();
	~ModuleEditorCamera();

	bool Init() override;
	update_status Update(float DeltaTime) override;
	bool CleanUp() override;

	void SetFOV(float fov) const;
	void SetAspectRatio(float ratio) const;
	void SetPlaneDistances(float near, float far) const;
	float3 Position() const;
	float3 Orientation() const;
	void LookAt(float x, float y, float z) const;
	float* GetProjectionMatrix() const;
	float* GetViewMatrix() const;

	CameraComponent* GetCamera() const;

private:
	CameraComponent* _cameraComponent;

	std::shared_ptr<ModuleInput> _moduleInput;
};

#endif // __MODULEEDITORCAMERA_H__
