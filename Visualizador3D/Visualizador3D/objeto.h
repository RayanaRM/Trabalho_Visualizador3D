#ifndef OBJETO_H
#define OBJETO_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "camera.h"
#include <vector>


class Objeto
{
public:
    bool isSelected;
    int id;
    bool hasCurves;
    float escala;
    glm::vec3 positionTranslate;
    std::string path;

    // constructor with vectors
    Objeto(int id, bool isSelected, bool hasCurves, float escala, glm::vec3 positionTranslate, std::string path)
    {
        this->id = id;
        this->isSelected = isSelected;
        this->hasCurves = hasCurves;
        this->escala = escala;
        this->positionTranslate = positionTranslate;
        this->path = path;
    }
};
#endif