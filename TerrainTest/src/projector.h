#include "gishandler.h"
#include <vector>
#include "renderer.h"
#include "buffer.h"
#include "Terrain.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <string>

using namespace std;

#ifndef _PROJECTOR_H_
#define _PROJECTOR_H_
class projector
{
public:
    glm::mat4 model;

    enum PROJECTOR_TYPE
    {
        IMAGE,
        DATA
    };

    projector();
    void setup();
    void update(float dt);
    void render(glm::mat4& view, glm::mat4& projection);
    void setScreenDims(int width, int height)
    {
        SCREEN_WIDTH = width;
        SCREEN_HEIGHT = height;
    };

    void setFile(std::string fname,PROJECTOR_TYPE type = IMAGE,int band=1)
    {
        filename = fname;
        projtype = type;
        bandnum = band;
    };

    void SetPosition(glm::vec3 p)
    {
        position = p;
    };

    void SetDimensions(float width, float height)
    {
        projection = glm::ortho<float>(-width / 2.0f, width / 2.0f, height / 2.0f, -height / 2.0f, 0.1f, 10000.0f);
    };
    void setToMainCoordinateSystem(OGRSpatialReference* main, glm::vec2 mainorigin)
    {

        // translate to main coordinate system
        OGRCoordinateTransformation* transform;
        if (main != NULL)
        {
            transform = OGRCreateCoordinateTransformation( &sr, main );
        }
        else
        {
            return;
        }
        double x = origin.x;
        double y = origin.y;
        double xright,ybottom;
        xright = x + xres*width;
        ybottom = y - yres*height;


        transform->Transform(1,&x,&y);
        transform->Transform(1,&xright,&ybottom);

        double XRES = (xright-x) / width;
        double YRES = (y - ybottom) / height;
        cout << xres << " " << yres << endl;
        cout << XRES << " " << YRES << endl;
        //exit(0);
        SetDimensions((xright-x),(y - ybottom));
        x = x - mainorigin.x;
        y = mainorigin.y - y;
        position.x = x;
        position.z = y;


        cout << "projector position" << x << " " << y;
        //exit(0);
    };

private:
    renderer Renderer;
    OGRSpatialReference sr;
    int SCREEN_WIDTH, SCREEN_HEIGHT;
    glm::mat4 view;
    glm::mat4 projection;
    glm::mat4 Texgen;
    glm::vec3 direction;
    glm::vec3 position;
    glm::vec3 up;
    glm::vec2 origin;
    buffer Buffer;
    GLuint tex;
    string filename;
    int width, height;
    double xres, yres;
    PROJECTOR_TYPE projtype;
    int bandnum;
};
#endif