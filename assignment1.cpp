//  ========================================================================
//  COSC363: Assignment 1
//  ========================================================================
 
#include <iostream>
#include <fstream>
#include <climits>
#include <cmath> 
#include <GL/freeglut.h>
#include "loadTGA.h"
using namespace std;
GLuint txId[2];   //Texture ids

void loadTexture()				
{
	glGenTextures(2, txId); 	// Create 2 texture ids

	glBindTexture(GL_TEXTURE_2D, txId[0]);  //Use this texture
    loadTGA("wall.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	

	glBindTexture(GL_TEXTURE_2D, txId[1]);  //Use this texture
    loadTGA("floor.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	
	
	
	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
}

//--Globals ---------------------------------------------------------------
float *x, *y, *z;  //vertex coordinate arrays
int *t1, *t2, *t3; //triangles
int nvrt, ntri;    //total number of vertices and triangles
float cam_hgt = 1; //Camera height
float xmin, xmax, ymin, ymax; //min, max values of  object coordinates

//-- Loads mesh data in OFF format    -------------------------------------
void loadMeshFile(const char* fname)  
{
	ifstream fp_in;
	int num, ne;

	fp_in.open(fname, ios::in);
	if(!fp_in.is_open())
	{
		cout << "Error opening mesh file" << endl;
		exit(1);
	}

	fp_in.ignore(INT_MAX, '\n');				//ignore first line
	fp_in >> nvrt >> ntri >> ne;			    // read number of vertices, polygons, edges

    x = new float[nvrt];                        //create arrays
    y = new float[nvrt];
    z = new float[nvrt];

    t1 = new int[ntri];
    t2 = new int[ntri];
    t3 = new int[ntri];

	for(int i=0; i < nvrt; i++)                         //read vertex list 
		fp_in >> x[i] >> y[i] >> z[i];

	for(int i=0; i < ntri; i++)                         //read polygon list 
	{
		fp_in >> num >> t1[i] >> t2[i] >> t3[i];
		if(num != 3)
		{
			cout << "ERROR: Polygon with index " << i  << " is not a triangle." << endl;  //not a triangle!!
			exit(1);
		}
	}

	fp_in.close();
	cout << " File successfully read." << endl;
}

//-- Computes the min, max values of coordinates  -----------------------
void computeMinMax()
{
	xmin = xmax = x[0];
	ymin = ymax = y[0];
	for(int i = 1; i < nvrt; i++)
	{
		if(x[i] < xmin) xmin = x[i];
		else if(x[i] > xmax) xmax = x[i];
		if(y[i] < ymin) ymin = y[i];
		else if(y[i] > ymax) ymax = y[i];
	}
}

//--Function to compute the normal vector of a triangle with index tindx ----------
void normal(int tindx)
{
	float x1 = x[t1[tindx]], x2 = x[t2[tindx]], x3 = x[t3[tindx]];
	float y1 = y[t1[tindx]], y2 = y[t2[tindx]], y3 = y[t3[tindx]];
	float z1 = z[t1[tindx]], z2 = z[t2[tindx]], z3 = z[t3[tindx]];
	float nx, ny, nz;
	nx = y1*(z2-z3) + y2*(z3-z1) + y3*(z1-z2);
	ny = z1*(x2-x3) + z2*(x3-x1) + z3*(x1-x2);
	nz = x1*(y2-y3) + x2*(y3-y1) + x3*(y1-y2);
	glNormal3f(nx, ny, nz);
}

//--Display: ----------------------------------------------------------------------
//--This is the main display module containing function calls for generating
//--the scene.
void displayBunny()  
{
	glColor3f(230/255.0, 228/255.0, 216/255.0);

    //Construct the object model here using GL primitives
	glBegin(GL_TRIANGLES);
		for(int tindx = 0; tindx < ntri; tindx++)
		{
		   normal(tindx);
		   glVertex3d(x[t1[tindx]], y[t1[tindx]], z[t1[tindx]]);
		   glVertex3d(x[t2[tindx]], y[t2[tindx]], z[t2[tindx]]);
		   glVertex3d(x[t3[tindx]], y[t3[tindx]], z[t3[tindx]]);
		}
	glEnd();

}

//--Globals ---------------------------------------------------------------

// Camera controls
// angle of rotation for the camera direction
float angle=0.0;
// actual vector representing the camera's direction
float lx=0.0f,lz=-1.0f;
// XZ position of the camera
float xcam=-25,zcam=25.0f;
// camera height
float cam=7;

// Robot Controls
float arm_angle=0;
int shoulder=-20, elbow=-60, shoulderY=0;
int knee_left=120,knee_right=145, pelvis=-120, pelvisZ=65;
float body_pos_z=0;
float pelvis_y=0;

// Box
float box_pos=0;

GLUquadricObj *q;

//Draws a cylinder
void cylinder(float base_rad, float height){

	glPushMatrix();
		q = gluNewQuadric();
		gluCylinder(q,base_rad,base_rad,height,64,64);
	glPopMatrix();
	
	glPushMatrix();
		gluDisk(q,0,base_rad,64,64);
	glPopMatrix();
	
	glPushMatrix();
		glTranslatef(0,0,height);
		gluDisk(q,0,base_rad,64,64);
	glPopMatrix();
	

	}

//--Draws a grid of lines on the floor plane -------------------------------
void drawFloor()
{
	glColor3f(1,1,1);			//Floor colour
	glNormal3f(0,1,0);
	glBegin(GL_QUADS);
	glBindTexture(GL_TEXTURE_2D, txId[1]);  //Use this texture
	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
	for(int i = -50; i < 50; i++)
	{
		for(int j = -25;  j < 25; j++)
		{
			glTexCoord2f(1,0);glVertex3f(i, 0.0, j);
			glTexCoord2f(1,1);glVertex3f(i, 0.0, j++);
			glTexCoord2f(0,1);glVertex3f(i++, 0.0, j++);
			glTexCoord2f(0,0);glVertex3f(i++, 0.0, j);
		}
	}
	glEnd();
}
float robot_arm_angle=0;
float robot_joint_angle=0;
float robot_arm_y=0;
float claw_angle=80;
//--Draws a robot------------------------------------------------------------
void drawRobot()
{
	glColor3f(1,1,0);

	
	
	//arm
	glPushMatrix();
	  glTranslatef(0,5,0);
	  glRotatef(robot_arm_y,0,1,0);
	  glRotatef(robot_joint_angle,0,0,1);
	  glTranslatef(0,-5,0);
	  glPushMatrix();
		glTranslatef(0,9,0);
		glScalef(1,8,1);
	    glutSolidCube(1);
	  glPopMatrix();
	
	  glTranslatef(0,13,0);
	  glRotatef(robot_arm_angle,0,0,1);	  
	  glTranslatef(0,-13,0);
	  glPushMatrix();
		glTranslatef(0,16,0);
		glScalef(1,6,1);
	    glutSolidCube(1);
	  glPopMatrix();

	//claws
	glPushMatrix();
	  glTranslatef(0,19,-0.25);
	  glRotatef(-claw_angle,1,0,0);	  
	  glTranslatef(0,-19,0.25);
	  glPushMatrix();
		glTranslatef(0,20,-0.25);
		glScalef(0.25,2,0.25);
	    glutSolidCube(1);
	  glPopMatrix();
	  
	  glTranslatef(0,21,-0.25);
	  glRotatef(90,1,0,0);
	  glTranslatef(0,-21,0.25);
	  glPushMatrix();
		glTranslatef(0,22,-0.25);
		glScalef(0.25,2,0.25);
	    glutSolidCube(1);
	  glPopMatrix();
	glPopMatrix();
	
	glPushMatrix();
	  glTranslatef(0,19,0.25);
	  glRotatef(claw_angle,1,0,0);	  
	  glTranslatef(0,-19,-0.25);
	  glPushMatrix();
		glTranslatef(0,20,0.25);
		glScalef(0.25,2,0.25);
	    glutSolidCube(1);
	  glPopMatrix();
	  
	  glTranslatef(0,21,0.25);
	  glRotatef(-90,1,0,0);
	  glTranslatef(0,-21,-0.25);
	  glPushMatrix();
		glTranslatef(0,22,0.25);
		glScalef(0.25,2,0.25);
	    glutSolidCube(1);
	  glPopMatrix();
	glPopMatrix();

	glPopMatrix();
	
	
	//wheels
	glPushMatrix();
		glTranslatef(2,1,2);
		cylinder(1,0.5);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-2,1,2);
		cylinder(1,0.5);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(2,1,-2.5);
		cylinder(1,0.5);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-2,1,-2.5);
		cylinder(1,0.5);
	glPopMatrix();
	
	//body
	glPushMatrix();
		glTranslatef(0,3,0);
		glScalef(4,4,4);
		glutSolidCube(1);
	glPopMatrix();
 
    }
    
//--Draws a transmission belt -----------------------------------------------
void drawBelt(float rad, float height, float length, int hide=0){
	if(!hide){
	glPushMatrix();
		glTranslatef(0,2,-0.25);
		glScalef(0.5,4,0.5);
		glutSolidCube(1);
	glPopMatrix();
	
	glPushMatrix();
		glTranslatef(0,2,height+0.25);
		glScalef(0.5,4,0.5);
		glutSolidCube(1);
	glPopMatrix();
}
	glPushMatrix();
		glTranslatef(length,2,-0.25);
		glScalef(0.5,4,0.5);
		glutSolidCube(1);
	glPopMatrix();
	
	glPushMatrix();
		glTranslatef(length,2,height+0.25);
		glScalef(0.5,4,0.5);
		glutSolidCube(1);
	glPopMatrix();
	
	glTranslatef(0,4,0);
		
	glPushMatrix();
		glTranslatef(length/2,rad,height/2);
		glScalef(length,0,height);
		glutSolidCube(1);
	glPopMatrix();
	
	glPushMatrix();
		cylinder(rad,height);
		glTranslatef(length,0,0);
		cylinder(rad,height);
	glPopMatrix();
	
	glPushMatrix();
		glTranslatef(length/2,-rad,height/2);
		glScalef(length,0,height);
		glutSolidCube(1);
	glPopMatrix();
	}

//--Draws a production machine
void drawMachine()
{
	glColor3f(1,0,0);
	glPushMatrix();
		glTranslatef(-2,2,-2);
		glScalef(1,4,1);
		glutSolidCube(1);
	glPopMatrix();
		
	glPushMatrix();
		glTranslatef(2,2,-2);
		glScalef(1,4,1);
		glutSolidCube(1);
	glPopMatrix();
		
	
	glPushMatrix();
		glTranslatef(-2,2,2);
		glScalef(1,4,1);
		glutSolidCube(1);
	glPopMatrix();
	
	
	glPushMatrix();
		glTranslatef(2,2,2);
		glScalef(1,4,1);
		glutSolidCube(1);
	glPopMatrix();
	
	//main body
	glPushMatrix();
		glTranslatef(0,6,0);
		glScalef(5,7,5);
		glutSolidCube(1);
	glPopMatrix();
	}

//--Draws a buddha model constructed using GLUT objects ------------------
void drawBuddha()
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
float white[4]={1,1,1,1,};
float black[4]={0,0,0,0};
void drawFactory(){
	int wall_height=20;
	int house_width=100;
	int house_length=50;

	//North wall
	glColor3f(299/255,299/255.0, 299/255.0);

	glPushMatrix();
	  glTranslatef(0, wall_height/2, -house_length/2);
	  glScalef(house_width, wall_height, 0.5);
	  glutSolidCube(1);
	glPopMatrix();
	
	//South wall

	glPushMatrix();
	  glTranslatef(0, wall_height/2, house_length/2);
	  glScalef(house_width, wall_height, 0.5);
	  glutSolidCube(1);
	glPopMatrix();
	
	//West wall
	glPushMatrix();
	  glTranslatef(-house_width/2, wall_height/2, 0);
	  glScalef(0.5, wall_height, house_length);
	  glutSolidCube(1);
	glPopMatrix();
	
	//East wall
	glPushMatrix();
	  glTranslatef(house_width/2, wall_height/2, 0);
	  glScalef(0.5, wall_height, house_length);
	  glutSolidCube(1);
	glPopMatrix();
	
	//draw ceiling
	glPushMatrix();
	  glTranslatef(0, wall_height, 0);
	  glScalef(house_width, 0.5, house_length);
	  glutSolidCube(1);
	glPopMatrix();
	
	
	
	//Transmission belt
	glColor3f(0, 0, 1);
	glPushMatrix();
		glTranslatef(-40,0,0);
		drawBelt(1,4,20,1);
	glPopMatrix();
	
	glPushMatrix();
		glTranslatef(5,0,0);
		drawBelt(1,4,30,0);
	glPopMatrix();
	
	
	//draw production machine
	glPushMatrix();
		glTranslatef(-40,0,2);
		drawMachine();
	glPopMatrix();
	
	//draw transformation machine
	glPushMatrix();
		glTranslatef(20,0,2);
		drawMachine();
	glPopMatrix();
			
			
			
	//big vacuum
	glPushMatrix();
		glTranslatef(43,20,2);
		glRotatef(90,1,0,0);
		cylinder(2,5);
	glPopMatrix();

	}
int second=7887;
int minute=0;
int hour=0;
//--Draws a clock ---------------------------------------------------------
void drawClock(){
	glPushMatrix();
			glColor3f(0,0,0);
	//hours
		glPushMatrix();
			glRotatef(hour,0,0,1);
			glTranslatef(0,0.75,0);
			glScalef(0.4,1.5,0.1);
			glutSolidCube(1);
		glPopMatrix();
		
		//minutes
		glPushMatrix();
			glRotatef(minute,0,0,1);
			glTranslatef(0,1.1,0);
			glScalef(0.2,2.2,0.2);
			glutSolidCube(1);
		glPopMatrix();
		
		//seconds
		glPushMatrix();
			glRotatef(second,0,0,1);
			glTranslatef(0,1.1,0);
			glScalef(0.1,2.2,0.1);
			glutSolidCube(1);
		glPopMatrix();
		
		glColor3f(1,1,1);
		
		q = gluNewQuadric();
		gluDisk(q,0,2.5,64,64);
		glColor3f(0,0,0);
		glTranslatef(0,0,-0.51);
		cylinder(2.6,0.5);
		
		
	glPopMatrix();
	
	}


float robot_pos=0;
float box_pos_stage_2=0;
float teapot_pos=0;
float box_y=0;
float bunny_pos_y=0;
//--Display: ---------------------------------------------------------------
//--This is the main display module containing function calls for generating
//--the scene.
void display()  
{
	float lpos[4] = {0, 18., 0., 1.0};  //light's position
	float spot_pos[] = {-40, 20, 3, 1.0}; 
	float spotdir[]={0, -1.0, 0};
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(	xcam, cam, zcam,
			xcam+lx, cam,  zcam+lz,
			0.0f, 1.0f,  0.0f);

	glLightfv(GL_LIGHT0,GL_POSITION, lpos);   //Set light position
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotdir);

	
    drawFloor();
	
	glPushMatrix();
	glTranslatef(0,1000,0);
		drawBuddha();
	glPopMatrix();

	
	drawFactory();
	glPushMatrix();
		glTranslatef(teapot_pos,bunny_pos_y,0);
		glTranslatef(box_pos_stage_2,0,0);
		glTranslatef(-10,5,2);
		glRotatef(box_y,0,1,0);
		glTranslatef(10,-5,-2);
		glTranslatef(box_pos,0,0);
		glLightfv(GL_LIGHT1, GL_POSITION, spot_pos);
	glPopMatrix();
	
	//draw boxes
	glColor3f(230/255.0, 228/255.0, 216/255.0);
	glPushMatrix();
		glTranslatef(box_pos_stage_2,0,0);
		glTranslatef(-10,5,2);
		glRotatef(box_y,0,1,0);
		glTranslatef(10,-5,-2);
		glTranslatef(box_pos,0,0);
		glTranslatef(-40,6,2);
		glutSolidCube(2);
	glPopMatrix();
	
	//draw bunny
	glPushMatrix();
		glTranslatef(teapot_pos,bunny_pos_y,0);
		glTranslatef(20,4.5,2);
		glScalef(15,15,15);
		displayBunny();
	glPopMatrix();
	
	glPushMatrix();
		glTranslatef(-robot_pos,0,0);
		glTranslatef(-10,0,2);
		drawRobot();
	glPopMatrix();
	
	//draw clock in factory
	glPushMatrix();
		glTranslatef(0,15,-24.25);
		drawClock();
	glPopMatrix();
		glutSwapBuffers();
	glFlush();

}

//------- Initialize OpenGL parameters -----------------------------------
void initialize()
{
	float grey[4] = {0.2, 0.2, 0.2, 1.0};
	
	loadMeshFile("195.off");
	loadTexture();

	glEnable(GL_LIGHTING);					//Enable OpenGL states
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT0, GL_SPECULAR, black);
    
    glEnable(GL_LIGHT1);
    
    glLightfv(GL_LIGHT1, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT1, GL_SPECULAR, white);    
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 0.01);
    
	
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
 	glEnable(GL_COLOR_MATERIAL);
 	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialf(GL_FRONT, GL_SHININESS, 10);
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
			xcam += lx * fraction;
			zcam += lz * fraction;
			break;
		case GLUT_KEY_DOWN :
			xcam -= lx * fraction;
			zcam -= lz * fraction;
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

float dx=0.5;
float dtheta=10;
float dpelvis=0.7;
int stage=1;
float suction=8;
float Vvelocity=0;
//  ------- Timer ----------------------------------------------------------
void stage1(){
	if(box_pos<19){
	box_pos+=dx;
	}
	if(robot_arm_angle<97){
		robot_arm_angle+=dtheta;
		}
	if(robot_arm_angle<35){
		robot_joint_angle+=dtheta;
		}
	if(box_pos==19){
		stage=2;
		}
	
	}
void stage2(){
	if (claw_angle>63){
		claw_angle-=dtheta;
		}
	else{
	if(robot_arm_y<180){
	robot_arm_y+=dtheta;
	box_y=robot_arm_y;
	}
	else{
		if(robot_pos>-5){
			robot_pos-=dx;
			box_pos_stage_2+=dx;
			}else{
				stage=3;}
		}
}

	}
void stage3(int value){

	if(claw_angle<80){
					claw_angle+=dtheta;
					}else{
	//reverse robot
	if(robot_pos<0){
		robot_pos+=dx;
		}
	if(robot_arm_y>0){
		robot_arm_y-=dtheta;
		}
		//done reverse
		if (box_pos_stage_2<19){
			box_pos_stage_2+=dx;
				}else{
					if (teapot_pos<24){
		teapot_pos+=dx;
		if(teapot_pos>16 && bunny_pos_y<15){
				Vvelocity+=suction*value/1000;
				bunny_pos_y+=Vvelocity*value/1000;
				
		}
		}else{
			stage=4;
			}
		}
		}
			

	}
void resetObj(){
	box_pos=0;
	box_pos_stage_2=0;
	box_y=0;
	Vvelocity=0;
	bunny_pos_y=0;
	teapot_pos=0;
	stage=1;
	}
void Timer(int value){
	second-=6;
	minute=second/60;
	hour=minute/60;
	
	switch(stage){
	case 1:
	stage1();
	break;
	case 2:
	stage2();
	break;
	case 3:
	stage3(value);
	break;
	case 4:
	resetObj();
	break;
	}
	
	    glutPostRedisplay();
    glutTimerFunc(100, Timer, value);
}

//  ------- Main: Initialize glut window and register call backs -----------
int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH);
   glutInitWindowSize (600, 600); 
   glutInitWindowPosition (10, 10);
   glutCreateWindow ("Factory");
   initialize();

   glutDisplayFunc(display);
   glutSpecialFunc(processSpecialKeys); 
   glutTimerFunc(100,Timer,100);
   glutMainLoop();
   return 0;
}
