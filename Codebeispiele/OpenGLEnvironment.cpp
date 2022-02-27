// OpenGLEnvironment.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>

#include "Matrix.h"

struct EngineData
{
    bool running = true;
    int width, height;
};

static EngineData* data = new EngineData();
static GLFWwindow* window = nullptr;

//Vertex shader program
const char* vsSource = R"(

#version 430 core
layout (location = 0) in vec3 v_position;
layout (location = 1) in vec2 v_texCoords;

uniform mat4 u_transformation;
uniform mat4 u_viewProjection;

void main()
{
    gl_Position = u_viewProjection * u_transformation * vec4(v_position, 1.0);
} 
)";

//Fragment shader program
const char* fsSource = R"(

#version 430 core

out vec4 out_color;

uniform vec4 u_color;

void main()
{
   out_color = u_color;
}

)";

int main()
{
    glfwInit();

    glfwWindowHint(GLFW_SAMPLES, 16);

    data->width = 1000;
    data->height = 1000;

    //Create window with glfw
    window = glfwCreateWindow(data->width, data->height, "Test Window", NULL, NULL);

    glfwMakeContextCurrent(window);

    //Set data which can be accessed by callbacks
    glfwSetWindowUserPointer(window, data);

    //Create close button
    glfwSetWindowCloseCallback(window, [](GLFWwindow* window)
    {
        EngineData* data = (EngineData*)glfwGetWindowUserPointer(window);

        data->running = false;
    });

    glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height)
    {
        EngineData* data = (EngineData*)glfwGetWindowUserPointer(window);

        data->width = width;
        data->height = height;
        glViewport(0, 0, width, height);
    });

    //Load opengl
    int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    
    if (!status) std::cout << "Error while initializing glad!" << std::endl;

    //Cube vertices
    float vertices[] = {
    -1.0f,-1.0f,-1.0f, 1.0f, 0.0f,
    -1.0f,-1.0f, 1.0f, 0.0f, 0.0f,
    -1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

    1.0f, 1.0f,-1.0f, 1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f, 0.0f, 0.0f,
    -1.0f, 1.0f,-1.0f, 0.0f, 1.0f,

    1.0f,-1.0f, 1.0f, 1.0f, 0.0f,
    -1.0f,-1.0f,-1.0f, 0.0f, 1.0f,
    1.0f,-1.0f,-1.0f, 1.0f, 1.0f,

    1.0f, 1.0f,-1.0f, 1.0f, 1.0f,
    1.0f,-1.0f,-1.0f, 1.0f, 0.0f,
    -1.0f,-1.0f,-1.0f, 0.0f, 0.0f,

    -1.0f,-1.0f,-1.0f, 1.0f, 0.0f,
    -1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    -1.0f, 1.0f,-1.0f, 1.0f, 1.0f,

    1.0f,-1.0f, 1.0f, 1.0f, 0.0f,
    -1.0f,-1.0f, 1.0f, 0.0f, 0.0f,
    -1.0f,-1.0f,-1.0f, 0.0f, 1.0f,

    -1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f, 1.0f, 0.0f,
    1.0f,-1.0f, 1.0f, 0.0f, 0.0f,

    1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    1.0f,-1.0f,-1.0f, 0.0f, 0.0f,
    1.0f, 1.0f,-1.0f, 0.0f, 1.0f,

    1.0f,-1.0f,-1.0f, 0.0f, 0.0f,
    1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f, 1.0f, 0.0f,

    1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    1.0f, 1.0f,-1.0f, 1.0f, 0.0f,
    -1.0f, 1.0f,-1.0f, 0.0f, 0.0f,

    1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f, 0.0f, 0.0f,
    -1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

    1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    -1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f, 0.0f, 0.0f };

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);

    //Create Buffers and load the vertices in it
    uint32_t vao; glGenVertexArrays(1, &vao);
    uint32_t vbo; glGenBuffers(1, &vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //Set the layout of the buffer to 3 floats per vertex  - index(0), parts(3 because 3 floats), type(float), norm(false), stride(size of the whole vertex), offset(from begin vertex to attrib)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //Compile vertex shader
    uint32_t vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vsSource, NULL);
    glCompileShader(vs);

    int success;
    char info[1024];
    glGetShaderiv(vs, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(vs, 1024, NULL, info);
        std::cerr << "Error: " << info << std::endl;
    }

    //Compile fragment shader
    uint32_t fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fsSource, NULL);
    glCompileShader(fs);

    glGetShaderiv(fs, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(fs, 1024, NULL, info);
        std::cerr << "Error: " << info << std::endl;
    }

    //Compile to program
    uint32_t prg = glCreateProgram();
    glAttachShader(prg, vs);
    glAttachShader(prg, fs);
    glLinkProgram(prg);
    glUseProgram(prg);

    //Texture loading
    unsigned int texture;

    int width, height, channels;
    stbi_set_flip_vertically_on_load(1);
    stbi_uc* img = stbi_load("back.TGA", &width, &height, &channels, 0);

    if (!img)
    {
        std::cerr << "Texture load failed" << std::endl;
    }

    GLenum internalFormat = 0, dataFormat = 0;

    if (channels == 4)
    {
        internalFormat = GL_RGBA8;
        dataFormat = GL_RGBA;
    }
    else if (channels == 3)
    {
        internalFormat = GL_RGB8;
        dataFormat = GL_RGB;
    }

    glCreateTextures(GL_TEXTURE_2D, 1, &texture);
    glTextureStorage2D(texture, 1, internalFormat, width, height);

    glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTextureSubImage2D(texture, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, img);
    
    stbi_image_free(img);

    std::cout << "Texture loaded" << std::endl;

    glUniform4f(glGetUniformLocation(prg, "u_color"), 0.0f, 0.0f, 1.0f, 1.0f);
   
    float rotation = 0.0f;

    Vector3f cameraPosition(0.0f, 0.0f, 0.0f);
  
    while (data->running)
    {
        //Set background to green
        glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Set shader
        glUseProgram(prg);

        //Move camera when key is pressed
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            cameraPosition.z -= 0.1f;
        }

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            cameraPosition.x += 0.1f;
        }

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            cameraPosition.z += 0.1f;
        }

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            cameraPosition.x -= 0.1f;
        }

        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        {
            cameraPosition.y -= 0.1f;
        }

        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        {
            cameraPosition.y += 0.1f;
        }

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            data->running = false;
        }

        //Create camera matrix and cube matrix
        Matrix4f transform = rotate(Vector3f(1.0f, 0.0f, 0.0f), rotation * 0.5f) * translate(Vector3f(0.0f, 1.0f, 5.0f)) * scale(Vector3f(0.5f, 0.75f, 0.5f));
        //Create camera matrix and cube matrix
        Matrix4f transform1 = rotate(Vector3f(0.0f, 1.0f, 0.0f), rotation * 0.75f) * translate(Vector3f(4.0f, 0.0f, 5.0f)) * scale(Vector3f(0.5f, 0.5f, 0.5f));
        //Create camera matrix and cube matrix
        Matrix4f transform2 = rotate(Vector3f(0.0f, 0.0f, 1.0f), rotation * 1.0f) * translate(Vector3f(8.0f, -1.0f, 5.0f)) * scale(Vector3f(0.5f, 0.25f, 0.25f));

        Matrix4f viewProjection = translate(cameraPosition) * perspective(data->width, data->height, to_radian(70), 1000.0f, 0.1f);

       // rotation += 0.05f;

        //Set texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        //Load matrixes into the shader
        glUniformMatrix4fv(glGetUniformLocation(prg, "u_transformation"), 1, false, transform.values);
        glUniformMatrix4fv(glGetUniformLocation(prg, "u_viewProjection"), 1, false, viewProjection.values);
        //Just for safety, technically it is already bound
        glBindVertexArray(vao);

        //Draw the cube
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //Load matrixes into the shader
        glUniformMatrix4fv(glGetUniformLocation(prg, "u_transformation"), 1, false, transform1.values);

        //Draw the cube
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //Load matrixes into the shader
        glUniformMatrix4fv(glGetUniformLocation(prg, "u_transformation"), 1, false, transform2.values);

        //Draw the cube
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //Show the buffer
        glfwPollEvents();


        glfwSwapBuffers(window);
    }

    //Destroy window
    glfwDestroyWindow(window);
    glfwTerminate();

    delete data;
}