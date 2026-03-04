#ifndef VAO_HPP
#define VAO_HPP

#include "../include/glad/glad.h"

class VAO
{
    public:
        GLuint VAOID {};
        VAO()
        {
            
            // settings things up for the GPU
            // Creating vertex array objects to specify
            // how we read our vertexPos
            // always define before VBO
            glGenVertexArrays(1, &VAOID);
            glBindVertexArray(VAOID);

        }
};

#endif
