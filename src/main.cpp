// compilation: g++ -std=c++17 ./src/*.cpp ./src/glad.c -o prog -I ./include -I./thirdparty/glm-master/ -lSDL2 -ldl
 
#include "../include/ShaderProgram.hpp"
#include "../include/VBO.hpp"
#include "../include/VAO.hpp"
#include "../include/glad/glad.h"
#include <SDL2/SDL.h>
#include <iostream>
#include <vector>

// GLM HEADERS:
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp> // glm::pi
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

// our libraries
#include "../include/Camera.hpp"
//Globals

constexpr int gScreenHeight =600;
constexpr int gScreenWidth = 800;

SDL_Window* gGraphicsApplicationWindow = nullptr;
SDL_GLContext gOpenGLContext = nullptr;
 
bool gQuit = false;


Camera gCamera;
// ----------------- ERROR HANDLING ROUTINES -----------------------
static void GLClearAllError()
{
    while(glGetError() != GL_NO_ERROR)
    {
        
    }
}

// return true if error
static bool GLCheckErrorStatus(const char* function, int line)
{
    while(GLenum error = glGetError())
    {
        std::cout << "OpenGL ERROR: " << std::to_string(error)  << "\tLine: "<< line<<
            "\tfunction: " << function << std::endl;
        return true;
    }
    return false;
}
// macro
#define GLCheck(x) GLClearAllError(); x; GLCheckErrorStatus(#x, __LINE__);

// -------------------------------------------------------------------



void GetOpenGLVersionInfo()
{
    std::cout << "Vendor:" << glGetString(GL_VENDOR)  << std::endl;

    std::cout << "Renderer:" << glGetString(GL_RENDERER)  << std::endl;

    std::cout << "Version:" << glGetString(GL_VERSION)  << std::endl;

    std::cout << "Shading Language:" <<
    glGetString(GL_SHADING_LANGUAGE_VERSION)  << std::endl;


}

// Cube vertex data (36 vertices for 6 faces)
const std::vector<GLfloat> gCubeVertices = {
    // Back face
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    // Front face
    -0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    // Left face
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    // Right face
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    // Bottom face
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,
    // Top face
    -0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f
};

//helper to setup a VAO with position attribute from a shared VBO
void SetupCubeVAO(VAO& vao, VBO& vbo)
{
    vao.create();
    vao.bind();
    vbo.bind();
    //position attribute: location 0, 3 floats, stride = 3 floats, offset = 0
    vao.setVertexAttrib(0, 3, sizeof(GLfloat) * 3, 0);
}

void VertexSpecification(VAO& cubeVAO, VBO& cubeVBO, VAO& lightCubeVAO)
{
    //create and upload shared VBO with cube vertex data
    cubeVBO.create();
    cubeVBO.bind();
    cubeVBO.setData(gCubeVertices);

    //setup VAO for main cube
    SetupCubeVAO(cubeVAO, cubeVBO);

    //setup VAO for light cube (shares the same VBO)
    SetupCubeVAO(lightCubeVAO, cubeVBO);

    //unbind to prevent accidental modification
    VAO::unbind();
    VBO::unbind();
}


void InitializeProgram()
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL2 COUNT NOT INITILAIZE VIDEO SUB SYSTEM";
        exit(1);
    }

    // render window
    gGraphicsApplicationWindow = SDL_CreateWindow("OpenGL window", 0, 0,
            gScreenWidth, gScreenHeight,
            SDL_WINDOW_OPENGL);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);



    if(gGraphicsApplicationWindow == nullptr)
    {
        std::cout << "SDL_Window was not created" << std::endl;
        exit(1);
    }

    // the openGL context is basically all objects and methods that 
    // openGL encompasses
    gOpenGLContext = SDL_GL_CreateContext(gGraphicsApplicationWindow);

    if(gOpenGLContext == nullptr)
    {
        std::cout << "OpenGL context not available"  << std::endl;
        exit(1);
    }

    // initialize glad lib
    // the glad loader is a function pointer that points to
    // openGL functions at runtime
    // so yeah it gives us all the openGL function!
    if(!gladLoadGLLoader(SDL_GL_GetProcAddress))
    {
        std::cout << "Glad was not initiliazed"  << std::endl;
        exit(1);
    }

    GetOpenGLVersionInfo();
                                    
}
void Input()
{
    static int mouseX {gScreenWidth / 2};
    static int mouseY {gScreenHeight / 2};
    SDL_Event e;
    while(SDL_PollEvent(&e) != 0)
    {
        if(e.type == SDL_QUIT)
        {
            std::cout << "Cya" << std::endl;
            gQuit = true;
        }
        if(e.type == SDL_MOUSEMOTION)
        {
            mouseX += e.motion.xrel;
            mouseY += e.motion.yrel;
            gCamera.MouseLook(mouseX -1 , mouseY - 1); 
            std::cout << e.motion.xrel << std::endl;
        }
    }

    // key presses!
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if(state[SDL_SCANCODE_W])
    {
        gCamera.MoveForward(gCamera.speed);
    }
    if(state[SDL_SCANCODE_S])
    {
        gCamera.MoveBackward(gCamera.speed);
    }
    if(state[SDL_SCANCODE_D])
    {
        gCamera.MoveRight(gCamera.speed);
    }
    if(state[SDL_SCANCODE_A])
    {
        gCamera.MoveLeft(gCamera.speed);
    }
}

void MainLoop(ShaderProgram &cubeShad,ShaderProgram &lightShad, VAO &vao, VBO &vbo, VAO &lightCubeVAO)
{
    SDL_WarpMouseInWindow(gGraphicsApplicationWindow, gScreenWidth / 2, gScreenHeight / 2);
    while(!gQuit)
    {
        Input();


        glEnable(GL_DEPTH_TEST);

        glDisable(GL_CULL_FACE);

        glViewport(0, 0, gScreenWidth, gScreenHeight);
        glClearColor(1.f, 1.f, 0.0f, 1.f);

        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        // use the vertex and fragment shader that we defined earlier
        cubeShad.use();

           for(size_t i {}; i < 1; i++)
        {
            // translating our model obect from local space to wordspace

            glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(1.2f, 1.0f, 1.0f));

           // model = glm::rotate(model, glm::radians(gCamera.u_rotate * (i + 1)), glm::vec3(0.0f, 1.0f, 1.0f));

            model           = glm::scale(model, glm::vec3(0.5f));

            // link the uniform variables to our shaders
            cubeShad.setMat4("u_ModelMatrix", model);
            
            glm::mat4 view  = gCamera.GetViewMatrix();
            cubeShad.setMat4("u_ViewMatrix", view);
            // Projection matrix in per
            glm::mat4 perspective = glm::perspective(glm::radians(45.0f), 
                    (float)gScreenWidth/(float)gScreenHeight, 
                    0.1f,
                    10.0f);

            // link the uniform variables to our shaders
            cubeShad.setMat4("u_Projection", perspective);

            // takes in the primitive render type
            // first index to render
            // number of indicies to render (ie number of vertices for triangles)
            vao.bind();
            GLCheck(glDrawArrays(GL_TRIANGLES, 0, 36);)

            
            lightShad.use();
            lightShad.setVec3("u_objColor", 1.0f, 0.5f, 0.31f);
            lightShad.setVec3("u_lightColor", 1.0f, 1.0f, 1.0f);

            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(1.2f, 1.0f, 2.0f));
            model = glm::scale(model, glm::vec3(1.0f));
            lightShad.setMat4("u_ModelMatrix", model);
            lightShad.setMat4("u_ViewMatrix", view);
            lightShad.setMat4("u_Projection", perspective);

            lightCubeVAO.bind();
            GLCheck(glDrawArrays(GL_TRIANGLES, 0, 36);)
            

            

        }

        // update screen
        SDL_GL_SwapWindow(gGraphicsApplicationWindow);
    }
}

void CleanUp()
{

    SDL_DestroyWindow(gGraphicsApplicationWindow);
    SDL_Quit();
}
int main()
{
   
    // set up graphics program
    InitializeProgram();

    VAO vao;
    VBO vbo;
    VAO lightCubeVAO;

    // setup the geometry
    VertexSpecification(vao, vbo, lightCubeVAO);

    // crrate the graphics pipelines
    // MINIMUM: setting vertex and fragment shaders
    ShaderProgram lightShad("shaders/cube_vert.glsl", "shaders/colors_frag.glsl"); 
    ShaderProgram cubeShad("shaders/cube_vert.glsl", "shaders/cube_frag.glsl");
    
    // shaderPrograms for my lightcube and light


    // drawing;
    MainLoop(cubeShad,lightShad, vao, vbo, lightCubeVAO);

    CleanUp();
    return 0;
    
}
