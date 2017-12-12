#ifndef __MODULERENDER_H__
#define __MODULERENDER_H__

#include "Module.h"
#include "Rectangle3.h"

#define CHECKERS_WIDTH 64
#define CHECKERS_HEIGHT 64

class Primitive;
class Level;

class ModuleRender : public Module
{
public:
	
	ModuleRender();
	~ModuleRender();

	bool Init();
	bool Start();
	update_status PreUpdate(float DeltaTime);
	update_status Update(float DeltaTime);
	update_status PostUpdate(float DeltaTime);
	bool CleanUp();

	void SetVSync(int interval) const;

public:
	void* context = nullptr;
		
private:
	std::list<Primitive*> objects;

	std::shared_ptr<class ModuleWindow> _moduleWindow;
	std::shared_ptr<class ModuleInput> _moduleInput;
	std::shared_ptr<class ModuleCameraManager> _cameraManager;
};

#endif // __MODULERENDER_H__
