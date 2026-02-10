#include "../include/glad/glad.h"
#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <fstream>
//Globals

int gScreenHeight = 480;
int gScreenWidth = 640;

SDL_Window* gGraphicsApplicationWindow = nullptr;
SDL_GLContext gOpenGLContext = nullptr;
 
bool gQuit = false;

GLuint gVAO = 0;
GLuint gVBO = 0;



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
    const std::vector<GLfloat> vertexPos
    {
        //x     y       z
        -0.8f, -0.8f, 0.0f,
        0.8f, -0.8f, 0.0f,
        0.0f, 0.8f, 0.0f,
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
            vertexPos.size() * sizeof(GLfloat),
            vertexPos.data(), 
            GL_STATIC_DRAW);

    // basically just defining how much data we point to and which attributes we look at when we
    // jump from vertex to vertex
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,
                        3,
                        GL_FLOAT,
                        GL_FALSE,
                        0,
                        (void*)0
                        );

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
    SDL_Event e;
    while(SDL_PollEvent(&e) != 0)
    {
        if(e.type == SDL_QUIT)
        {
            std::cout << "Cya" << std::endl;
            gQuit = true;
        }
    }
}

void PreDraw()
{
    glDisable(GL_DEPTH_TEST);

    glDisable(GL_CULL_FACE);
    
    glViewport(0, 0, gScreenWidth, gScreenHeight);
    glClearColor(1.f, 1.f, 0.0f, 1.f);

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glUseProgram(gGraphicsPipelineShaderProgram);

}

void Draw()
{
    glBindVertexArray(gVAO);
    glBindBuffer(GL_ARRAY_BUFFER, gVBO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

}


void MainLoop()
{
    while(!gQuit)
    {
        Input();

        PreDraw();

        Draw();
        
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
