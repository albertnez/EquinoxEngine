#ifndef __PROGRAM_MANAGER__H__
#define __PROGRAM_MANAGER__H__

#include "Module.h"
#include <map>

struct ShaderProgram
{
	GLuint id;
	std::list<GLuint> shaders;
};

class ProgramManager : public Module
{
public:
	ProgramManager();
	~ProgramManager();

	bool Start() override;
	bool CleanUp() override;

	std::shared_ptr<ShaderProgram> CreateProgram(const std::string &name);
	void AddShaderToProgram(std::shared_ptr<ShaderProgram> program, const char* filepath, const GLenum shaderType, const std::vector<char*>& preprocessor = std::vector<char*>(0)) const;

	std::shared_ptr<ShaderProgram> GetProgramByName(const std::string &name) const;
	bool UseProgram(const std::string &name) const;
	bool UseProgram(std::shared_ptr<ShaderProgram> program) const;
	void UseDefaultProgram() const;

	bool CompileAndAttachProgramShaders(std::shared_ptr<ShaderProgram> program) const;

private:
	std::map<std::string, std::shared_ptr<ShaderProgram>> programs;

	void logShaderCompiler(const GLuint shader) const;
	void logProgramLinker(const std::shared_ptr<ShaderProgram> program) const;
};

#endif

