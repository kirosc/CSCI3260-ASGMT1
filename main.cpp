///////////////////////////////////////////
////Type your name and student ID here/////
////Name:
////Student ID:

#define BUFFER_OFFSET(i) ((char *)NULL + (i * sizeof(float)))
#define _USE_MATH_DEFINES
#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Dependencies\glm\glm.hpp"
#include "Dependencies\glm\gtc\matrix_transform.hpp"
#include "Dependencies\glm\gtc\type_ptr.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
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
GLuint cubeTopVAO;
GLuint cubeTopVBO;
GLuint cubeTopEBO;
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
GLuint sunVAO;
GLuint sunVBO;

// Variable for keyboard actions
float rotate_delta = 0.1f;
float translate_delta = 0.01f;
float scale_delta = 1.0f;
float scale_press_num = 1.0f;
int rotate_press_num = 0;
int translate_press_num = 0;

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
	if (key == 'q')
	{
		rotate_press_num -= 1;
	} 
	else if (key == 'e')
	{
		rotate_press_num += 1;
	}
	else if (key == 'w')
	{
		scale_press_num *= 1.01;
	}
	else if (key == 's')
	{
		scale_press_num *= 0.99;
	}
	else if (key == 'a')
	{
		translate_press_num -= 1;
	}
	else if (key == 'd')
	{
		translate_press_num += 1;
	}
	else if (key == 'u')
	{
	}
	else if (key == 'b') 
	{
		static int back;
		back ^= 1;
		GLfloat colors[][3] = { { 0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f } };
		glClearColor(colors[0][0], colors[0][1], colors[0][2], 1.0f);
		glutPostRedisplay();
	}
}

void drawCircle(GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLint numberOfSides, vector<GLfloat> &allCircleVertices) {
	const int numberOfVertices = numberOfSides + 2;

	GLfloat twicePi = 2.0f * M_PI;

	GLfloat* circleVerticesX = new GLfloat[numberOfVertices];
	GLfloat* circleVerticesY = new GLfloat[numberOfVertices];
	GLfloat* circleVerticesZ = new GLfloat[numberOfVertices];

	circleVerticesX[0] = x;
	circleVerticesY[0] = y;
	circleVerticesZ[0] = z;

	// Calculate the vertices position
	for (int i = 1; i < numberOfVertices; i++)
	{
		circleVerticesX[i] = x + (radius * cos(i * twicePi / numberOfSides));
		circleVerticesY[i] = y;
		circleVerticesZ[i] = z + (radius * sin(i * twicePi / numberOfSides));
	}

	// Assign the position to the GLfloat array
	for (int i = 0; i < numberOfVertices; i++)
	{
		allCircleVertices.push_back(circleVerticesX[i]);
		allCircleVertices.push_back(circleVerticesY[i]);
		allCircleVertices.push_back(circleVerticesZ[i]);
	}
}

void sendDataToOpenGL()
{
	// Vertices
	const GLfloat ground[] = {
		// X	 Y	   Z	 R	   G	 B
		-0.68f,  0.5f, 0.0f, 0.827f, 0.815f, 0.788f, // Top-left
		 0.68f,  0.5f, 0.0f, 0.827f, 0.815f, 0.788f, // Top-right
		 0.50f, -0.5f, 0.0f, 0.827f, 0.815f, 0.788f, // Bottom-right
		-0.50f, -0.5f, 0.0f, 0.827f, 0.815f, 0.788f  // Bottom-left
	};

	const GLfloat cubeLeft[] = {
		-0.2f,  0.0f, 0.00f, 0.917f, 0.878f, 0.831f, // Bottom-Top-left
		-0.1f,  0.0f, 0.00f, 0.909f, 0.827f, 0.729f, // Bottom-Top-right
		-0.1f, -0.1f, 0.00f, 0.196f, 0.192f, 0.223f, // Bottom-Bottom-right
		-0.2f, -0.1f, 0.00f, 0.309f, 0.282f, 0.262f, // Bottom-Bottom-left
		-0.2f,  0.0f, 0.22f, 0.917f, 0.878f, 0.831f, // Top-Top-left
		-0.1f,  0.0f, 0.22f, 0.560f, 0.501f, 0.427f, // Top-Top-right
		-0.1f, -0.1f, 0.22f, 0.235f, 0.215f, 0.203f, // Top-Bottom-right
		-0.2f, -0.1f, 0.22f, 0.486f, 0.450f, 0.443f  // Top-Bottom-left
	};

	const GLfloat cubeRight[] = {
		 0.1f,  0.0f, 0.00f, 0.917f, 0.878f, 0.831f, // Bottom-Top-left
		 0.2f,  0.0f, 0.00f, 0.909f, 0.827f, 0.729f, // Bottom-Top-right
		 0.2f, -0.1f, 0.00f, 0.196f, 0.192f, 0.223f, // Bottom-Bottom-right
		 0.1f, -0.1f, 0.00f, 0.309f, 0.282f, 0.262f, // Bottom-Bottom-left
		 0.1f,  0.0f, 0.15f, 0.917f, 0.878f, 0.831f, // Top-Top-left
		 0.2f,  0.0f, 0.15f, 0.560f, 0.501f, 0.427f, // Top-Top-right
		 0.2f, -0.1f, 0.15f, 0.235f, 0.215f, 0.203f, // Top-Bottom-right
		 0.1f, -0.1f, 0.15f, 0.411f, 0.376f, 0.364f  // Top-Bottom-left
	};

	const GLfloat cubeTop[] = {
		 0.20f, -0.10f, 0.15f, 0.235f, 0.215f, 0.203f, // Bottom-Top-left
		 0.20f,  0.00f, 0.15f, 0.909f, 0.827f, 0.729f, // Bottom-Top-right
		 0.22f,  0.00f, 0.22f, 0.560f, 0.501f, 0.427f, // Bottom-Bottom-right
		 0.22f, -0.10f, 0.22f, 0.235f, 0.215f, 0.203f, // Bottom-Bottom-left
		-0.10f, -0.10f, 0.22f, 0.235f, 0.215f, 0.203f, // Top-Top-left
		-0.10f,  0.00f, 0.22f, 0.917f, 0.878f, 0.831f, // Top-Top-right
		-0.08f,  0.00f, 0.29f, 0.917f, 0.878f, 0.831f, // Top-Bottom-right
		-0.08f, -0.10f, 0.29f, 0.560f, 0.501f, 0.427f, // Top-Bottom-left
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
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), BUFFER_OFFSET(3));

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
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), BUFFER_OFFSET(3));

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
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), BUFFER_OFFSET(3));

	// Cube Top
	glGenVertexArrays(1, &cubeTopVAO);
	glBindVertexArray(cubeTopVAO);
	glGenBuffers(1, &cubeTopVBO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeTopVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeTop), cubeTop, GL_STATIC_DRAW);
	glGenBuffers(1, &cubeTopEBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeTopEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeElements), cubeElements, GL_STATIC_DRAW);

	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);

	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), BUFFER_OFFSET(3));

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
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), BUFFER_OFFSET(3));

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
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), BUFFER_OFFSET(3));

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
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), BUFFER_OFFSET(3));

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
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), BUFFER_OFFSET(3));

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
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), BUFFER_OFFSET(3));

	// Mountain Left
	glGenVertexArrays(1, &mountainLeftVAO);
	glBindVertexArray(mountainLeftVAO);
	glGenBuffers(1, &mountainLeftVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mountainLeftVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(mountainLeft), mountainLeft, GL_STATIC_DRAW);

	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);

	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), BUFFER_OFFSET(3));

	// Mountain Right
	glGenVertexArrays(1, &mountainRightVAO);
	glBindVertexArray(mountainRightVAO);
	glGenBuffers(1, &mountainRightVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mountainRightVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(mountainRight), mountainRight, GL_STATIC_DRAW);

	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);

	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), BUFFER_OFFSET(3));

	// Sun
	glGenVertexArrays(1, &sunVAO);
	glBindVertexArray(sunVAO);
	glGenBuffers(1, &sunVBO);
	glBindBuffer(GL_ARRAY_BUFFER, sunVBO);
	vector <GLfloat> sunVector;	// Create a vector for vertices position
	drawCircle(-0.55f, 0.71f, 0.52f, 0.05f, 50, sunVector);
	GLfloat* sun = sunVector.data(); // Return pointer of an array of vertices
	
	glBufferData(GL_ARRAY_BUFFER, sunVector.size() * sizeof(float), sun, GL_STATIC_DRAW);
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	////////////////////////////////////////////////////

	// Set up view transformation
	mat4 view = lookAt(
		vec3(0.0f, -1.2f,  0.8f),	// Position of the camera
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
	if (name == "cubeTop" || name == "cubeLeft" || name == "cubeRight") {
		model = glm::rotate(mat4(1.0f), rotate_delta * rotate_press_num * glm::radians(45.0f), vec3(0.0f, 0.0f, 1.0f));
		model *= glm::translate(mat4(1.0f), vec3(translate_delta * translate_press_num, 0.0f, 0.0f));
		glUniformMatrix4fv(uniTrans, 1, GL_FALSE, value_ptr(model));
	}
	else if (name == "mountainLeft" || name == "mountainRight")
	{
		model = glm::scale(mat4(1.0f), vec3(scale_delta * scale_press_num, 1.0f, scale_delta * scale_press_num));
		glUniformMatrix4fv(uniTrans, 1, GL_FALSE, value_ptr(model));
	}
	else {
		glUniformMatrix4fv(uniTrans, 1, GL_FALSE, value_ptr(model));
	}
}

void paintGL(void)
{
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Get reference of model variable
	uniTrans = glGetUniformLocation(programID, "model");

	// Ground
	transform("ground");
	glBindVertexArray(groundVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // Rendering

	// Cube Left
	transform("cubeLeft");
	glBindVertexArray(cubeLeftVAO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	// Cube Right
	transform("cubeRight");
	glBindVertexArray(cubeRightVAO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	// Cube Top
	transform("cubeTop");
	glBindVertexArray(cubeTopVAO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	// Building Bottom
	transform("buildingBottom");
	glBindVertexArray(buildingBottomVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	// Building Middle
	transform("buildingMiddle");
	glBindVertexArray(buildingMiddleVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	// Building Top
	transform("buildingTop");
	glBindVertexArray(buildingTopVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	// Rooftop Bottom
	transform("rooftopBottom");
	glBindVertexArray(rooftopBottomVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	// Rooftop Top
	transform("rooftopTop");
	glBindVertexArray(rooftopTopVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	// Mountain Left
	transform("mountainLeft");
	glBindVertexArray(mountainLeftVAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// Mountain Right
	transform("mountainRight");
	glBindVertexArray(mountainRightVAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glBindVertexArray(sunVAO);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 52);

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
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Paint the background color once
	glutDisplayFunc(paintGL);
	glutKeyboardFunc(keyboard);

	/*Enter the GLUT event processing loop which never returns.
	it will call different registered CALLBACK according
	to different events. */
	glutMainLoop();

	return 0;
}