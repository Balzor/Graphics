#include "..//..//nclgl/Camera.h"
#include "..//..//nclgl/OGLRenderer.h"
#include "HeightMap.h"
#include "..//..//nclgl/MD5Mesh.h"
#include "..//..//nclgl/MD5Node.h"
#include "textmesh.h"

#define SHADOWSIZE 2048

class Renderer : public OGLRenderer{
public:
	Renderer(Window& parent);
	virtual ~Renderer(void);

	virtual void RenderScene();
	virtual void UpdateScene(float msec);
protected:
	//show text on screen
	void DrawText(const std::string& text, const Vector3& position, const float size = 10.0f, const bool perspective = false);
	//using skybox water etc
	void DrawHeightMap();
	void DrawWater();
	void DrawSkybox();
	float waterRotate;

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

	//using reflections
	Shader* lightShader;
	Shader* reflectShader;
	Shader* skyboxShader;

	HeightMap* heightMap;
	Mesh* quad;

	//always here
	Light* light;
	Camera* camera;

	GLuint cubeMap;

	Font* basicFont;

	Mesh* sun;
};