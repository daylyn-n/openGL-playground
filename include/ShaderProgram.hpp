#ifndef SHADERPROGRAM_HPP
#define SHADERPROGRAM_HPP


#include "../include/glad/glad.h"
#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <fstream>

// GLM HEADERS:
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp> // glm::pi
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
class ShaderProgram
{

    public:
        std::string LoadShaderAsString(const std::string& filename)
        {
            std::string res = "";

            std::string line = "";
            std::ifstream myFile(filename.c_str());

            if(myFile.is_open())
            {
                while(std::getline(myFile, line))
                {
                    res+=line + '\n';
                }
                myFile.close();
            }
            return res;
        }


        // program object for our shaders
        // has control of our vertex shader and fragment shader
        // to define how our vertices and triangles are used

        GLuint gGraphicsPipelineShaderProgram = 0;

        GLuint CompileShader(GLuint type, const std::string &sourceCode)
        {
            GLuint shaderObject;
            if(type == GL_VERTEX_SHADER)
            {
                shaderObject = glCreateShader(GL_VERTEX_SHADER);
            }
            else if(type == GL_FRAGMENT_SHADER)
            {
                shaderObject = glCreateShader(GL_FRAGMENT_SHADER);
            }
            const char* src = sourceCode.c_str();
            glShaderSource(shaderObject, 1, &src, nullptr);
            glCompileShader(shaderObject);
            return shaderObject;
        }
        // takes in vertex and fragment shader source codes

        GLuint CreateShaderProgram(const std::string &vs, const std::string &fs)
        {
            GLuint programObject = glCreateProgram();

            GLuint myVertexShader = CompileShader(GL_VERTEX_SHADER, vs);
            GLuint myFragmentShader = CompileShader(GL_FRAGMENT_SHADER, fs);

            // linking our two shader programs(vertex and fragment) into one executable
            // file, for our graphicsPipelne
            glAttachShader(programObject, myVertexShader);
            glAttachShader(programObject, myFragmentShader);
            glLinkProgram(programObject);

            // validate our progam
            glValidateProgram(programObject);
            //detach shader

            return programObject;
        }
    private:

        
};

#endif

