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

GLuint groundVAO;
GLuint groundVBO;
GLuint groundEBO;
GLuint cubeVAO;
GLuint cubeVBO;
GLuint cubeEBO;

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
	//TODO:
	//create point, line, 2D object and 3D object here and bind to VAOs & VBOs

	const GLfloat ground[] = {
		// X	 Y	   Z	 R	   G	 B
		-0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // Top-left
		 0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // Top-right
		 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // Bottom-right
		-0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f  // Bottom-left
	};

	const GLfloat cube[] = {
		-0.1f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // Bottom-Top-left
		 0.0f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f, // Bottom-Top-right
		 0.0f, -0.1f, 0.0f, 1.0f, 1.0f, 1.0f, // Bottom-Bottom-right
		-0.1f, -0.1f, 0.0f, 0.0f, 0.0f, 1.0f, // Bottom-Bottom-left
		-0.1f,  0.0f, 0.1f, 1.0f, 0.0f, 0.0f, // Top-Top-left
		 0.0f,  0.0f, 0.1f, 0.0f, 1.0f, 0.0f, // Top-Top-right
		 0.0f, -0.1f, 0.1f, 1.0f, 1.0f, 1.0f, // Top-Bottom-right
		-0.1f, -0.1f, 0.1f, 0.0f, 0.0f, 1.0f // Top-Bottom-left
	};

	// Ground
	glGenVertexArrays(1, &groundVAO);
	glBindVertexArray(groundVAO);
	glGenBuffers(1, &groundVBO);
	glBindBuffer(GL_ARRAY_BUFFER, groundVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ground), ground, GL_STATIC_DRAW);
	glGenBuffers(1, &groundEBO); // Element array

	GLuint groundElements[] = {
		0, 1, 2,
		2, 3, 0
	};

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, groundEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(groundElements), groundElements, GL_STATIC_DRAW);

	// Vertex position
	GLint posAttrib = glGetAttribLocation(programID, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);

	// Vertex color
	GLint colAttrib = glGetAttribLocation(programID, "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));

	// Cube
	glGenVertexArrays(1, &cubeVAO);
	glBindVertexArray(cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);
	glGenBuffers(1, &cubeEBO);

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

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeElements), cubeElements, GL_STATIC_DRAW);

	// Vertex position
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);

	// Vertex color
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));

	////////////////////////////////////////////////////

	// Set up view transformation
	mat4 view = lookAt(
		vec3(0.0f, -1.5f,  1.0f),	// Position of the camera
		vec3(0.0f,  0.0f,  0.0f),	// The point to be centered on-screen
		vec3(0.0f,  1.0f,  0.0f)	// The up axis
	);
	GLint uniView = glGetUniformLocation(programID, "view");
	glUniformMatrix4fv(uniView, 1, GL_FALSE, value_ptr(view));

	// Create a perspective projection matrix
	// Specify FOV, aspect ratio, near and far
	mat4 proj = glm::perspective(glm::radians(45.0f), 512.0f / 512.0f, 1.0f, 10.0f);
	GLint uniProj = glGetUniformLocation(programID, "proj");
	glUniformMatrix4fv(uniProj, 1, GL_FALSE, value_ptr(proj));
}

void paintGL(void)
{
	//TODO:
	//render your objects and control the transformation here
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); //specify the background color
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Get reference of model variable
	GLint uniTrans = glGetUniformLocation(programID, "model");

	// Ground
	// Transformation
	mat4 model = mat4(1.0f);
	glUniformMatrix4fv(uniTrans, 1, GL_FALSE, value_ptr(model));

	// Rendering
	glBindVertexArray(groundVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	// Cube
	model = rotate(mat4(1.0f), x_delta * x_press_num * glm::radians(45.0f), vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(uniTrans, 1, GL_FALSE, value_ptr(model));

	glBindVertexArray(cubeVAO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

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