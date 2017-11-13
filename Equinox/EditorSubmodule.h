#pragma once

#include "ModuleEditor.h"

class EditorSubmodule
{
public:
	EditorSubmodule();
	virtual ~EditorSubmodule();

	virtual void Init();
	virtual void Start();
	virtual void Update();
	virtual void CleanUp();
};
