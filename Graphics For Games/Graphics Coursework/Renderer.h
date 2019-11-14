#include "..//..//nclgl/Camera.h"
#include "..//..//nclgl/OGLRenderer.h"
#include "HeightMap.h"
#include "..//..//nclgl/MD5Mesh.h"
#include "..//..//nclgl/MD5Node.h"
#include "textmesh.h"
#include "../../nclgl/OBJMesh.h"
#include <sstream>

#define SHADOWSIZE 2048

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

	void DrawIsland();
	//using shadows
	void DrawSun();
	void DrawMesh();
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

	HeightMap* heightMap;
	Mesh* quad;
	

	//always here
	Light* light;
	Camera* camera;

	GLuint cubeMap;

	Font* basicFont;

	OBJMesh* sun;
	OBJMesh* sea;
};