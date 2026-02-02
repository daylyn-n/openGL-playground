#include "include/glad/glad.h"
#include <SDL2/SDL.h>
#include <iostream>
//Globals

int gScreenHeight = 480;
int gScreenWidth = 640;

SDL_Window* gGraphicsApplicationWindow = nullptr;
SDL_GLContext gOpenGLContext = nullptr;
 
bool gQuit = false;

void GetOpenGLVersionInfo()
{
    std::cout << "Vendor:" << glGetString(GL_VENDOR)  << std::endl;

    std::cout << "Renderer:" << glGetString(GL_RENDERER)  << std::endl;

    std::cout << "Version:" << glGetString(GL_VERSION)  << std::endl;

    std::cout << "Shading Language:" <<
    glGetString(GL_SHADING_LANGUAGE_VERSION)  << std::endl;


}
void InitializeProgram()
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL2 COUNT NOT INITILAIZE VIDEO SUB SYSTEM";
        exit(1);
    }
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

    gOpenGLContext = SDL_GL_CreateContext(gGraphicsApplicationWindow);

    if(gOpenGLContext == nullptr)
    {
        std::cout << "OpenGL context not available"  << std::endl;
        exit(1);
    }

    // initialize glad lib
    if(!gladLoadGLLoader(SDL_GL_GetProcAddress))
    {
        std::cout << "Glad was not initiliazed"  << std::endl;
        exit(1);
    }
                                    
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

}

void Draw()
{

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
   
    InitializeProgram();

    MainLoop();

    CleanUp();
    return 0;
    
}
