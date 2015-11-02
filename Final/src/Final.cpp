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
    flagPole.loadModel("../bin/objects/flagpole2.obj");
    tree.loadModel("../bin/tree/tree.obj");

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

    //set up projector
    proj = new Projector();
    proj->setFile("../bin/greenBunny.jpg");
    proj->setup();
    proj->setScreenDims(windowWidth, windowHeight);
    proj->setDimensions(500,500);
    proj->setPosition(glm::vec3(-200,1000,-200));

    fireworks.initWithPos(glm::vec3(429.6, 384, 389.8));

    t2 = t1 = std::chrono::high_resolution_clock::now();
    time = 0;
}

void Final::tick(float dt)
{
    sphere.model = glm::mat4(1.0f);
    quad.model = glm::mat4(1.0f);
    flagPole.model = glm::mat4(1.0f);
    tree.model = glm::mat4(1.0f);

}


void Final::render()
{

    m_gbuffer.StartFrame();

    DSGeometryPass();

    if(Engine::getEngine()->project){
        proj->render(Engine::getEngine()->graphics->view, Engine::getEngine()->graphics->projection);
    }
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

    if(Engine::getEngine()->clicked) {
        picker->update();
        glm::vec3 temp = picker->getCurrentRay();
        if(Engine::getEngine()->rayColor) {
            temp = glm::abs(temp) ;
            m_dirLight.Color = temp;
        }
        else{
            m_dirLight.Direction = temp;
        }
        //flag.model = glm::translate(flag.model, *picker->getTerrainPoint());
        cout << picker->CurrentRay.x << " " << picker->CurrentRay.y << " " << picker->CurrentRay.z << endl;
    }

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
    flag_program.set("gColorMap", 6);

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
    water_program.set("gColorMap", 6);
    water_program.set("time", terrain->time);

    water_program.set("waveTime", waveTime);
    water_program.set("waveWidth", waveWidth);
    water_program.set("waveHeight", waveHeight);
    water.render();
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


    m_DSGeomPassTech.enable();

    flagPole.model = glm::translate(glm::mat4(1.0f), glm::vec3(430, 260, 390));
    flagPole.model = glm::scale(flagPole.model, glm::vec3(1, 3, 1));

    glm::mat4 mvp2 = Engine::getEngine()->graphics->projection * Engine::getEngine()->graphics->camera->getView() * flagPole.model;

    m_DSGeomPassTech.set("gWVP", mvp2);
    m_DSGeomPassTech.set("gWorld", flagPole.model);
    m_DSGeomPassTech.set("gColorMap", 7);
    m_DSGeomPassTech.set("time", time);

    flagPole.renderModel();

    tree.model = glm::translate(glm::mat4(1.0f), glm::vec3(-206, 143, 180));
    tree.model = glm::scale(tree.model, glm::vec3(10,10,10));

    mvp2 = Engine::getEngine()->graphics->projection * Engine::getEngine()->graphics->camera->getView() * tree.model;

    m_DSGeomPassTech.set("gWVP", mvp2);
    m_DSGeomPassTech.set("gWorld", tree.model);
    m_DSGeomPassTech.set("gColorMap", 7);

    tree.renderModel();

    glDepthMask(GL_FALSE);

//    glBindTexture(GL_TEXTURE_2D, 0);
//    glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
    return 100*ret;
}

void Final::DSDirectionalLightPass()
{

    m_gbuffer.BindForLightPass();
    m_DSDirLightPassTech.enable();

    glDisable(GL_CULL_FACE);
    //glCullFace(GL_FRONT);

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_ONE, GL_ONE);


    quad.model = glm::mat4(1.0f);
    m_DSDirLightPassTech.SetDirectionalLight(m_dirLight);
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
    temp.DiffuseIntensity = 1.0f;
    temp.Color = COLOR_BLUE;
    temp.Position = glm::vec3(-218,154,173);
    temp.Attenuation.Constant = .10f;
    temp.Attenuation.Linear = 0.1f;
    temp.Attenuation.Exp = .1f;
    m_pointLight.push_back(temp);

    temp.Color = COLOR_RED;
    temp.Position = glm::vec3(0.0f,5.0f,5.0f);
    m_pointLight.push_back(temp);

    temp.Color = COLOR_WHITE;
    temp.Position = glm::vec3(5.0f,10.0f,0.0f);
    m_pointLight.push_back(temp);
}