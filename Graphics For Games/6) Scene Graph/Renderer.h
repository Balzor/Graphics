#pragma once

#include "SceneNode.h"
#include "Robot.h"
#include "../../nclgl/OGLRenderer.h"
#include "../../nclgl/Camera.h"

class Renderer : public OGLRenderer {
public:
	Renderer(Window& parent);
	virtual ~Renderer(void);

	virtual void UpdateScene(float msec);
	virtual void RenderScene();

protected:
	void DrawNode(SceneNode* n);

	SceneNode* root;
	Camera* camera;
};	