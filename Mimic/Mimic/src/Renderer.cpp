#include "Renderer.h"
#include "Core/Camera.h"
#include "UI_Manager.h"
#include <iostream>
#include "ResourceManager.h"
#include "Core/FBO.h"
#include "Scene.h"
#include "Core/Shader.h"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"

#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_internal.h"


extern Camera camera;
extern UI_Manager UI_Mgr;



Renderer::Renderer(Scene const* scene)
    :lightShader(new Shader("res/Shaders/basic.shader")),
    debugMode(debugMode)
{

    //std::cout <<  "Renderer Constructor" << std::endl;
	glEnable(GL_DEPTH_TEST);    
}



Renderer::~Renderer()
{}



void Renderer::RenderUI()
{
    ImGui::Begin("UI");                          

    ImGui::Text("UI");

    
    ImGui::End();
}




void Renderer::Render(Scene const* scene)  
{    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    lightShader->setMat4("model", glm::mat4(1.0));

    lightShader->setMat4("view", camera.getViewMatrix());
    lightShader->setMat4("projection", camera.getProjectionMatrix());

    scene->RenderCube(lightShader);


    RenderUI();


}