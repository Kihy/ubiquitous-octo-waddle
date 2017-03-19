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
int shoulder=0, elbow=0, shoulderY=0;
int knee_left=120,knee_right=145, pelvis=-120, pelvisZ=65;
float body_pos_z=0;
float pelvis_y=0;

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
	//Head
	glColor3f(1., 0.78, 0.06);		
	glPushMatrix();
	  glTranslatef(0, 7.5, body_pos_z);
	  glutSolidCube(1);
	glPopMatrix();

	//Neck

	glPushMatrix();
	  glTranslatef(0,6.75,body_pos_z);
	  glutSolidCube(0.5);
	glPopMatrix();

    //Torso
    glColor3f(0,0,1);
	glPushMatrix();
	  glTranslatef(0, 5.75, body_pos_z);
	  glScalef(1.5,1.5,1);
	  glutSolidCube(1);
	glPopMatrix();
	
	//Pelvis
	glPushMatrix();
	  glTranslatef(0,4.5,body_pos_z);
	  glRotatef(pelvis_y,0,1,0);
	  glScalef(1.5,1,1);
	  glutSolidCube(1);
	glPopMatrix();

	//Right arm
	glColor3f(1,0,0);
	glPushMatrix();
	  glTranslatef(0,0,body_pos_z);
	  glTranslatef(-1,6.5,0);
	  glRotatef(shoulderY,0,1,0);
	  glRotatef(shoulder,1,0,0);
	  glTranslatef(1,-6.5,0);
	  glPushMatrix();
		glTranslatef(-1,5.75,0);
		glScalef(0.5,1.5,0.5);
	    glutSolidCube(1);
	  glPopMatrix();
	 	 
	  glTranslatef(-1,5,0);
	  glRotatef(elbow,1,0,0);
	  glTranslatef(1,-5,0);
	  glPushMatrix();
		glTranslatef(-1,4.25,0);
		glScalef(0.5,1.5,0.5);
	    glutSolidCube(1);
	  glPopMatrix();
	glPopMatrix();
	
	//Left arm
	glPushMatrix();
	  glTranslatef(0,0,body_pos_z);
	  glTranslatef(1,6.5,0);
	  glRotatef(shoulder,1,0,0);
	  glTranslatef(-1,-6.5,0);
	  glPushMatrix();
		glTranslatef(1,5.75,0);
		glScalef(0.5,1.5,0.5);
	    glutSolidCube(1);
	  glPopMatrix();
	 	 
	  glTranslatef(1,5,0);
	  glRotatef(elbow,1,0,0);
	  glTranslatef(-1,-5,0);
	  glPushMatrix();
		glTranslatef(1,4.25,0);
		glScalef(0.5,1.5,0.5);
	    glutSolidCube(1);
	  glPopMatrix();
	glPopMatrix();
	
	//Right leg
	glColor3f(0,1,0);
	glPushMatrix();
	  glTranslatef(0,0,body_pos_z);
	  glTranslatef(-0.5,4,0);
	  glRotatef(pelvisZ,0,0,1);
	  glRotatef(pelvis,1,0,0);
	  glTranslatef(0.5,-4,0);

	  glPushMatrix();
	    
		glTranslatef(-0.5,3,0);
		glScalef(0.5,2,0.5);
	    glutSolidCube(1);
	  glPopMatrix();
	 	 
	  glTranslatef(-0.5,2,0);
	  glRotatef(knee_right,1,0,0);
	  glTranslatef(0.5,-2,0);
	  glPushMatrix();
		glTranslatef(-0.5,1,0);
		glScalef(0.5,2,0.5);
	    glutSolidCube(1);
	  glPopMatrix();
	glPopMatrix();

	//Left leg
	glPushMatrix();
	  glTranslatef(0,0,body_pos_z);
	  glTranslatef(0.5,4,0);
	  glRotatef(-pelvisZ,0,0,1);
	  glRotatef(pelvis,1,0,0);
	  glTranslatef(-0.5,-4,0);
	  
	  glPushMatrix();

		glTranslatef(0.5,3,0);
		glScalef(0.5,2,0.5);
	    glutSolidCube(1);
	  glPopMatrix();
	 	 
	  glTranslatef(0.5,2,0);
	  glRotatef(knee_left,1,0,0);
	  glTranslatef(-0.5,-2,0);
	  glPushMatrix();
		glTranslatef(0.5,1,0);
		glScalef(0.5,2,0.5);
	    glutSolidCube(1);
	  glPopMatrix();

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
	gluLookAt(	x, cam, z,
			x+lx, cam,  z+lz,
			0.0f, 1.0f,  0.0f);

	glLightfv(GL_LIGHT0,GL_POSITION, lpos);   //Set light position


	glDisable(GL_LIGHTING);			//Disable lighting when drawing floor.
    drawFloor();

	glEnable(GL_LIGHTING);	       //Enable lighting when drawing the model
	
	glPushMatrix();
	drawModel();
	glPopMatrix();
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

float dx=0.05;
float dtheta=1;
float dpelvis=0.7;
//  ------- Timer ----------------------------------------------------------
void Timer(int value){
	if (body_pos_z>=10)
	dx=-dx;
	if (body_pos_z<=-10)
	dx=-dx;
	
	body_pos_z+=dx;
	


	
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
