#include "Camera.h"

/*
Polls the camera for keyboard / mouse movement.
Should be done once per frame! Pass it the msec since
last frame (default value is for simplicities sake...)
*/
float lerp(float x, float y, float z) {
	//return(1 - z) * x + z * y;
	return x + z * (y - x);
	
}

void Camera::SetMovePosition() {
	cameraPosition.push_back(Vector3(395.723, 4765.44, 3690.96));
	cameraPosition.push_back(Vector3(-1537.94, 1507.27, -448.692));
	cameraPosition.push_back(Vector3(172.605, 1941.04, -286.829));
	cameraPosition.push_back(Vector3(1661.2, 1941.04, -294.37));
	cameraPosition.push_back(Vector3(309.659, 3224.31, 1517.42));
}
void Camera::SetMovePitch() {
	cameraPitch.push_back(Vector2(-36.7313, 24.9861));
	cameraPitch.push_back(Vector2(-12.7766, -45));
	cameraPitch.push_back(Vector2(-28.9552, 57.1643));
	cameraPitch.push_back(Vector2(-37.4951, 85.9956));
	cameraPitch.push_back(Vector2(-50.9335, 23.049));
}

bool autoCamera=1;
void Camera::UpdateCamera(float msec)	{
	//Update the mouse by how much
	pitch -= (Window::GetMouse()->GetRelativePosition().y);
	yaw	  -= (Window::GetMouse()->GetRelativePosition().x);

	//Bounds check the pitch, to be between straight up and straight down ;)
	pitch = min(pitch,90.0f);
	pitch = max(pitch,-90.0f);

	if(yaw <0) {
		//yaw+= 360.0f;
	}
	if(yaw > 360.0f) {
		//yaw -= 360.0f;
	}

	msec *= 1.0f;

	if(Window::GetKeyboard()->KeyDown(KEYBOARD_W)) {
		position += Matrix4::Rotation(yaw, Vector3(0,1,0)) * Vector3(0,0,-1) * msec;
	}
	if(Window::GetKeyboard()->KeyDown(KEYBOARD_S)) {
		position -= Matrix4::Rotation(yaw, Vector3(0,1,0)) * Vector3(0,0,-1) * msec;
	}

	if(Window::GetKeyboard()->KeyDown(KEYBOARD_A)) {
		position += Matrix4::Rotation(yaw, Vector3(0,1,0)) * Vector3(-1,0,0) * msec;
	}
	if(Window::GetKeyboard()->KeyDown(KEYBOARD_D)) {
		position -= Matrix4::Rotation(yaw, Vector3(0,1,0)) * Vector3(-1,0,0) * msec;
	}

	if(Window::GetKeyboard()->KeyDown(KEYBOARD_SHIFT)) {
		position.y += msec;
	}
	if(Window::GetKeyboard()->KeyDown(KEYBOARD_SPACE)) {
		position.y -= msec;
	}
	temp+=0.01f;
	if (autoCamera) {
		position=Vector3(lerp(this->GetPosition().x,cameraPosition[i].x,0.03f),
			lerp(this->GetPosition().y, cameraPosition[i].y, 0.03f), 
			lerp(this->GetPosition().z, cameraPosition[i].z, 0.03f));


		this->SetPitch(lerp(this->GetPitch(), cameraPitch[i].x, 0.03f));
		this->SetYaw(lerp(this->GetYaw(), cameraPitch[i].y, 0.03f));

		if (temp >= 3.0f) {
			i++;
			temp = 0;
		}
		if (i >= cameraPosition.size()) {
			i = 0;
		}
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_T)) {
		autoCamera = true;
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_U)) {
		autoCamera = false;
	}
}

/*
Generates a view matrix for the camera's viewpoint. This matrix can be sent
straight to the shader...it's already an 'inverse camera' matrix.
*/
Matrix4 Camera::BuildViewMatrix()	{
	//Why do a complicated matrix inversion, when we can just generate the matrix
	//using the negative values ;). The matrix multiplication order is important!
	return	Matrix4::Rotation(-pitch, Vector3(1,0,0)) * 
			Matrix4::Rotation(-yaw, Vector3(0,1,0)) * 
			Matrix4::Translation(-position);
};
