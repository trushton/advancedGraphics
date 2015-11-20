//
// Created by trushton on 11/1/15.
//

#ifndef PROJECTOR_H
#define PROJECTOR_H

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <gdal.h>
#include <gdal_priv.h>
#include <gdalwarper.h>
#include <ogrsf_frmts.h>
#include "buffer.h"
#include "renderer.h"
#include "Texture.h"

using namespace std;

class Projector {
public:
    enum PROJECTOR_TYPE{
        IMAGE,
        DATA
    };

    //constructor
    Projector();
    void setup();
    void update(float dt);
    void render(glm::mat4& view, glm::mat4& projection);

    //a function for setting the screen dimension
    void setScreenDims(int width, int height){
        SCREEN_WIDTH = width,
        SCREEN_HEIGHT = height;
    }

    void setFile(std::string fname, PROJECTOR_TYPE type = IMAGE){
        filename = fname;
        projtype = type;
    }


    void setPosition(glm::vec3 pos){
        position = pos;
    }

    void setDimensions(float width, float height){
        projection = glm::ortho(-width / 2.0f, width /2.0f, height / 2.0f, -height/2.0f, 0.1f, 10000.0f);

    }

    void setPosition(float x, float y){
        position.x = x;
        position.z = y;
        position.y = 100;
    }

    void setView(){
        view = glm::lookAt( position, //Eye Position
                            position + direction, //Focus point
                            up); //Positive Y is up
    }

    glm::vec3 position;

private:
    int SCREEN_WIDTH, SCREEN_HEIGHT;
    glm::mat4 view;
    glm::mat4 projection;
    glm::mat4 TexGen;
    glm::mat4 model;
    float alpha;

    //these are necessary for the view matrix
    glm::vec3 direction;
    glm::vec3 up;
    glm::vec3 origin;

    //something to hold our screen rect
    buffer Buffer;

    //something to hold our projector texture
    Texture *tex;
    string filename;

    //the width and height of the projector texture
    int width, height;

    //the resolution per pixel
    double xres, yres;

    //used for different typesof projectors
    PROJECTOR_TYPE projtype;

    renderer Renderer;

};


#endif //FINAL_PROJECTOR_H
