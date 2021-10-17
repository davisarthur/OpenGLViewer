// Based on templates from learnopengl.com
#include "helperFunctions.h"
#include <GL/glew.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "glm/ext.hpp"

#include <iostream>
#include <math.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main() {
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // // glew: load all OpenGL function pointers
    glewInit();

    int mode;
    cout << "Shader codes\n"
        "0 - Default shader (white)\n"
        "1 - Custom color\n"
        "2 - Switch x and y coordinates\n"
        "3 - Scale x position by 0.5 and y position by 1.5\n"
        "4 - Color based on normalized position (RGB = (x, y, z))\n"
        "Enter the integer code of the shader you would like to use: ";
    cin >> mode;
    bool wireframe;
    cout << "Enter 1 to render in wireframe mode. Otherwise enter 0: ";
    cin >> wireframe;

    string vertexShaderFileName;
    string fragmentShaderFileName;
    if (mode == 0) {
        vertexShaderFileName = "source.vs";
        fragmentShaderFileName = "source.fs";
    }
    else if (mode == 1) {
        vertexShaderFileName = "source.vs";
        fragmentShaderFileName = "customColor.fs";
    }
    else if (mode == 2) {
        vertexShaderFileName = "switchCoords.vs";
        fragmentShaderFileName = "source.fs";
    }
    else if (mode == 3) {
        vertexShaderFileName = "scale.vs";
        fragmentShaderFileName = "source.fs";
    }
    else if (mode == 4) {
        vertexShaderFileName = "colorCoords.vs";
        fragmentShaderFileName = "colorCoords.fs";
    }
    else {
        cout << "Invalid code!";
    }

    // read vertex shader
    string vertexShaderSourceString = readFile(vertexShaderFileName);
    char* vertexShaderSource = &vertexShaderSourceString[0];

    // read fragment shader
    string fragmentShaderSourceString = readFile(fragmentShaderFileName);
    char* fragmentShaderSource = &fragmentShaderSourceString[0];

    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    vector<Triangle> triangles = readVertexData("data/dolphins.obj");

    GLint colorID;
    float red;
    float green;
    float blue;
    if (mode == 1) {
        cout << "Define the RGB color to apply to each vertex in the mesh.";
        cout << "\nInput the value of the red channel (0 to 1): ";
        cin >> red;
        cout << "Input the value of the green channel (0 to 1): ";
        cin >> green;
        cout << "Input the value of the blue channel (0 to 1): ";
        cin >> blue;

        colorID = glGetUniformLocation(shaderProgram, "vcolor");
        glUniform3f(colorID, red, green, blue);
    }
    

    int numBytes = triangles.size() * sizeof(triangles[0]);
    int vertexSize = sizeof(triangles[0].vertex1);
    glm::mat4 lookAt = glm::lookAt(glm::vec3(0.0, 0.0, 50.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
    glm::mat4 projMatrix = glm::ortho(-500.0f, 500.0f, -500.0f, 500.0f, -100.0f, 100.0f);
    glm::mat4 transformMatrix = projMatrix * lookAt;

    GLint pMatID = glGetUniformLocation(shaderProgram, "transformMatrix");
    glUniformMatrix4fv(pMatID, 1, GL_FALSE, glm::value_ptr(transformMatrix));

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, numBytes, triangles.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    // uncomment this call to draw in wireframe polygons.
    if (wireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window)) {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        glUseProgram(shaderProgram);
        glUniformMatrix4fv(pMatID, 1, GL_FALSE, glm::value_ptr(transformMatrix));
        if (mode == 1) {
            glUniform3f(colorID, red, green, blue);
        }
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawArrays(GL_TRIANGLES, 0, triangles.size() * 3);
        // glBindVertexArray(0); // no need to unbind it every time 
 
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}