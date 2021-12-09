//=============================================================================
//
//   Exercise code for the lecture "Computer Graphics"
//     by Prof. Mario Botsch, TU Dortmund
//
//   Copyright (C) by Computer Graphics Group, TU Dortmund
//
//=============================================================================
#include "glfw_window.h"
#include <iostream>
//=============================================================================


GLFW_window *GLFW_window::instance__ = NULL;


//-----------------------------------------------------------------------------


GLFW_window::GLFW_window(const char* _title, int _width, int _height)
{
    // initialize glfw window
    if (!glfwInit())
    {
        std::cerr << "Cannot initialize GLFW!\n";
        exit(EXIT_FAILURE);
    }


    // request core profile and OpenGL version 3.2
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);


    // try to create window
    window_ = glfwCreateWindow(_width, _height, _title, NULL, NULL);
    if (!window_)
    {
        glfwTerminate();
        std::cerr << "Window creation failed!\n";
        exit(EXIT_FAILURE);
    }


    // make this window the current one
    glfwMakeContextCurrent(window_);


    // enable vsync
    glfwSwapInterval(1);


    // register glfw callbacks
    glfwSetKeyCallback(window_, keyboard__);
    glfwSetMouseButtonCallback(window_, mouse__);
    glfwSetFramebufferSizeCallback(window_, resize__);
    glfwSetCursorPosCallback(window_, motion__);
    glfwSetScrollCallback(window_, scroll__);


    // now that we have a GL context, initialize GLEW
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if(err != GLEW_OK)
    {
        std::cerr << "Error initializing GLEW: " << glewGetErrorString(err) << std::endl;
        exit(1);
    }

    //define blending function
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // debug: print GL and GLSL version
    std::cout << "GLEW   " << glewGetString(GLEW_VERSION) << std::endl;
    std::cout << "GL     " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL   " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;


    // call glGetError once to clear error queue
    GLenum error = glGetError();


    instance__ = this;
}


//-----------------------------------------------------------------------------


GLFW_window::~GLFW_window()
{
    glfwTerminate();
}


//-----------------------------------------------------------------------------


int GLFW_window::run()
{
    // initialize OpenGL
    initialize();

    // query framebuffer width and height
    // call resize to initialize viewport
    int width, height;
    glfwGetFramebufferSize(window_, &width, &height);
    resize(width, height);

    // now run the event loop
    while (!glfwWindowShouldClose(window_))
    {
        // call timer function
        timer();

        // draw scene
        render();

        // swap buffers
        glfwSwapBuffers(window_);

        // handle events
        glfwPollEvents();
    }

    glfwDestroyWindow(window_);

    return EXIT_SUCCESS;
}


//-----------------------------------------------------------------------------


void GLFW_window::error__(int error, const char *description) 
{
    fputs(description, stderr);
}


void GLFW_window::keyboard__(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    instance__->keyboard(key, scancode, action, mods);
}

void GLFW_window::mouse__(GLFWwindow *window, int button, int action, int mods)
{
    instance__->mouse(button, action, mods);
}


void GLFW_window::motion__(GLFWwindow* window, double xpos, double ypos)
{
    instance__->motion(xpos, ypos);
}

void GLFW_window::scroll__(GLFWwindow* window, double xoffset, double yoffset)
{
    instance__->scroll(xoffset, yoffset);
}

bool GLFW_window::map_to_sphere(const int point_x, const int point_y, vec3& result)
{
    if ((point_x >= 0) && (point_x <= width_) && (point_y >= 0) &&
        (point_y <= height_))
    {
        double w = width_;
        double h = height_;
        double x = (double)(point_x - 0.5*w) / w;
        double y = (double)(0.5*h - point_y) / h;
        double sinx = sin(M_PI * x * 0.5);
        double siny = sin(M_PI * y * 0.5);
        double sinx2siny2 = sinx * sinx + siny * siny;

        result[0] = sinx;
        result[1] = siny;
        result[2] = sinx2siny2 < 1.0 ? sqrt(1.0 - sinx2siny2) : 0.0;

        return true;
    }
    else
        return false;
}

void GLFW_window::resize__(GLFWwindow* window, int width, int height)
{
    instance__->resize(width, height);
}


//=============================================================================
