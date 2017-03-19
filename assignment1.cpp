//  ========================================================================
//  COSC363: Assignment 1
//  ========================================================================
 
#include <iostream>
#include <fstream>
#include <cmath> 
#include <GL/freeglut.h>
using namespace std;

//--Globals ---------------------------------------------------------------

// Camera controls
// angle of rotation for the camera direction
float angle=0.0;
// actual vector representing the camera's direction
float lx=0.0f,lz=-1.0f;
// XZ position of the camera
float x=0.0f,z=10.0f;
// camera height
float cam=7;

// Robot Controls
float arm_angle=0;
int shoulder=0, elbow=0;

//--Draws a grid of lines on the floor plane -------------------------------
void drawFloor()
{
	glColor3f(0., 0.5, 0.);			//Floor colour
	for(float i = -50; i <= 50; i ++)
	{
		glBegin(GL_LINES);			//A set of grid lines on the xz-plane
			glVertex3f(-50, 0, i);
			glVertex3f(50, 0, i);
			glVertex3f(i, 0, -50);
			glVertex3f(i, 0, 50);
		glEnd();
	}
}

//--Draws a character model constructed using GLUT objects ------------------
void drawModel()
{
	glColor3f(1., 0.78, 0.06);		//Head
	glPushMatrix();
	  glTranslatef(0, 7.7, 0);
	  glutSolidCube(1.4);
	glPopMatrix();

	glColor3f(1., 0., 0.);			//Torso
	glPushMatrix();
	  glTranslatef(0, 5.5, 0);
	  glScalef(3, 3, 1.4);
	  glutSolidCube(1);
	glPopMatrix();

	glColor3f(0., 0., 1.);			//Right leg
	glPushMatrix();
	  glTranslatef(-0.8, 2.2, 0);
	  glScalef(1, 4.4, 1);
	  glutSolidCube(1);
	glPopMatrix();

	glColor3f(0., 0., 1.);			//Left leg
	glPushMatrix();
	  glTranslatef(0.8, 2.2, 0);
	  glScalef(1, 4.4, 1);
	  glutSolidCube(1);
	glPopMatrix();

	glColor3f(0., 0., 1.);			//Right arm
	glPushMatrix();
	  
	  glTranslatef(-2,6.5,0);
	  glRotatef(arm_angle,1,0,0);
	  glTranslatef(2,-6.5,0);
	  glTranslatef(-2, 5, 0);
	  glScalef(1, 4, 1);
	  glutSolidCube(1);
	glPopMatrix();
	
	

	glColor3f(0., 0., 1.);			//Left arm
	glPushMatrix();
	  
	  glTranslatef(-2,6.5,0);
	  glRotatef(arm_angle,1,0,0);
	  glTranslatef(2,-6.5,0);
	  glTranslatef(-2, 5, 0);
	  glScalef(1, 4, 1);
	  glutSolidCube(1);
	glPopMatrix();
	
}

void drawHouse(){
	int wall_height=15;
	int house_width=100;
	int house_length=50;
	//North wall
	glColor3f(0., 0., 0.);
	glPushMatrix();
	  glTranslatef(0, wall_height/2, -house_length/2);
	  glScalef(house_width, wall_height, 0.5);
	  glutSolidCube(1);
	glPopMatrix();
	
	//South wall
		glColor3f(0., 0., 0.);
	glPushMatrix();
	  glTranslatef(0, wall_height/2, house_length/2);
	  glScalef(house_width, wall_height, 0.5);
	  glutSolidCube(1);
	glPopMatrix();
	
	//West wall
		glColor3f(0., 0., 0.);
	glPushMatrix();
	  glTranslatef(-house_width/2, wall_height/2, 0);
	  glScalef(0.5, wall_height, house_length);
	  glutSolidCube(1);
	glPopMatrix();
	
		//East wall
		glColor3f(0., 0., 0.);
	glPushMatrix();
	  glTranslatef(house_width/2, wall_height/2, 0);
	  glScalef(0.5, wall_height, house_length);
	  glutSolidCube(1);
	glPopMatrix();
	}

//--Display: ---------------------------------------------------------------
//--This is the main display module containing function calls for generating
//--the scene.
void display()  
{
	float lpos[4] = {10., 10., 10., 1.0};  //light's position

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(	x, 7, z,
			x+lx, cam,  z+lz,
			0.0f, 1.0f,  0.0f);

	glLightfv(GL_LIGHT0,GL_POSITION, lpos);   //Set light position


	glDisable(GL_LIGHTING);			//Disable lighting when drawing floor.
    drawFloor();

	glEnable(GL_LIGHTING);	       //Enable lighting when drawing the model

	drawModel();
	drawHouse();
	
	glFlush();
}

//------- Initialize OpenGL parameters -----------------------------------
void initialize()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);	//Background colour

	glEnable(GL_LIGHTING);					//Enable OpenGL states
	glEnable(GL_LIGHT0);
 	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 1000.0);   //Camera Frustum
}

//------------ Special key event callback ---------------------------------
// To enable the use of left and right arrow keys to rotate the scene
void processSpecialKeys(int key, int xx, int yy) {

	float fraction = 1;

	switch (key) {
		case GLUT_KEY_LEFT :
			angle -= 0.1f;
			lx = sin(angle);
			lz = -cos(angle);
			break;
		case GLUT_KEY_RIGHT :
			angle += 0.1f;
			lx = sin(angle);
			lz = -cos(angle);
			break;
		case GLUT_KEY_UP :
			x += lx * fraction;
			z += lz * fraction;
			break;
		case GLUT_KEY_DOWN :
			x -= lx * fraction;
			z -= lz * fraction;
			break;
			
		case GLUT_KEY_PAGE_UP:
		cam+=0.2;
		break;
		case GLUT_KEY_PAGE_DOWN:
		cam-=0.2;
		break;
	}
	
	glutPostRedisplay();
}

//  ------- Timer ----------------------------------------------------------
void Timer(int value){
    arm_angle += 1;
    //shoulder+=1;
    glutPostRedisplay();
    glutTimerFunc(100, Timer, 100);
}

//  ------- Main: Initialize glut window and register call backs -----------
int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_DEPTH);
   glutInitWindowSize (600, 600); 
   glutInitWindowPosition (10, 10);
   glutCreateWindow ("Humanoid");
   initialize();

   glutDisplayFunc(display);
   glutSpecialFunc(processSpecialKeys); 
   glutTimerFunc(100,Timer,100);
   glutMainLoop();
   return 0;
}
