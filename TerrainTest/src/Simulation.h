
#ifndef SIMULATION_H
#define SIMULATION_H

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

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

class Simulation {

public:


    Simulation();
    virtual ~Simulation();

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

private:
    GBuffer m_gbuffer;
    Model object;
    DSGeomPassTech m_DSGeomPassTech;
    Technique m_nullTech;

    DSPointLightPassTech m_DSPointLightPassTech;
    DSDirLightPassTech m_DSDirLightPassTech;
    //DSSpotLightPassTech m_DSSpotLightPassTech;

    SpotLight m_spotLight;
    DirectionalLight m_dirLight;
    PointLight m_pointLight[3];

    Model box, sphere, quad, cone, box2;
    Skybox * skybox;
    Terrain terrain;
    vector<shape> shap;
    vector<projector> proj;

    //glm::vec3 m_boxPositions[5];

    int windowWidth;
    int windowHeight;

    ParticleSystem fireworks;
    long long currentTime, timeNow;

};


#endif //SIMULATION_H
