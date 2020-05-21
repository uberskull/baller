

#include "SDL.h"
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#include <iostream>
#include <string>
#include <memory>

GLuint programObject;

class Graphics
{
private:
    
    SDL_Window* _window;
    
public:
    Graphics(SDL_Window* window)
    {
        _window = window;
    }
    
    void update()
    {
        glClearColor(255.0f, 0.0f, 255.0f, 1);
        
        GLfloat vVertices[] =
        {
            0.0f, 1.0f, 0.0f,
            -1.0f, -1.0f, 0.0f,
            1.0f, -1.0f, 0.0f
        };
        
        glViewport (0, 0, 320, 480);
        
        glColorMask (GL_TRUE, GL_TRUE, GL_TRUE, GL_FALSE);
        glClear (GL_COLOR_BUFFER_BIT);
        
        glUseProgram (programObject);
        
        glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, vVertices);
        
        glEnableVertexAttribArray (0);
        glDrawArrays (GL_TRIANGLES, 0, 3);

        SDL_GL_SwapWindow(_window);
    }
};

void UpdateFrame(void* param)
{
    Graphics* graphics = (Graphics*)param;
    graphics->update();
}

void check_gl_error()
{
    GLenum err (glGetError());
    
    while(err != GL_NO_ERROR)
    {
        char error[40];
        
        switch(err)
        {
            case GL_INVALID_OPERATION:
                strncpy(error, "INVALID_OPERATION", sizeof("INVALID_OPERATION"));
            break;
            case GL_INVALID_ENUM:
                strncpy(error, "INVALID_ENUM", sizeof("INVALID_ENUM"));
            break;
            case GL_INVALID_VALUE:
                strncpy(error, "INVALID_VALUE", sizeof("INVALID_VALUE"));
            break;
            case GL_OUT_OF_MEMORY:
                strncpy(error, "OUT_OF_MEMORY", sizeof("OUT_OF_MEMORY"));
            break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                strncpy(error, "INVALID_FRAMEBUFFER_OPERATION", sizeof("INVALID_FRAMEBUFFER_OPERATION"));
            break;
        }
        
        SDL_Log("Error: %s", error);
        err = glGetError();
    }
}///
// Create a shader object, load the shader source, and
// compile the shader.
//
GLuint LoadShader(GLenum type, const GLchar *shaderSrc)
{
    GLuint shader;
    GLint compiled;
    // Create the shader object
    check_gl_error();

    shader = glCreateShader(type);
    check_gl_error();

    if(shader == 0)
    {
        SDL_Log("Could not create OpenGL shader\n");
        return 0;
    }
    
    // Load the shader source
    
    glShaderSource(shader, 1, &shaderSrc, NULL);
    
    // Compile the shader
    glCompileShader(shader);
    
    // Check the compile status
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    
    if(!compiled)
    {
        GLint infoLen = 0;
        
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
        
        if(infoLen > 1)
        {
            char* infoLog = (char*)malloc(sizeof(char) * infoLen);
            
            glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
            SDL_Log("Error compiling shader:\n%s\n", infoLog);
            free(infoLog);
        }
        
        glDeleteShader(shader);
        return 0;
    }
    
    return shader;
    
}

bool parseFileIntoString(const char *fileName, char *string, int maxLength)
{
    FILE *file = fopen(fileName, "r");
    
    if(!file)
    {
        SDL_Log("ERROR: opening file for reading: %s\n", fileName);
        return false;
    }
    size_t count = fread(string, 1, maxLength - 1, file);
    if((int)count >= maxLength - 1)
    {
        SDL_Log("WARNING: file %s too big - truncated.\n", fileName);
    }
    if(ferror(file))
    {
        SDL_Log("ERROR: reading shader file %s\n", fileName);
        fclose(file);
        return false;
    }
    // append \0 to end of file string
    string[count] = 0;
    fclose(file);
    return true;
}

///
// Initialize the shader and program object
//
int Init()
{
    char vertexShaderString[900];
    char fragmentShaderString[900];
    
    parseFileIntoString("vertexshader.glsl", vertexShaderString, sizeof(vertexShaderString));
    parseFileIntoString("fragmentshader.glsl", fragmentShaderString, sizeof(fragmentShaderString));
    
    GLuint vertexShader;
    GLuint fragmentShader;
    GLint linked;
    
    vertexShader = LoadShader (GL_VERTEX_SHADER, vertexShaderString);
    fragmentShader = LoadShader (GL_FRAGMENT_SHADER, fragmentShaderString);
    
    programObject = glCreateProgram();
    
    if(programObject == 0)
    {
        
        SDL_Log("Could not create OpenGL program");
        return 0;
        
    }
    
    glAttachShader(programObject, vertexShader);
    glAttachShader(programObject, fragmentShader);
    glBindAttribLocation (programObject, 0, "vPosition");
    glLinkProgram(programObject);
    
    glGetProgramiv(programObject, GL_LINK_STATUS, &linked);
    
    if(!linked)
    {
        GLint infoLen = 0;
        
        glGetProgramiv (programObject, GL_INFO_LOG_LENGTH, &infoLen);
        
        if (infoLen > 1)
        {
            
            char* infoLog = (char*) malloc (sizeof (char) * infoLen);
            glGetProgramInfoLog (programObject, infoLen, NULL, infoLog);
            SDL_Log("Error linking program: %s", infoLog);
            free (infoLog);
            
        }
        
        glDeleteProgram(programObject);
        return 0;
        
    }
    
    glClearColor (0.0f, 0.0f, 0.0f, 1.0f);
    return true;
}

int EventFilter(void* userdata, SDL_Event* event)
{
    switch(event->type)
    {
        case SDL_FINGERMOTION:
            SDL_Log("Finger Motion");
            return 0;
            
        case SDL_FINGERDOWN:
            SDL_Log("Finger Down");
            return 0;
            
        case SDL_FINGERUP:
            SDL_Log("Finger Up");
            return 0;
    }
    return 1;
}


int main(int argc, char** argv)
{

    // initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        SDL_Log("Could not initialize SDL\n");
        return 1;
    }
    
    SDL_DisplayMode displayMode;
    SDL_GetDesktopDisplayMode(0, &displayMode);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    // create window and renderer
    SDL_Window* window = SDL_CreateWindow(NULL, 0, 0, displayMode.w, displayMode.h, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN | SDL_WINDOW_RESIZABLE);
    
    if(!window)
    {
        SDL_Log("Could not initialize Window\n");
        return 1;
    }
    
    auto gl = SDL_GL_CreateContext(window);
    if (!Init())
    {
        SDL_Log("Error initializing OpenGL\n");
        return 1;
    }
    
    std::unique_ptr<Graphics> graphics = std::unique_ptr<Graphics>(new Graphics(window));
    SDL_iPhoneSetAnimationCallback(window, 1, UpdateFrame, graphics.get());
    SDL_AddEventWatch(EventFilter, NULL);
    
    //Game Loop
    SDL_Event event;
    auto done = false;
    while (!done)
    {
        SDL_PumpEvents();
        while (SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_QUIT:
                    done = true;
                    break;
                    
                case SDL_APP_DIDENTERFOREGROUND:
                    SDL_Log("SDL_APP_DIDENTERFOREGROUND");
                    break;
                    
                case SDL_APP_DIDENTERBACKGROUND:
                    SDL_Log("SDL_APP_DIDENTERBACKGROUND");
                    break;
                    
                case SDL_APP_LOWMEMORY:
                    SDL_Log("SDL_APP_LOWMEMORY");
                    break;
                    
                case SDL_APP_TERMINATING:
                    SDL_Log("SDL_APP_TERMINATING");
                    break;
                    
                case SDL_APP_WILLENTERBACKGROUND:
                    SDL_Log("SDL_APP_WILLENTERBACKGROUND");
                    break;
                    
                case SDL_APP_WILLENTERFOREGROUND:
                    SDL_Log("SDL_APP_WILLENTERFOREGROUND");
                    break;
                    
                case SDL_WINDOWEVENT:
                {
                    switch (event.window.event)
                    {
                        case SDL_WINDOWEVENT_RESIZED:
                        {
                            SDL_Log("Window %d resized to %dx%d", event.window.windowID, event.window.data1, event.window.data2);
                            
                            break;
                        }
                    }
                }
            }
        }
    }
    
    SDL_GL_DeleteContext(gl);
    
    // Done! Close the window, clean-up and exit the program.
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}
