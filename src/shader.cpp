#include "shader.h"

unsigned Shader::get_id()
{
	return id;
}

void Shader::use()
{
	glUseProgram(id);
}

Shader::Shader(const char* vertex_path, const char* fragment_path)
{
	std::string vertex_code, fragment_code;
	std::ifstream vertex_file, fragment_file;

	//TODO the rest.

}
