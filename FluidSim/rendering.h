#include <GL/glew.h>  //glew must be included first
#include <GL/freeglut.h> 
#include <GL/glut.h>
#include <GL/GLU.h>

#include <vector>
#include <iostream>
#include <cstdio>
#include <ctime>

namespace rendering {

	void initRendering();
	void drawSnowMan();
	void renderScene();
	void Display();
	void Reshape(int w, int h);
	void processNormalKeys(unsigned char key, int x, int y);
	void processSpecialKeys(int key, int x, int y);
	void drawSnowMenTut(int argc, char *argv[]);

}