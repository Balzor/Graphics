//#include "Shader.h"
//
//Shader::Shader(string vFile, string fFile, string gFile) {
//	program = glCreateProgram();
//	objects[SHADER_VERTEX] = GenerateShader(vFile, GL_VERTEX_SHADER);
//	objects[SHADER_FRAGMENT] = GenerateShader(fFile, GL_FRAGMENT_SHADER);
//	objects[SHADER_GEOMETRY] = 0;
//	
//	if (!gFile.empty()) {
//		objects[SHADER_GEOMETRY] = GenerateShader(gFile,GL_GEOMETRY_SHADER);
//		glAttachShader(program, objects[SHADER_GEOMETRY]);
//		
//	}
//	glAttachShader(program, objects[SHADER_VERTEX]);
//	glAttachShader(program, objects[SHADER_FRAGMENT]);
//	SetDefaultAttributes();
//}
//Shader ::~Shader(void) {
//	for (int i = 0; i < 3; ++i) {
//		glDetachShader(program, objects[i]);
//		glDeleteShader(objects[i]);
//	}
//	glDeleteProgram(program);
//}
//GLuint Shader::GenerateShader(string from, GLenum type) {
//	cout << " Compiling Shader ... " << endl;
//	string load;
//	if (!LoadShaderFile(from, load)) {
//		cout << " Compiling failed !" << endl;
//		loadFailed = true;
//		return 0;
//		
//	}
//	GLuint shader = glCreateShader(type);
//	
//	const char* chars = load.c_str();
//	glShaderSource(shader, 1, &chars, NULL);
//	glCompileShader(shader);
//	
//	GLint status;
//	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
//	
//	if (status == GL_FALSE) {
//		cout << " Compiling failed !" << endl;
//		char error[512];
//		glGetInfoLogARB(shader, sizeof(error), NULL, error);
//		cout << error;
//		loadFailed = true;
//		return 0;
//		
//	}
//	cout << " Compiling success !" << endl << endl;
//	loadFailed = false;
//	return shader;
//	
//}
// bool Shader::LoadShaderFile(string from, string & into) {
//	 ifstream file;
//	 string temp;
//	
//	cout << " Loading shader text from " << from << endl << endl;
//	
//	file.open(from.c_str());
//	if (!file.is_open()) {
//		 cout << " File does not exist !" << endl;
//		 return false;
//	}
//	while (!file.eof()) {
//		getline(file, temp);
//		into += temp + "\n";
//	}
//	file.close();
//	cout << into << endl << endl;
//	cout << " Loaded shader text !" << endl << endl;
//	return true;
// }
// void Shader::SetDefaultAttributes() {
//	 glBindAttribLocation(program, VERTEX_BUFFER, "position");
//	 glBindAttribLocation(program, COLOUR_BUFFER, "colour");
//	 glBindAttribLocation(program, NORMAL_BUFFER, "normal");
//	 glBindAttribLocation(program, TANGENT_BUFFER, "tangent");
//	 glBindAttribLocation(program, TEXTURE_BUFFER, "texCoord");
//
// }
// bool Shader::LinkProgram() {
//	 if (loadFailed) {
//		 return false;
//	 }
//	 glLinkProgram(program);
//
//	 GLint code;
//	 glGetProgramiv(program, GL_LINK_STATUS, &code);
//	 return code == GL_TRUE ? true : false;
// }
#include "Shader.h"
//Matches our storage order to the magic numbers OpenGL uses!
GLuint GLSLTypes[] =
{
	GL_VERTEX_SHADER,
	GL_FRAGMENT_SHADER,
	GL_GEOMETRY_SHADER,
	GL_TESS_CONTROL_SHADER,
	GL_TESS_EVALUATION_SHADER
};

Shader::Shader(const string& vFile, const string& fFile, const string& gFile, const string& tcsFile, const string& tesFile) {
	shaderNames[SHADER_VERTEX] = vFile;
	shaderNames[SHADER_FRAGMENT] = fFile;
	shaderNames[SHADER_GEOMETRY] = gFile;
	shaderNames[SHADER_TESSCONTROL] = tcsFile;
	shaderNames[SHADER_TESSEVAL] = tesFile;

	objects[SHADER_VERTEX] = 0;
	objects[SHADER_FRAGMENT] = 0;
	objects[SHADER_GEOMETRY] = 0;
	objects[SHADER_TESSCONTROL] = 0;
	objects[SHADER_TESSEVAL] = 0;

	program = 0;

	Reload();
}

Shader::~Shader(void) {
	ClearShaderObjects();
}

void	Shader::ClearShaderObjects() {
	for (int i = 0; i < SHADER_MAX; ++i) {
		glDetachShader(program, objects[i]);
		glDeleteShader(objects[i]);
	}
	glDeleteProgram(program);
}

void	Shader::Reload() {
	ClearShaderObjects();

	program = glCreateProgram();

	for (int i = 0; i < SHADER_MAX; ++i) {
		if (shaderNames[i].empty()) {
			continue;
		}
		objects[i] = GenerateShader(shaderNames[i], GLSLTypes[i]);
		if (objects[i]) {
			glAttachShader(program, objects[i]);
		}
	}

	SetDefaultAttributes();

	LinkProgram();
}


bool	Shader::LoadShaderFile(const string& from, string& into) {
	ifstream	file;
	string		temp;

	cout << "Loading shader text from " << from << endl << endl;

	file.open(from.c_str());
	if (!file.is_open()) {
		cout << "File does not exist!" << endl;
		return false;
	}

	while (!file.eof()) {
		getline(file, temp);
		into += temp + "\n";
	}

	cout << into << endl << endl;

	file.close();
	cout << "Loaded shader text!" << endl << endl;
	return true;
}

GLuint	Shader::GenerateShader(const string& from, GLenum type) {
	cout << "Compiling Shader..." << endl;

	string load;
	if (!LoadShaderFile(from, load)) {
		cout << "Compiling failed!" << endl;
		loadFailed = true;
		return 0;
	}

	GLuint shader = glCreateShader(type);

	const char* chars = load.c_str();
	glShaderSource(shader, 1, &chars, NULL);
	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (status == GL_FALSE) {
		cout << "Compiling failed!" << endl;
		char error[2048];
		glGetShaderInfoLog(shader, 2048, NULL, error);

		cout << error;
		loadFailed = true;
		return 0;
	}
	cout << "Compiling success!" << endl << endl;
	loadFailed = false;
	return shader;
}

bool Shader::LinkProgram() {
	if (loadFailed) {
		return false;
	}
	glLinkProgram(program);

	GLint status = 1;
	glGetShaderiv(program, GL_LINK_STATUS, &status);

	if (status != GL_TRUE) {
		cout << "Linking failed!" << endl;
		char error[8192];
		glGetProgramInfoLog(program, 8192, NULL, error);
		cout << error;
		loadFailed = true;
		return false;
	}
	cout << "Linking success!" << endl;
	return true;
}


void	Shader::SetDefaultAttributes() {
	glBindAttribLocation(program, VERTEX_BUFFER, "position");
	glBindAttribLocation(program, COLOUR_BUFFER, "colour");
	glBindAttribLocation(program, NORMAL_BUFFER, "normal");
	glBindAttribLocation(program, TANGENT_BUFFER, "tangent");
	glBindAttribLocation(program, TEXTURE_BUFFER, "texCoord");
}