#ifndef SHADER_H
#define SHADER_H
#pragma once

#include "glad/glad.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

static unsigned int ID;

class Shader
{
public:
    Shader(const char *vertexPath, const char *fragmentPath);
    void use();
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setMat4(const std::string &name, const float *value) const;

private:
    void checkCompileErrors(unsigned int shader, std::string type);
};

#endif
