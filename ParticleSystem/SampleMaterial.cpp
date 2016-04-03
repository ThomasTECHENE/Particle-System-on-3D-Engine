#include "SampleMaterial.h"
#include "Node.h"



SampleMaterial::SampleMaterial(std::string name) :
MaterialGL(name,"SampleMaterial")
{
	modelViewProjMatrix = vp->uniforms()->getGPUmat4("CPU_modelViewProjMatrix");
	modelViewMatrix = vp->uniforms()->getGPUmat4("CPU_modelViewMatrix");
	//viewMatrix = vp->uniforms()->getGPUmat4("viewMatrix");
	CamPos = vp->uniforms()->getGPUvec3("CamPos");

	
	
	glEnable(GL_BLEND);
	//additive blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	//transparency
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//premiere texture
	texture = new GPUTexture2D("C:/Users/Thomas/Desktop/GoblimNouvelleversion/Goblim-Win32-core.git/SampleProject/Objets/Textures/flame_transparent.png");
	sampler = fp->uniforms()->getGPUsampler("sampler");
	sampler->Set(0);

	texture2 = new GPUTexture2D("C:/Users/Thomas/Desktop/GoblimNouvelleversion/Goblim-Win32-core.git/SampleProject/Objets/Textures/smoke1.png");
	sampler2 = fp->uniforms()->getGPUsampler("sampler2");
	sampler2->Set(1);

	texture3 = new GPUTexture2D("C:/Users/Thomas/Desktop/GoblimNouvelleversion/Goblim-Win32-core.git/SampleProject/Objets/Textures/sparkTransparent.png");
	sampler3 = fp->uniforms()->getGPUsampler("sampler3");
	sampler3->Set(2);

	//textureArray = new GPUTexture2DArray("textureArray", 1024, 1024, 2, GL_RGBA, GL_RGBA, GL_FLOAT);
	/*textureArray = new GPUTexture2DArray("textureArray");
	textureArray->create(2048, 2048, 3);
	//textureArray->addLayer("C:/Users/Thomas/Desktop/GoblimNouvelleversion/Goblim-Win32-core.git/SampleProject/Objets/Textures/flame_transparent.png", 0, true);
	textureArray->addLayer("C:/Users/Thomas/Desktop/GoblimNouvelleversion/Goblim-Win32-core.git/SampleProject/Objets/Textures/flame_transparent.png", 0);
	samplerArray = fp->uniforms()->getGPUsampler("samplerArray");
	samplerArray->Set(1);*/

	GPUdureeVieMax = fp->uniforms()->getGPUfloat("lifetimeMax");
	GPUdureeVieMax->Set(dureeVieMax);

	height = fp->uniforms()->getGPUint("height");
	height->Set(rowParticleAtlas);
	width = fp->uniforms()->getGPUint("width");
	width->Set(columnParticleAtlas);

	/*color = fp->uniforms()->getGPUvec3("color");
	color->Set(glm::vec3(0.6, 0.0, 0.0));
	

	time = vp->uniforms()->getGPUfloat("temps");

	translationVector = vp->uniforms()->getGPUvec3("translationVector");
	translationVector->Set(glm::vec3(0.0f, 0.1f, 0.0f));*/

	//initialisation du tableau de particules
	tabParticles = new Particle[maxParticles];
	for (int i = 0; i < maxParticles; i++){
		float x = minPos + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxPos - minPos)));
		float z = minPos + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxPos - minPos)));
		float angle = minAngle + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxAngle - minAngle)));
		float speed = minSpeed + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxSpeed - minSpeed)));
		float dureeVie = minLife + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxLife - minLife)));
		Particle p = Particle(glm::vec3(x,0.0,z), dureeVie, angle, speed);
		tabParticles[i] = p;
	}

}


SampleMaterial::~SampleMaterial()
{	

}


void SampleMaterial::render(Node *o)
{	
	if (m_ProgramPipeline)
	{
		
		glClearColor(0.0,0.0,0.0,0.0);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDepthMask(0);
		texture->bind(0);	
		texture2->bind(1);
		texture3->bind(2);
		m_ProgramPipeline->bind();
		//o->drawGeometry(GL_TRIANGLES);
		mod->drawInstancedGeometry(GL_TRIANGLES, particleAtSameTime);
		m_ProgramPipeline->release();
		texture->release();
		texture2->release();
		texture3->release();
		glDepthMask(1);
	}	
}

void SampleMaterial::update(Node* o, const int elapsedTime)
{
	if (o->frame()->updateNeeded())
	{
		Scene* scene = Scene::getInstance();
		//matrice modelView
		modelViewMatrix->Set(scene->camera()->getModelViewMatrix(o->frame()));
		//position de la caméra
		glm::vec3 cameraPos = scene->camera()->convertPtTo(glm::vec3(0.0), scene->getRoot()->frame());
		CamPos->Set(cameraPos);
		modelViewProjMatrix->Set(o->frame()->getTransformMatrix());
		o->frame()->setUpdate(true);
		//o->frame()->rotate(glm::vec3(0.0,1.0,0.0), 20);
		//tableau de positions mises a jour pour l'envoi dans le buffer
		const int taille = particleAtSameTime;
		glm::vec3* positionsMaj = new glm::vec3[taille];
		//tableau de duree de vie pour les particules
		float* lifetimeMaj = new float[taille];
		//tableau d'angle aléatoire pour les constructions de matrice
		float* angleMaj = new float[taille];
		float angleTmp;
		//maj du tableau de particules
		for (int i = 0; i < particleAtSameTime; i++){
			//particule morte
			if (tabParticles[i].life < 0){
				//swap de particule vivante et morte
				Particle partJustDied = tabParticles[i];
				tabParticles[i] = lastParticleAlive(tabParticles);
				tabParticles[indSwapParticle] = partJustDied;
				// plus de particules en vie
				if (tabParticles[i].life == -10.0){
					tabParticles = fillEmptyTabParticle(tabParticles);
				}
			}
			tabParticles[i].updateParticles(0.1f);
			positionsMaj[i] = tabParticles[i].position;
			tabParticles[i].life--;
			lifetimeMaj[i] = tabParticles[i].life;
			angleTmp = minAngle + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxAngle - minAngle)));
			angleMaj[i] = angleTmp;
		}

		//envoi des données nécessaires sur le gpu (position des particules)
		//buffer de positions
		glGenBuffers(1, &VBOpos);
		glBindBuffer(GL_ARRAY_BUFFER, VBOpos);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * taille, &positionsMaj[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
		//buffer de duree de vie
		glGenBuffers(1, &VBOlife);
		glBindBuffer(GL_ARRAY_BUFFER, VBOlife);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * taille, &lifetimeMaj[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//buffer angle
		glGenBuffers(1, &VBOangle);
		glBindBuffer(GL_ARRAY_BUFFER, VBOangle);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * taille, &angleMaj[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//vertex array du modèle
		GLuint VAO = mod->VA_Main;
		//glGenVertexArrays(1, &VAO); créé un nouveau VAO, pas celui associé au modèle
		glBindVertexArray(VAO);

		glEnableVertexAttribArray(6);
		glBindBuffer(GL_ARRAY_BUFFER, VBOpos);
		glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glVertexAttribDivisor(6, 1);

		glEnableVertexAttribArray(7);
		glBindBuffer(GL_ARRAY_BUFFER, VBOlife);
		glVertexAttribPointer(7, 1, GL_FLOAT, GL_FALSE, sizeof(GLfloat), (GLvoid*)0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glVertexAttribDivisor(7, 1);

		glEnableVertexAttribArray(8);
		glBindBuffer(GL_ARRAY_BUFFER, VBOangle);
		glVertexAttribPointer(8, 1, GL_FLOAT, GL_FALSE, sizeof(GLfloat), (GLvoid*)0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glVertexAttribDivisor(8, 1);

		glBindVertexArray(0);

	}
	

}

Particle SampleMaterial::lastParticleAlive(Particle* tab){
	for (int i = maxParticles-1; i > particleAtSameTime; i--){
		if (tab[i].life > 0){
			indSwapParticle = i;
			return tab[i];
		}
	}
	Particle pNull = Particle(glm::vec3(15.0, 15.0, 15.0), -10.0, 0, 0);
	return pNull;
}

Particle* SampleMaterial::fillEmptyTabParticle(Particle* tab){
	for (int i = particleAtSameTime; i < maxParticles; i++){
		//valeur d'angle entre 0 et 180
		float x = minPos + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxPos - minPos)));
		float z = minPos + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxPos - minPos)));
		float angle = minAngle + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxAngle - minAngle)));
		float speed = minSpeed + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxSpeed - minSpeed)));
		float dureeVie = minLife + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxLife - minLife)));
		Particle p = Particle(glm::vec3(x, 0.0, z), dureeVie, angle, speed);
		tab[i] = p;
	}
	return tab;
}