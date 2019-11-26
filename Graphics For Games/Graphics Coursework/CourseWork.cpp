#include "../../nclgl/window.h"
#include "../..//GLFW/include/GLFW/glfw3.h"
#include "Renderer.h"


#pragma comment(lib, "nclgl.lib")

int main() {
	Window w("Coursework!", 1920, 1080, true);
	if (!w.HasInitialised()) {
		return -1;
	}

	Renderer renderer(w);
	if (!renderer.HasInitialised()) {
		return -1;
	}

	w.LockMouseToWindow(true);
	w.ShowOSPointer(false);

	int counter = 0;
	float time_acc = 0.0f;
	while (w.UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)) {
		float dt = w.GetTimer()->GetTimedMS();
		time_acc += dt;
		counter++;
		if (time_acc >= 1000.0f) {
			renderer.SetFps(counter);
			time_acc = 0.0f;
			counter = 0;
		}

		renderer.UpdateScene(dt);
		renderer.RenderScene();
	}
	return 0;

	
}
