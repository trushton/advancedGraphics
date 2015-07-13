
#ifndef DEFERREDSHADING_H
#define DEFERREDSHADING_H

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

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

class DeferredShading {

public:


    DeferredShading();
    virtual ~DeferredShading();

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

    glm::vec3 m_boxPositions[5];

    int windowWidth;
    int windowHeight;
};


#endif //DEFERREDSHADING_H
