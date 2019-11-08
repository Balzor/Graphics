#pragma once
#include "SceneNode.h"
#include "../../nclgl/OBJMesh.h"

class CuteRobot : public SceneNode {
public:
	CuteRobot(void);
	~CuteRobot(void) {};
	virtual void Update(float msec);

	static void CreateCube() {
		OBJMesh* m = new OBJMesh();
		m->LoadOBJMesh(MESHDIR"cube.obj");
		cube = m;
	}
	static void DeleteCube() { delete cube; }

protected:
	static Mesh* cube;
	SceneNode* body;
	SceneNode* head;
	SceneNode* leftArm;
	SceneNode* rightArm;
};