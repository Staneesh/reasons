#include "shader.hpp"

unsigned Shader::get_id() const
{
	return id;
}

void Shader::use() const
{
	glUseProgram(id);
}

Shader::~Shader()
{
    
}

Shader::Shader(const char* vertex_path, const char* fragment_path, Utils::Report& r)
{
    r = Utils::Report();
	unsigned vertex, fragment;
	int program;
	int success;
    char infoLog[512];
    
    unsigned char* vsCode;
    unsigned char* fsCode;

    auto vert_read_err = Utils::load_entire_file(vertex_path, &vsCode);
    r.append(vert_read_err);
    if (r.is_bad())
    {
        return;
    }

	auto frag_read_err = Utils::load_entire_file(fragment_path, &fsCode);
    r.append(frag_read_err);

    if(r.is_bad())
    {
        free(vsCode);
        return;
    }
    
    vertex = glCreateShader(GL_VERTEX_SHADER);
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(vertex, 1, (const char**)&vsCode, NULL);
    glShaderSource(fragment, 1, (const char**)&fsCode, NULL);
    
    glCompileShader(vertex);
    glCompileShader(fragment);
    
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    
    if(!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        r.append(Utils::Report::Type::ERROR, "vertex shader: " + std::string(infoLog));
    }
    
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    
    if(!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        r.append(Utils::Report::Type::ERROR, "fragment shader: " + std::string(infoLog));
    }
    program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);
    
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        r.append(Utils::Report::Type::ERROR, "program linking: " + std::string(infoLog));
    }
    
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    
    free(vsCode);
    free(fsCode);

    id = program;
}

void Shader::set_mat4(const std::string &name, const glm::mat4 &mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}