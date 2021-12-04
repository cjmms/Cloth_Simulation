#include "Engine.h"
#include "Core/Camera.h"
#include "UI_Manager.h"
#include "Scene.h"
#include <iostream>
#include "Renderer.h"
#include "Core/Model.h"
#include "Core/Shader.h"
#include "Core//FBO.h"
#include "ResourceManager.h"
#include <sstream>


#define SHADOW_MAP_DEBUG 0





Camera camera;
UI_Manager UI_Mgr;


void Engine::init()
{
	UI_Mgr.init();

    glEnable(GL_DEPTH_TEST);
    
    // init scene
    scene = new Scene();
    scene->addLightSource(glm::vec3(-20.0f, 35.0f, 10.0f), glm::vec3(550.0f, 550.0f, 550.0f));
    scene->addLightSource(glm::vec3(40.0f, 30.0f, -20.0f), glm::vec3(350.0f, 350.0f, 350.0f));
    scene->addLightSource(glm::vec3(110.0f, 20.0f, -20.0f), glm::vec3(350.0f, 350.0f, 350.0f));
    scene->addLightSource(glm::vec3(-70.0f, 70.0f, 0.0f), glm::vec3(350.0f, 350.0f, 350.0f));

    //scene->addObjects("res/objects/sponza/sponza.obj", glm::vec3(0.1));

    
}

void Engine::close()
{
	UI_Mgr.close();
}



void Engine::run()
{
    Renderer renderer(scene);

    //UI_Mgr.enableCursor();


    while (!UI_Mgr.windowClosed())
    {
        UI_Mgr.update();

        //camera.Print();
        //camera.disable();

        camera.cameraUpdateFrameTime();

        UI_Mgr.NewUIFrame();      

        renderer.Render(scene);
        //renderer.RenderLightSources(scene);

        UI_Mgr.RenderUI();
    }
}