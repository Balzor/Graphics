#include "Renderer.h"

int heightport = 720;
int widthport = 1024;
Renderer::Renderer(Window& parent) :OGLRenderer(parent) {
	Vector2 v = parent.GetScreenSize();
	widthport = v.x;
	heightport = v.y;

	camera = new Camera();
	camera->SetPosition(Vector3(20.0f, 2000, 5000.0f));

	heightMap = new HeightMap(TEXTUREDIR"/heightmaps/volcano.raw");
	heightMap->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"rocks.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	quad2 = Mesh::GenerateQuad();

	quad = Mesh::GenerateQuad();;
	quad->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"cartoonWater.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	quad->SetBumpMap(SOIL_load_OGL_texture(TEXTUREDIR"cartoonWater_N.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	sea = new OBJMesh();
	sea->LoadOBJMesh(MESHDIR"SEA.obj");
	sea->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"cartoonWater.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	sea->SetBumpMap(SOIL_load_OGL_texture(TEXTUREDIR"cartoonWater_N.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	lake = Mesh::GenerateQuad();
	lake->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"cartoonWater.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	lake->SetBumpMap(SOIL_load_OGL_texture(TEXTUREDIR"cartoonWater_N.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	lava = Mesh::GenerateQuad();
	lava->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"lava_0.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	lava->SetBumpMap(SOIL_load_OGL_texture(TEXTUREDIR"lava_0_N.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	lavaLight = new Light(Vector3(1000, 900, 0), Vector4(0.81, 0.06, 0.13, 1), 5000.0f);
	if (!lava->GetTexture()) {
		return;
	}
	campfire = new OBJMesh();
	campfire->LoadOBJMesh(MESHDIR"campfire.obj");
	campfire->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"campfire/DefaultMaterial_albedo.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y));
	campfire->SetBumpMap(SOIL_load_OGL_texture(TEXTUREDIR"campfire/DefaultMaterial_normal.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y));
	campfireLight = new Light(Vector3(-1100, 1265, -850), Vector4(0.81, 0.06, 0.13, 1), 300.0f);
	if (!campfire->GetTexture() || !campfire->GetBumpMap()) {
		return;
	}
	sun = new OBJMesh();
	sun->LoadOBJMesh(MESHDIR"sphere.obj");
	sun->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"water.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	light = new Light(Vector3(4000.0f, 4000.0f, .0f), Vector4(1, 1, 1, .3), 50000.0f);

	moon = new OBJMesh();
	moon->LoadOBJMesh(MESHDIR"othersphere.obj");
	moon->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"moon.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	moon->SetBumpMap(SOIL_load_OGL_texture(TEXTUREDIR"moon_N.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	moonlight = new Light(Vector3(-2000.0f, 4000.0f, .0f), Vector4(1, 1, 1, 1), 50000.0f);
	moonlight2 = new Light(Vector3(-2000.0f, 4000.0f, .0f), Vector4(0.31, 0.41, 0.53, 1), 50000.0f);

	//emptylight
	emptyLight = new Light(Vector3(-4000.0f, 4000.0f, .0f), Vector4(1, 1, 1, 1), 0.0f);
	//rmptylight

	house1 = new OBJMesh();
	house1->LoadOBJMesh(MESHDIR"WoodenCabinObj.obj");
	house1->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"houses/WoodCabinDif.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y));
	house1->SetBumpMap(SOIL_load_OGL_texture(TEXTUREDIR"houses/WoodCabinNM.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y));
	house1->SetPosition(Vector3(-1100, 0, -850));

	tree1 = new OBJMesh();
	tree1->LoadOBJMesh(MESHDIR"plainTree1.obj");
	tree1->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"trees/plainTree1.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	tree1->SetBumpMap(SOIL_load_OGL_texture(TEXTUREDIR"trees/plainTree1_n.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	bobData = new MD5FileData(MESHDIR"bob_lamp_update_export.md5mesh");
	bobNode = new MD5Node(*bobData);

	bobData->AddAnim(MESHDIR"bob_lamp_update_export.md5anim");
	bobNode->PlayAnim(MESHDIR"bob_lamp_update_export.md5anim");

	hellData = new MD5FileData(MESHDIR"hellknight.md5mesh");
	hellNode = new MD5Node(*hellData);

	hellData->AddAnim(MESHDIR"idle2.md5anim");
	hellData->AddAnim(MESHDIR"walk7.md5anim");
	hellData->AddAnim(MESHDIR"attack2.md5anim");
	hellNode->PlayAnim(MESHDIR"idle2.md5anim");

	textShader = new Shader(SHADERDIR"TexturedVertex.glsl", SHADERDIR"TexturedFragment.glsl");

	sceneShader = new Shader(SHADERDIR"shadowscenevert.glsl", SHADERDIR"shadowscenefrag.glsl");

	shadowShader = new Shader(SHADERDIR"shadowVert.glsl", SHADERDIR"shadowFrag.glsl");

	reflectShader = new Shader(SHADERDIR"PerPixelVertex.glsl", SHADERDIR"reflectFragment.glsl");

	skyboxShader = new Shader(SHADERDIR"skyboxVertex.glsl", SHADERDIR"skyboxFragment.glsl");

	lightShader = new Shader(SHADERDIR"PerPixelVertex.glsl", SHADERDIR"PerPixelFragment.glsl");

	heightmapshader = new Shader(SHADERDIR"heightmapVertex.glsl", SHADERDIR"heightmapFragment.glsl");

	skeletonShader = new Shader(SHADERDIR"skeletonVertexSimple.glsl", SHADERDIR"texturedfragment.glsl");

	gammaShader = new Shader(SHADERDIR"gammaVertex.glsl", SHADERDIR"gammaFragment.glsl");

	processShader = new Shader(SHADERDIR"TexturedVertex.glsl", SHADERDIR"processfrag.glsl");

	simpleShader = new Shader(SHADERDIR"simplevertex.glsl", SHADERDIR"simplefragment.glsl");

	particleShader = new Shader(SHADERDIR"vertex.glsl",
		SHADERDIR"fragment.glsl"
		,SHADERDIR"geometry.glsl"
	);

	tessShader = new Shader(SHADERDIR"tessVert.glsl", SHADERDIR"displaceFrag.glsl","", SHADERDIR"displaceTCS.glsl",SHADERDIR"displaceTES.glsl");

	if (!tessShader->LinkProgram() || !particleShader->LinkProgram() || !simpleShader->LinkProgram() || !processShader->LinkProgram() || !gammaShader->LinkProgram() || !heightmapshader->LinkProgram() || !reflectShader->LinkProgram() || !skyboxShader->LinkProgram() || !lightShader->LinkProgram() || !sceneShader->LinkProgram() || !shadowShader->LinkProgram() ||!textShader->LinkProgram()) {
		return;
	}

	emitter = new ParticleEmitter();

	glGenTextures(1, &shadowTex);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOWSIZE, SHADOWSIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	glBindTexture(GL_TEXTURE_2D, 0);
	glGenFramebuffers(1, &shadowFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowTex, 0);
	glDrawBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glGenTextures(1, &bufferDepthTex);
	glBindTexture(GL_TEXTURE_2D, bufferDepthTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height,0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);

	for (int i = 0; i < 2; ++i) {
		glGenTextures(1, &bufferColourTex[i]);
		glBindTexture(GL_TEXTURE_2D, bufferColourTex[i]);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0,GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	}
	glGenFramebuffers(1, &bufferFBO); 
	glGenFramebuffers(1, &processFBO);

	glBindFramebuffer(GL_FRAMEBUFFER, bufferFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D, bufferDepthTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT,GL_TEXTURE_2D, bufferDepthTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D, bufferColourTex[0], 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	basicFont = new Font(SOIL_load_OGL_texture(TEXTUREDIR"tahoma.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_COMPRESS_TO_DXT), 16, 16);

	cubeMap = SOIL_load_OGL_cubemap(
		TEXTUREDIR"skymaps/bluecloud_ft.jpg",
		TEXTUREDIR"skymaps/bluecloud_bk.jpg",
		TEXTUREDIR"skymaps/bluecloud_up.jpg",
		TEXTUREDIR"skymaps/bluecloud_dn.jpg",
		TEXTUREDIR"skymaps/bluecloud_rt.jpg",
		TEXTUREDIR"skymaps/bluecloud_lf.jpg",
		SOIL_LOAD_RGB,
		SOIL_CREATE_NEW_ID, 0
	);
	cubeMapNight = SOIL_load_OGL_cubemap(
		TEXTUREDIR"skymaps/darkskies_ft.tga",
		TEXTUREDIR"skymaps/darkskies_bk.tga",
		TEXTUREDIR"skymaps/darkskies_up.tga",
		TEXTUREDIR"skymaps/darkskies_dn.tga",
		TEXTUREDIR"skymaps/darkskies_rt.tga",
		TEXTUREDIR"skymaps/darkskies_lf.tga",
		SOIL_LOAD_RGB,
		SOIL_CREATE_NEW_ID, 0
	);
	if (!cubeMapNight || !cubeMap || !quad->GetTexture() || !heightMap->GetTexture() ) {
		return;
	}
	SetTextureRepeating(tree1->GetTexture(), true);
	SetTextureRepeating(sea->GetTexture(), true);
	SetTextureRepeating(lake->GetTexture(), true);
	SetTextureRepeating(quad->GetTexture(), true);
	SetTextureRepeating(lava->GetTexture(), true);
	SetTextureRepeating(heightMap->GetTexture(), true);
	SetTextureRepeating(heightMap->GetBumpMap(), true);

	init = true;
	waterRotate = 0.0f;

	projMatrix = Matrix4::Perspective(1.0f, 150000.0f, (float)width / (float)height, 45.0f);

	sandTex = SOIL_load_OGL_texture(TEXTUREDIR"Seamless ground rock.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	glBindTexture(GL_TEXTURE_2D, sandTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D, 0);

	rockTex = SOIL_load_OGL_texture(TEXTUREDIR"moon.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	glBindTexture(GL_TEXTURE_2D, rockTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, sandTex);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, rockTex);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
}
Renderer::~Renderer(void) {
	delete emitter;
	glDeleteTextures(1, &shadowTex);
	glDeleteFramebuffers(1, &shadowFBO);
	delete camera;
	delete heightMap;
	delete quad;
	delete lava;
	delete reflectShader;
	delete skyboxShader;
	delete lightShader;
	delete light;
	delete lavaLight;
	delete moonlight;
	delete moonlight2;
	delete hellData;
	delete hellNode;
	delete bobData;
	delete bobNode;
	delete heightmapshader;
	delete textShader;

	delete sceneShader;
	delete shadowShader;

	delete basicFont;
	glDeleteTextures(2, bufferColourTex);
	glDeleteTextures(1, &bufferDepthTex);
	glDeleteFramebuffers(1, &bufferFBO);
	glDeleteFramebuffers(1, &processFBO);
	currentShader = NULL;
}
int counter = 0;
int counterMove = 0;
bool played=true;
bool playedAttack=true;
bool rotateAnim = true;
bool death=true;
int counterPosition = 0;
bool destroyScene = false;
float time2;
float time3;
bool stop = false;
int speedCam = 100;
void Renderer::UpdateScene(float msec) {
	time2+=msec;
	time3 = msec;
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_Z)) {
		speedCam +=1;
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_X)) {
		speedCam -=1;
		if (speedCam <= 0) {
			speedCam = 0;
		}
	}

	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_P)) {
		stop = !stop;
	}
	if (stop) {
		msec = 0;
	}
	projMatrix = Matrix4::Perspective(10.0f, 150000.0f, (float)width / (float)height, 45.0f);
	emitter->Update(msec);
	camera->UpdateCamera(speedCam);
	viewMatrix = camera->BuildViewMatrix();

	 Vector3 lightPos = light->GetPosition();
	 Matrix4 sunRot = Matrix4::Rotation(time3 /100,Vector3(0,1,0));
	 lightPos = sunRot * lightPos;
	 light->SetPosition(lightPos);

	 Vector3 moonPos = moonlight->GetPosition();
	 Matrix4 moonRot = Matrix4::Rotation(time3 / 50, Vector3(0, 0, 1));
	 moonPos = moonRot * moonPos;
	 moonlight->SetPosition(moonPos);
	 moonlight2->SetPosition(moonPos);

	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_R)) {
		counter = 0;
		counterMove = 0;
		counterPosition = 0;
		played = true;
		playedAttack = true;
		hellNode->SetModelScale(Vector3(1, 1, 1));
		hellNode->PlayAnim(MESHDIR"idle2.md5anim");
		house1->SetPosition(Vector3(-1100, 0, -850));

		Matrix4 move = Matrix4::Translation(Vector3(0, 0, 0));
		hellNode->SetTransform(move);

		Matrix4 move2 = Matrix4::Translation(Vector3(0, 0, 0));
		bobNode->SetTransform(move2);
	}
	if (!stop) {
		waterRotate += msec / 1000.0f;

		if (counter < 100) {
			Vector3 node = hellNode->GetModelScale();
			Matrix4 big = Matrix4::Scale(Vector3(1.01, 1.01, 1.01));
			node = big * node;
			hellNode->SetModelScale(node);
			counter++;
		}
		if (death == true) {
			if (counterMove > 1265 / 5 - 10) {
				if (counterPosition < 300) {
					Matrix4 positiondemon = hellNode->GetWorldTransform();
					Matrix4 movedemon = Matrix4::Translation(Vector3(-2, 0, 0));
					positiondemon = movedemon * positiondemon;
					hellNode->SetTransform(positiondemon);
					if (played) {
						hellNode->PlayAnim(MESHDIR"walk7.md5anim");
						played = false;
					}
				}
				if (counterPosition >= 250) {
					if (playedAttack == true) {
						hellNode->PlayAnim(MESHDIR"attack2.md5anim");
						playedAttack = false;
					}
					Vector3 position = house1->GetPosition();
					Matrix4 positionbob = bobNode->GetWorldTransform();
					Matrix4 move = Matrix4::Translation(Vector3(0, -1, 0));
					Matrix4 movebob = Matrix4::Translation(Vector3(0, -1, 0));
					position = move * position;
					positionbob = movebob * positionbob;
					house1->SetPosition(position);
					bobNode->SetTransform(positionbob);
				}
				if (counterPosition > 300) {
					if (rotateAnim == true) {
						hellNode->SetModelScale(Vector3(-3, 3, -3));
						hellNode->PlayAnim(MESHDIR"walk7.md5anim");
						rotateAnim = false;
					}
					Matrix4 positiondemon = hellNode->GetWorldTransform();
					Matrix4 movedemon = Matrix4::Translation(Vector3(2, 0, 0));
					positiondemon = movedemon * positiondemon;
					hellNode->SetTransform(positiondemon);
				}
				if (counterPosition > 700) {
					Matrix4 positiondemon = hellNode->GetWorldTransform();
					Matrix4 movedemon = Matrix4::Translation(Vector3(1, -2, 1));
					positiondemon = movedemon * positiondemon;
					hellNode->SetTransform(positiondemon);
				}
				counterPosition++;
			}
		}
		if (counterMove < 1265 / 5) {
			Vector3 position = house1->GetPosition();
			Matrix4 positionbob = bobNode->GetWorldTransform();

			Matrix4 move = Matrix4::Translation(Vector3(0, 5, 0));
			Matrix4 movebob = Matrix4::Translation(Vector3(0, 5, 0));

			position = move * position;
			positionbob = movebob * positionbob;

			house1->SetPosition(position);
			bobNode->SetTransform(positionbob);

			counterMove++;
		}
		hellNode->Update(msec);
		bobNode->Update(msec);
	}
	
}
bool divided_view_port=false;
bool show_info = false;
bool blurr=false;
void Renderer::RenderScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	DrawShadowScene();

	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_K)) {
		divided_view_port = !divided_view_port;
	}
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_J)) {
		blurr = !blurr;
	}
	if (!divided_view_port) {
		glViewport(0, 0, width, height);

		if (blurr) {
			glBindFramebuffer(GL_FRAMEBUFFER, bufferFBO);
			glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		}
		DrawCombinedScene();

		if (blurr) {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			DrawPostProcess();
			PresentScene();
		}
	}else{
		projMatrix = Matrix4::Perspective(10.0f, 150000.0f, (float)width / ((float)height / 2), 45.0f);
		
		glViewport(0, 0, width, height / 2);
		DrawCombinedScene();

		projMatrix = Matrix4::Perspective(10.0f, 150000.0f, (float)width / ((float)height/2), 45.0f);
		
		glViewport(0, height / 2, width, height / 2);
		DrawCombinedScene();
	}
	glViewport(0, 0, width, height);

	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_Q)) {
		show_info = !show_info;
	}
	if (show_info) {
		SetCurrentShader(textShader);
		glDisable(GL_DEPTH_TEST);
		stringstream s;
		s << fps;
		int temp = 60;
		DrawTexts("FPS:" + s.str(), Vector3(0, temp, 0), 17.0f);
		DrawTexts("(Z) Increase CamSpeed", Vector3(0, 532 - temp, 0), 17.0f);
		DrawTexts("(X) Lower CamSpeed", Vector3(0, 549 - temp, 0), 17.0f);
		DrawTexts("(P) Freeze Screen", Vector3(0, 566 - temp, 0), 17.0f);
		DrawTexts("(Q) Toggle Help", Vector3(0, -17+600 - temp, 0), 17.0f);
		DrawTexts("(U) Disable autoCam", Vector3(0, 600 - temp, 0), 17.0f);
		DrawTexts("(T) Enable autoCam", Vector3(0, 616 - temp, 0), 17.0f);
		DrawTexts("(L) Hold SunCam", Vector3(0, 633 - temp, 0), 17.0f);
		DrawTexts("(M) Hold MoonCam", Vector3(0, 650 - temp, 0), 17.0f);
		DrawTexts("(F) Destroy Island", Vector3(0, 667 - temp, 0), 17.0f);
		DrawTexts("(G) Rebuild Island", Vector3(0, 684 - temp, 0), 17.0f);
		DrawTexts("(K) Toggle SplitScreen", Vector3(0, 701 - temp, 0), 17.0f);
		DrawTexts("(J) Toggle Blur", Vector3(0, 718 - temp, 0), 17.0f);
		glEnable(GL_DEPTH_TEST);
	}
	SwapBuffers();
}
void Renderer::SetShaderParticleSize(float f) {
	glUniform1f(glGetUniformLocation(currentShader->GetProgram(), "particleSize"), f);
}
void Renderer::DrawTexts(const std::string& text, const Vector3& position, const float size, const bool perspective) {
	TextMesh* mesh = new TextMesh(text, *basicFont);
	if (perspective) {
		return;
	}
	else {
		modelMatrix = Matrix4::Translation(Vector3(position.x, height - position.y, position.z)) * Matrix4::Scale(Vector3(size, size, 1));
		viewMatrix.ToIdentity();
		textureMatrix.ToIdentity();
		projMatrix = Matrix4::Orthographic(-1.0f, 1.0f, (float)width, 0.0f, (float)height, 0.0f);
	}
	UpdateShaderMatrices();
	mesh->Draw();
	delete mesh;
}
bool rebuild = false;

void Renderer::DrawShadowScene() {
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, SHADOWSIZE, SHADOWSIZE);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	SetCurrentShader(shadowShader);
	viewMatrix = Matrix4::BuildViewMatrix(light->GetPosition(), Vector3(0, 0, 0));
	shadowMatrix = biasMatrix * (projMatrix * viewMatrix);

	DrawHeightMap();
	
	DrawLava();
	DrawWater();
	SetCurrentShader(shadowShader);
	DrawMoon();
	if (!destroyScene) {
		DrawKaiju();
		DrawHumans();
		DrawHouses();
		DrawTrees();
		DrawCampFire();
	}
	
	UpdateShaderMatrices();
	glUseProgram(0);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glViewport(0, 0, width, height);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void Renderer::DrawCombinedScene() {
	modelMatrix.ToIdentity();
	textureMatrix.ToIdentity();
	SetCurrentShader(sceneShader);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "bumpTex"), 1);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "shadowTex"), 2);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "sandTex"), 4);

	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float*)& camera->GetPosition());

	SetShaderLight({ *light, *emptyLight,*emptyLight,*emptyLight });

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, shadowTex);

	viewMatrix = camera->BuildViewMatrix();

	if (Window::GetKeyboard()->KeyDown(KEYBOARD_L)) {
		viewMatrix = Matrix4::BuildViewMatrix(light->GetPosition(), Vector3(0, 0, 0));
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_M)) {
		viewMatrix = Matrix4::BuildViewMatrix(moonlight->GetPosition(), Vector3(0, 0, 0));
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_I)) {
		cout<<" pitch: "  << camera->GetPitch();
		cout<<" yaw: "  << camera->GetYaw();
		cout<<" position: "  << camera->GetPosition();
	}

	UpdateShaderMatrices();

	DrawSkybox();
	DrawHeightMap();
	DrawLava();


	DrawWater();

	
	SetCurrentShader(sceneShader);
	if (!destroyScene) {
		DrawKaiju();
		DrawHumans();
		DrawHouses();
		DrawTrees();
		DrawCampFire();
		DrawLake();
	};
	
	DrawSun();
	DrawMoon();
	DrawEmitter();
	

	glUseProgram(0);
}
void Renderer::DrawSkybox() {
	modelMatrix.ToIdentity();
	textureMatrix.ToIdentity();
	glDepthMask(GL_FALSE);
	glDisable(GL_CULL_FACE);
	SetCurrentShader(skyboxShader);

	UpdateShaderMatrices();

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "cubeTex"), 2);

	glActiveTexture(GL_TEXTURE2);

	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);
	if (moonlight->GetPosition().y > 0) {
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);
	}
	else {
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapNight);
	}

	quad->Draw();

	glUseProgram(0);
	glDepthMask(GL_TRUE);
	glEnable(GL_CULL_FACE);
}
float time = 0.0f;
bool start = true;
bool destroy = false;
void Renderer::DrawHeightMap() {
	SetCurrentShader(heightmapshader);
	SetShaderLight({ *light, *lavaLight, *campfireLight,*moonlight2 });

	if (start) {
		if (time < 1.0f) {
			time += 0.01f;
		}
		else {
			start = false;
		}
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_G)) {
		if (time < 1.0f) {
			time += 0.01f;
		}
		rebuild = true;
	}
	if (rebuild) {
		if (time < 1.0f) {
			time += 0.01f;
		}
		else {
			rebuild = false;
		}
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_F)) {
		destroy = true;
	}
	if (destroy) {
		if (time > 0.0f) {
			time -= 0.01f;
		}
		else {
			destroy = false;
		}
		
	}
	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float*)& camera->GetPosition());

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "bumpTex"), 1);

	glUniform1f(glGetUniformLocation(currentShader->GetProgram(), "time"), time);

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "shadowTex"), 2);

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "sandTex"), 4);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, shadowTex);

	modelMatrix = Matrix4::Translation(Vector3(-4000, 1, -4000));
	textureMatrix.ToIdentity();

	UpdateShaderMatrices();
	Matrix4 tempMatrix = shadowMatrix * modelMatrix;

	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "shadowMatrix"), 1, false, *&tempMatrix.values);
	
	heightMap->Draw();
}
float timeLava = 0.0f;
void Renderer::DrawLava() {
	SetCurrentShader(reflectShader);
	SetShaderLight({ *emptyLight,*emptyLight, *emptyLight, *moonlight });
	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float*)& camera->GetPosition());

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "cubeTex"), 2);

	if (!destroyScene) {
		if (timeLava < 1.0f) {
			timeLava += 0.01f;
		}
		else {
			modelMatrix = Matrix4::Translation(Vector3(1000, 500, 1)) *
				Matrix4::Scale(Vector3(2000, 1, 2000)) *
				Matrix4::Rotation(90, Vector3(1.0f, 0.0f, 0.0f));
		}
	}
	
	textureMatrix = Matrix4::Scale(Vector3(10.0f, 10.0f, 10.0f)) *
		Matrix4::Rotation(waterRotate / .1f, Vector3(0.0f, 0.0f, 1.0f));

	if (Window::GetKeyboard()->KeyDown(KEYBOARD_F)) {
		destroyScene = true;
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_G)) {
		destroyScene = false;
		timeLava = 0.0f;
	}
	if (destroyScene) {
		Matrix4 moveLava = Matrix4::Translation(Vector3(1000, 100, 1));
	}

	UpdateShaderMatrices();

	lava->Draw();
}

void Renderer::DrawWater() {
	glDepthMask(GL_FALSE);
	SetCurrentShader(tessShader);
	SetShaderLight({ *light, *emptyLight,*emptyLight,*emptyLight });
	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"),1, (float*)& camera->GetPosition());

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "cubeTex"), 2);

	glUniform1f(glGetUniformLocation(currentShader->GetProgram(), "time"), time2 * 0.01f);

	modelMatrix = Matrix4::Translation(Vector3(1,150,1)) *
					Matrix4::Scale(Vector3(1000000, 1, 1000000)) *
					Matrix4::Rotation(0, Vector3(1.0f, 0.0f, 0.0f));

	textureMatrix= Matrix4::Scale(Vector3(1000.0f, 1000.0f, 1000.0f))*
			Matrix4::Rotation(waterRotate/100, Vector3(0.0f, 0.0f, 1.0f));

	UpdateShaderMatrices();

	vector<Vector3> verts = { 
		Vector3(-1.0f,1.0f,0.0f),
		Vector3(-1.0f,1.0f,0.0f),
		Vector3(1.0f,1.0f,0.0f),
		Vector3(1.0f,1.0f,0.0f) 
	};
	vector<Vector2> texCoords = {
		Vector2(0.0f,1.0f),
		Vector2(0.0f,0.0f),
		Vector2(1.0f,1.0f),
		Vector2(1.0f,0.0f)
	};
	sea->Patches();
	glPatchParameteri(GL_PATCH_VERTICES, 3);

	sea->Draw();
	
	glDepthMask(GL_TRUE);
 }
void Renderer::DrawKaiju() {
	SetShaderLight({ *light, *campfireLight,*lavaLight,*moonlight2 });
	UpdateShaderMatrices();
	modelMatrix = Matrix4::Translation(Vector3(-500,1265,-850))* (hellNode->GetWorldTransform() * Matrix4::Scale(hellNode->GetModelScale()));

	Matrix4 tempMatrix = shadowMatrix * modelMatrix;

	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "shadowMatrix"), 1, false, *&tempMatrix.values);

	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "modelMatrix"), 1, false, *&modelMatrix.values);
	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "modelMatrix"), 1, false, (float*) &modelMatrix);

	hellNode->Draw(*this);	
}
int humans = 10;
float angle = 0.0f;
void Renderer::DrawHumans() {
	SetShaderLight({ *light, *campfireLight,*moonlight2,*emptyLight });
	for (int i = 0; i < humans; i++) {
		angle = 360 / humans;
		modelMatrix = Matrix4::Translation(Vector3(-1100, 0, -850)) * Matrix4::Rotation(angle * i, Vector3(0, 1, 0)) * Matrix4::Translation(Vector3(100, 0, 0)) * Matrix4::Rotation(90, Vector3(0, 1, 0)) * (bobNode->GetWorldTransform() * Matrix4::Scale(Vector3(3, 3, 3)));

		Matrix4 tempMatrix = shadowMatrix * modelMatrix;
		SetCurrentShader(sceneShader);

		UpdateShaderMatrices();

		glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "shadowMatrix"), 1, false, *&tempMatrix.values);

		glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "modelMatrix"), 1, false, *&modelMatrix.values);

		bobNode->Draw(*this);
	}
}
void Renderer::DrawCampFire() {
	float temp = 2000;
	SetShaderLight({ *light, *campfireLight,*emptyLight,*emptyLight });
	modelMatrix = Matrix4::Translation(Vector3(-1100, 1265, -850)) * Matrix4::Scale(Vector3(temp, temp, temp));
	Matrix4 tempMatrix = shadowMatrix * modelMatrix;

	SetCurrentShader(sceneShader);

	UpdateShaderMatrices();

	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "shadowMatrix"), 1, false, *&tempMatrix.values);
	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "modelMatrix"), 1, false, *&modelMatrix.values);

	campfire->Draw();
}
int houses = 10;
void Renderer::DrawHouses() {
	for (int i = 0; i < houses; i++) {
		angle = 360 / houses;
		modelMatrix = Matrix4::Translation(house1->GetPosition()) * Matrix4::Rotation(angle*i, Vector3(0, 1, 0)) * Matrix4::Translation(Vector3(200, 0, 0)) * Matrix4::Rotation(-90, Vector3(0, 1, 0));

		Matrix4 tempMatrix = shadowMatrix * modelMatrix;

		SetCurrentShader(sceneShader);

		UpdateShaderMatrices();

		glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "shadowMatrix"), 1, false, *&tempMatrix.values);
		glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "modelMatrix"), 1, false, *&modelMatrix.values);

		house1->Draw();
	}
}
void Renderer::DrawLake() {
	SetCurrentShader(reflectShader);
	SetShaderLight({ *light, *campfireLight,*moonlight2,*emptyLight });

	modelMatrix = Matrix4::Translation(Vector3(-1100, 1265, -500)) *
		Matrix4::Scale(Vector3(100, 1, 100)) *
		Matrix4::Rotation(90, Vector3(1.0f, 0.0f, 0.0f));

	textureMatrix = Matrix4::Scale(Vector3(100.0f, 100.0f, 100.0f)) *
		Matrix4::Rotation(waterRotate / 100, Vector3(0.0f, 0.0f, 1.0f));
	

	Matrix4 tempMatrix = textureMatrix * modelMatrix;
	UpdateShaderMatrices();

	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float*)& camera->GetPosition());

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "cubeTex"), 2);

	lake->Draw();
}
void Renderer::DrawTrees() {
	SetShaderLight({ *light, *campfireLight,*moonlight2,*emptyLight });
	modelMatrix = Matrix4::Translation(Vector3(-900, 1265, -950));
	Matrix4 tempMatrix = shadowMatrix * modelMatrix;

	SetCurrentShader(sceneShader);

	UpdateShaderMatrices();

	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "shadowMatrix"), 1, false, *&tempMatrix.values);

	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "modelMatrix"), 1, false, *&modelMatrix.values);

	tree1->Draw();
}

void Renderer::DrawSun() {
	float temp = 100;

	modelMatrix = Matrix4::Translation(light->GetPosition())* Matrix4::Rotation(90, Vector3(1, 1, 0)) *
		Matrix4::Scale(Vector3(temp, temp, temp));
		Matrix4 tempMatrix = textureMatrix * modelMatrix;

	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "textureMatrix"), 1, false, *&tempMatrix.values);

	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "modelMatrix"), 1, false, *&modelMatrix.values);

	sun->Draw();
}

void Renderer::DrawMoon() {
	float temp = 100;

	modelMatrix = Matrix4::Translation(moonlight->GetPosition()) * Matrix4::Rotation(90, Vector3(1, 1, 0)) *
		Matrix4::Scale(Vector3(temp, temp, temp));
	Matrix4 tempMatrix = textureMatrix * modelMatrix;

	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "textureMatrix"), 1, false, *&tempMatrix.values);

	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "modelMatrix"), 1, false, *&modelMatrix.values);

	moon->Draw();
}
void Renderer::DrawPostProcess() {
	glBindFramebuffer(GL_FRAMEBUFFER, processFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, bufferColourTex[1], 0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	glViewport(0, 0, width, height);
	SetCurrentShader(processShader);
	projMatrix = Matrix4::Orthographic(-1, 1, 1, -1, -1, 1);
	viewMatrix.ToIdentity();
	modelMatrix.ToIdentity();
	textureMatrix.ToIdentity();
	shadowMatrix.ToIdentity();
	UpdateShaderMatrices();
	UpdateShaderMatrices();

	glDisable(GL_DEPTH_TEST);

	glUniform2f(glGetUniformLocation(currentShader->GetProgram(), "pixelSize"), 1.0f / width, 1.0f / height);

	for (int i = 0; i < POST_PASSES; ++i) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_2D, bufferColourTex[1], 0);
		glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "isVertical"), 0);

		quad2->SetTexture(bufferColourTex[0]);
		quad2->Draw();

		glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "isVertical"), 1);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_2D, bufferColourTex[0], 0);

		quad2->SetTexture(bufferColourTex[1]);
		quad2->Draw();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(0);
	glEnable(GL_DEPTH_TEST);
}
void Renderer::PresentScene() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	SetCurrentShader(textShader);
	projMatrix = Matrix4::Orthographic(-1, 1, 1, -1, -1, 1);
	viewMatrix.ToIdentity();
	modelMatrix.ToIdentity();
	textureMatrix.ToIdentity();
	shadowMatrix.ToIdentity();
	UpdateShaderMatrices();
	quad2->SetTexture(bufferColourTex[0]);
	quad2->Draw();
	glUseProgram(0);
}
void Renderer::DrawEmitter() {
	glClearColor(0, 0, 0, 1);
	SetCurrentShader(particleShader);

	modelMatrix = Matrix4::Translation(Vector3(-1100, 1265, -850));

	glUseProgram(currentShader->GetProgram());
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);

	SetShaderParticleSize(emitter->GetParticleSize());
	
	emitter->SetParticleSize(5.0f);
	emitter->SetParticleVariance(1.0f);
	emitter->SetLaunchParticles(10.0f);
	emitter->SetParticleLifetime(250.0f/2);
	emitter->SetParticleSpeed(.5f);
	UpdateShaderMatrices();

	emitter->Draw();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}