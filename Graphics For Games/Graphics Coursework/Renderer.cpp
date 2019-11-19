#include "Renderer.h"

Renderer::Renderer(Window& parent) :OGLRenderer(parent) {
	//https://blackboard.ncl.ac.uk/bbcswebdav/pid-3988113-dt-content-rid-13884232_1/courses/J1920-CSC8502/Tutorial%2011%20-%20Geometry%20Shader%20Focus.pdf
	//https://blackboard.ncl.ac.uk/bbcswebdav/pid-3988112-dt-content-rid-13884233_1/courses/J1920-CSC8502/Tutorial%2012%20-%20Tessellation%20Shader%20Focus.pdf
	camera = new Camera();
	camera->SetPosition(Vector3(20.0f, 2000, 5000.0f));

	heightMap = new HeightMap(TEXTUREDIR"/heightmaps/volcano.raw");
	heightMap->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"rocks.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	//heightMap->SetBumpMap(SOIL_load_OGL_texture(TEXTUREDIR"rocks_N.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	quad = Mesh::GenerateQuad();
	quad->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"cartoonWater.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	quad->SetBumpMap(SOIL_load_OGL_texture(TEXTUREDIR"cartoonWater_N.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	lava = Mesh::GenerateQuad();
	lava->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"lava_0.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	lava->SetBumpMap(SOIL_load_OGL_texture(TEXTUREDIR"lava_0_N.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	lavaLight = new Light(Vector3(1000, 900, 0), Vector4(0.81, 0.06, 0.13, 1), 5000.0f);
	if (!lava->GetTexture()) {
		return;
	}
	campfire = new OBJMesh();
	campfire->LoadOBJMesh(MESHDIR"sphere.obj");
	campfire->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"water.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	campfireLight = new Light(Vector3(-1100, 1265, -850), Vector4(0.81, 0.06, 0.13, 1), 100.0f);

	floor = Mesh::GenerateQuad();
	floor->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"Asphalt road texture-1.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	floor->SetBumpMap(SOIL_load_OGL_texture(TEXTUREDIR"asphaltBumpMap.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	sun = new OBJMesh();
	sun->LoadOBJMesh(MESHDIR"sphere.obj");
	sun->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"water.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	light = new Light(Vector3(4000.0f, 4000.0f, .0f), Vector4(1, 1, 1, .3), 50000.0f);

	moon = new OBJMesh();
	moon->LoadOBJMesh(MESHDIR"othersphere.obj");
	moon->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"moon.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	moon->SetBumpMap(SOIL_load_OGL_texture(TEXTUREDIR"moon_N.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	moonlight = new Light(Vector3(-4000.0f, 4000.0f, .0f), Vector4(1, 1, 1, 1), 50000.0f);

	emptyLight = new Light(Vector3(-4000.0f, 4000.0f, .0f), Vector4(1, 1, 1, 1), 0.0f);
	
	house1 = new OBJMesh();
	house1->LoadOBJMesh(MESHDIR"WoodenCabinObj.obj");
	house1->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"houses/WoodCabinDif.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y));
	house1->SetBumpMap(SOIL_load_OGL_texture(TEXTUREDIR"houses/WoodCabinNM.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y));

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
	hellNode->PlayAnim(MESHDIR"idle2.md5anim");

	textShader = new Shader(SHADERDIR"TexturedVertex.glsl", SHADERDIR"TexturedFragment.glsl");

	sceneShader = new Shader(SHADERDIR"shadowscenevert.glsl", SHADERDIR"shadowscenefrag.glsl");

	shadowShader = new Shader(SHADERDIR"shadowVert.glsl", SHADERDIR"shadowFrag.glsl");

	reflectShader = new Shader(SHADERDIR"PerPixelVertex.glsl", SHADERDIR"reflectFragment.glsl");

	skyboxShader = new Shader(SHADERDIR"skyboxVertex.glsl", SHADERDIR"skyboxFragment.glsl");

	lightShader = new Shader(SHADERDIR"PerPixelVertex.glsl", SHADERDIR"PerPixelFragment.glsl");

	heightmapshader = new Shader(SHADERDIR"heightmapVertex.glsl", SHADERDIR"heightmapFragment.glsl");

	if (!heightmapshader->LinkProgram() || !reflectShader->LinkProgram() || !skyboxShader->LinkProgram() || !lightShader->LinkProgram() || !sceneShader->LinkProgram() || !shadowShader->LinkProgram() ||!textShader->LinkProgram()) {
		return;
	}

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

	if ( !cubeMap || !quad->GetTexture() || !heightMap->GetTexture() ) {
		return;
	}

	SetTextureRepeating(floor->GetTexture(), true);
	SetTextureRepeating(tree1->GetTexture(), true);
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
	//glEnable(GL_CULL_FACE);
	
}
Renderer::~Renderer(void) {
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
	delete hellData;
	delete hellNode;
	delete bobData;
	delete bobNode;
	delete heightmapshader;
	delete textShader;

	delete sceneShader;
	delete shadowShader;

	delete basicFont;
	currentShader = NULL;
}
int counter = 0;
void Renderer::UpdateScene(float msec) {
	projMatrix = Matrix4::Perspective(10.0f, 150000.0f, (float)width / (float)height, 45.0f);
	camera->UpdateCamera(msec);
	viewMatrix = camera->BuildViewMatrix();

	//light circling
	 Vector3 lightPos = light->GetPosition();
	 Matrix4 sunRot = Matrix4::Rotation(msec/100,Vector3(0,1,0));
	 lightPos = sunRot * lightPos;
	 light->SetPosition(lightPos);
	//end of light
	//moon circle
	 Vector3 moonPos = moonlight->GetPosition();
	 Matrix4 moonRot = Matrix4::Rotation(msec / 100, Vector3(0, 1, 0));
	 moonPos = moonRot * moonPos;
	 moonlight->SetPosition(moonPos);
	//end moon circle

	waterRotate += msec / 1000.0f;

	//scale model
	if (counter < 100) {
		Vector3 node = hellNode->GetModelScale();
		Matrix4 big = Matrix4::Scale(Vector3(1.01, 1.01, 1.01));
		node = big * node;
		hellNode->SetModelScale(node);
		counter++;
	}
	//end of scale model

	hellNode->Update(msec);
	bobNode->Update(msec);
}

void Renderer::RenderScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	DrawSkybox();
	
	DrawShadowScene();
	DrawCombinedScene();
	
	SwapBuffers();
}

void Renderer::DrawTexts(const std::string& text, const Vector3& position, const float size, const bool perspective) {
	TextMesh* mesh = new TextMesh(text, *basicFont);
	if (perspective) {
		//	modelMatrix = Matrix4::Translation(position) * Matrix4::Scale(Vector3(size, size, 1));
		//	viewMatrix = camera->BuildViewMatrix();
		//	projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);
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

	DrawFloor();
	DrawKaiju();
	DrawHumans();
	DrawHouses();
	DrawTrees();

	UpdateShaderMatrices();
	glUseProgram(0);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glViewport(0, 0, width, height);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::DrawCombinedScene() {
	SetCurrentShader(sceneShader);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "bumpTex"), 1);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "shadowTex"), 2);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "sandTex"), 4);

	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float*)& camera->GetPosition());

	SetShaderLight({ *light, *emptyLight });

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	//glActiveTexture(GL_TEXTURE3);
	//glBindTexture(GL_TEXTURE_2D, sandTex);

	viewMatrix = camera->BuildViewMatrix();

	if (Window::GetKeyboard()->KeyDown(KEYBOARD_L)) {
		viewMatrix = Matrix4::BuildViewMatrix(light->GetPosition(), Vector3(0, 0, 0));
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_M)) {
		viewMatrix = Matrix4::BuildViewMatrix(moonlight->GetPosition(), Vector3(0, 0, 0));
	}

	UpdateShaderMatrices();

	DrawHeightMap();
	DrawLava();
	DrawWater();
	SetCurrentShader(sceneShader);
	DrawFloor();
	DrawSun();
	DrawMoon();
	DrawKaiju();
	DrawHumans();
	DrawHouses();
	DrawTrees();
	DrawCampFire();

	glUseProgram(0);

	//for texts
	SetCurrentShader(textShader);
	glDisable(GL_DEPTH_TEST);
	stringstream s;
	s << fps;
	DrawTexts("FPS:" + s.str(), Vector3(0, 0, 0), 17.0f);
	glEnable(GL_DEPTH_TEST);
}

void Renderer::DrawSkybox() {
	glDepthMask(GL_FALSE);
	glDisable(GL_CULL_FACE);
	SetCurrentShader(skyboxShader);

	UpdateShaderMatrices();

	quad->Draw();
	lava->Draw();

	glUseProgram(0);
	glDepthMask(GL_TRUE);
	glEnable(GL_CULL_FACE);
}

void Renderer::DrawFloor() {
	modelMatrix = Matrix4::Rotation(90, Vector3(1, 0, 0)) *
		Matrix4::Scale(Vector3(100, 100, 1));
		Matrix4 tempMatrix = shadowMatrix * modelMatrix;

	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "shadowMatrix"), 1, false, *&tempMatrix.values);

	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "modelMatrix"), 1, false, *&modelMatrix.values);

	floor->Draw();	
}
float time = 0.0f;
void Renderer::DrawHeightMap() {
	SetCurrentShader(heightmapshader);
	SetShaderLight({ *light, *lavaLight });

	if (time < 1.0f) {
		time += 0.01f;
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
	//glUseProgram(0);
}
void Renderer::DrawLava() {
	SetCurrentShader(reflectShader);
	SetShaderLight({ *emptyLight, *moonlight });
	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float*)& camera->GetPosition());

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "cubeTex"), 2);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);

	modelMatrix = Matrix4::Translation(Vector3(1000, 500, 1)) *
		Matrix4::Scale(Vector3(2000, 1, 2000)) *
		Matrix4::Rotation(90, Vector3(1.0f, 0.0f, 0.0f));

	textureMatrix = Matrix4::Scale(Vector3(10.0f, 10.0f, 10.0f)) *
		Matrix4::Rotation(waterRotate / .1f, Vector3(0.0f, 0.0f, 1.0f));

	UpdateShaderMatrices();

	lava->Draw();

	//glUseProgram(0);
}

void Renderer::DrawWater() {
	glDepthMask(GL_FALSE);
	SetCurrentShader(reflectShader);
	SetShaderLight({ *light, *emptyLight });
	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"),1, (float*)& camera->GetPosition());

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "cubeTex"), 2);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);

	modelMatrix = Matrix4::Translation(Vector3(1,150,1)) *
					Matrix4::Scale(Vector3(1000000, 1, 1000000)) *
					Matrix4::Rotation(90, Vector3(1.0f, 0.0f, 0.0f));

	textureMatrix= Matrix4::Scale(Vector3(1000.0f, 1000.0f, 1000.0f))*
			Matrix4::Rotation(waterRotate/100, Vector3(0.0f, 0.0f, 1.0f));

	UpdateShaderMatrices();

	quad->Draw();

	//glUseProgram(0);
	glDepthMask(GL_TRUE);

 }
void Renderer::DrawKaiju() {
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
	SetCurrentShader(sceneShader);
	SetShaderLight({ *light, *emptyLight });
	for (int i = 0; i < humans; i++) {
		angle = 360 / humans;
		modelMatrix = Matrix4::Translation(Vector3(-1100, 1265, -850)) * Matrix4::Rotation(angle * i, Vector3(0, 1, 0)) * Matrix4::Translation(Vector3(100, 0, 0)) * Matrix4::Rotation(90, Vector3(0, 1, 0)) * (bobNode->GetWorldTransform() * Matrix4::Scale(Vector3(3, 3, 3)));

		//modelMatrix = Matrix4::Translation(Vector3(-1200, 1265, -850)) * (bobNode->GetWorldTransform() * Matrix4::Scale(Vector3(3, 3, 3)));

		Matrix4 tempMatrix = shadowMatrix * modelMatrix;

		UpdateShaderMatrices();

		glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "shadowMatrix"), 1, false, *&tempMatrix.values);

		glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "modelMatrix"), 1, false, *&modelMatrix.values);
		glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "modelMatrix"), 1, false, (float*)& modelMatrix);

		bobNode->Draw(*this);
	}
}
int houses = 10;
void Renderer::DrawHouses() {
	for (int i = 0; i < houses; i++) {
		angle = 360 / houses;
		modelMatrix = Matrix4::Translation(Vector3(-1100, 1265, -850)) * Matrix4::Rotation(angle*i, Vector3(0, 1, 0)) * Matrix4::Translation(Vector3(200, 0, 0)) * Matrix4::Rotation(-90, Vector3(0, 1, 0));

		Matrix4 tempMatrix = shadowMatrix * modelMatrix;

		SetCurrentShader(sceneShader);

		UpdateShaderMatrices();

		glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "shadowMatrix"), 1, false, *&tempMatrix.values);

		glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "modelMatrix"), 1, false, *&modelMatrix.values);

		house1->Draw();
	}
}
//modelMatrix = Matrix4::Translation(Vector3(-1100, 1265, -850))
void Renderer::DrawTrees() {
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
void Renderer::DrawCampFire() {
	float temp = campfireLight->GetRadius();

	modelMatrix = Matrix4::Translation(campfireLight->GetPosition()) * Matrix4::Rotation(90, Vector3(1, 1, 0)) *
		Matrix4::Scale(Vector3(temp, temp, temp));
	Matrix4 tempMatrix = textureMatrix * modelMatrix;

	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "textureMatrix"), 1, false, *&tempMatrix.values);

	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "modelMatrix"), 1, false, *&modelMatrix.values);

	campfire->Draw();
}

