#include "main.h"
#include "MACgrid.h"
#include <iostream>
#include <string>

//using namespace gridDef;
//using namespace MathLib;

const int WIDTH = 600;
const int HEIGHT = 700;

//OpenGL methods
void initRendering() {
	glClearColor(0, .5, 1, 1);
}

void Display() {

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1, 1, 1);

	glBegin(GL_LINES);
	glVertex3f(0, 0, -5);
	glVertex3f(2, 2, -5);
	glVertex3f(4, 0, -5);
	glEnd();

	glutSwapBuffers();
}

void Reshape(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (GLdouble)w / (GLdouble)h, 1.0, 200.0);
}


int main(int argc, char *argv[]) {

	MACgrid* grid = new MACgrid();
	//grid->printDim();
	//cout<<"max is "<<grid->max(grid->temp)<<endl;
	grid->fill(11, 1, 5);
	int count0 = 0;
	int count1 = 0;
	/*
	for (int i = 0; i<grid->nx; i++){
		for (int j = 0; j<grid->ny; j++){
			for (int k = 0; k<grid->nz; k++){
				//cout << "fillArray[" << i << "][" << j << "][" << k << "] = " << grid->getValue(11, i, j, k) << endl;

			}
		}
	}*/
//	cout << "max = " << max(&(grid->pressure)) << endl;
	cout << "min = " << grid->min(11) << endl;
	cout << "please hit enter to continue" << endl;
	string a;
	getline(cin, a);



	/* Glut setup function calls */
	/*
	//STEP 1: Initialize GLUT 
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	//glutInitWindowPosition(100,100);
	glutInitWindowSize(400, 400);
	//STEP 2: Create windows
	glutCreateWindow("Hello OpenGL");
	initRendering();
	//STEP 3: Handlers functions
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	//STEP 4: Main loop
	glutMainLoop();
	*/
	//Tutorial 1 - draw a triangle 
	const GLfloat g_vertex_buffer_data[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};
	/*
	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle

	glDisableVertexAttribArray(0);
	
	glutMainLoop();
	
	*/
	
	return 0;
}





