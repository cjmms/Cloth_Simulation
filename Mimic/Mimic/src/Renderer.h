#pragma once

class Scene;
class Shader;

/*
* Purpose of Renderer: render object base on input, hide all details of how rendering works
* 
* The renderer will render base on the infromation inside scene object
* There are 2 types of renderer: deferred renderer, forward renderer.
* 
* 1. Renderer will init and run differerntly base on its type
* 
* 2. Type can't be changed
* 
* 3. Shaders will be inside the renderer, bind/unbind in side the renderer, pass uniform inside renderer
* 
* 4. Other part of the engine should not access shader at all
*/





class Renderer
{
private:
	Shader *lightShader;
	bool debugMode;


	void RenderUI();



public:
	Renderer(Scene const* scene);


	~Renderer();
	
	void Render(Scene const* scene);
	
};

