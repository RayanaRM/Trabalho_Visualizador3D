#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include "objeto.h"

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
    // camera Attributes for object One
    glm::vec3 Position[10];
    glm::vec3 Front[10];
    glm::vec3 Up[10];
    glm::vec3 Right[10];
    glm::vec3 WorldUp[10];
    glm::vec3 axis[10];
    glm::vec3 positionTranslate[10];
    float angle[10];
    float escala[10];

    // euler Angles for 
    float Yaw;
    float Pitch;

    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    float type;
    float inc;
    int mvm = 0;

    // constructor with vectors
    Camera(glm::vec3 position, glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        for (unsigned int i = 0; i < 10; i++) {
            Position[i] = glm::vec3(position[0] + (i * 5), position[1], position[2]);
            WorldUp[i] = up;

            Front[i] = glm::vec3(0.0f, 0.0f, -1.0f);
            axis[i] = glm::vec3(0.0f, 1.0f, 0.0f);
            positionTranslate[i] = glm::vec3(0.0f, 0.0f, 0.0f);
            angle[i] = 1.0f;
            escala[i] = 0.5f;
        }

        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    glm::mat4 GetViewMatrix(int id)
    {
        return glm::lookAt(Position[id], Position[id] + Front[id], Up[id]);
    }

    void AtualizaCamera(std::vector <Objeto> objetos) {
        for (int i = 0; i < objetos.size(); i++) {
            escala[objetos[i].id] = objetos[i].escala;
            positionTranslate[objetos[i].id] = objetos[i].positionTranslate;
        }
    }

    glm::mat4 GetModelMatrix(glm::mat4 model, Objeto obj, std::vector<glm::vec3*>* pontosCurva)
    {
        int id = obj.id;
        model = glm::rotate(model, angle[id], axis[id]);
        model = glm::translate(model, positionTranslate[id]);
        model = glm::scale(model, glm::vec3(escala[id], escala[id], escala[id]));

        if (obj.hasCurves) {
            if (pontosCurva->size() - 5 == mvm)
                mvm = 0;

            model = glm::translate(model, glm::vec3(pontosCurva->at(mvm)->x, pontosCurva->at(mvm)->y, pontosCurva->at(mvm)->z));
            angle[id] = -calcularAnguloOBJ(mvm, mvm + 5, pontosCurva);
            mvm++;
        }
        return model;
    }

    void ProcessKeyboard(KEY_INPUTS key, float deltaTime, Objeto objeto)
    {
        float velocity = MovementSpeed * deltaTime;

        // Teclas de controle (rota��o/ transla��o)
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
        if (key == FORWARD) {
            Position[objeto.id] += Front[objeto.id] * velocity;
        }
        if (key == BACKWARD) {
            Position[objeto.id] -= Front[objeto.id] * velocity;
        }
        if (key == LEFT) {
            Position[objeto.id] -= Right[objeto.id] * velocity;
        }
        if (key == RIGHT) {
            Position[objeto.id] += Right[objeto.id] * velocity;
        }

        // Rota��o
        if (key == X && type == 2) {
            angle[objeto.id] = (GLfloat)glfwGetTime();
            axis[objeto.id] = glm::vec3(1.0f, 0.0f, 0.0f);
        }
        if (key == Y && type == 2) {
            angle[objeto.id] = (GLfloat)glfwGetTime();
            axis[objeto.id] = glm::vec3(0.0f, 1.0f, 0.0f);
        }
        if (key == Z && type == 2) {
            angle[objeto.id] = (GLfloat)glfwGetTime();
            axis[objeto.id] = glm::vec3(0.0f, 0.0f, 1.0f);
        }

        // Transla��o
        if (key == X && type == 1) {
            positionTranslate[objeto.id][0] += inc == 1 ? 0.01 : -0.01;
        }
        if (key == Y && type == 1) {
            positionTranslate[objeto.id][1] += inc == 1 ? 0.01 : -0.01;
        }
        if (key == Z && type == 1) {
            positionTranslate[objeto.id][2] += inc == 1 ? 0.01 : -0.01;
        }

        // escala
        if (key == ESCALA) {
            escala[objeto.id] += inc == 1 ? 0.0005f : -0.0005f;
            if (escala[objeto.id] > 1.0f)
                escala[objeto.id] = 1.0f;
            else if (escala[objeto.id] < 0.01f)
                escala[objeto.id] = 0.01f;
        }
    }

    float calcularAnguloOBJ(int indexA, int indexB, std::vector<glm::vec3*>* pontosCurva) {

        glm::vec3* a = pontosCurva->at(indexA);
        glm::vec3* b;

        if (indexA == pontosCurva->size() - 5) {
            b = pontosCurva->at(0);
        }
        else {
            b = pontosCurva->at(indexB);
        }

        GLfloat dx = b->x - a->x;
        GLfloat dz = b->z - a->z;

        GLfloat angle = glm::atan(dz, dx);

        return angle;
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
    // tentar passar array
private:
    void updateCameraVectors()
    {
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

        for (unsigned int i = 0; i < 10; i++) {
            Front[i] = glm::normalize(front);
            Right[i] = glm::normalize(glm::cross(Front[i], WorldUp[i]));
            Up[i] = glm::normalize(glm::cross(Right[i], Front[i]));
        }
    }
};
#endif