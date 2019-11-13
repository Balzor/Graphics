#include "../../nclgl/window.h"
#include "../..//GLFW/include/GLFW/glfw3.h"
#include "Renderer.h"

#pragma comment(lib, "nclgl.lib")

int main() {
	double lastTime = glfwGetTime();
	int nbFrames = 0;

	Window w("Coursework!", 1200, 1000, false);
	if (!w.HasInitialised()) {
		return -1;
	}

	Renderer renderer(w);
	if (!renderer.HasInitialised()) {
		return -1;
	}

	w.LockMouseToWindow(true);
	w.ShowOSPointer(false);

	while (w.UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)) {
		double currentTime = glfwGetTime();
		nbFrames++;
		if (currentTime - lastTime >= 1.0) {
			printf("%f ms/frame\n", 1000.0 / double(nbFrames));
			nbFrames = 0;
			lastTime += 1.0;
		}

		renderer.UpdateScene(w.GetTimer()->GetTimedMS());
		renderer.RenderScene();
	}
	return 0;
}