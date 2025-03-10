#include "SceneNode.h"

SceneNode::SceneNode(Mesh* mesh, Vector4 colour) {
	this->mesh = mesh;
	this->colour = colour;
	parent = NULL;
	modelScale = Vector3(1, 1, 1);
	//scene management
	boundingRadius = 1.0f;
	distanceFromCamera = 1.0f;
}

SceneNode::~SceneNode(void) {
	for (unsigned int i = 0; i < children.size(); ++i) {
		delete children[i];
	}
}

void SceneNode::AddChild(SceneNode* s) {
	children.push_back(s);
	s->parent = this;
}
void SceneNode::Draw(const OGLRenderer& r) {
	if (mesh) { mesh->Draw(); }
}
void SceneNode::Update(float msec) {
	if (parent) { //this node has a parent
		worldTransform = parent->worldTransform * transform;
	}
	else { //root node, world transform is local transfrom
		worldTransform = transform;
	}
	for (vector<SceneNode*>::iterator i = children.begin(); i != children.end(); i++) {
		(*i)->Update(msec);
	}
}
