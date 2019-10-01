///////////////////////////////////////////
////Type your name and student ID here/////
////Name:
////Student ID:

#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Dependencies\glm\glm.hpp"
#include "Dependencies\glm\gtc\matrix_transform.hpp"
#include "Dependencies\glm\gtc\type_ptr.hpp"
#include <iostream>
#include <fstream>
using namespace std;
using glm::vec3;
using glm::mat4;

GLint programID;
GLint uniTrans;

GLuint groundVAO;
GLuint groundVBO;
GLuint groundEBO;
GLuint cubeLeftVAO;
GLuint cubeLeftVBO;
GLuint cubeLeftEBO;
GLuint cubeRightVAO;
GLuint cubeRightVBO;
GLuint cubeRightEBO;
GLuint buildingBottomVAO;
GLuint buildingBottomVBO;
GLuint buildingBottomEBO;
GLuint buildingMiddleVAO;
GLuint buildingMiddleVBO;
GLuint buildingMiddleEBO; 
GLuint buildingTopVAO;
GLuint buildingTopVBO;
GLuint buildingTopEBO;
GLuint rooftopBottomVAO;
GLuint rooftopBottomVBO;
GLuint rooftopBottomEBO;
GLuint rooftopTopVAO;
GLuint rooftopTopVBO;
GLuint rooftopTopEBO;
GLuint mountainLeftVAO;
GLuint mountainLeftVBO;
GLuint mountainRightVAO;
GLuint mountainRightVBO;

float x_delta = 0.1f;
int x_press_num = 0;

bool checkStatus(
	GLuint objectID,
	PFNGLGETSHADERIVPROC objectPropertyGetterFunc,
	PFNGLGETSHADERINFOLOGPROC getInfoLogFunc,
	GLenum statusType)
{
	GLint status;
	objectPropertyGetterFunc(objectID, statusType, &status);
	if (status != GL_TRUE)
	{
		GLint infoLogLength;
		objectPropertyGetterFunc(objectID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* buffer = new GLchar[infoLogLength];

		GLsizei bufferSize;
		getInfoLogFunc(objectID, infoLogLength, &bufferSize, buffer);
		cout << buffer << endl;

		delete[] buffer;
		return false;
	}
	return true;
}

bool checkShaderStatus(GLuint shaderID)
{
	return checkStatus(shaderID, glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS);
}

bool checkProgramStatus(GLuint programID)
{
	return checkStatus(programID, glGetProgramiv, glGetProgramInfoLog, GL_LINK_STATUS);
}

string readShaderCode(const char* fileName)
{
	ifstream meInput(fileName);
	if (!meInput.good())
	{
		cout << "File failed to load..." << fileName;
		exit(1);
	}
	return std::string(
		std::istreambuf_iterator<char>(meInput),
		std::istreambuf_iterator<char>()
	);
}

void installShaders()
{
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar* adapter[1];
	//adapter[0] = vertexShaderCode;
	string temp = readShaderCode("VertexShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(vertexShaderID, 1, adapter, 0);
	//adapter[0] = fragmentShaderCode;
	temp = readShaderCode("FragmentShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(fragmentShaderID, 1, adapter, 0);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	if (!checkShaderStatus(vertexShaderID) || !checkShaderStatus(fragmentShaderID))
		return;

	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	if (!checkProgramStatus(programID))
		return;

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	glUseProgram(programID);
}

void keyboard(unsigned char key, int x, int y)
{
	if (key == 'a')
	{
		x_press_num -= 1;
	}
	if (key == 'd')
	{
		x_press_num += 1;
	}
}

void sendDataToOpenGL()
{
	// Vertices
	const GLfloat ground[] = {
		// X	 Y	   Z	 R	   G	 B
		-0.67f,  0.5f, 0.0f, 0.827f, 0.815f, 0.788f, // Top-left
		 0.67f,  0.5f, 0.0f, 0.827f, 0.815f, 0.788f, // Top-right
		 0.5f, -0.5f, 0.0f, 0.827f, 0.815f, 0.788f, // Bottom-right
		-0.5f, -0.5f, 0.0f, 0.827f, 0.815f, 0.788f  // Bottom-left
	};

	const GLfloat cubeLeft[] = {
		-0.2f,  0.0f, 0.00f, 1.0f, 0.0f, 0.0f, // Bottom-Top-left
		-0.1f,  0.0f, 0.00f, 0.0f, 1.0f, 0.0f, // Bottom-Top-right
		-0.1f, -0.1f, 0.00f, 0.196f, 0.192f, 0.223f, // Bottom-Bottom-right
		-0.2f, -0.1f, 0.00f, 0.309f, 0.282f, 0.262f, // Bottom-Bottom-left
		-0.2f,  0.0f, 0.22f, 1.0f, 0.0f, 0.0f, // Top-Top-left
		-0.1f,  0.0f, 0.22f, 0.0f, 1.0f, 0.0f, // Top-Top-right
		-0.1f, -0.1f, 0.22f, 0.235f, 0.215f, 0.203f, // Top-Bottom-right
		-0.2f, -0.1f, 0.22f, 0.317f, 0.294f, 0.337f  // Top-Bottom-left
	};

	const GLfloat cubeRight[] = {
		 0.1f,  0.0f, 0.00f, 1.0f, 0.0f, 0.0f, // Bottom-Top-left
		 0.2f,  0.0f, 0.00f, 0.0f, 1.0f, 0.0f, // Bottom-Top-right
		 0.2f, -0.1f, 0.00f, 0.196f, 0.192f, 0.223f, // Bottom-Bottom-right
		 0.1f, -0.1f, 0.00f, 0.309f, 0.282f, 0.262f, // Bottom-Bottom-left
		 0.1f,  0.0f, 0.15f, 1.0f, 0.0f, 0.0f, // Top-Top-left
		 0.2f,  0.0f, 0.15f, 0.0f, 1.0f, 0.0f, // Top-Top-right
		 0.2f, -0.1f, 0.15f, 0.235f, 0.215f, 0.203f, // Top-Bottom-right
		 0.1f, -0.1f, 0.15f, 0.317f, 0.294f, 0.337f  // Top-Bottom-left
	};

	const GLfloat buildingBottom[] = {
		// X	 Y	   Z	 R	   G	 B
		-0.4f,  0.5f, 0.2f, 0.482f, 0.474f, 0.529f, // Top-left
		 0.4f,  0.5f, 0.2f, 0.482f, 0.474f, 0.529f, // Top-right
		 0.4f,  0.5f, 0.0f, 0.482f, 0.474f, 0.529f, // Bottom-right
		-0.4f,  0.5f, 0.0f, 0.482f, 0.474f, 0.529f  // Bottom-left
	};

	const GLfloat buildingMiddle[] = {
		// X	 Y	   Z	 R	   G	 B
		-0.45f,  0.45f, 0.3f, 0.658f, 0.654f, 0.674f, // Top-left
		 0.45f,  0.45f, 0.3f, 0.658f, 0.654f, 0.674f, // Top-right
		 0.45f,  0.45f, 0.2f, 0.658f, 0.654f, 0.674f, // Bottom-right
		-0.45f,  0.45f, 0.2f, 0.658f, 0.654f, 0.674f  // Bottom-left
	};

	const GLfloat buildingTop[] = {
		// X	 Y	   Z	 R	   G	 B
		-0.5f,  0.42f, 0.5f, 0.713f, 0.737f, 0.784f, // Top-left
		 0.5f,  0.42f, 0.5f, 0.713f, 0.737f, 0.784f, // Top-right
		 0.5f,  0.42f, 0.3f, 0.713f, 0.737f, 0.784f, // Bottom-right
		-0.5f,  0.42f, 0.3f, 0.713f, 0.737f, 0.784f  // Bottom-left
	};

	const GLfloat rooftopBottom[] = {
		// X	 Y	   Z	 R	   G	 B
		-0.3f,  0.5f, 0.6f, 0.772f, 0.788f, 0.823f, // Top-left
		 0.3f,  0.5f, 0.6f, 0.772f, 0.788f, 0.823f, // Top-right
		 0.3f,  0.5f, 0.4f, 0.772f, 0.788f, 0.823f, // Bottom-right
		-0.3f,  0.5f, 0.4f, 0.772f, 0.788f, 0.823f  // Bottom-left
	};

	const GLfloat rooftopTop[] = {
		// X	 Y	   Z	 R	   G	 B
		-0.2f,  0.5f, 0.62f, 0.811f, 0.811f, 0.843f, // Top-left
		 0.2f,  0.5f, 0.62f, 0.811f, 0.811f, 0.843f, // Top-right
		 0.2f,  0.5f, 0.60f, 0.811f, 0.811f, 0.843f, // Bottom-right
		-0.2f,  0.5f, 0.60f, 0.811f, 0.811f, 0.843f  // Bottom-left
	};

	const GLfloat mountainLeft[] = {
		// X	 Y	   Z	 R	   G	 B
		-0.62f,  0.7f,  0.3f, 0.431f, 0.525f, 0.317f, // Top
		-0.30f,  0.7f, -0.1f, 0.431f, 0.525f, 0.317f, // Bottom-right
		-1.00f,  0.7f, -0.1f, 0.431f, 0.525f, 0.317f  // Bottom-left
	};

	const GLfloat mountainRight[] = {
		// X	 Y	   Z	 R	   G	 B
		 0.65f,  0.7f,  0.3f, 0.270f, 0.349f, 0.117f, // Top
		 0.40f,  0.7f, -0.1f, 0.270f, 0.349f, 0.117f, // Bottom-right
		 0.90f,  0.7f, -0.1f, 0.270f, 0.349f, 0.117f  // Bottom-left
	};

	// Indexing
	GLuint rectangleElements[] = {
		0, 1, 2,
		2, 3, 0
	};

	GLuint cubeElements[] = {
		// Bottom
		0, 1, 2,
		2, 3, 0,
		// Top
		4, 5, 6,
		6, 7, 4,
		// Right
		6, 5, 1,
		1, 2, 6,
		// Left
		4, 7, 3,
		3, 0, 4,
		// Near
		7, 6, 2,
		2, 3, 7,
		// Far
		5, 4, 0,
		0, 1, 5
	};

	// Ground
	glGenVertexArrays(1, &groundVAO);
	glBindVertexArray(groundVAO);
	glGenBuffers(1, &groundVBO);
	glBindBuffer(GL_ARRAY_BUFFER, groundVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ground), ground, GL_STATIC_DRAW);
	glGenBuffers(1, &groundEBO); // Element array

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, groundEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rectangleElements), rectangleElements, GL_STATIC_DRAW);

	// Vertex position
	GLint posAttrib = glGetAttribLocation(programID, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);

	// Vertex color
	GLint colAttrib = glGetAttribLocation(programID, "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));

	// Cube Left
	glGenVertexArrays(1, &cubeLeftVAO);
	glBindVertexArray(cubeLeftVAO);
	glGenBuffers(1, &cubeLeftVBO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeLeftVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeLeft), cubeLeft, GL_STATIC_DRAW);
	glGenBuffers(1, &cubeLeftEBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeLeftEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeElements), cubeElements, GL_STATIC_DRAW);

	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);

	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));

	// Cube Right
	glGenVertexArrays(1, &cubeRightVAO);
	glBindVertexArray(cubeRightVAO);
	glGenBuffers(1, &cubeRightVBO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeRightVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeRight), cubeRight, GL_STATIC_DRAW);
	glGenBuffers(1, &cubeRightEBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeRightEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeElements), cubeElements, GL_STATIC_DRAW);

	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);

	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));

	// Buildng Bottom
	glGenVertexArrays(1, &buildingBottomVAO);
	glBindVertexArray(buildingBottomVAO);
	glGenBuffers(1, &buildingBottomVBO);
	glBindBuffer(GL_ARRAY_BUFFER, buildingBottomVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(buildingBottom), buildingBottom, GL_STATIC_DRAW);
	glGenBuffers(1, &buildingBottomEBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buildingBottomEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rectangleElements), rectangleElements, GL_STATIC_DRAW);

	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);

	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));

	// Buildng Middle
	glGenVertexArrays(1, &buildingMiddleVAO);
	glBindVertexArray(buildingMiddleVAO);
	glGenBuffers(1, &buildingMiddleVBO);
	glBindBuffer(GL_ARRAY_BUFFER, buildingMiddleVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(buildingMiddle), buildingMiddle, GL_STATIC_DRAW);
	glGenBuffers(1, &buildingMiddleEBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buildingMiddleEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rectangleElements), rectangleElements, GL_STATIC_DRAW);

	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);

	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));

	// Buildng Top
	glGenVertexArrays(1, &buildingTopVAO);
	glBindVertexArray(buildingTopVAO);
	glGenBuffers(1, &buildingTopVBO);
	glBindBuffer(GL_ARRAY_BUFFER, buildingTopVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(buildingTop), buildingTop, GL_STATIC_DRAW);
	glGenBuffers(1, &buildingTopEBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buildingTopEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rectangleElements), rectangleElements, GL_STATIC_DRAW);

	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);

	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));

	// Rooftop Bottom
	glGenVertexArrays(1, &rooftopBottomVAO);
	glBindVertexArray(rooftopBottomVAO);
	glGenBuffers(1, &rooftopBottomVBO);
	glBindBuffer(GL_ARRAY_BUFFER, rooftopBottomVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rooftopBottom), rooftopBottom, GL_STATIC_DRAW);
	glGenBuffers(1, &rooftopBottomEBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rooftopBottomEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rectangleElements), rectangleElements, GL_STATIC_DRAW);

	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);

	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));

	// Rooftop Top
	glGenVertexArrays(1, &rooftopTopVAO);
	glBindVertexArray(rooftopTopVAO);
	glGenBuffers(1, &rooftopTopVBO);
	glBindBuffer(GL_ARRAY_BUFFER, rooftopTopVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rooftopTop), rooftopTop, GL_STATIC_DRAW);
	glGenBuffers(1, &rooftopTopEBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rooftopTopEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rectangleElements), rectangleElements, GL_STATIC_DRAW);

	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);

	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));

	// Mountain Left
	glGenVertexArrays(1, &mountainLeftVAO);
	glBindVertexArray(mountainLeftVAO);
	glGenBuffers(1, &mountainLeftVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mountainLeftVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(mountainLeft), mountainLeft, GL_STATIC_DRAW);

	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);

	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));

	// Mountain Right
	glGenVertexArrays(1, &mountainRightVAO);
	glBindVertexArray(mountainRightVAO);
	glGenBuffers(1, &mountainRightVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mountainRightVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(mountainRight), mountainRight, GL_STATIC_DRAW);

	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);

	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));

	////////////////////////////////////////////////////

	// Set up view transformation
	mat4 view = lookAt(
		vec3(0.0f, -1.2f,  0.7f),	// Position of the camera
		vec3(0.0f,  0.5f,  0.0f),	// The point to be centered on-screen
		vec3(0.0f,  1.0f,  0.0f)	// The up axis
	);
	GLint uniView = glGetUniformLocation(programID, "view");
	glUniformMatrix4fv(uniView, 1, GL_FALSE, value_ptr(view));

	// Create a perspective projection matrix
	// Specify FOV, aspect ratio, near and far
	mat4 proj = glm::perspective(glm::radians(40.0f), 512.0f / 512.0f, 1.0f, 10.0f);
	GLint uniProj = glGetUniformLocation(programID, "proj");
	glUniformMatrix4fv(uniProj, 1, GL_FALSE, value_ptr(proj));
}

// Transform the object based on the given name
void transform(string name) {
	mat4 model = mat4(1.0f);
	if (name == "ground") {
		glUniformMatrix4fv(uniTrans, 1, GL_FALSE, value_ptr(model));
	}
	else if (name == "cube") {
		model = rotate(mat4(1.0f), x_delta * x_press_num * glm::radians(45.0f), vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniTrans, 1, GL_FALSE, value_ptr(model));
	}
}

void paintGL(void)
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); //specify the background color
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Get reference of model variable
	uniTrans = glGetUniformLocation(programID, "model");

	// Ground
	transform("ground");
	glBindVertexArray(groundVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // Rendering

	// Cube
	transform("cube");
	glBindVertexArray(cubeLeftVAO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	// Cube
	transform("cube");
	glBindVertexArray(cubeRightVAO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	// Building Bottom
	glBindVertexArray(buildingBottomVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	// Building Middle
	glBindVertexArray(buildingMiddleVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	// Building Top
	glBindVertexArray(buildingTopVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	// Rooftop Bottom
	glBindVertexArray(rooftopBottomVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	// Rooftop Top
	glBindVertexArray(rooftopTopVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	// Mountain Left
	glBindVertexArray(mountainLeftVAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// Mountain Right
	glBindVertexArray(mountainRightVAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glFlush();
	glutPostRedisplay();
}

void initializedGL(void) //run only once
{
	installShaders();
	sendDataToOpenGL();
}

int main(int argc, char* argv[])
{
	/*Initialization*/
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(512, 512);
	glutCreateWindow("Assignment 1");
	glewInit();

	/*Register different CALLBACK function for GLUT to response
	with different events, e.g. window sizing, mouse click or
	keyboard stroke */
	initializedGL();
	glutDisplayFunc(paintGL);
	glutKeyboardFunc(keyboard);

	/*Enter the GLUT event processing loop which never returns.
	it will call different registered CALLBACK according
	to different events. */
	glutMainLoop();

	return 0;
}