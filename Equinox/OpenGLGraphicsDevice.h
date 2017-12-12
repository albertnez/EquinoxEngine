#pragma once

#include "Module.h"

class OpenGLGraphicsDevice : public Module
{
public:
	~OpenGLGraphicsDevice() = default;

	uint32_t GenerateBuffer() const;
	void BindBuffer(GLenum bufferType, uint32_t buffer) const;
	void UnbindBuffer(GLenum buffer) const;
	void LoadDataToBuffer(GLenum bufferType, size_t size, const void* data, GLenum usage);
};

