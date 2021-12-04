#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

class Cube
{
private:
	glm::mat4 scale;
	glm::mat4 rotate;
	glm::mat4 translate;

	glm::mat4 phi;
	glm::mat4 theta;
	glm::mat4 psi;

	// mesh
	unsigned int VBO, cubeVAO;

	// create Mesh
	void Init();

public:
	Cube(glm::vec3 scale, glm::vec3 translate);

	// delete move, delete mesh
	~Cube();

	// bind VAO, Draw cube
	void Draw();

	// setters and getters
	inline void setrotate(glm::mat4 rotat) { this->rotate = rotat; }
	inline glm::mat4 getrotate() const { return this->rotate; }

	inline void setTranslate(glm::mat4 translate) { this->translate = translate; }
	inline glm::mat4 getTranslate() const { return this->translate; }

	inline glm::mat4 getscale() const { return this->scale; }

	inline glm::mat4 getphi() { return this->phi; }
	inline glm::mat4 gettheta() {return this->theta;}
	inline glm::mat4 getpsi() { return this->psi; }

	inline void setpsi(glm::mat4 psi) { this->psi = psi; }
	inline void setphi(glm::mat4 phi) { this->phi = phi; }
	inline void setthata(glm::mat4 theta) { this->theta = theta; }
};

