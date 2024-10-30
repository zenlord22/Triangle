#include "include/glad/glad.h"
//#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

// Vertex Shader Source Code
const char* vertexShaderSource =
"#version 330 core"
"layout (location = 0) in vec3 aPos;"
"uniform float angle;"
"void main()"
"gl_Position = vec4(rotation * aPos, 1.0);"
"}\0";

// Fragment Shader Source Code
const char* fragmentShaderSource =
"#version 330 core"
"out vec4 FragColor;"
"void main()"
"{"
"FragColor = vec4(1.0, 0.5, 0.2, 1.0);"
"}\0";


void processInput(GLFWwindow *window, float &angle, bool &rotateLeft, bool &rotateRight) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        rotateLeft = true;
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        rotateRight = true;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

unsigned int compileShader(unsigned int type, const char* source) {
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    return shader;
}
unsigned int createProgram(const char* vertexSource, const char* fragmentSource) {
    unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
    unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    int success; char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return shaderProgram;
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl; return -1;
    }
    
    GLFWwindow* window = glfwCreateWindow(800, 600, "Rotating Triangle", NULL, NULL); if (!window) {
        glfwTerminate(); return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    // Build and compile our shader program
    unsigned int shaderProgram = createProgram(vertexShaderSource, fragmentShaderSource);
    
    // Set up vertex data
    float vertices[] = { 0.0f, 0.5f, 0.0f, -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f };
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    float angle = 0.0f;
    bool rotateLeft = false, rotateRight = false;

    while (!glfwWindowShouldClose(window)) {
        processInput(window, angle, rotateLeft, rotateRight);
        if (rotateLeft) {
            angle += 90.0f * (3.14159f / 180.0f); // 90 degrees in radians
            rotateLeft = false;
        }
        if (rotateRight) {
            angle -= 90.0f * (3.14159f / 180.0f); // 90 degrees in radians
            rotateRight = false;
        }
        
        float cosAngle = cos(angle);
        float sinAngle = sin(angle);
        float rotationMatrix[9] = {
            cosAngle, sinAngle, 0.0f, -sinAngle, cosAngle, 0.0f, 0.0f, 0.0f, 1.0f
        };
        
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
        int vertexRotationLocation = glGetUniformLocation(shaderProgram, "rotation");
        glUniformMatrix3fv(vertexRotationLocation, 1, GL_FALSE, rotationMatrix);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}
