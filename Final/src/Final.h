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
#include "graphics.h"
#include "lights.h"
#include "Terrain.h"
#include "shape.h"
#include "projector.h"
#include "particle_system.h"
#include "water_tech.h"
#include "Plane.h"
#include "Flag.h"
#include "skybox.h"

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))


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

//    void renderParticles();
//    void renderFlag();
//    void renderWater();

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
    Terrain terrain;
    vector<shape> shap;
    vector<projector> proj;


    int windowWidth;
    int windowHeight;

    ParticleSystem fireworks;
    std::chrono::time_point<std::chrono::high_resolution_clock> t1, t2;
    int time;

    water_tech water_program;

    Flag flag_program;
    Plane flag, water;

};


#endif //FINAL_FINAL_H
