///////////////////////////////////////////
////Type your name and student ID here/////
////Name:
////Student ID:

#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Dependencies\glm\glm.hpp"
#include "Dependencies\glm\gtc\matrix_transform.hpp"
#include <iostream>
#include <fstream>
using namespace std;
using glm::vec3;
using glm::mat4;

GLint programID;
GLuint groundVAO;
GLuint groundVBO;

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
	//TODO:
}

void sendDataToOpenGL()
{
	//TODO:
	//create point, line, 2D object and 3D object here and bind to VAOs & VBOs

	//const GLfloat ground[] = {
	//	// X	 Y		Z		 R		  G		   B
	//	-1.0f,  0.0f, -1.0f,  1.0f,  0.0f,  0.0f,
	//	-1.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.0f,
	//	 1.0f,  0.0f, -1.0f,  1.0f,  0.0f,  0.0f,
	//	-1.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.0f,
	//	 1.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.0f,
	//	 1.0f,  0.0f, -1.0f,  1.0f,  0.0f,  0.0f
	//};

	//glGenVertexArrays(1, &groundVAO);
	//glBindVertexArray(groundVAO);
	//glGenBuffers(1, &groundVBO);
	//glBindBuffer(GL_ARRAY_BUFFER, groundVBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(ground), ground, GL_STATIC_DRAW);
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));

	const GLfloat triangle[] =
	{
		-0.5f, -0.5f, +0.0f, //left
		+1.0f, +0.0f, +0.0f, //color

		+0.5f, -0.5f, +0.0f, //right 
		+1.0f, +0.0f, +0.0f,

		+0.0f, +0.5f, +0.0f, //top 
		+1.0f, +0.0f, +0.0f,
	};
	GLuint vaoID;
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);  //first VAO
	GLuint vboID;
	glGenBuffers(1, &vboID);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);
	//vertex position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	//vertex color
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));
}

void paintGL(void)
{
	//TODO:
	//render your objects and control the transformation here

	// Depth Buffer
	/*glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);*/

	// Scaling
	/*mat4 trans = mat4(1.0f);
	trans = scale(trans, vec3(3.0f, 1.0f, 3.0f));*/
	// glBindVertexArray(groundVAO);
	// glDrawArrays(GL_TRIANGLES, 0, 6);

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); //specify the background color
	glClear(GL_COLOR_BUFFER_BIT);

	glm::mat4 modelTransformMatrix = glm::mat4(1.0f);
	modelTransformMatrix = glm::translate(glm::mat4(),
		glm::vec3(0.0, 0.0f, 0.0f));;
	GLint modelTransformMatrixUniformLocation =
		glGetUniformLocation(programID, "modelTransformMatrix");
	glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1,
		GL_FALSE, &modelTransformMatrix[0][0]);

	glDrawArrays(GL_TRIANGLES, 0, 6); //render primitives from array data

	glFlush();
	glutPostRedisplay();
}

void initializedGL(void) //run only once
{
	sendDataToOpenGL();
	installShaders();
}

int main(int argc, char *argv[])
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