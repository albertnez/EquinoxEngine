#include "Globals.h"
#include "Engine.h"
#include "ProgramManager.h"

ProgramManager::ProgramManager()
{

}

ProgramManager::~ProgramManager()
{

}

bool ProgramManager::Start()
{
	std::shared_ptr<ShaderProgram> unlit = CreateProgram("Unlit");

	AddShaderToProgram(unlit, "Shaders/SimpleVertexShader.ver", GL_VERTEX_SHADER);
	AddShaderToProgram(unlit, "Shaders/SimpleFragmentShader.frag", GL_FRAGMENT_SHADER, { "#define TEXTURE\n" });

	CompileAndAttachProgramShaders(unlit);

	return true;
}

bool ProgramManager::CleanUp()
{
	for (auto elem : programs)
	{
		std::shared_ptr<ShaderProgram> program = elem.second;
		for (GLuint shader : program->shaders)
			glDeleteShader(shader);

		glDeleteProgram(program->id);

		program.reset();
	}

	programs.clear();
	
	return true;
}

std::shared_ptr<ShaderProgram> ProgramManager::CreateProgram(const std::string& name)
{
	auto it = programs.find(name);
	assert(it == programs.end() && "Shader is being created twice!");
	if (it == programs.end())
	{
		GLuint shaderProgramId = glCreateProgram();
		std::shared_ptr<ShaderProgram> shaderProgram = std::make_shared<ShaderProgram>();
		shaderProgram->id = shaderProgramId;
		shaderProgram->shaders.clear();
		programs.insert(std::pair<std::string, std::shared_ptr<ShaderProgram>>(name, shaderProgram));

		return shaderProgram;
	}

	return nullptr;
}

void ProgramManager::AddShaderToProgram(std::shared_ptr<ShaderProgram> program, const char* filepath, const GLenum shaderType, const std::vector<char*>& preprocessor) const
{
	if (program != nullptr)
	{
		FILE* shaderFile = fopen(filepath, "rb");
		int fileSize = 0;

		char* shaderSource;

		fseek(shaderFile, 0, SEEK_END);
		fileSize = ftell(shaderFile);
		rewind(shaderFile);

		shaderSource = new char[fileSize + 1];
		fread(shaderSource, sizeof(char), fileSize, shaderFile);
		shaderSource[fileSize] = '\0';
		fclose(shaderFile);

		char** source = new char*[preprocessor.size() + 1];

		for (int i = 0; i < preprocessor.size(); ++i)
			source[i] = preprocessor[i];

		source[preprocessor.size()] = shaderSource;

		unsigned int shader = glCreateShader(shaderType);
		glShaderSource(shader, preprocessor.size() + 1, const_cast<const GLchar**>(source), nullptr);

		program->shaders.push_back(shader);

		delete[] shaderSource;
		delete[] source;
	}
}

std::shared_ptr<ShaderProgram> ProgramManager::GetProgramByName(const std::string &name) const
{
	auto it = programs.find(name);
	return it != programs.end() ? it->second : nullptr;
}

bool ProgramManager::UseProgram(std::shared_ptr<ShaderProgram> program) const
{
	if (program != nullptr)
	{
		glUseProgram(program->id);
		return true;
	}
	return false;
}

void ProgramManager::UseDefaultProgram() const
{
	glUseProgram(0);
}

bool ProgramManager::UseProgram(const std::string &name) const
{
	std::shared_ptr<ShaderProgram> itShader = GetProgramByName(name);
	if (itShader != nullptr)
	{
		return UseProgram(itShader);
	}
	return false;
}

void ProgramManager::logShaderCompiler(const GLuint shader) const
{
	GLint maxLength = 0;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

	// The maxLength includes the NULL character
	std::vector<GLchar> errorLog(maxLength);
	glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);

	// Delete to avoid leak
	glDeleteShader(shader);

	LOG("Shader Compiler LOG Start");
	LOG(&errorLog[0]);
	LOG("Shader Compiler LOG End");
}

void ProgramManager::logProgramLinker(const std::shared_ptr<ShaderProgram> program) const
{
	GLint maxLength = 0;
	glGetProgramiv(program->id, GL_INFO_LOG_LENGTH, &maxLength);

	// The maxLength includes the NULL character
	std::vector<GLchar> infoLog(maxLength);
	glGetProgramInfoLog(program->id, maxLength, &maxLength, &infoLog[0]);

	// Delete to avoid leak
	for (GLuint shader : program->shaders)
		glDeleteShader(shader);

	glDeleteProgram(program->id);

	LOG("Program Linker LOG Start");
	LOG(&infoLog[0]);
	LOG("Program Linker LOG End");
}

bool ProgramManager::CompileAndAttachProgramShaders(std::shared_ptr<ShaderProgram> program) const
{
	for (GLuint shader : program->shaders) {
		glCompileShader(shader);

		GLint isCompiled = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			logShaderCompiler(shader);
			return false;
		}
		else {
			LOG("SHADER COMPILED: OK");
			glAttachShader(program->id, shader);
		}
	}

	glLinkProgram(program->id);

	GLint isLinked = 0;
	glGetProgramiv(program->id, GL_LINK_STATUS, &isLinked);
	if (isLinked == GL_FALSE)
	{
		logProgramLinker(program);
		return false;
	}
	else
	{
		LOG("PROGRAM LINKED: OK");
		return true;
	}
	return true;
}
