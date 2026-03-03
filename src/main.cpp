// compilation: g++ -std=c++17 ./src/*.cpp ./src/glad.c -o prog -I ./include -I./thirdparty/glm-master/ -lSDL2 -ldl
 

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

// our libraries
#include "../include/Camera.hpp"
//Globals

int gScreenHeight =600;
int gScreenWidth = 800;

SDL_Window* gGraphicsApplicationWindow = nullptr;
SDL_GLContext gOpenGLContext = nullptr;
 
bool gQuit = false;

GLuint gVAO = 0;
GLuint gVBO = 0;
GLuint gLightVAO = 0;
// used to store the array of indicies used to draw our verts
GLuint gIBO = 0;

float g_u_offset = -2.0f;
float g_u_Rotate = 0;
float g_u_size = 1;
float gSpeed = 0.05f;
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
void CreateGraphicsPipeline()
{
    std::string vertexShaderSrc     = LoadShaderAsString("shaders/vert.glsl");
    std::string fragmentShaderSrc   = LoadShaderAsString("shaders/frag.glsl");


    gGraphicsPipelineShaderProgram = CreateShaderProgram(vertexShaderSrc, fragmentShaderSrc);
}
void GetOpenGLVersionInfo()
{
    std::cout << "Vendor:" << glGetString(GL_VENDOR)  << std::endl;

    std::cout << "Renderer:" << glGetString(GL_RENDERER)  << std::endl;

    std::cout << "Version:" << glGetString(GL_VERSION)  << std::endl;

    std::cout << "Shading Language:" <<
    glGetString(GL_SHADING_LANGUAGE_VERSION)  << std::endl;


}

void VertexSpecification()
{

    // Lives on CPU
    const std::vector<GLfloat> vertexData
    {
     -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f, 
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f, 

    -0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,

    -0.5f,  0.5f,  0.5f, 
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,  
    -0.5f, -0.5f, -0.5f, 
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,

     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f, -0.5f, -0.5f, 
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,

    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f, 
    -0.5f, -0.5f, -0.5f,

    -0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    };



    // settings things up for the GPU
    // Creating vertex array objects to specify
    // how we read our vertexPos
    // always define before VBO
    glGenVertexArrays(1, &gVAO);
    glBindVertexArray(gVAO);

    // vertex buffer object
    // buffer objects to store our
    // vertexPos and define how we
    // read the data in glBufferData
     
    glGenBuffers(1, &gVBO);
    glBindBuffer(GL_ARRAY_BUFFER, gVBO);
    // takes in a enum target, in this case array buffer (VAO)
    // takes in the size of our vertexPos, in this case its
    // the size of our vertex multiplied by the size of each element
    // takes in the data
    // and the usage
// CONCLUSION WITH glBUUfferData: we end up with our CPU vertex data being stored on the GPU
    glBufferData(GL_ARRAY_BUFFER,
            vertexData.size() * sizeof(GLfloat),
            vertexData.data(), 
            GL_STATIC_DRAW);

   

    // basically just defining how much data we point to and which attributes we look at when we
    // jump from vertex to vertex
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,
                        3,
                        GL_FLOAT,
                        GL_FALSE,
                        sizeof(GLfloat) * 3,
                        (GLvoid*)0
                        );
    
    
    glGenVertexArrays(1, &gLightVAO);
    glBindVertexArray(gLightVAO);

    glBindBuffer(GL_ARRAY_BUFFER_BINDING, gVBO);
   
    glVertexAttribPointer(0, 3 , GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, (GLvoid*)0);
    
    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
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
        gCamera.MoveForward(gSpeed);
    }
    if(state[SDL_SCANCODE_S])
    {
        gCamera.MoveBackward(gSpeed);
    }
    if(state[SDL_SCANCODE_D])
    {
        gCamera.MoveRight(gSpeed);
    }
    if(state[SDL_SCANCODE_A])
    {
        gCamera.MoveLeft(gSpeed);
    }
}

void PreDraw()
{
    glEnable(GL_DEPTH_TEST);

    glDisable(GL_CULL_FACE);
    
    glViewport(0, 0, gScreenWidth, gScreenHeight);
    glClearColor(1.f, 1.f, 0.0f, 1.f);

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    
    // use the vertex and fragment shader that we defined earlier
    glUseProgram(gGraphicsPipelineShaderProgram);
    
    glBindVertexArray(gVAO);
    glBindBuffer(GL_ARRAY_BUFFER, gVBO);
   
    // adding light
    
    glUniform3fv(glGetUniformLocation(gGraphicsPipelineShaderProgram,"u_objectColor" ))


    g_u_Rotate -=0.1f;
    for(size_t i {}; i < 10; i++)
    {
        // translating our model obect from local space to wordspace

        // get 10 cube position
        glm::vec3 cubePositions[] = {
            glm::vec3( 0.0f,  0.0f,  0.0f), 
            glm::vec3( 2.0f,  5.0f, -15.0f), 
            glm::vec3(-1.5f, -2.2f, -2.5f),  
            glm::vec3(-3.8f, -2.0f, -12.3f),  
            glm::vec3( 2.4f, -0.4f, -3.5f),  
            glm::vec3(-1.7f,  3.0f, -7.5f),  
            glm::vec3( 1.3f, -2.0f, -2.5f),  
            glm::vec3( 1.5f,  2.0f, -2.5f), 
            glm::vec3( 1.5f,  0.2f, -1.5f), 
            glm::vec3(-1.3f,  1.0f, -1.5f)  
        };
        glm::mat4 model = glm::translate(glm::mat4(1.0f), cubePositions[i]);

        model = glm::rotate(model, glm::radians(g_u_Rotate * (i + 1)), glm::vec3(0.0f, 1.0f, 1.0f));

        model           = glm::scale(model, glm::vec3(g_u_size, g_u_size, g_u_size));

        // link the uniform variables to our shaders
        GLint u_ModelMatrixLlocation = glGetUniformLocation(gGraphicsPipelineShaderProgram, "u_ModelMatrix");
        glUniformMatrix4fv(u_ModelMatrixLlocation , 1,
                GL_FALSE, &model[0][0]);

        glm::mat4 view  = gCamera.GetViewMatrix();
        GLint u_ViewLocation = glGetUniformLocation(gGraphicsPipelineShaderProgram, "u_ViewMatrix");
        glUniformMatrix4fv(u_ViewLocation, 1,
                GL_FALSE, &view[0][0]);
        // Projection matrix in per
        glm::mat4 perspective = glm::perspective(glm::radians(45.0f), 
                (float)gScreenWidth/(float)gScreenHeight, 
                0.1f,
                10.0f);

        // link the uniform variables to our shaders
        GLint u_ProjectionLocation = glGetUniformLocation(gGraphicsPipelineShaderProgram, "u_Projection");
        glUniformMatrix4fv(u_ProjectionLocation, 1,
                GL_FALSE, &perspective[0][0]);
   
        // takes in the primitive render type
        // first index to render
        // number of indicies to render (ie number of vertices for triangles)
        GLCheck(glDrawArrays(GL_TRIANGLES, 0, 36);)

    }

}



void MainLoop()
{
    SDL_WarpMouseInWindow(gGraphicsApplicationWindow, gScreenWidth / 2, gScreenHeight / 2);
    while(!gQuit)
    {
        Input();

        PreDraw();

//        Draw();
        
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
    
    // setup the geometry
    VertexSpecification();

    // crrate the graphics pipelines
    // MINIMUM: setting vertex and fragment shaders
    CreateGraphicsPipeline();

    // drawing;
    MainLoop();

    CleanUp();
    return 0;
    
}
