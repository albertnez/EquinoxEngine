#include "OpenGLGraphicsDevice.h"

uint32_t OpenGLGraphicsDevice::GenerateBuffer() const
{
	uint32_t bufId = 0;
	glGenBuffers(1, &bufId);

	return bufId;
}

void OpenGLGraphicsDevice::BindBuffer(GLenum bufferType, uint32_t buffer) const
{
	glBindBuffer(bufferType, buffer);
}

void OpenGLGraphicsDevice::UnbindBuffer(GLenum buffer) const
{
	glBindBuffer(buffer, 0);
}

void OpenGLGraphicsDevice::LoadDataToBuffer(GLenum bufferType, size_t size, const void* data, GLenum usage)
{
	glBufferData(bufferType, size, data, usage);
}
