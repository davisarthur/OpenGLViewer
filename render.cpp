// Based on templates from learnopengl.com
#include "helperFunctions.h"
#include <GL/glew.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "glm/ext.hpp"
#include <math.h>
#include <iostream>
#include <fstream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void readZBuffer(GLfloat* zBufferData, int& zBufferSaveCount);

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

    string shadingCode;
    cout << "\nEnter 'p' for Phong shading or 'g' for Gouraud shading: ";
    cin >> shadingCode;

    string vertexShaderSourceString;
    string fragmentShaderSourceString;
    if (shadingCode == "p") {
        vertexShaderSourceString = readFile("phong.vs");
        fragmentShaderSourceString = readFile("phong.fs");
    }
    else if (shadingCode == "g") {
        vertexShaderSourceString = readFile("gouraud.vs");
        fragmentShaderSourceString = readFile("gouraud.fs");
    }
    else {
        cout << "Invalid shading option! Defaulting to Phong shading." << endl;
        vertexShaderSourceString = readFile("phong.vs");
        fragmentShaderSourceString = readFile("phong.fs");
    }
    char* vertexShaderSource = &vertexShaderSourceString[0];
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

    // model initialization
    string modelName;
    cout << "\nModel Options: 'pawn', 'head', 'rose', 'soccerball', 'F16'" << endl;
    cout << "\nEnter the name of the model you would like to render: ";
    cin >> modelName;

    DirectionalLight lightSource;
    SceneObject model;
    glm::vec3 eye;
    float znear;
    float zfar;
    if (modelName == "pawn") {
        loadPawn(model, eye, znear, zfar, lightSource);
    }
    else if (modelName == "porsche") {
        loadPorsche(model, eye, znear, zfar, lightSource);
    }
    else if (modelName == "head") {
        loadHead(model, eye, znear, zfar, lightSource);
    }
    else if (modelName == "rose") {
        loadRose(model, eye, znear, zfar, lightSource);
    }
    else if (modelName == "soccerball") {
        loadSoccerBall(model, eye, znear, zfar, lightSource);
    }
    else if (modelName == "F16") {
        loadF16(model, eye, znear, zfar, lightSource);
    }
    else {
        cout << "Invalid model name! Defaulting to head model." << endl;
        loadHead(model, eye, znear, zfar, lightSource);
    }

    // build camera matrix
    glm::vec3 center = glm::vec3(0.0, 0.0, 0.0);
    glm::vec3 up = glm::vec3(0.0, 1.0, 0.0);
    glm::mat4 lookAt = glm::lookAt(eye, center, up);

    // build projection matrix
    float fov = 60.0f * M_PI / 180.0f;
    float aspect = (float) SCR_WIDTH / SCR_HEIGHT;
    glm::mat4 projMatrix = glm::perspective(fov, aspect, znear, zfar);
    glm::mat4 transformMatrix = projMatrix * lookAt;

    GLint modelMatID = glGetUniformLocation(shaderProgram, "modelMatrix");
    GLint transformMatID = glGetUniformLocation(shaderProgram, "transformMatrix");
    GLint intensityID = glGetUniformLocation(shaderProgram, "intensity");
    GLint lightDirID = glGetUniformLocation(shaderProgram, "lightDir");
    GLint eyeID = glGetUniformLocation(shaderProgram, "eye");

    lightSource.dir = glm::normalize(lightSource.dir);
    
    glUniform1f(intensityID, lightSource.intensity);
    glUniform3f(lightDirID, lightSource.dir.x, lightSource.dir.y, lightSource.dir.z);
    glUniform3f(eyeID, eye.x, eye.y, eye.z);
    glUniformMatrix4fv(transformMatID, 1, GL_FALSE, glm::value_ptr(transformMatrix));
    glUniformMatrix4fv(modelMatID, 1, GL_FALSE, glm::value_ptr(model.modelMat));

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, model.numBytes, model.triangles.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, model.vertexSize, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, model.vertexSize, (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, model.vertexSize, (void*)(sizeof(float) * 6));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, model.vertexSize, (void*)(sizeof(float) * 9));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, model.vertexSize, (void*)(sizeof(float) * 12));
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, model.vertexSize, (void*)(sizeof(float) * 15));
    glEnableVertexAttribArray(5);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0); 


    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // use z-buffer
    int zBufferSaveCount = 0;
    glEnable(GL_DEPTH_TEST);
    GLfloat zBufferData[SCR_HEIGHT * SCR_WIDTH];

    bool reading = false;

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window)) {
        // clear z buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        glUseProgram(shaderProgram);
        glUniform1f(intensityID, lightSource.intensity);
        glUniform3f(lightDirID, lightSource.dir.x, lightSource.dir.y, lightSource.dir.z);
        glUniformMatrix4fv(transformMatID, 1, GL_FALSE, glm::value_ptr(transformMatrix));
        glUniformMatrix4fv(modelMatID, 1, GL_FALSE, glm::value_ptr(model.modelMat));
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawArrays(GL_TRIANGLES, 0, model.triangles.size() * 3);
        // glBindVertexArray(0); // no need to unbind it every time 
 
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();

        // respond to user input (based off of example: https://www.glfw.org/docs/3.3/input_guide.html#input_keyboard)
        if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
            model.updateModelMat(glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.01, 1.01, 1.01), glm::vec3(0.0, 0.0, 0.0));
        }
        else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            model.updateModelMat(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.99, 0.99, 0.99), glm::vec3(0.0, 0.0, 0.0));
        }
        else if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
            model.updateModelMat(glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0), glm::vec3(0.02, 0.0, 0.0));
        }
        else if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
            model.updateModelMat(glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0), glm::vec3(0.0, 0.02, 0.0));
        }
        else if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
            model.updateModelMat(glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0), glm::vec3(0.0, 0.0, 0.02));
        }
        else if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
            if (!reading) {
                readZBuffer(&zBufferData[0], zBufferSaveCount);
                reading = true;
            }
        }
        else if (glfwGetKey(window, GLFW_KEY_P) == GLFW_RELEASE) {
            reading = false;
        }
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

void readZBuffer(GLfloat* zBufferData, int& zBufferSaveCount) {
    glReadPixels(SCR_WIDTH / 2, SCR_HEIGHT / 2, SCR_WIDTH, SCR_HEIGHT, GL_DEPTH_COMPONENT, GL_FLOAT, zBufferData);
    string baseName = "ZBufferSaves/save";
    string fileExt = ".txt";
    ofstream myfile(baseName + to_string(zBufferSaveCount) + fileExt);
    zBufferSaveCount++;
    if (myfile.is_open()) {
        for (int i = 0; i < SCR_WIDTH * SCR_HEIGHT; i++) {
            int x = i % SCR_WIDTH;
            if (i > 0 && x == 0) {
                myfile << endl;
            }
            myfile << zBufferData[i] << ", ";
        }
        myfile.close();
    }
    else cout << "Unable to open file";
}