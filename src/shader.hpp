#ifndef SHADER_H
#define SHADER_H

#include "../include/glad/glad.h"
#include "utils.hpp"
#include <string>
#include <fstream>
#include <glm/glm.hpp>


class Shader
{
	unsigned id;
public:
	Shader(const char* vertex_path, const char* fragment_path, Utils::Report& r);
	~Shader();
	
	void use();

	unsigned get_id();
	void set_bool(const std::string &name, bool value) const;
	void set_int(const std::string &name, int value) const;
	void set_float(const std::string &name, float value) const;
	void set_mat4(const std::string& name, const glm::mat4& value) const;


};

#endif
