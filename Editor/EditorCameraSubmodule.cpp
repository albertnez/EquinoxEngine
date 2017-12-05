#include "EditorSubmodule.h"
#include "EditorUtils.h"
#include "CameraComponent.h"
#include "ModuleInput.h"

#include <SDL/include/SDL.h>
#include <Math/Quat.h>
#include "ModuleCameraManager.h"

class EditorCameraSubmodule : public EditorSubmodule
{
public:
	void Start() override;
	void Update() override;
	void CleanUp() override;

private:
	CameraComponent* _cameraComponent;

	std::shared_ptr<ModuleInput> _moduleInput;
};

REGISTER_EDITOR_SUBMODULE(EditorCameraSubmodule)

void EditorCameraSubmodule::Start()
{
	_moduleInput = App->GetModule<ModuleInput>();
	_cameraComponent = new CameraComponent;
	App->GetModule<ModuleCameraManager>()->SetMainCamera(_cameraComponent);
}

void EditorCameraSubmodule::Update()
{
	float3 movement = float3::zero;

	float rotateUp = 0;
	float rotateRight = 0;
	SDL_ShowCursor(1);
	SDL_SetRelativeMouseMode(SDL_FALSE);
	if (_moduleInput->GetMouseButtonDown(SDL_BUTTON_RIGHT))
	{
		SDL_SetRelativeMouseMode(SDL_TRUE);
		SDL_ShowCursor(0);
		iPoint mouse_movement = _moduleInput->GetMouseMotion();
		float2 mouse_drag = { float(mouse_movement.x), float(mouse_movement.y) };

		if (!mouse_drag.IsZero())
			mouse_drag.Normalize();

		rotateUp = -mouse_drag.y * 3.f;
		rotateRight = -mouse_drag.x * 3.f;
	}

	if (_moduleInput->GetKey(SDL_SCANCODE_UP))
		rotateUp += 1;
	if (_moduleInput->GetKey(SDL_SCANCODE_DOWN))
		rotateUp -= 1;
	if (_moduleInput->GetKey(SDL_SCANCODE_RIGHT))
		rotateRight -= 1;
	if (_moduleInput->GetKey(SDL_SCANCODE_LEFT))
		rotateRight += 1;

	CLAMP(rotateUp, -1, 1);
	CLAMP(rotateRight, -1, 1);

	if (rotateRight != 0)
	{
		Quat rot = Quat::RotateY(DegToRad(rotateRight));

		//_frustum.SetFront(rot.Mul(_frustum.Front()).Normalized());
		_cameraComponent->SetFront(rot.Mul(_cameraComponent->Orientation()).Normalized());
		_cameraComponent->SetUp(rot.Mul(_cameraComponent->GetUp()).Normalized());
	}

	if (rotateUp != 0.f)
	{
		Quat rot = Quat::RotateAxisAngle(_cameraComponent->GetWorldRight(), DegToRad(rotateUp));

		float3 up = rot.Mul(_cameraComponent->GetUp()).Normalized();

		if (up.y > 0.f)
		{
			_cameraComponent->SetFront(rot.Mul(_cameraComponent->Orientation()).Normalized());
			_cameraComponent->SetUp(up);
		}
	}

	iPoint wheel_movement = _moduleInput->GetMouseWheel();

	if (wheel_movement.y != 0)
	{
		if (wheel_movement.y > 0)
			movement += _cameraComponent->Orientation();
		else
			movement -= _cameraComponent->Orientation();
	}

	if (_moduleInput->GetKey(SDL_SCANCODE_Q))
		movement += float3::unitY;
	if (_moduleInput->GetKey(SDL_SCANCODE_E))
		movement -= float3::unitY;
	if (_moduleInput->GetKey(SDL_SCANCODE_W))
		movement += _cameraComponent->Orientation();
	if (_moduleInput->GetKey(SDL_SCANCODE_S))
		movement -= _cameraComponent->Orientation();
	if (_moduleInput->GetKey(SDL_SCANCODE_A))
		movement -= _cameraComponent->GetWorldRight();
	if (_moduleInput->GetKey(SDL_SCANCODE_D))
		movement += _cameraComponent->GetWorldRight();

	float velocity = (_moduleInput->GetKey(SDL_SCANCODE_LSHIFT) || wheel_movement.y != 0) ? 0.6f : 0.1f;

	_cameraComponent->SetPos(_cameraComponent->Position() + movement*velocity);
}

void EditorCameraSubmodule::CleanUp()
{
	_cameraComponent->CleanUp();
	RELEASE(_cameraComponent);
}
