//
// Created by trushton on 8/21/15.
//

#ifndef FINAL_FINAL_H
#define FINAL_FINAL_H

#include "GBuffer.h"
#include "Model.h"
#include "ds_geom_pass_tech.h"
#include "ds_dir_light_pass_tech.h"
#include "ds_point_light_pass_tech.h"
#include "ds_spot_light_pass_tech.h"
#include "Technique.h"
#include "Engine.h"
#include "SDL.h"
#include "graphics.h"
#include "lights.h"
#include "particle_system.h"
#include "water_tech.h"
#include "Plane.h"
#include "Flag.h"
#include "skybox.h"
#include "waterPlane.h"
#include "Terrain.h"
#include "MousePicker.h"

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

class MousePicker;
class Final {

public:


    Final();
    virtual ~Final();

    virtual void init();
    virtual void tick(float dt);
    virtual void render();

    void DSGeometryPass();
    void DSStencilPass(unsigned int PointLightIndex);
    void BeginLightPasses();
    void DSPointLightsPass(unsigned int PointLightIndex);

    static float CalcPointLightBSphere(const PointLight& Light);
    float CalcSpotLightBSphere(const SpotLight& Light);

    void DSDirectionalLightPass();
    void DSFinalPass();

    void InitLights();
    void InitBoxPositions();
    void DSLightPass();

    void renderParticles();
    void renderFlag();
    void renderWater();
    void renderWater2();
    void DrawBMP(SDL_Surface* MouseImage, int mouseX, int mouseY);

    Plane flag;
    ParticleSystem fireworks;
    MousePicker* picker;


private:
    GBuffer m_gbuffer;
    Model object;
    DSGeomPassTech m_DSGeomPassTech;
    Technique m_nullTech;

    DSPointLightPassTech m_DSPointLightPassTech;
    DSDirLightPassTech m_DSDirLightPassTech;

    DirectionalLight m_dirLight;
    vector<PointLight> m_pointLight;

    Model box, sphere, quad, box2;
    Skybox * skybox;
    Terrain *terrain;



    int windowWidth;
    int windowHeight;

    std::chrono::time_point<std::chrono::high_resolution_clock> t1, t2;
    int time;

    water_tech water_program, water_program2, water_program3;
    waterPlane water, water2, water3;
    Flag flag_program;

};


#endif //FINAL_FINAL_H
