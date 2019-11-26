#pragma once

#include "..//..//nclgl/Camera.h"
#include "..//..//nclgl/OGLRenderer.h"
#include "HeightMap.h"
#include "..//..//nclgl/MD5Mesh.h"
#include "..//..//nclgl/MD5Node.h"
#include "textmesh.h"
#include "../../nclgl/OBJMesh.h"
#include <sstream>
#include "ParticleEmitter.h"	//A new class!

#define SHADOWSIZE 2048
# define POST_PASSES 10

class Renderer : public OGLRenderer{
public:
	Renderer(Window& parent);
	virtual ~Renderer(void);

	virtual void RenderScene();
	virtual void UpdateScene(float msec);

	void DrawTexts(const std::string& text, const Vector3& position, const float size = 10.0f, const bool perspective = false);

	void SetFps(float s) { fps = s; }

	void	SetShaderParticleSize(float f);	

	ParticleEmitter* emitter;	
protected:
	//tesseletion
	Shader* tessShader;
	//particles
	Shader* particleShader;
	void DrawEmitter();

	float fps;
	//show text on screen
	//using skybox water etc
	void DrawHeightMap();
	void DrawWater();
	void DrawSkybox();
	float waterRotate;

	//postpross
	GLuint bufferFBO;
	GLuint processFBO;
	GLuint bufferDepthTex;
	GLuint bufferColourTex[2];
	void PresentScene();
	void DrawPostProcess();
	void Shaky();
	GLuint fbo, fbo_texture, rbo_depth;
	GLuint vbo_fbo_vertices;

	void DrawHouses();
	void DrawHumans();
	void DrawTrees();
	void DrawLava();
	void DrawMoon();
	void DrawCampFire();
	void DrawLake();
	//using shadows
	void DrawSun();
	void DrawKaiju();
	void DrawShadowScene();
	void DrawCombinedScene();


	Shader* sceneShader;
	Shader* shadowShader;

	GLuint shadowTex;
	GLuint shadowFBO;

	MD5FileData* hellData;
	MD5Node* hellNode;
	MD5FileData* bobData;
	MD5Node* bobNode;

	//using reflections
	Shader* lightShader;
	Shader* reflectShader;
	Shader* skyboxShader;
	Shader* textShader;
	Shader* heightmapshader;
	Shader* skeletonShader;
	Shader* gammaShader;
	Shader* processShader;
	Shader* simpleShader;

	HeightMap* heightMap;
	
	Mesh* quad2;
	Mesh* lava;
	Mesh* quad;
	Mesh* lake;
	

	//always here
	Light* light;
	Light* moonlight;
	Light* moonlight2;
	Light* lavaLight;
	Light* emptyLight;
	Light* sunLight;
	Light* campfireLight;

	Camera* camera;
	Camera* camera2;

	GLuint cubeMap;
	GLuint cubeMapNight;

	GLuint sandTex;
	GLuint rockTex;

	Font* basicFont;

	OBJMesh* sea;
	//OBJMesh* lake;
	OBJMesh* sun;
	OBJMesh* moon;
	OBJMesh* house1;
	OBJMesh* tree1;
	OBJMesh* campfire;
};