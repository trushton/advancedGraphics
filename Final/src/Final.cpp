//
// Created by trushton on 8/21/15.
//

#include "Final.h"

Final::Final()
{
    init();

}

Final::~Final()
{

}

void Final::init()
{
    windowWidth = 1600;
    windowHeight = 900;
    InitLights();


    m_gbuffer.Init(windowWidth, windowHeight);
    m_DSGeomPassTech.init();

    m_DSPointLightPassTech.init();
    m_DSPointLightPassTech.SetPointLight(m_pointLight[0]);

    m_DSDirLightPassTech.init();
    m_DSDirLightPassTech.SetDirectionalLight(m_dirLight);

    m_nullTech.init();

    sphere.loadModel("../bin/sphere.obj");
    quad.loadModel("../bin/quad.obj");
    box.loadModel("../bin/objects/flagpole2.obj");
    //box2.loadModel("../bin/tutorial.obj");

    skybox = new Skybox();

    //terrain.SetFile("../bin/data/drycreek2.tif");
    //terrain.setup();
    terrain = new Terrain(glm::vec3(1000,300,1000), "../bin/terrain/output.jpg");
    terrain->initialize();

    flag_program.init();
    water_program.init();
    //water_program2.init();

    flag.init(flag_program, "../bin/unionJack.png", 30, 30);
    flag.model = glm::translate(glm::mat4(1.0f), glm::vec3(430, 345, 390));
    flag.model = glm::rotate(flag.model, 4.697f, glm::vec3(1,0,0));

    water.init(water_program, "../bin/water.jpg", 400, 400);
    water.model = glm::translate(glm::mat4(1.0f), glm::vec3(-130, 97, -110));
    //water2.init(water_program2, "../bin/water.jpg", 3000, 3000);
    //water2.model = glm::translate(glm::mat4(1.0f), glm::vec3(-265, 120, -5050));

    //water.model = glm::rotate(water.model, 90.0f, glm::vec3(1,1,0));

    picker = new MousePicker(Engine::getEngine()->graphics->projection, Engine::getEngine()->graphics->camera->getView(), windowWidth, windowHeight, terrain);

    fireworks.initWithPos(glm::vec3(-5, 0, 15));

    t2 = t1 = std::chrono::high_resolution_clock::now();
    time = 0;
}

void Final::tick(float dt)
{
    sphere.model = glm::mat4(1.0f);
    quad.model = glm::mat4(1.0f);
    box.model = glm::mat4(1.0f);
    box2.model = glm::mat4(1.0f);

}


void Final::render()
{

    m_gbuffer.StartFrame();

    DSGeometryPass();
    skybox->render();


    glEnable(GL_STENCIL_TEST);
    for (unsigned int i = 0 ; i < m_pointLight.size(); i++) {
        DSStencilPass(i);
        DSPointLightsPass(i);
    }


    glDisable(GL_STENCIL_TEST);

    DSDirectionalLightPass();

    renderParticles();


    DSFinalPass();

    picker->update();
    cout << picker->CurrentRay.x << " " << picker->CurrentRay.y << " " << picker->CurrentRay.z << endl;

    //cout << picker->CurrentRay.x << " " << picker->CurrentRay.y << " " << picker->CurrentRay.z << endl;
}

void Final::renderParticles()
{
    glEnable(GL_DEPTH_TEST);
    t2 = std::chrono::high_resolution_clock::now();
    time = (int)(1000*std::chrono::duration_cast<std::chrono::duration<float>>(t2 - t1).count());
    t1 = t2;
    fireworks.renderWithDT(time);
    glDisable(GL_DEPTH_TEST);
}


void Final::renderFlag() {
    flag_program.enable();

    static float waveTime = 0.2f, waveWidth = 0.2f, waveHeight = 4.0f, waveFreq = 0.05f;
    waveTime += waveFreq;

    glm::mat4 mvp =
            Engine::getEngine()->graphics->projection * Engine::getEngine()->graphics->camera->getView() * flag.model;

    flag_program.set("gWVP", mvp);
    flag_program.set("gWorld", flag.model);
    flag_program.set("gColorMap", 0);

    flag_program.set("waveTime", waveTime);
    flag_program.set("waveWidth", waveWidth);
    flag_program.set("waveHeight", waveHeight);

    flag.render();
}
void Final::renderWater()
{
    water_program.enable();

    static float waveTime = 0.2f, waveWidth = .05f, waveHeight = 2.0f, waveFreq = 0.05f;
    waveTime += waveFreq;

    glm::mat4 mvp = Engine::getEngine()->graphics->projection * Engine::getEngine()->graphics->camera->getView() * water.model;
    water_program.set("gWVP", mvp);
    water_program.set("gWorld", water.model);
    water_program.set("gColorMap", 0);
    water_program.set("time", terrain->time);

    water_program.set("waveTime", waveTime);
    water_program.set("waveWidth", waveWidth);
    water_program.set("waveHeight", waveHeight);
    water.render();
}

void Final::renderWater2()
{
    water_program2.enable();
    static float waveTime = 0.2f, waveWidth = 0.2f, waveHeight = 4.0f, waveFreq = 0.05f;
    waveTime += waveFreq;

    glm::mat4 mvp = Engine::getEngine()->graphics->projection * Engine::getEngine()->graphics->camera->getView() * water2.model;
    water_program2.set("gWVP", mvp);
    water_program2.set("gWorld", water2.model);
    water_program2.set("gColorMap", 0);
    water_program2.set("time", terrain->time);


    water_program2.set("waveTime", waveTime);
    water_program2.set("waveWidth", waveWidth);
    water_program2.set("waveHeight", waveHeight);

    water2.render();
}


void Final::DSGeometryPass()
{
    m_gbuffer.BindForGeomPass();
    m_DSGeomPassTech.enable();

    // Only the geometry pass updates the depth buffer

    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);



    //terrain.render(Engine::getEngine()->graphics->view, Engine::getEngine()->graphics->projection);
    terrain->enable();
    terrain->model = glm::translate(glm::mat4(1.0f), glm::vec3(0,0,0));
    terrain->render(terrain->time);


    renderFlag();
    renderWater();
    //renderWater2();


    //box2
//    box2.model = glm::translate(glm::mat4(1.0f), glm::vec3(0, -200, 0));
//    box2.model = glm::scale(box2.model, glm::vec3(1, 1, 1));
//
//    glm::mat4 mvp = Engine::getEngine()->graphics->projection * Engine::getEngine()->graphics->camera->getView() * box2.model;
//
//    m_DSGeomPassTech.set("gWVP", mvp);
//    m_DSGeomPassTech.set("gWorld", box2.model);
//    m_DSGeomPassTech.set("gColorMap", 0);
//
//    box2.renderModel();


    m_DSGeomPassTech.enable();
//    m_DSGeomPassTech.set("gWVP", mvp);
//    m_DSGeomPassTech.set("gWorld", terrain.model);
//    m_DSGeomPassTech.set("gColorMap", 0);



    //box1

    box.model = glm::translate(glm::mat4(1.0f), glm::vec3(430, 260, 390));
    box.model = glm::scale(box.model, glm::vec3(1, 3, 1));

    glm::mat4 mvp2 = Engine::getEngine()->graphics->projection * Engine::getEngine()->graphics->camera->getView() * box.model;

    m_DSGeomPassTech.set("gWVP", mvp2);
    m_DSGeomPassTech.set("gWorld", box.model);
    m_DSGeomPassTech.set("gColorMap", 0);
    m_DSGeomPassTech.set("time", time);

    box.renderModel();



    glDepthMask(GL_FALSE);

}

void Final::DSStencilPass(unsigned int PointLightIndex)
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

    sphere.model = glm::translate(glm::mat4(1.0f), m_pointLight[PointLightIndex].Position);

    float BSphereScale = CalcPointLightBSphere(m_pointLight[PointLightIndex]);
    sphere.model = glm::scale(sphere.model, glm::vec3(BSphereScale, BSphereScale, BSphereScale));
    glm::mat4 mvp = Engine::getEngine()->graphics->projection * Engine::getEngine()->graphics->camera->getView() * sphere.model;
    m_nullTech.set("gWVP", mvp);


    sphere.renderModel();
}

void Final::DSPointLightsPass(unsigned int PointLightIndex)
{

    m_gbuffer.BindForLightPass();
    m_DSPointLightPassTech.enable();


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

    glm::mat4 mvp = Engine::getEngine()->graphics->projection * Engine::getEngine()->graphics->camera->getView() * sphere.model;

    m_DSPointLightPassTech.SetPointLight(m_pointLight[PointLightIndex]);
    m_DSPointLightPassTech.set("gWVP", mvp);
    m_DSPointLightPassTech.set("gEyeWorldPos", Engine::getEngine()->graphics->camera->getPos());
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

float Final::CalcPointLightBSphere(const PointLight &Light)
{
    float MaxChannel = fmax(fmax(Light.Color.x, Light.Color.y), Light.Color.z);

    float ret = (-Light.Attenuation.Linear + sqrtf(Light.Attenuation.Linear * Light.Attenuation.Linear - 4 * Light.Attenuation.Exp * (Light.Attenuation.Exp - 256 * MaxChannel * Light.DiffuseIntensity)))
                /
                2 * Light.Attenuation.Exp;
    //return 1000;
    return ret;
}

void Final::DSDirectionalLightPass()
{
    static float spin = 0.02;
    spin += 0.02;
    m_gbuffer.BindForLightPass();
    m_DSDirLightPassTech.enable();

    glDisable(GL_CULL_FACE);
    //glCullFace(GL_FRONT);

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_ONE, GL_ONE);


    quad.model = glm::mat4(1.0f);
    m_dirLight.Direction = glm::vec3(cos(spin), -1, sin(spin));
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

void Final::DSFinalPass()
{
    m_gbuffer.BindForFinalPass();
    glBlitFramebuffer(0, 0, windowWidth, windowHeight, 0, 0, windowWidth, windowHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
}

void Final::InitLights()
{
    m_dirLight.AmbientIntensity = 0.2f;
    m_dirLight.Color = COLOR_WHITE;
    m_dirLight.DiffuseIntensity = 0.25f;
    m_dirLight.Direction = glm::vec3(-1.0f, -1.0f, -1.0f);

    PointLight temp;
    temp.DiffuseIntensity = 0.9f;
    temp.Color = COLOR_BLUE;
    temp.Position = glm::vec3(0.0f,0.0f,15.0f);
    temp.Attenuation.Constant = .6f;
    temp.Attenuation.Linear = .5f;
    temp.Attenuation.Exp = .3f;
    m_pointLight.push_back(temp);

    temp.Color = COLOR_RED;
    temp.Position = glm::vec3(0.0f,5.0f,5.0f);
    m_pointLight.push_back(temp);

    temp.Color = COLOR_WHITE;
    temp.Position = glm::vec3(5.0f,10.0f,0.0f);
    m_pointLight.push_back(temp);
}