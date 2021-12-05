#include "Engine.h"
#include "Core/Camera.h"
#include "UI_Manager.h"
#include "Scene.h"
#include <iostream>
#include "Renderer.h"






Camera camera;
UI_Manager UI_Mgr;


void Engine::init()
{
	UI_Mgr.init();
    
    // init scene
    scene = new Scene();   
}

void Engine::close()
{
	UI_Mgr.close();
}



void Engine::run()
{
    Renderer renderer(scene);

    UI_Mgr.enableCursor();

    // render loop
    while (!UI_Mgr.windowClosed())
    {
        UI_Mgr.update();

        //camera.Print();
        camera.disable();

        camera.cameraUpdateFrameTime();

        UI_Mgr.NewUIFrame();      

        renderer.Render(scene);

        UI_Mgr.RenderUI();
    }
}