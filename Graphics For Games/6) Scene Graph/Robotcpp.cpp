#include "Robot.h"

Mesh* CuteRobot::cube = NULL;

Matrix4 cachedHeadTransform;
Matrix4 cachedLeftArmTransform;
Matrix4 cachedRightArmTransform;

CuteRobot::CuteRobot(void) {
	SetMesh(cube);

	body = new SceneNode(cube, Vector4(1, 0, 0, 1)); //red
	body->SetModelScale(Vector3(10, 15, 5));
	body->SetTransform(Matrix4::Translation(Vector3(0, 35, 0)));
	AddChild(body);

	SceneNode* neck = new SceneNode(cube, Vector4(1, 0, 0, 1));
	neck->SetModelScale(Vector3(1, 5, 1.5));
	neck->SetTransform(Matrix4::Translation(Vector3(0, 28, 0)));
	body->AddChild(neck);

	head = new SceneNode(cube, Vector4(0, 1, 0, 1)); //green
	head->SetModelScale(Vector3(5, 5, 5));
	head->SetTransform(Matrix4::Translation(Vector3(0, 33, 0)));
	body->AddChild(head);
	cachedHeadTransform = head->GetTransform();

	SceneNode* eye1 = new SceneNode(cube, Vector4(1, 0, 0, 1));//red
	eye1->SetModelScale(Vector3(1, 1, 1));
	eye1->SetTransform(Matrix4::Translation(Vector3(3, 5, 5)));
	head->AddChild(eye1);

	SceneNode* eye2 = new SceneNode(cube, Vector4(1, 0, 0, 1));//red
	eye2->SetModelScale(Vector3(1, 1, 1));
	eye2->SetTransform(Matrix4::Translation(Vector3(-3, 5, 5)));
	head->AddChild(eye2);

	leftArm = new SceneNode(cube, Vector4(0, 0, 1, 1)); //blue
	leftArm->SetModelScale(Vector3(3, -18, 3));
	leftArm->SetTransform(Matrix4::Translation(Vector3(-12, 30, -1)));
	body->AddChild(leftArm);
	cachedLeftArmTransform = leftArm->GetTransform();

	rightArm = new SceneNode(cube, Vector4(0, 0, 1, 1)); //blue
	rightArm->SetModelScale(Vector3(3, -18, 3));
	rightArm->SetTransform(Matrix4::Translation(Vector3(12, 30, -1)));
	body->AddChild(rightArm);
	cachedRightArmTransform = rightArm->GetTransform();

	SceneNode* leftLeg = new SceneNode(cube, Vector4(0, 0, 1, 1));
	leftLeg->SetModelScale(Vector3(3, -17.5, 3));
	leftLeg->SetTransform(Matrix4::Translation(Vector3(-8, 0, 0)));
	body->AddChild(leftLeg);
	
	SceneNode* rightLeg = new SceneNode(cube, Vector4(0, 0, 1, 1));
	rightLeg->SetModelScale(Vector3(3, -17.5, 3));
	rightLeg->SetTransform(Matrix4::Translation(Vector3(8, 0, 0)));
	body->AddChild(rightLeg);
}

//float counter = 0.f;
bool flip = false;
float rAngle = 50.f;
float accTime = 0.f;
float rSpeed = 10.f;

void CuteRobot::Update(float msec) {
	transform = transform * Matrix4::Rotation(msec / 10.0f, Vector3(0, 1, 0));

	// 0 -> 90 -> 0 -> -90 
	// sin(0) -> sin(pi/2) -> sin(0) -> sin(-pi/2)
	//printf("%.2f\n", sinf(counter));
	//printf("%.2f\n", sinf(PI / 2.f));

	//if (counter >= PI/2.f)
	//	flip = true;
	//
	//if (counter <= -PI/2.f)
	//	flip = false;

	//if (flip)
	//	counter -= 0.16f;
	//else
	//	counter += 0.16f;
	
	printf("%.2f\n", sinf(accTime));
	//printf("%.2f\n", RadToDeg(sinf(counter)));
	//if(counter)
	head->SetTransform(cachedHeadTransform * Matrix4::Rotation(rAngle * sinf(accTime * rSpeed) , Vector3(0, 0, 1)));
	leftArm->SetTransform(cachedLeftArmTransform * Matrix4::Rotation(rAngle * sinf(-accTime * rSpeed), Vector3(1, 0, 0)));
	rightArm->SetTransform(cachedRightArmTransform * Matrix4::Rotation(rAngle * sinf(accTime * rSpeed), Vector3(1, 0, 0)));
	accTime += msec / 1000.f;

	//leftArm
	//rightArm


	SceneNode::Update(msec);
}
