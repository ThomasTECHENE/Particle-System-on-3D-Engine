#ifndef TEXTURE_GENERATOR_2
#define TEXTURE_GENERATOR_2

#include "MaterialGL.h"
#include "Particle.h"
#include <memory.h>
#include "../GPUResources/Textures/GPUTexture2D.h"
#include "../GPUResources/Textures/GPUTexImage2DArray.h"
#include <vector>

class SampleMaterial : public MaterialGL
{
	public:

		Camera* camera;
		SampleMaterial(std::string name);
		~SampleMaterial();

	virtual void render(Node *o);
	virtual void update(Node* o, const int elapsedTime);

	GLuint VBOpos;
	GLuint VBOlife;
	GLuint VBOangle;
	//GLuint VAO = mod->VA_Main;

	GPUTexture2DArray* textureArray;
	GPUsampler* samplerArray;

	GPUTexture2D* texture;
	GPUsampler* sampler;

	GPUTexture2D* texture2;
	GPUsampler* sampler2;

	GPUTexture2D* texture3;
	GPUsampler* sampler3;

	
	GPUvec3* color;
	GPUfloat* time;
	float last_time = 0;
	GPUvec3* translationVector;
	glm::vec3 lastPosition = glm::vec3(0.0,0.0,0.0);
	GPUvec3 * CamPos;

	GPUfloat* GPUdureeVieMax;
	float dureeVieMax = 50.0f;

	int rowParticleAtlas = 8;
	int columnParticleAtlas = 8;
	GPUint* height;
	GPUint* width;

	//données pour la génération des particules
	float maxSpeed = 1.1f;
	float minSpeed = 1.0f;
	float maxPos = -1.5f;
	float minPos = 1.5;
	int maxAngle = -10;
	int minAngle = 10;
	int maxLife = 50;
	int minLife = 1;
	//////////////////////////////////////////
	int maxParticles = 100000;
	int particleAtSameTime = 3000;
	int indSwapParticle;
	bool empty = false;
	Particle* tabParticles;
	ModelGL* mod = new ModelGL(ressourceObjPath + "Quad.obj");

	Particle lastParticleAlive(Particle* tab);
	Particle* fillEmptyTabParticle(Particle* tab);
	
protected:

	GPUmat4* modelViewProjMatrix;
	GPUmat4* modelViewMatrix;

};

#endif