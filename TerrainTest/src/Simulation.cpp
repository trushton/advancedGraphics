#include <sys/time.h>
#include "lights.h"
#include "Simulation.h"
#include "skybox.h"

Simulation::Simulation()
{
    init();
}

Simulation::~Simulation()
{

}

void Simulation::init()
{
    fireworks.InitParticleSystem(glm::vec3(0,-11,0));
    m_pointLight.resize(3);
    windowWidth = 1600;
    windowHeight = 900;
    InitLights();

    m_gbuffer.Init(windowWidth, windowHeight);
    m_DSGeomPassTech.init();

    m_DSPointLightPassTech.init();

    m_DSPointLightPassTech.SetPointLight(m_pointLight[1]);

    m_DSDirLightPassTech.init();

    m_DSDirLightPassTech.SetDirectionalLight(m_dirLight);

    m_nullTech.init();
    InitLights();

    water.init();

    sphere.loadModel("../bin/sphere.obj");
    quad.loadModel("../bin/quad.obj");
    box.loadModel("../bin/box.obj");
    box2.loadModel("../bin/box.obj");

    skybox = new Skybox();

    terrain.SetFile("../bin/data/drycreek2.tif");
    terrain.setup();

    shap.resize(3);

    shap[0].load("../bin/data/streamDCEW2/stream2.shp");
    shap[0].createMesh(terrain.GetProjection(),terrain.GetOrigin(),glm::vec2(1,1),terrain);
    shap[1].load("../bin/data/boundDCEW/boundDCEW.shp");
    shap[1].createMesh(terrain.GetProjection(),terrain.GetOrigin(),glm::vec2(1,1),terrain);
    shap[2].load("../bin/data/sitesDCEW2012/DCEWsites2013.shp");
    shap[2].createMesh(terrain.GetProjection(),terrain.GetOrigin(),glm::vec2(1,1),terrain);

    proj.resize(7);

    proj[0].setFile("../bin/data/satellite/res.tif");
    proj[0].setup();
    proj[0].setScreenDims(windowWidth,windowHeight);

    proj[1].setFile("../bin/data/satellite/res2.tif");
    proj[1].setup();
    proj[1].setScreenDims(windowWidth,windowHeight);

    proj[2].setFile("../bin/data/satellite/res3.tif");
    proj[2].setup();
    proj[2].setScreenDims(windowWidth,windowHeight);

    proj[3].setFile("../bin/data/satellite/res4.tif");
    proj[3].setup();
    proj[3].setScreenDims(windowWidth,windowHeight);

    proj[4].setFile("../bin/data/satellite/res5.tif");
    proj[4].setup();
    proj[4].setScreenDims(windowWidth,windowHeight);

    proj[5].setFile("../bin/data/satellite/res6.tif");
    proj[5].setup();
    proj[5].setScreenDims(windowWidth,windowHeight);

    proj[6].setmask("../bin/data/tl2p5mask.ipw.tif");
    proj[6].setFile("../bin/data/em.1000.tif", projector::PROJECTOR_TYPE::DATA);
    proj[6].setup();
    proj[6].setScreenDims(windowWidth,windowHeight);

    for(uint i = 0; i < proj.size(); i++)
    {
        proj[i].setToMainCoordinateSystem(terrain.GetProjection(), terrain.GetOrigin());
    }

    t2 = t1 = std::chrono::high_resolution_clock::now();


    flag_program.init();

    flag.init(flag_program, "../bin/flag.jpg", 15, 15);
    flag.model = glm::translate(glm::mat4(1.0f), glm::vec3(0,0,0));
    flag.model = glm::rotate(flag.model, 60.0f, glm::vec3(1,0,0));
}

void Simulation::tick(float dt)
{
    for(uint i = 0; i < shap.size(); i++)
    {
        shap[i].model = glm::mat4(1.0f);
    }
    terrain.model = glm::mat4(1.0f);
    object.model = glm::mat4(1.0f);
    sphere.model = glm::mat4(1.0f);
    quad.model = glm::mat4(1.0f);
    box.model = glm::mat4(1.0f);

    for(uint i = 0; i < proj.size(); i++)
    {
        proj[i].model = glm::mat4(1.0f);
    }

}

void Simulation::render()
{
    m_gbuffer.StartFrame();




    DSGeometryPass();

    skybox->render();




    glEnable(GL_STENCIL_TEST);

    for (unsigned int i = 0 ; i < m_pointLight.size(); i++) {
        DSStencilPass(i);
        DSPointLightsPass(i);
    }
    for(uint i = 0; i < proj.size(); i++)
    {
        proj[i].model = glm::mat4(1.0f);
        proj[i].render(Engine::getEngine()->graphics->view, Engine::getEngine()->graphics->projection);
    }

    glDisable(GL_STENCIL_TEST);

    DSDirectionalLightPass();




    DSFinalPass();

    renderParticles();


}

void Simulation::renderParticles()
{
    t2 = std::chrono::high_resolution_clock::now();
    time = (int)(1000*std::chrono::duration_cast<std::chrono::duration<float>>(t2 - t1).count());
    t1 = t2;
    glEnable(GL_DEPTH_TEST);
    fireworks.Render(time);
    glDisable(GL_DEPTH_TEST);
}

void Simulation::DSGeometryPass()
{
    static float scale = 0.00f;
    scale += 0.002;
    m_DSGeomPassTech.enable();
    m_gbuffer.BindForGeomPass();

    // Only the geometry pass updates the depth buffer

    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);



    terrain.model = glm::translate(glm::mat4(1.0f), glm::vec3(1250,-2000, -800));
//
//    glm::mat4 mvp = Engine::getEngine()->graphics->projection * Engine::getEngine()->graphics->view * terrain.model;
//
//    m_DSGeomPassTech.set("gWVP", mvp);
//    m_DSGeomPassTech.set("gWorld", terrain.model);
//    m_DSGeomPassTech.set("gColorMap", 0);

    terrain.render(Engine::getEngine()->graphics->view, Engine::getEngine()->graphics->projection);

    renderFlag();

    m_DSGeomPassTech.enable();


    box.model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
    //box.model = glm::rotate(box.model, scale * 5, glm::vec3(0,1,0));
    box.model = glm::scale(box.model, glm::vec3(20, 20, 20));

    glm::mat4 mvp = Engine::getEngine()->graphics->projection * Engine::getEngine()->graphics->view * box.model;

    m_DSGeomPassTech.set("gWVP", mvp);
    m_DSGeomPassTech.set("gWorld", box.model);
    m_DSGeomPassTech.set("gColorMap", 0);
    m_DSGeomPassTech.set("time", time);

    box.renderModel();

    box2.model = glm::translate(glm::mat4(1.0f), glm::vec3(100, 0, 100));
    box2.model = glm::scale(box2.model, glm::vec3(10, 10, 10));

    mvp = Engine::getEngine()->graphics->projection * Engine::getEngine()->graphics->view * box2.model;

    m_DSGeomPassTech.set("gWVP", mvp);
    m_DSGeomPassTech.set("gWorld", box2.model);
    m_DSGeomPassTech.set("gColorMap", 0);

    box2.renderModel();



//    for(uint i = 0; i < shap.size(); i++){
//        shap[i].model = glm::translate(shap[i].model, glm::vec3(1250,-2000, -800));
//
//        mvp = Engine::getEngine()->graphics->projection * Engine::getEngine()->graphics->view * shap[i].model;
//
//
//        m_DSGeomPassTech.set("gWVP", mvp);
//        m_DSGeomPassTech.set("gWorld", shap[i].model);
//        m_DSGeomPassTech.set("gColorMap", 0);
//
//        shap[i].render(Engine::getEngine()->graphics->view, Engine::getEngine()->graphics->projection);
//    }

    glDepthMask(GL_FALSE);
}

void Simulation::renderFlag()
{
    flag_program.enable();

    static float waveTime = 0.2f, waveWidth = 0.2f, waveHeight = 3.0f, waveFreq = 0.05f;
    waveTime += waveFreq;

    glm::mat4 mvp = Engine::getEngine()->graphics->projection * Engine::getEngine()->graphics->view * flag.model;

    flag_program.set("gWVP", mvp);
    flag_program.set("gWorld", flag.model);
    flag_program.set("gColorMap", 0);

    flag_program.set("waveTime", waveTime);
    flag_program.set("waveWidth", waveWidth);
    flag_program.set("waveHeight", waveHeight);

    flag.render();
}

void Simulation::DSStencilPass(unsigned int PointLightIndex)
{
    m_nullTech.enable();

    // Disable color/depth write and enable stencil
    m_gbuffer.BindForStencilPass();
    glEnable(GL_DEPTH_TEST);

    glDisable(GL_CULL_FACE);

    glClear(GL_STENCIL_BUFFER_BIT);

    // We need the stencil test to be enabled but we want it
    // to succeed always. Only the depth test matters.
    glStencilFunc(GL_ALWAYS, 0, 0);

    glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
    glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);

    m_pointLight[PointLightIndex].Position = Engine::getEngine()->graphics->camera->getPos();
    cout << "X: " << m_pointLight[PointLightIndex].Position.x << " Y: " << m_pointLight[PointLightIndex].Position.y << " Z: " << m_pointLight[PointLightIndex].Position.z << endl;
    sphere.model = glm::translate(glm::mat4(1.0f), glm::vec3(m_pointLight[PointLightIndex].Position));

    float BSphereScale = CalcPointLightBSphere(m_pointLight[PointLightIndex]);
    sphere.model = glm::scale(sphere.model, glm::vec3(BSphereScale, BSphereScale, BSphereScale));

    glm::mat4 mvp = Engine::getEngine()->graphics->projection * Engine::getEngine()->graphics->view * sphere.model;
    m_nullTech.set("gWVP", mvp);

    // m_DSPointLightPassTech.SetScreenSize(WINDOW_WIDTH, WINDOW_HEIGHT);

    sphere.renderModel();
}



void Simulation::DSPointLightsPass(unsigned int PointLightIndex)
{

    m_gbuffer.BindForLightPass();
    m_DSPointLightPassTech.enable();
    m_DSPointLightPassTech.set("gEyeWorldPos", Engine::getEngine()->graphics->camera->getPos());


    glStencilFunc(GL_NOTEQUAL, 0, 0xFF);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_ONE, GL_ONE);


    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);


    sphere.model = glm::translate(glm::mat4(1.0f), glm::vec3(m_pointLight[PointLightIndex].Position));

    float BSphereScale = CalcPointLightBSphere(m_pointLight[PointLightIndex]);
    sphere.model = glm::scale(sphere.model, glm::vec3(BSphereScale, BSphereScale, BSphereScale));

    glm::mat4 mvp = Engine::getEngine()->graphics->projection * Engine::getEngine()->graphics->view * sphere.model;

    m_DSPointLightPassTech.SetPointLight(m_pointLight[PointLightIndex]);
    m_DSPointLightPassTech.set("gWVP", mvp);
    m_DSPointLightPassTech.set("gPositionMap", GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
    m_DSPointLightPassTech.set("gColorMap", GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
    m_DSPointLightPassTech.set("gNormalMap", GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
    m_DSPointLightPassTech.set("gMatSpecularIntensity", 150.0f);
    m_DSPointLightPassTech.set("gSpecularPower", 32.0f);


    glUniform2f(m_DSPointLightPassTech.getLocation("gScreenSize"), (float) windowWidth, (float) windowHeight);

    sphere.renderModel();



    glCullFace(GL_BACK);
    glDisable(GL_BLEND);

}

float Simulation::CalcPointLightBSphere(const PointLight &Light)
{
    float MaxChannel = fmax(fmax(Light.Color.x, Light.Color.y), Light.Color.z);

    float ret = (-Light.Attenuation.Linear + sqrtf(Light.Attenuation.Linear * Light.Attenuation.Linear - 4 * Light.Attenuation.Exp * (Light.Attenuation.Exp - 256 * MaxChannel * Light.DiffuseIntensity)))
                /
                2 * Light.Attenuation.Exp;
    return 1000;
    //return ret;
}


void Simulation::DSDirectionalLightPass()
{
    m_gbuffer.BindForLightPass();
    m_DSDirLightPassTech.enable();

    glDisable(GL_CULL_FACE);
    //glCullFace(GL_FRONT);

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_ONE, GL_ONE);


    quad.model = glm::translate(glm::mat4(1.0f), Engine::getEngine()->graphics->camera->getPos());
    m_DSDirLightPassTech.set("gWVP", glm::mat4(1.0f));
    m_DSDirLightPassTech.set("gPositionMap", GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
    m_DSDirLightPassTech.set("gColorMap", GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
    m_DSDirLightPassTech.set("gNormalMap", GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
    m_DSDirLightPassTech.set("gEyeWorldPos", Engine::getEngine()->graphics->camera->getPos());
    m_DSDirLightPassTech.set("gMatSpecularIntensity", 0.10f);
    m_DSDirLightPassTech.set("gSpecularPower", 32.0f);

    glUniform2f(m_DSDirLightPassTech.getLocation("gScreenSize"), (float) windowWidth, (float) windowHeight);

    quad.renderModel();

    glDisable(GL_BLEND);
    //glCullFace(GL_BACK);
}

void Simulation::DSFinalPass()
{
    m_gbuffer.BindForFinalPass();
    glBlitFramebuffer(0, 0, windowWidth, windowHeight, 0, 0, windowWidth, windowHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
}

void Simulation::InitLights()
{
    m_dirLight.AmbientIntensity = 0.2f;
    m_dirLight.Color = COLOR_WHITE;
    m_dirLight.DiffuseIntensity = 0.25f;
    m_dirLight.Direction = glm::vec3(-1.0f, -1.0f, -1.0f);

    m_pointLight[0].DiffuseIntensity = 30.0f;
    m_pointLight[0].Color = COLOR_RED;
    m_pointLight[0].Position = glm::vec3(100.0f, -3.0f, 100.0f);
    m_pointLight[0].Attenuation.Constant = .6f;
    m_pointLight[0].Attenuation.Linear = .2f;
    m_pointLight[0].Attenuation.Exp = .3f;

    m_pointLight[1].DiffuseIntensity = 20.0f;
    m_pointLight[1].Color = COLOR_BLUE;
    m_pointLight[1].Position = glm::vec3(0.0f,0.0f,5.0f);
    m_pointLight[1].Attenuation.Constant = .6f;
    m_pointLight[1].Attenuation.Linear = .2f;
    m_pointLight[1].Attenuation.Exp = .3f;

    m_pointLight[2].DiffuseIntensity = 10.0f;
    m_pointLight[2].Color = COLOR_GREEN;
    m_pointLight[2].Position = glm::vec3(0.0f, -3.0f, 0.0f);
    m_pointLight[2].Attenuation.Constant = .6f;
    m_pointLight[2].Attenuation.Linear = .2f;
    m_pointLight[2].Attenuation.Exp = .3f;
}
