#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp> // glm::pi
#include <iostream>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

int main()
{
    glm::vec3 A(3.0f, 0.0f, 0.0f);
    glm::vec3 B(0.5f, 1.0f, 0.0f);


    //std::cout << "Length of vector A: " << glm::length(A) << '\n'; 
    
    glm::vec4 vertex(1.0f, 5.0f, 1.0f, 1.0f);

    glm::mat4 model(1.0f);
    model = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 2.0f));

    for(int i = 0; i < 4; i++)
    {
        std::cout << glm::to_string(model[i]) << std::endl;
    }
}
