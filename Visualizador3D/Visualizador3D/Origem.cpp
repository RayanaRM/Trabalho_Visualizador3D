#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "model.h"
#include "objeto.h"

#include <iostream>
#include <vector>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
string img_name;


void setObject(Shader shader, Objeto objeto, Model modelo);

vector <Objeto> objetos;

// camera
Camera camera(glm::vec3(20.0f, 0.0f, 15.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// selection
bool isSelected1 = false;
bool isSelected2 = false;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Visualizador 3D", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // metodos de callback
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    stbi_set_flip_vertically_on_load(false);

    glEnable(GL_DEPTH_TEST);

    Shader ourShader("shaders/shader_model.vs", "shaders/shader_model.fs");
    Shader selectedShader("shaders/shader_model.vs", "shaders/selected_shader_model.fs");

    Shader currentShader = ourShader;
    objetos.push_back(Objeto(1, true));
    objetos.push_back(Objeto(2, false));

    Model modelo1("../modelos/Pokemon/Pikachu.obj");
    Model modelo2("../modelos/Pokemon/PikachuF.obj");

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.933f, 0.933f, 0.929f, 1.0f); //cor de fundo
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        currentShader = (objetos[0].isSelected) ? selectedShader : ourShader;
        currentShader.use();
        setObject(currentShader, objetos[0], modelo1);

        currentShader = (objetos[1].isSelected) ? selectedShader : ourShader;
        currentShader.use();
        setObject(currentShader, objetos[1], modelo2);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    Objeto objeto = objetos[0];
    for (unsigned int i = 0; i < objetos.size(); i++){
        objeto = objetos[i].isSelected ? objetos[i] : objetos[0];
    }
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // movimenta camera
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime, objeto);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime, objeto);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime, objeto);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime, objeto);

    // altera entre translacao e rotacao
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        camera.ProcessKeyboard(ROTACAO, deltaTime, objeto);
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
        camera.ProcessKeyboard(TRANSLACAO, deltaTime, objeto);

    // incrementa ou decrementa movimento de translacao
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
        camera.ProcessKeyboard(I, deltaTime, objeto);
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
        camera.ProcessKeyboard(U, deltaTime, objeto);

    // eixos
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        camera.ProcessKeyboard(X, deltaTime, objeto);
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
        camera.ProcessKeyboard(Y, deltaTime, objeto);
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
        camera.ProcessKeyboard(Z, deltaTime, objeto);

    // escala
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.ProcessKeyboard(ESCALA, deltaTime, objeto);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void setObject(Shader shader, Objeto objeto, Model modelo)
{
        // matrizes view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        shader.setMat4("projection", projection);
        GLint projectionLoc = glGetUniformLocation(shader.ID, "projection");
        glUniformMatrix4fv(projectionLoc, 1, false, glm::value_ptr(projection));

        glm::mat4 view = camera.GetViewMatrix(objeto.id);
        shader.setMat4("view", view);
        GLint viewLoc = glGetUniformLocation(shader.ID, "view");
        glUniformMatrix4fv(viewLoc, 1, false, glm::value_ptr(view));

        // model
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = camera.GetModelMatrix(modelMatrix, objeto.id);
        shader.setMat4("model", modelMatrix);
        GLint modelLoc = glGetUniformLocation(shader.ID, "model");
        glUniformMatrix4fv(modelLoc, 1, false, glm::value_ptr(modelMatrix));
        modelo.Draw(shader);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
    {
        for(unsigned int i = 0; i < objetos.size(); i++){
            if((i+1) < objetos.size()){
                objetos[i].isSelected = false;
                objetos[i+1].isSelected = true;
            }
        }
    }

    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
    {
        for(unsigned int i = objetos.size()-1; i > 0; i--){
            if((i-1) >= 0){
                objetos[i].isSelected = false;
                objetos[i-1].isSelected = true;
            }
        }
    }
}

