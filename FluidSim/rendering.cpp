
#include "rendering.h"


namespace rendering {


	//OpenGL methods
	float angle = 0.0f, angley = 0.0f;
	double red = 0.0;
	double green = 0.0;
	double blue = 0.0;
	float x = 0.0f, y = 1.0f , z = 5.0f ;
	float lx = 0.0f, ly = 0.0f, lz = -1.0f;


	void Reshape(int w, int h) {
		if (h == 0) h = 1;
		float ratio = w * 1.0 / h;
		glViewport(0, 0, w, h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0, ratio, 0.1f, 100.0f);
		glMatrixMode(GL_MODELVIEW);
	}
////TUTORIAL BULLSHIT
	void drawSnowMan() {

		glColor3f(1.0f, 1.0f, 1.0f);

		// Draw Body
		glTranslatef(0.0f, 0.75f, 0.0f);
		glutSolidSphere(0.75f, 20, 20);

		// Draw Head
		glTranslatef(0.0f, 1.0f, 0.0f);
		glutSolidSphere(0.25f, 20, 20);

		// Draw Eyes
		glPushMatrix();
		glColor3f(0.0f, 0.0f, 0.0f);
		glTranslatef(0.05f, 0.10f, 0.18f);
		glutSolidSphere(0.05f, 10, 10);
		glTranslatef(-0.1f, 0.0f, 0.0f);
		glutSolidSphere(0.05f, 10, 10);
		glPopMatrix();

		// Draw Nose
		glColor3f(1.0f, 0.5f, 0.5f);
		glutSolidCone(0.08f, 0.5f, 10, 2);
	}

	void renderScene() {

		// Clear Color and Depth Buffers

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Reset transformations
		glLoadIdentity();
		// Set the camera
		

		gluLookAt(x, y, z,
			x + lx, y + ly, z + lz,
			0.0f, y, 0.0f);

		// Draw ground
		glColor3f(0.9f, 0.9f, 0.9f);
		glBegin(GL_QUADS);
		glVertex3f(-100.0f, 0.0f, -100.0f);
		glVertex3f(-100.0f, 0.0f, 100.0f);
		glVertex3f(100.0f, 0.0f, 100.0f);
		glVertex3f(100.0f, 0.0f, -100.0f);
		glEnd();

		// Draw 36 SnowMen
		for (int i = -3; i < 3; i++)
			for (int j = -3; j < 3; j++) {
			glPushMatrix();
			glTranslatef(i*10.0, 0, j * 10.0);
			drawSnowMan();
			glPopMatrix();
			}

		glutSwapBuffers();
	}

/////KEY PROCESSING 
	void processNormalKeys(unsigned char key, int x, int y){
		int mod = glutGetModifiers();

		if (key == 27) exit(0);
		if (key == 'v')
		std::cout <<"angle ="<< angle << "x =" << x << "   z =" << z << std::endl;
		else if (key == 'r'){
			if (mod == GLUT_ACTIVE_ALT)
				red = 0.0;
			else
				red = 1.0;
		}
	}
	void processSpecialKeys(int key, int mousex, int mousey){
		int mod = glutGetModifiers();
		float fraction = 0.5f;

		switch (key){
		case GLUT_KEY_F1:
			//if CTRL+ALT+F1 or CTRL+F1 or ALT+F1 is pressed reset to black. else (just F1 pressed) make red
			if (mod == (GLUT_ACTIVE_ALT | GLUT_ACTIVE_CTRL) || mod == GLUT_ACTIVE_ALT || mod == GLUT_ACTIVE_CTRL){
				red = 0.0; green = 0.0; blue = 0.0;
			}
			else { red = 1.0; green = 0.0; blue = 0.0; }
			break;
		case GLUT_KEY_F2:
			red = 0.0;
			green = 1.0;
			blue = 0.0; break;
		case GLUT_KEY_F3:
			red = 0.0;
			green = 0.0;
			blue = 1.0; break;
		case GLUT_KEY_F4:
			red = 0.0;
			green = 0.0;
			blue = 0.0; break;
		case GLUT_KEY_LEFT:
			if (mod == GLUT_ACTIVE_ALT || mod == GLUT_ACTIVE_CTRL){
				angle -= 0.1f;
			}
			else {
				angle -= 0.01f;
			}
			lx = sin(angle);
			lz = -cos(angle); 
			break;
		case GLUT_KEY_RIGHT:
			if (mod == GLUT_ACTIVE_ALT || mod == GLUT_ACTIVE_CTRL){
				angle += 0.1f;
			} 
			else {
				angle += 0.01f;
			}
			lx = sin(angle);
			lz = -cos(angle);
			break;
		case GLUT_KEY_UP :
			if (mod == GLUT_ACTIVE_ALT || mod == GLUT_ACTIVE_CTRL){
				angley += 0.01f;
				ly = sin(angley);
			}
			else {
				x += lx * fraction;
				z += lz * fraction;
			}
			break;
		case GLUT_KEY_DOWN:
			if (mod == GLUT_ACTIVE_ALT || mod == GLUT_ACTIVE_CTRL){
				angley -= 0.01f;
				ly = sin(angley);
			}
			else {
				x -= lx * fraction;
				z -= lz * fraction;
			}
			break;

		}
	}

	void drawSnowMenTut(int argc, char *argv[]){
		//STEP 1: Initialize GLUT 
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
		glutInitWindowPosition(100, 100);
		glutInitWindowSize(500, 500);
		//STEP 2: Create windows
		glutCreateWindow("Hello OpenGL");
		//initRendering();
		//STEP 3: Handlers functions
		glutDisplayFunc(renderScene);
		glutReshapeFunc(Reshape);
		glutIdleFunc(renderScene);

		glutKeyboardFunc(processNormalKeys);
		glutSpecialFunc(processSpecialKeys);

		//OpenGL init
		glEnable(GL_DEPTH_TEST);


		//STEP 4: Main loop
		glutMainLoop();

	}
	/*
	void initRendering() {
	//glClearColor(0, 0, 0, 1);
	x = 0;
	y = 0;
	z = 5.0f;
	lx = 0;
	ly = 0;
	lz = -1.0f;
	}
	void Display() {

		//Clear color and depth buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//reset transformations
		glLoadIdentity(); //loads the identity matrix
		glColor3f(red, green, blue); //set the color of the triangle

		//Set the camera
		gluLookAt(0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f);
		glRotatef(angle, 0.0f, 1.0f, 0.0f);


		glBegin(GL_TRIANGLES);
		glVertex3f(-2, -2, 0);
		glVertex3f(2, 0, 0);
		glVertex3f(0, 2, 0);
		glEnd();

		angle += .1f; //radians??

		glutSwapBuffers();

	}

*/

	/*
	//////DRAW SNOWMEN TUTORIAL. 
	///just uncomment this block and put drawSnowMenTut(argc,argv); in main somewhere and you will get snowmen
	
	//OpenGL methods
	float angle = 0.0f, angley = 0.0f;
	double red = 0.0;
	double green = 0.0;
	double blue = 0.0;
	float x = 0.0f, y = 1.0f , z = 5.0f ;
	float lx = 0.0f, ly = 0.0f, lz = -1.0f;


	void Reshape(int w, int h) {
		if (h == 0) h = 1;
		float ratio = w * 1.0 / h;
		glViewport(0, 0, w, h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0, ratio, 0.1f, 100.0f);
		glMatrixMode(GL_MODELVIEW);
	}
////TUTORIAL BULLSHIT
	void drawSnowMan() {

		glColor3f(1.0f, 1.0f, 1.0f);

		// Draw Body
		glTranslatef(0.0f, 0.75f, 0.0f);
		glutSolidSphere(0.75f, 20, 20);

		// Draw Head
		glTranslatef(0.0f, 1.0f, 0.0f);
		glutSolidSphere(0.25f, 20, 20);

		// Draw Eyes
		glPushMatrix();
		glColor3f(0.0f, 0.0f, 0.0f);
		glTranslatef(0.05f, 0.10f, 0.18f);
		glutSolidSphere(0.05f, 10, 10);
		glTranslatef(-0.1f, 0.0f, 0.0f);
		glutSolidSphere(0.05f, 10, 10);
		glPopMatrix();

		// Draw Nose
		glColor3f(1.0f, 0.5f, 0.5f);
		glutSolidCone(0.08f, 0.5f, 10, 2);
	}

	void renderScene() {

		// Clear Color and Depth Buffers

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Reset transformations
		glLoadIdentity();
		// Set the camera
		

		gluLookAt(x, y, z,
			x + lx, y + ly, z + lz,
			0.0f, y, 0.0f);

		// Draw ground
		glColor3f(0.9f, 0.9f, 0.9f);
		glBegin(GL_QUADS);
		glVertex3f(-100.0f, 0.0f, -100.0f);
		glVertex3f(-100.0f, 0.0f, 100.0f);
		glVertex3f(100.0f, 0.0f, 100.0f);
		glVertex3f(100.0f, 0.0f, -100.0f);
		glEnd();

		// Draw 36 SnowMen
		for (int i = -3; i < 3; i++)
			for (int j = -3; j < 3; j++) {
			glPushMatrix();
			glTranslatef(i*10.0, 0, j * 10.0);
			drawSnowMan();
			glPopMatrix();
			}

		glutSwapBuffers();
	}

/////KEY PROCESSING 
	void processNormalKeys(unsigned char key, int x, int y){
		int mod = glutGetModifiers();

		if (key == 27) exit(0);
		if (key == 'v')
		std::cout <<"angle ="<< angle << "x =" << x << "   z =" << z << std::endl;
		else if (key == 'r'){
			if (mod == GLUT_ACTIVE_ALT)
				red = 0.0;
			else
				red = 1.0;
		}
	}
	void processSpecialKeys(int key, int mousex, int mousey){
		int mod = glutGetModifiers();
		float fraction = 0.5f;

		switch (key){
		case GLUT_KEY_F1:
			//if CTRL+ALT+F1 or CTRL+F1 or ALT+F1 is pressed reset to black. else (just F1 pressed) make red
			if (mod == (GLUT_ACTIVE_ALT | GLUT_ACTIVE_CTRL) || mod == GLUT_ACTIVE_ALT || mod == GLUT_ACTIVE_CTRL){
				red = 0.0; green = 0.0; blue = 0.0;
			}
			else { red = 1.0; green = 0.0; blue = 0.0; }
			break;
		case GLUT_KEY_F2:
			red = 0.0;
			green = 1.0;
			blue = 0.0; break;
		case GLUT_KEY_F3:
			red = 0.0;
			green = 0.0;
			blue = 1.0; break;
		case GLUT_KEY_F4:
			red = 0.0;
			green = 0.0;
			blue = 0.0; break;
		case GLUT_KEY_LEFT:
			if (mod == GLUT_ACTIVE_ALT || mod == GLUT_ACTIVE_CTRL){
				angle -= 0.1f;
			}
			else {
				angle -= 0.01f;
			}
			lx = sin(angle);
			lz = -cos(angle); 
			break;
		case GLUT_KEY_RIGHT:
			if (mod == GLUT_ACTIVE_ALT || mod == GLUT_ACTIVE_CTRL){
				angle += 0.1f;
			} 
			else {
				angle += 0.01f;
			}
			lx = sin(angle);
			lz = -cos(angle);
			break;
		case GLUT_KEY_UP :
			if (mod == GLUT_ACTIVE_ALT || mod == GLUT_ACTIVE_CTRL){
				angley += 0.01f;
				ly = sin(angley);
			}
			else {
				x += lx * fraction;
				z += lz * fraction;
			}
			break;
		case GLUT_KEY_DOWN:
			if (mod == GLUT_ACTIVE_ALT || mod == GLUT_ACTIVE_CTRL){
				angley -= 0.01f;
				ly = sin(angley);
			}
			else {
				x -= lx * fraction;
				z -= lz * fraction;
			}
			break;

		}
	}

		void drawSnowMenTut(int argc, char *argv[]){
		//STEP 1: Initialize GLUT 
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
		glutInitWindowPosition(100, 100);
		glutInitWindowSize(500, 500);
		//STEP 2: Create windows
		glutCreateWindow("Hello OpenGL");
		//initRendering();
		//STEP 3: Handlers functions
		glutDisplayFunc(renderScene);
		glutReshapeFunc(Reshape);
		glutIdleFunc(renderScene);

		glutKeyboardFunc(processNormalKeys);
		glutSpecialFunc(processSpecialKeys);

		//OpenGL init
		glEnable(GL_DEPTH_TEST);


		//STEP 4: Main loop
		glutMainLoop();

	}
	
	*/
































}