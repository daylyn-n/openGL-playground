#ifndef VBO_HPP
#define VBO_HPP

#include "../include/glad/glad.h"
class VBO
{
    public:
        GLuint VBOID{};
        VBO()
        {
            // vertex buffer object
            // buffer objects to store our
            // vertexPos and define how we
            // read the data in glBufferData

            glGenBuffers(1, &VBOID);
            glBindBuffer(GL_ARRAY_BUFFER, VBOID);

        }
};

#endif
