#include "..//..//nclgl/Camera.h"
#include "..//..//nclgl/OGLRenderer.h"
#include "HeightMap.h"
#include "..//..//nclgl/MD5Mesh.h"
#include "..//..//nclgl/MD5Node.h"
#include "textmesh.h"
#include "../../nclgl/OBJMesh.h"
#include <sstream>

#define SHADOWSIZE 2048
#define LIGHTNUM 8


class Renderer : public OGLRenderer{
public:
	Renderer(Window& parent);
	virtual ~Renderer(void);

	virtual void RenderScene();
	virtual void UpdateScene(float msec);

	void DrawTexts(const std::string& text, const Vector3& position, const float size = 10.0f, const bool perspective = false);

	void SetFps(float s) { fps = s; }


protected:
	float fps;
	//show text on screen
	//using skybox water etc
	void DrawHeightMap();
	void DrawWater();
	void DrawSkybox();
	float waterRotate;

	void DrawHouses();
	void DrawHumans();
	void DrawTrees();
	void DrawLava();
	void DrawMoon();
	void DrawCampFire();
	//using shadows
	void DrawSun();
	void DrawKaiju();
	void DrawFloor();
	void DrawShadowScene();
	void DrawCombinedScene();

	Shader* sceneShader;
	Shader* shadowShader;

	GLuint shadowTex;
	GLuint shadowFBO;

	Mesh* floor;
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

	HeightMap* heightMap;
	Mesh* quad;
	Mesh* lava;
	

	//always here
	Light* light;
	Light* moonlight;
	Light* lavaLight;
	Light* emptyLight;
	Light* sunLight;
	Light* campfireLight;

	Camera* camera;

	GLuint cubeMap;
	GLuint sandTex;
	GLuint rockTex;

	Font* basicFont;

	OBJMesh* sun;
	OBJMesh* moon;
	OBJMesh* house1;
	OBJMesh* tree1;
	OBJMesh* campfire;
};