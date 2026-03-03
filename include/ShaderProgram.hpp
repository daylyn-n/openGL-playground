#ifndef SHADERPROGRAM_HPP
#define SHADERPROGRAM_HPP


#include "../include/glad/glad.h"
#include <SDL2/SDL.h>
#include <fstream>


#include "../thirdparty/glm-master/glm/glm.hpp"
#include "../thirdparty/glm-master/glm/gtc/matrix_transform.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "../thirdparty/glm-master/glm/gtx/rotate_vector.hpp"
class ShaderProgram
{

    public:
        
        // program object for our shaders
        // has control of our vertex shader and fragment shader
        // to define how our vertices and triangles are used

        GLuint GraphicsPipelineShaderProgram = 0;
        
        // ---------- HELPERS TO LOAD SHADER FILES, COMPILE SHADERS AND THEN ATTACH AND CREATE THEM TO OUR GRAPHICS PIPELINE PROGRAM -----------
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

            glDeleteShader(myVertexShader);
            glDeleteShader(myFragmentShader);
            return programObject;
        }
        void CreateGraphicsPipeline()
        {
            std::string vertexShaderSrc     = LoadShaderAsString("shaders/vert.glsl");
            std::string fragmentShaderSrc   = LoadShaderAsString("shaders/frag.glsl");


            GraphicsPipelineShaderProgram = CreateShaderProgram(vertexShaderSrc, fragmentShaderSrc);
        }

        //------- HELPERS END HERE ----------
    


        // constructor should read the src code for the shaders and then compile them, attaching it to our 
        // graphicspipleline shaderID;
        ShaderProgram(const std::string &vertPath, const std::string &fragPath)
        {
            std::string vertexShaderSrc     = LoadShaderAsString(vertPath);
            std::string fragmentShaderSrc   = LoadShaderAsString(fragPath);
            GraphicsPipelineShaderProgram = CreateShaderProgram(vertexShaderSrc, fragmentShaderSrc);
        }

};

#endif

