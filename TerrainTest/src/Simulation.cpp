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

    windowWidth = 1600;
    windowHeight = 900;
    InitLights();
    InitBoxPositions();
    m_gbuffer.Init(windowWidth, windowHeight);
    m_DSGeomPassTech.init();
    m_DSPointLightPassTech.init();
    m_DSPointLightPassTech.SetPointLight(m_pointLight[0]);
    m_DSDirLightPassTech.init();
    m_DSDirLightPassTech.SetDirectionalLight(m_dirLight);
    //m_DSSpotLightPassTech.init();
    //m_DSSpotLightPassTech.SetSpotLight(m_spotLight);
    m_nullTech.init();

    //object.loadModel("../bin/phoenix_ugv.md2");
    sphere.loadModel("../bin/sphere.obj");
    quad.loadModel("../bin/quad.obj");
    box.loadModel("../bin/box.obj");
    //cone.loadModel("../bin/sphere.obj");
    //box2.loadModel("../bin/table2.obj");

    skybox = new Skybox();

    terrain.SetFile("../bin/data/drycreek2.tif");
    terrain.setup();



}

void Simulation::tick(float dt)
{
    terrain.model = glm::mat4(1.0f);
    object.model = glm::mat4(1.0f);
    sphere.model = glm::mat4(1.0f);
    quad.model = glm::mat4(1.0f);
    box.model = glm::mat4(1.0f);
}

void Simulation::render()
{
    m_gbuffer.StartFrame();

    DSGeometryPass();

    skybox->render();

    glEnable(GL_STENCIL_TEST);

    for (unsigned int i = 0 ; i < ARRAY_SIZE_IN_ELEMENTS(m_pointLight); i++) {
        DSStencilPass(i);
        DSPointLightsPass(i);
    }

    glDisable(GL_STENCIL_TEST);

    DSDirectionalLightPass();
    //terrain.render(view, projection);


    DSFinalPass();
}

void Simulation::DSGeometryPass()
{
    m_DSGeomPassTech.enable();
    //m_gbuffer.BindForWriting();
    m_gbuffer.BindForGeomPass();

    // Only the geometry pass updates the depth buffer

    glDepthMask(GL_TRUE);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);


    // object.renderModel();
    //for (unsigned int i = 0 ; i < ARRAY_SIZE_IN_ELEMENTS(m_boxPositions); i++) {
    box.model = glm::translate(glm::mat4(1.0f), glm::vec3(0, -5, 0));
    box.model = glm::scale(box.model, glm::vec3(5, 5, 5));

    //box2.model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 1000, 0));



    glm::mat4 mvp = Engine::getEngine()->graphics->projection * Engine::getEngine()->graphics->view * box.model;


    m_DSGeomPassTech.set("gWVP", mvp);
    m_DSGeomPassTech.set("gWorld", box.model);
    m_DSGeomPassTech.set("gColorMap", 0);

    box.renderModel();

    terrain.model = glm::translate(terrain.model, glm::vec3(0,-1,0));


    mvp = Engine::getEngine()->graphics->projection * Engine::getEngine()->graphics->view * terrain.model;


    m_DSGeomPassTech.set("gWVP", mvp);
    m_DSGeomPassTech.set("gWorld", terrain.model);
    m_DSGeomPassTech.set("gColorMap", 0);

    terrain.render(Engine::getEngine()->graphics->view, Engine::getEngine()->graphics->projection);



//    mvp = Engine::getEngine()->graphics->projection * Engine::getEngine()->graphics->view * box2.model;
//
//    m_DSGeomPassTech.set("gWVP", mvp);
//    m_DSGeomPassTech.set("gWorld", box2.model);
//    m_DSGeomPassTech.set("gColorMap", 0);
//
//    box2.renderModel();

    //}

    glDepthMask(GL_FALSE);

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


    sphere.model = glm::translate(glm::mat4(1.0f), m_pointLight[PointLightIndex].Position); // MIGHT NEED TO PUT POINT LIGHT POS

    float BSphereScale = CalcPointLightBSphere(m_pointLight[PointLightIndex]);
    sphere.model = glm::scale(sphere.model, glm::vec3(BSphereScale, BSphereScale, BSphereScale));

    glm::mat4 mvp = Engine::getEngine()->graphics->projection * Engine::getEngine()->graphics->view * sphere.model;
    m_nullTech.set("gWVP", mvp);

    // m_DSPointLightPassTech.SetScreenSize(WINDOW_WIDTH, WINDOW_HEIGHT);

    sphere.renderModel();

        /*
        cone.model = glm::translate(glm::mat4(1.0f), m_spotLight.Position); // MIGHT NEED TO PUT POINT LIGHT POS

        BSphereScale = CalcSpotLightBSphere(m_spotLight);
        cone.model = glm::scale(cone.model, glm::vec3(10, 10, 10));

        mvp = Engine::getEngine()->graphics->projection * Engine::getEngine()->graphics->view * cone.model;
        m_nullTech.set("gWVP",     model = glm::translate(model, glm::vec3(0,-1,0));
mvp);

        // m_DSPointLightPassTech.SetScreenSize(WINDOW_WIDTH, WINDOW_HEIGHT);

        //cone.renderModel();
        */
}

/*void Simulation::BeginLightPasses()
    {
        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_ONE, GL_ONE);

        m_gbuffer.BindForReading();
        glClear(GL_COLOR_BUFFER_BIT);
    }*/


void Simulation::DSPointLightsPass(unsigned int PointLightIndex)
{
    m_gbuffer.BindForLightPass();
    m_DSPointLightPassTech.enable();
    // m_DSSpotLightPassTech.enable();

    glStencilFunc(GL_NOTEQUAL, 0, 0xFF);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_ONE, GL_ONE);


    //glFrontFace(GL_CW);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);


    sphere.model = glm::translate(glm::mat4(1.0f), m_pointLight[PointLightIndex].Position); //MIGHT NEED TO BE LIGHT POS


    m_DSPointLightPassTech.SetPointLight(m_pointLight[PointLightIndex]);

    float BSphereScale = CalcPointLightBSphere(m_pointLight[PointLightIndex]);
    sphere.model = glm::scale(sphere.model, glm::vec3(BSphereScale, BSphereScale, BSphereScale));

    glm::mat4 mvp = Engine::getEngine()->graphics->projection * Engine::getEngine()->graphics->view * sphere.model;
    m_DSPointLightPassTech.set("gWVP", mvp);

    m_DSPointLightPassTech.set("gPositionMap", GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
    m_DSPointLightPassTech.set("gColorMap", GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
    m_DSPointLightPassTech.set("gNormalMap", GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
    m_DSPointLightPassTech.set("gEyeWorldPos", Engine::getEngine()->graphics->camera->getPos());
    m_DSPointLightPassTech.set("gMatSpecularIntensity", 150.0f);
    m_DSPointLightPassTech.set("gSpecularPower", 32.0f);



    glUniform2f(m_DSPointLightPassTech.getLocation("gScreenSize"), (float) windowWidth, (float) windowHeight);

    // m_DSPointLightPassTech.SetScreenSize(WINDOW_WIDTH, WINDOW_HEIGHT);

    sphere.renderModel();



/*


        cone.model = glm::translate(glm::mat4(1.0f), m_spotLight.Position);
        BSphereScale = CalcSpotLightBSphere(m_spotLight);
        cone.model = glm::scale(cone.model, glm::vec3(10, 10, 10));

        mvp = Engine::getEngine()->graphics->projection * Engine::getEngine()->graphics->view * cone.model;

        m_DSPointLightPassTech.set("gWVP", mvp);

        m_DSSpotLightPassTech.set("gPositionMap", GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
        m_DSSpotLightPassTech.set("gColorMap", GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
        m_DSSpotLightPassTech.set("gNormalMap", GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
        m_DSSpotLightPassTech.set("gEyeWorldPos", Engine::getEngine()->graphics->camera->getPos());
        m_DSSpotLightPassTech.set("gMatSpecularIntensity", 1.0f);
        m_DSSpotLightPassTech.set("gSpecularPower", 0.5f);
        glUniform2f(m_DSSpotLightPassTech.getLocation("gScreenSize"), (float) windowWidth, (float) windowHeight);

        //cone.renderModel();*/
    glCullFace(GL_BACK);

    glDisable(GL_BLEND);
}

float Simulation::CalcPointLightBSphere(const PointLight &Light)
{
    float MaxChannel = fmax(fmax(Light.Color.x, Light.Color.y), Light.Color.z);

    float ret = (-Light.Attenuation.Linear + sqrtf(Light.Attenuation.Linear * Light.Attenuation.Linear - 4 * Light.Attenuation.Exp * (Light.Attenuation.Exp - 256 * MaxChannel * Light.DiffuseIntensity)))
                /
                2 * Light.Attenuation.Exp;
    //return 100;
    return ret;

}

float Simulation::CalcSpotLightBSphere(const SpotLight &Light)
{
    float MaxChannel = fmax(fmax(Light.Color.x, Light.Color.y), Light.Color.z);

    float ret = (-Light.Attenuation.Linear + sqrtf(Light.Attenuation.Linear * Light.Attenuation.Linear - 4 * Light.Attenuation.Exp * (Light.Attenuation.Exp - 256 * MaxChannel * Light.DiffuseIntensity)))
                /
                2 * Light.Attenuation.Exp;

    return ret;
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


    /*quad.model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 8.0, 16.0));
    glm::mat4 mvp = Engine::getEngine()->graphics->projection * Engine::getEngine()->graphics->view * quad.model;
    m_DSDirLightPassTech.set("gWVP", mvp);*/
    m_DSDirLightPassTech.set("gWVP", glm::mat4(1.0f));
    m_DSDirLightPassTech.set("gPositionMap", GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
    m_DSDirLightPassTech.set("gColorMap", GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
    m_DSDirLightPassTech.set("gNormalMap", GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
    m_DSDirLightPassTech.set("gEyeWorldPos", Engine::getEngine()->graphics->camera->getPos());
    m_DSDirLightPassTech.set("gMatSpecularIntensity", 1.0f);
    m_DSDirLightPassTech.set("gSpecularPower", 32.0f);

    glUniform2f(m_DSDirLightPassTech.getLocation("gScreenSize"), (float) windowWidth, (float) windowHeight);
    //WVP.InitIdentity();
    //m_DSDirLightPassTech.SetWVP(WVP);
    quad.renderModel();

    glDisable(GL_BLEND);
    // glCullFace(GL_BACK);

    // glEnable(GL_CULL_FACE);


}

void Simulation::DSFinalPass()
{
    m_gbuffer.BindForFinalPass();
    glBlitFramebuffer(0, 0, windowWidth, windowHeight, 0, 0, windowWidth, windowHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
}

void Simulation::InitLights()
{
    m_spotLight.AmbientIntensity = 0.010f;
    m_spotLight.DiffuseIntensity = 0.01f;
    m_spotLight.Color = COLOR_RED;
    m_spotLight.Attenuation.Linear = 0.0f;
    m_spotLight.Attenuation.Constant = 0.0f;
    m_spotLight.Attenuation.Exp =  0.3f;
    m_spotLight.Position = glm::vec3(0.0, 1.5, 0.0f);
    m_spotLight.Direction = glm::vec3(0.0f, -1.0f, 0.0f);
    m_spotLight.Cutoff = 50.0f;

    m_dirLight.AmbientIntensity = 0.5f;
    m_dirLight.Color = COLOR_CYAN;
    m_dirLight.DiffuseIntensity = 0.05f;
    m_dirLight.Direction = glm::vec3(0.0f, -1.0f, 0.0f);

    m_pointLight[0].DiffuseIntensity = 0.90f;
    m_pointLight[0].Color = COLOR_RED;
    m_pointLight[0].Position = glm::vec3(5.0f, 0.50f, 5.0f);
    m_pointLight[0].Attenuation.Constant = .50f;
    m_pointLight[0].Attenuation.Linear = .50f;
    m_pointLight[0].Attenuation.Exp = .50f;

    m_pointLight[1].DiffuseIntensity = 0.30f;
    m_pointLight[1].Color = COLOR_BLUE;
    m_pointLight[1].Position = glm::vec3(0.0f, 2.5f, 2.0f);
    m_pointLight[1].Attenuation.Constant = 5.0f;
    m_pointLight[1].Attenuation.Linear = 0.5f;
    m_pointLight[1].Attenuation.Exp = .60f;

    m_pointLight[2].DiffuseIntensity = 0.2f;
    m_pointLight[2].Color = COLOR_GREEN;
    m_pointLight[2].Position = glm::vec3(0.0f, 0.0f, 3.0f);
    m_pointLight[2].Attenuation.Constant = .60f;
    m_pointLight[2].Attenuation.Linear = 0.2f;
    m_pointLight[2].Attenuation.Exp = .2f;
}


void Simulation::InitBoxPositions()
{
    m_boxPositions[0] = glm::vec3(0.0f, 0.0f, 0.0f);
    m_boxPositions[1] = glm::vec3(1.0f, 1.0f, 1.0f);
    m_boxPositions[2] = glm::vec3(-5.0f, -1.0f, 12.0f);
    m_boxPositions[3] = glm::vec3(4.0f, 4.0f, 15.0f);
    m_boxPositions[4] = glm::vec3(-4.0f, 2.0f, 20.0f);
}

/*void Simulation::DSLightPass()
    {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_gbuffer.BindForReading();

        GLint HalfWidth = (GLint) (WINDOW_WIDTH / 2.0f);
        GLint HalfHeight = (GLint) (WINDOW_HEIGHT / 2.0f);

        m_gbuffer.SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
        glBlitFramebuffer(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, HalfWidth, HalfHeight, GL_COLOR_BUFFER_BIT,
                          GL_LINEAR);

        m_gbuffer.SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
        glBlitFramebuffer(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, HalfHeight, HalfWidth, WINDOW_HEIGHT,
                          GL_COLOR_BUFFER_BIT, GL_LINEAR);

        m_gbuffer.SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
        glBlitFramebuffer(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, HalfWidth, HalfHeight, WINDOW_WIDTH, WINDOW_HEIGHT,
                          GL_COLOR_BUFFER_BIT, GL_LINEAR);

        m_gbuffer.SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_TEXCOORD);
        glBlitFramebuffer(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, HalfWidth, 0, WINDOW_WIDTH, HalfHeight,
                          GL_COLOR_BUFFER_BIT, GL_LINEAR);
    }*/
