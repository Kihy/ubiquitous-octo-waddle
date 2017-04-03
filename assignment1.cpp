//  ========================================================================
//  COSC363: Assignment 1
//  ========================================================================
 
#include <iostream>
#include <fstream>
#include <climits>
#include <cmath> 
#include <GL/freeglut.h>
#include <GL/glut.h>
#include "loadTGA.h"
using namespace std;
GLuint txId[9];   //Texture ids
void loadTexture()				
{
	glGenTextures(9, txId); 	// Create 2 texture ids
	


	glBindTexture(GL_TEXTURE_2D, txId[0]);  //Use this texture
    loadTGA("wall.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	

	glBindTexture(GL_TEXTURE_2D, txId[1]);  //Use this texture
    loadTGA("floor.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	
	
	glBindTexture(GL_TEXTURE_2D, txId[2]);  //Use this texture
    loadTGA("ceiling.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	
	
	glBindTexture(GL_TEXTURE_2D, txId[3]);  //Use this texture
    loadTGA("nuke_bk.tga");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    glBindTexture(GL_TEXTURE_2D, txId[4]);  //Use this texture
    loadTGA("nuke_rt.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    glBindTexture(GL_TEXTURE_2D, txId[5]);  //Use this texture
    loadTGA("nuke_ft.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    glBindTexture(GL_TEXTURE_2D, txId[6]);  //Use this texture
    loadTGA("nuke_lf.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    glBindTexture(GL_TEXTURE_2D, txId[7]);  //Use this texture
    loadTGA("nuke_up.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    glBindTexture(GL_TEXTURE_2D, txId[8]);  //Use this texture
    loadTGA("nuke_dn.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
		cout << " File successfully read." << endl;
	
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

//-- Consts for le UFO
const int N = 42;  // Total number of vertices on the base curve
float vx[N] = {0,5,5,15,18,19,19.5,19.75,19.875,19.9,19.93,19.97,19.98,20,19.98,19.97,19.93,19.9,19.875,19.75,19.5,19,18,15,5,5,4.75,4.5,4.25,4,3.75,3.5,3,2.75,2.5,2.25,2,1.75,1.5,1.25,1,0};
float vy[N] = {0,0,4,4,4.2,4.6,4.8,5,5.2,5.4,5.6,5.8,6,6.2,6.4,6.6,6.8,7,7.2,7.4,7.6,7.8,8,8,12,14,15,15.5,15.75,15.85,15.9,15.91,15.92,15.93,15.94,15.95,15.96,15.97,15.98,15.99,16,16};
float vz[N] = {0};
void normal2(float x1, float y1, float z1, 
            float x2, float y2, float z2,
		      float x3, float y3, float z3 )
{
	  float nx, ny, nz;
	  nx = y1*(z2-z3)+ y2*(z3-z1)+ y3*(z1-z2);
	  ny = z1*(x2-x3)+ z2*(x3-x1)+ z3*(x1-x2);
	  nz = x1*(y2-y3)+ x2*(y3-y1)+ x3*(y1-y2);

      glNormal3f(nx, ny, nz);
}

void drawUFO(){
		float wx[N], wy[N], wz[N]; 
	float theta=0.174533;
	for(int j=0;j<36;j++){
	for(int i=0;i<N;i++){
		wx[i]=vx[i]*cos(theta)+vz[i]*sin(theta);
		wy[i]=vy[i];
		wz[i]=-vz[i]*sin(theta)+vz[i]*cos(theta);
		}
	glBegin(GL_TRIANGLE_STRIP);
	for(int i = 0; i < N; i++)
	{
		if(i > 0) normal2( wx[i-1], wy[i-1], wz[i-1],
		vx[i-1], vy[i-1], vz[i-1],
		vx[i], vy[i], vz[i] );
		glVertex3f(vx[i], vy[i], vz[i]);
		if(i > 0) normal2( wx[i-1], wy[i-1], wz[i-1],
		vx[i], vy[i], vz[i],
		wx[i], wy[i], wz[i] );
		glVertex3f(wx[i], wy[i], wz[i]);	
	}
	glEnd();
	
	for(int i=0;i<N;i++){
		vx[i]=wx[i];
		vy[i]=wy[i];
		vz[i]=wz[i];
		}


	}
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
	float floor_height=-0.1;
	glColor3f(0.9,0.9,0.9);	
	glNormal3f(0,1,0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, txId[1]);  //Use this texture
	float u=0,v=0;
	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
		glBegin(GL_QUADS);
			for(float i=-50;i<50;i+=0.1){
				for(float j=-25;j<25;j+=0.1){
			glTexCoord2f(u,v+0.01);glVertex3f(i, floor_height, j);
			glTexCoord2f(u,v);glVertex3f(i, floor_height, j+0.1);
			glTexCoord2f(u+0.02,v);glVertex3f(i+0.1, floor_height, j+0.1);
			glTexCoord2f(u+0.02,v+0.01);glVertex3f(i+0.1, floor_height, j);
			u+=0.01;
			v+=0.02; 
		}
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
}
float robot_arm_angle=0;
float robot_joint_angle=0;
float robot_arm_y=0;
float claw_angle=80;
//--Draws a robot------------------------------------------------------------
void drawRobot()
{


	
	
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
void drawBelt(float rad, float height, float length, int hide=0, int shadow=0){
//legs
if(!shadow){
glColor3f(0.5,0.5,0.5);
}
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
		

	if(!shadow){
	glColor3f(0,0,0);
}
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
	
	glPushMatrix();
		glTranslatef(length/2,rad,height/2);
		glScalef(length,0,height);
		glutSolidCube(1);
	glPopMatrix();
	}

//--Draws a production machine
void drawMachine(int doubleSided=0, int shadow=0)
{
	//legs
	//glColor3f(1,1,1);
	if(!shadow){
		glColor3f(207/255.0,181/255.0,59/255.0);
		}
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
	if(!shadow){
		glColor3f(212/255.0,175/255.0,55/255.0);
		}
	glPushMatrix();
		glTranslatef(0,6,0);
		glScalef(5,7,5);
		glutSolidCube(1);
	glPopMatrix();
	
	glPushMatrix();
	if(!shadow){
	glColor3f(0.1,0.1,0.1);
}
//layers
	glTranslatef(2.51,5.5,0);
	glScalef(0,5,4);
	glutSolidCube(1);
	
	glPopMatrix();
	
	if(doubleSided){
			glPushMatrix();
	glTranslatef(-2.51,5.5,0);
	glScalef(0,5,4);
	glutSolidCube(1);
	glPopMatrix();
		}
	
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
	float wall_bot=-0.1;
	int house_width=50;
	int house_length=25;
	float shadowMat[16]={18,0,0,0,
		0,0,0,-1,
		0,0,18,0,
		0,0,0,18};

	
	//walls
  glEnable(GL_TEXTURE_2D);
  	  		glBindTexture(GL_TEXTURE_2D, txId[0]);
  	  		glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
	glColor3f(1,1,1);
	glPushMatrix();
	  glBegin(GL_QUADS);
		glTexCoord2f(0,1);
		glVertex3f(-house_width, wall_height, -house_length);
		glTexCoord2f(0,0);
		glVertex3f(-house_width, wall_bot, -house_length);
		glTexCoord2f(1,0);
		glVertex3f(house_width, wall_bot, -house_length);
		glTexCoord2f(1,1);
		glVertex3f(house_width, wall_height, -house_length);
		
		glTexCoord2f(0,1);glVertex3f(house_width, wall_height, -house_length);
		glTexCoord2f(0,0);glVertex3f(house_width, wall_bot, -house_length);
		glTexCoord2f(1,0);glVertex3f(house_width, wall_bot, house_length);
		glTexCoord2f(1,1);glVertex3f(house_width, wall_height, house_length);
		
		glTexCoord2f(0,1);glVertex3f(house_width, wall_height, house_length);
		glTexCoord2f(0,0);glVertex3f(house_width, wall_bot, house_length);
		glTexCoord2f(1,0);glVertex3f(-house_width, wall_bot, house_length);
		glTexCoord2f(1,1);glVertex3f(-house_width, wall_height, house_length);
		
		glTexCoord2f(0,1);glVertex3f(-house_width, wall_height, house_length);
		glTexCoord2f(0,0);glVertex3f(-house_width, wall_bot, house_length);
		glTexCoord2f(1,0);glVertex3f(-house_width, wall_bot, -house_length);
		glTexCoord2f(1,1);glVertex3f(-house_width, wall_height, -house_length);
		
		
	  glEnd();
	  

	  		glBindTexture(GL_TEXTURE_2D, txId[2]);
	  glBegin(GL_QUADS);		
		//ceiling
		glTexCoord2f(0,10);glVertex3f(-house_width, wall_height-0.1, house_length);
		glTexCoord2f(0,0);glVertex3f(-house_width, wall_height-0.1, -house_length);
		glTexCoord2f(10,0);glVertex3f(house_width, wall_height-0.1, -house_length);
		glTexCoord2f(10,10);glVertex3f(house_width, wall_height-0.1, house_length);
		glEnd();
	glDisable(GL_TEXTURE_2D);
	glColor3f(299/255,299/255.0, 299/255.0);

	glPopMatrix();
	
	//Transmission belt
		glColor3f(0.5,0.5,0.5);
	glPushMatrix();
		glTranslatef(-40,0,0);
		drawBelt(1,4,20,1);
	glPopMatrix();
	
	glPushMatrix();
		glTranslatef(5,0,0);
		drawBelt(1,4,30,0);
	glPopMatrix();
	
	
	//draw production machine
		glColor3f(0.5,0.5,0.5);
	glPushMatrix();
		glTranslatef(-40,0,2);
		drawMachine(0,0);
	glPopMatrix();
	
	//draw transformation machine
	glColor3f(0.5,0.5,0.5);
	glPushMatrix();
		glTranslatef(20,0,2);
		drawMachine(1,0);
	glPopMatrix();
	
	//shadows
	glDisable(GL_LIGHTING);
	glPushMatrix();
	    glMultMatrixf(shadowMat);
		glTranslatef(-40,0,0);
		glColor4f(0.2,0.2,0.2,1);
		drawBelt(1,4,20,1,1);
	glPopMatrix();

	glPushMatrix();
	    glMultMatrixf(shadowMat);
		glTranslatef(5,0,0);
		glColor4f(0.2,0.2,0.2,1);
		drawBelt(1,4,30,0,1);
	glPopMatrix();


	glPushMatrix();
	    glMultMatrixf(shadowMat);
		glTranslatef(-40,0,2);
		glColor4f(0.2,0.2,0.2,1);
		drawMachine(0,1);
	glPopMatrix();
	
	glPushMatrix();
	    glMultMatrixf(shadowMat);
		glTranslatef(20,0,2);
		glColor4f(0.2,0.2,0.2,1);
		drawMachine(1,1);
	glPopMatrix();
	
	glEnable(GL_LIGHTING);
	

			
	//big vacuum
	glColor3f(105/255.0,105/255.0,105/255.0);
	glPushMatrix();
		glTranslatef(43,20,2);
		glRotatef(90,1,0,0);
		cylinder(2,5);
	glPopMatrix();
	
			
	

	}
int second=3600;
float minute=0;
float hour=0;
//--Draws a clock ---------------------------------------------------------
void drawClock(){
	glPushMatrix();
			glColor3f(0,0,0);
	//hours
		glPushMatrix();
			glRotatef(hour*5,0,0,1);
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
void loadSkybox(){
		glPushMatrix();
		//float white[4] = {1.0, 1.0, 1.0, 1.0};
		//float black[4] = {0.0, 0.0, 0.0, 1.0};
		//glMaterialfv(GL_FRONT, GL_SPECULAR, black);
		//glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glEnable(GL_TEXTURE_2D);
		// Back
		glBindTexture(GL_TEXTURE_2D, txId[3]);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0);		glVertex3f(-256.0, -256.0, -256.0);
			glTexCoord2f(1.0, 0.0);		glVertex3f( 256.0, -256.0, -256.0);
			glTexCoord2f(1.0, 1.0);		glVertex3f( 256.0,  256.0, -256.0);
			glTexCoord2f(0.0, 1.0);		glVertex3f(-256.0,  256.0, -256.0);
		glEnd();
		// Right
		glBindTexture(GL_TEXTURE_2D, txId[4]);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0);		glVertex3f(256.0, -256.0, -256.0);
			glTexCoord2f(1.0, 0.0);		glVertex3f(256.0, -256.0,  256.0);
			glTexCoord2f(1.0, 1.0);		glVertex3f(256.0,  256.0,  256.0);
			glTexCoord2f(0.0, 1.0);		glVertex3f(256.0,  256.0, -256.0);
		glEnd();
		// Front
		glBindTexture(GL_TEXTURE_2D, txId[5]);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0);		glVertex3f( 256.0, -256.0, 256.0);
			glTexCoord2f(1.0, 0.0);		glVertex3f(-256.0, -256.0, 256.0);
			glTexCoord2f(1.0, 1.0);		glVertex3f(-256.0,  256.0, 256.0);
			glTexCoord2f(0.0, 1.0);		glVertex3f( 256.0,  256.0, 256.0);
		glEnd();
		// Left
		glBindTexture(GL_TEXTURE_2D, txId[6]);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0);		glVertex3f(-256.0, -256.0,  256.0);
			glTexCoord2f(1.0, 0.0);		glVertex3f(-256.0, -256.0, -256.0);
			glTexCoord2f(1.0, 1.0);		glVertex3f(-256.0,  256.0, -256.0);
			glTexCoord2f(0.0, 1.0);		glVertex3f(-256.0,  256.0,  256.0);
		glEnd();
		// Top
		glBindTexture(GL_TEXTURE_2D, txId[7]);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0);		glVertex3f( 256.0,  256.0, -256.0);
			glTexCoord2f(1.0, 0.0);		glVertex3f( 256.0,  256.0,  256.0);
			glTexCoord2f(1.0, 1.0);		glVertex3f(-256.0,  256.0,  256.0);
			glTexCoord2f(0.0, 1.0);		glVertex3f(-256.0,  256.0, -256.0);
		glEnd();
		
	   glBindTexture(GL_TEXTURE_2D, txId[8]);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);		glVertex3f( 256.0,  -0.2, -256.0);
		glTexCoord2f(1.0, 0.0);		glVertex3f( 256.0,  -0.2,  256.0);
		glTexCoord2f(1.0, 1.0);		glVertex3f(-256.0,  -0.2,  256.0);
		glTexCoord2f(0.0, 1.0);		glVertex3f(-256.0,  -0.2, -256.0);

		 glEnd();
		glDisable(GL_TEXTURE_2D);
		//glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glPopMatrix();
	glDisable(GL_BLEND);
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
	float shadowMat[16]={18,0,0,0,
		0,0,0,-1,
		0,0,18,0,
		0,0,0,18};
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

	loadSkybox();
	
    drawFloor();
	
	glPushMatrix();
	glTranslatef(0,1000,0);
		drawBuddha();
	glPopMatrix();

	
	drawFactory();
	//spotlight
	glPushMatrix();
		glTranslatef(teapot_pos,bunny_pos_y,0);
		glTranslatef(box_pos_stage_2,0,0);
		glTranslatef(-10,5,2);
		glRotatef(box_y,0,1,0);
		glTranslatef(10,-5,-2);
		glTranslatef(box_pos,0,0);
		glColor3f(1,0,0);
		drawUFO();
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
	
	
	//robot
		glColor3f(139/255.0,69/255.0,19/255.0);
	glPushMatrix();
		glTranslatef(-robot_pos,0,0);
		glTranslatef(-10,0,2);
		drawRobot();
	glPopMatrix();
	
	//robot shadow
	glDisable(GL_LIGHTING);
	glColor4f(0.2,0.2,0.2,1);
	glPushMatrix();
		glMultMatrixf(shadowMat);
		glTranslatef(-robot_pos,0,0);
		glTranslatef(-10,0,2);
		drawRobot();
	glPopMatrix();
	glEnable(GL_LIGHTING);
	
	//draw clock in factory
	    glLightfv(GL_LIGHT0, GL_SPECULAR, black);
	glPushMatrix();
		glTranslatef(0,15,-24.25);
		drawClock();
	glPopMatrix();
	    glLightfv(GL_LIGHT0, GL_SPECULAR, white);
	
		glutSwapBuffers();
	glFlush();

}

//------- Initialize OpenGL parameters -----------------------------------
void initialize()
{
	float grey[4] = {0.2, 0.2, 0.2, 1.0};
	
	loadMeshFile("195.off");
	glEnable(GL_TEXTURE_2D);
	
	loadTexture();
		glClearColor(1., 1., 1., 1.);

	glEnable(GL_LIGHTING);					//Enable OpenGL states
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white);
    
    glEnable(GL_LIGHT1);
    
    glLightfv(GL_LIGHT1, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT1, GL_SPECULAR, white);    
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 0.01);
       glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
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
	second-=60;
	minute=second/60;
	hour=second/3600;
	
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
   glutInitWindowSize (1000, 1000); 
   glutInitWindowPosition (10, 10);
   glutCreateWindow ("Factory");
   initialize();

   glutDisplayFunc(display);
   glutSpecialFunc(processSpecialKeys); 
   glutTimerFunc(100,Timer,100);
   glutMainLoop();
   return 0;
}
