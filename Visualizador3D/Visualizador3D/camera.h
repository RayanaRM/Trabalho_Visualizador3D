#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum KEY_INPUTS {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    X,
    Y,
    Z,
    TRANSLACAO,
    ROTACAO,
    I,
    U,
    ESCALA
};

// Default para camera
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 20.0f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;


class Camera
{
public:
    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    glm::vec3 axis = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 positionTranslate = glm::vec3(0.0f, 0.0f, 0.0f);
    float angle = 1.0f;

    // euler Angles
    float Yaw;
    float Pitch;

    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    // variaveis de controle
    float escala = 0.1f;
    float type;
    float inc;

    // constructor with vectors
    Camera(glm::vec3 position, glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }

    glm::mat4 GetModelMatrix(glm::mat4 model)
    {
        model = glm::rotate(model, angle, axis);
        model = glm::translate(model, positionTranslate);
        model = glm::scale(model, glm::vec3(escala, escala, escala));
        return model;
    }

    void ProcessKeyboard(KEY_INPUTS key, float deltaTime)
    {
        float velocity = MovementSpeed * deltaTime;

        // Teclas de controle (rotaçâo/ translação)
        if (key == TRANSLACAO) {
            type = 1;
        }
        if (key == ROTACAO) {
            type = 2;
        }

        // Teclas de controle (incrementa/ decrementa)
        if (key == I) {
            inc = 1;
        }
        if (key == U) {
            inc = 0;
        }

        // movimentacao da camera
        if (key == FORWARD)
            Position += Front * velocity;
        if (key == BACKWARD)
            Position -= Front * velocity;
        if (key == LEFT)
            Position -= Right * velocity;
        if (key == RIGHT)
            Position += Right * velocity;

        // Rotação
        if (key == X  && type == 2) {
            angle = (GLfloat)glfwGetTime();
            axis = glm::vec3(1.0f, 0.0f, 0.0f);
        }
        if (key == Y && type == 2) {
            angle = (GLfloat)glfwGetTime();
            axis = glm::vec3(0.0f, 1.0f, 0.0f);
        }
        if (key == Z && type == 2) {
            angle = (GLfloat)glfwGetTime();
            axis = glm::vec3(0.0f, 0.0f, 1.0f);
        }

        // Translação
        if (key == X && type == 1) {
            positionTranslate[0] += inc == 1 ? 0.01 : -0.01;
        }
        if (key == Y && type == 1) {
            positionTranslate[1] += inc == 1 ? 0.01 : -0.01;
        }
        if (key == Z && type == 1) {
            positionTranslate[2] += inc == 1 ? 0.01 : -0.01;
        }

        // escala
        if (key == ESCALA) {
            escala += inc == 1 ? 0.0005f : -0.0005f;
            if (escala > 1.0f)  
                escala = 1.0f;
            else if (escala < 0.01f)
                escala = 0.01f;
        }
    }

    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;

        // faz update nos vetores Front, Right e Up
        updateCameraVectors();
    }

    // controle de zoom com scroll
    void ProcessMouseScroll(float yoffset)
    {
        Zoom -= (float)yoffset;
        if (Zoom < 1.0f)
            Zoom = 1.0f;
        if (Zoom > 45.0f)
            Zoom = 45.0f;
    }

private:
    void updateCameraVectors()
    {
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        Right = glm::normalize(glm::cross(Front, WorldUp));
        Up = glm::normalize(glm::cross(Right, Front));
    }
};
#endif