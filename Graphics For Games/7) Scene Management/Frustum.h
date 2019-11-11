#pragma once
#include "Plane.h"
#include "SceneNode.h"
#include "..//..//nclgl/Matrix4.h"
class Matrix4; //w have to compile the matrix4 first

class Frustum {
public:
	Frustum(void) {};
	~Frustum(void) {};
	void FromMatrix(const Matrix4& mvp);
	bool InsideFrustum(SceneNode& n);
protected:
	Plane planes[6];
};