#include "Renderer.h"
#include "Core/Camera.h"
#include "UI_Manager.h"
#include <iostream>
#include "Scene.h"
#include "Core/Shader.h"
#include "glm/gtc/matrix_transform.hpp"
#include "Cloth.h"


#include "imgui/imgui.h"

#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_internal.h"


extern Camera camera;
extern UI_Manager UI_Mgr;

Renderer::Renderer(Scene const* scene)
    :clothShader(new Shader("res/Shaders/cloth.shader")),
	sphereShader(new Shader("res/Shaders/sphere.shader")),
    debugMode(debugMode)
{
    //TextureID = ResourceManager::loadTexture("res/Assets/Disco.jpg");
	TextureID = Scene::loadTexture("res/Assets/13.jpg");

	cloth = new Cloth(glm::vec3(0), 16, 16, 4);

	glEnable(GL_DEPTH_TEST); 
	glEnable(GL_MULTISAMPLE);
}



Renderer::~Renderer()
{}



void Renderer::RenderUI()
{
    ImGui::Begin("UI");                          
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	ImGui::Text("wind simulation time: %fms", windSimTime * 1000);
	ImGui::Text("spring simulation time: %fms", springSimTime * 1000);
	ImGui::Text("node simulation time: %fms", nodesSimTime * 1000);
	ImGui::Text("normal update time: %fms", normalUpdateTime * 1000);
	ImGui::Text("gravity simulation time: %fms", gravitySimTime * 1000);

	ImGui::SliderInt("Vertex Size", &vertexSize, 1, 10);
    
    ImGui::End();
}




void Renderer::Render(Scene const* scene)  
{    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	double prev = glfwGetTime();
	double now;

	cloth->SimulateWind(glm::vec3(0.5, 0, 0.1));
	now = glfwGetTime();
	windSimTime = now - prev;
	prev = now;

	cloth->SimulateGravity();
	now = glfwGetTime();
	gravitySimTime = now - prev;
	prev = now;

	cloth->SimulateInternalForce(0.016);
	now = glfwGetTime();
	springSimTime = now - prev;
	prev = now;

	cloth->SimulateNodes(0.016);
	now = glfwGetTime();
	nodesSimTime = now - prev;
	prev = now;

	cloth->UpdateNormal();	// update normals of all vertices
	now = glfwGetTime();
	normalUpdateTime = now - prev;
	prev = now;

	clothShader->setTexture("tex", TextureID);

	clothShader->setMat4("model", glm::mat4(1.0));
	clothShader->setMat4("view", camera.getViewMatrix());
	clothShader->setMat4("projection", camera.getProjectionMatrix());

	clothShader->setVec3("viewPos", camera.getCameraPos());

	clothShader->Bind();
	RenderVertices(cloth->GetTriangles(), vertexSize);
	clothShader->unBind();

    RenderUI();
}


void Renderer::RenderVertices(std::vector<Vertex>& vertices, int size)
{
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER,  vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);	// position

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	glEnableVertexAttribArray(1);	// normal

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoord));
	glEnableVertexAttribArray(2);	// texture Coordinates

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glPointSize(size);
	glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
	//glDrawArrays(GL_POINTS, 0, vertices.size());
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	//glDrawArrays(GL_LINES, 0, vertices.size());
	glBindVertexArray(0);

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}