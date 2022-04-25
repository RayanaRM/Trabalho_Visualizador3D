#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    X,
    Y,
    Z,
    T,
    R,
    I,
    U
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 20.0f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
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
    float angle = 1.0f;
    float type;
    float inc;
    glm::vec3 positionTranslate = glm::vec3(0.0f, 0.0f, 0.0f);

    // euler Angles
    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    // constructor with vectors
    Camera(glm::vec3 position, glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }
    // constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }

    glm::mat4 GetModelMatrix(glm::mat4 model)
    {
        model = glm::rotate(model, angle, axis);
        model = glm::translate(model, positionTranslate);
        return model;
    }

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement key, float deltaTime)
    {
        float velocity = MovementSpeed * deltaTime;

        // Teclas de controle (rotaçâo/ translação)
        if (key == T) {
            type = 1;
        }
        if (key == R) {
            type = 2;
        }

        // Teclas de controle (incrementa/ decrementa)
        if (key == I) {
            inc = 1;
        }
        if (key == U) {
            inc = 0;
        }

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

        // update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

    void ProcessMouseScroll(float yoffset)
    {
        Zoom -= (float)yoffset;
        if (Zoom < 1.0f)
            Zoom = 1.0f;
        if (Zoom > 45.0f)
            Zoom = 45.0f;
    }

private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {
        // calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        // also re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up = glm::normalize(glm::cross(Right, Front));
    }
};
#endif