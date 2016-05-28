#pragma once

#include "camera.h"
#include "RenderManager.h"

namespace GlutCBInitializer {
	void init(Camera * camera, RenderManager * renderManager);
}

namespace GlutCB {

	void Display();
	void Reshape(int w, int h);
	void Mouse(int button, int state, int x, int y);
	void Motion(int x, int y);
	void PassiveMotion(int x, int y);
	void Keyboard(unsigned char key, int x, int y);
	void KeyboardUp(unsigned char key, int x, int y);
	void Special(int key, int x, int y);
	void SpecialUp(int key, int x, int y);
	void Timer(int v);
}
