// CS3241Lab1.cpp : Defines the entry point for the console application.
#include <cmath>
#include <iostream>

/* Include header files depending on platform */
#ifdef _WIN32
	#include "glut.h"
	#define M_PI 3.14159
#elif __APPLE__
	#include <OpenGL/gl.h>
	#include <GLUT/GLUT.h>
#endif

using namespace std;

float alpha = 0.0, k=1;
float tx = 0.0, ty=0.0;
int open = 1;
int background =0;

void drawArc(double cx,double cy,float r,int startDeg,int endDeg,float a,float b){
    int i;
    for(i=startDeg;i<endDeg;i++){
        glVertex2f(cx+sin(2.0*M_PI*i/360.0)*r*a,cy+cos(2.0*M_PI*i/360.0)*r*b);
    }
}

void drawArcReverse(double cx,double cy,float r,int startDeg,int endDeg,float a,float b){
    int i;
    for(i=startDeg;i>endDeg;i--){
        glVertex2f(cx+sin(2.0*M_PI*i/360.0)*r*a,cy+cos(2.0*M_PI*i/360.0)*r*b);
    }
}

void drawOval(double cx,double cy,float r,float red,float green,float blue,float a,float b,int mode){
    glColor3ub(red, green, blue);
    
    if(mode ==0){
        glBegin(GL_POLYGON);}
    else if(mode ==1){
        glBegin(GL_LINE_LOOP);
    }
    
    int i;
    for(i=0;i<361;i++){
        glVertex2f(cx+sin(2.0*M_PI*i/360.0)*r*a,cy+cos(2.0*M_PI*i/360.0)*r*b);
    }
    glEnd();
}

void drawSquare(double cx,double cy,float width,float length,float red,float green,float blue,int mode){
    glColor3ub(red, green, blue);
    
    if(mode ==0){
        glBegin(GL_POLYGON);
    }
    else{
        glBegin(GL_LINE_LOOP);
    }
    glVertex2f(cx-width/2, cy+length/2);
    glVertex2f(cx+width/2, cy+length/2);
    glVertex2f(cx+width/2, cy-length/2);
    glVertex2f(cx-width/2, cy-length/2);


    glEnd();
}

void drawHalfBowknot(){
    glPushMatrix();
    
    glRotatef(10, 0, 0, 1);
    
    drawOval(-0.7, 5, 1, 255,51,51, 1.2, 2.8, 0);
    glLineWidth(2);
    drawOval(-0.7, 5, 1, 0, 0, 0, 1.2, 2.8, 1);
    
    glPopMatrix();

}

void drawEye(){
    //eyes
    
    //outermost white one
    drawOval(-1.9, 0, 1.5, 1, 1, 1, 1, 1, 1);
    drawOval(-1.9, 0, 1.5, 255, 255, 255, 1, 1, 0);
    
    //pink middle
    drawOval(-1.77, 0, 1.35, 255, 153, 204, 1.05, 1.1, 0);
    //black middle
    drawOval(-1.5, 0, 1.2, 0, 0, 0, 1, 1, 0);
    //white innermost
    drawOval(-1, 0.2, 0.5, 255, 255, 255, 1, 1, 0);
}

void drawClosedEyes(){
    drawOval(-1.9, 0, 1.5, 1, 1, 1, 1, 1, 1);
    drawOval(-1.9, 0, 1.5, 204, 153, 255, 1, 1, 0);
    glColor3f(0, 0, 0);
    glLineWidth(2);
    glBegin(GL_LINE_STRIP);
    drawArc(-1.7, 0, 0.7, 78, 275, 1, 1);
    glEnd();
}

void drawHalfHair(){
    
    //hair with color
    glColor3ub(255, 128, 0);
    glBegin(GL_POLYGON);
    drawArc(0, -0.1, 3, -48, 0,1.2,1.0);
    glVertex2f(0, 2.9);
    glVertex2f(0, 2);
    glVertex2f(-0.5, 1.1);
    glVertex2f(-2.1, 1.1);
    glVertex2f(-2.3, 1.6);
    glVertex2f(-2.7, 1.1);
    glVertex2f(-3.3, 1);
    glEnd();
    
    //boundary
    glColor3ub(1, 1, 1);
    glLineWidth(1);
    glBegin(GL_LINE_STRIP);
    glVertex2f(0, 2);
    glVertex2f(-0.5, 1.1);
    glVertex2f(-2.1, 1.1);
    glVertex2f(-2.3, 1.6);
    glVertex2f(-2.7, 1.1);
    glVertex2f(-3.3, 1);
    glEnd();
    
    
}

void drawArm(){
    glColor3ub(255, 229, 204);
    glBegin(GL_POLYGON);
    drawArc(-3, 2.3, 5.5, 160, 185, 1, 1);
    drawArc(-3.5,-2.7,0.5,210,340,1,1);
    glVertex2f(-3, -2.3);
    glVertex2f(-2.5, -2.4);
    glEnd();
    
    //boundary
    glColor3ub(0, 0, 0);
    glBegin(GL_LINE_LOOP);
    drawArc(-3, 2.3, 5.5, 160, 185, 1, 1);
    drawArc(-3.5,-2.7,0.5,210,340,1,1);
    glVertex2f(-3, -2.3);
    glVertex2f(-2.5, -2.4);
    glEnd();
}

void drawBackHair(){
    drawArcReverse(0, -2, 4, 180, 137, 1, 1);
    drawArc(5.5, -2, 4, 225, 270, 1, 1);

}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_LINE_SMOOTH);
    
    //set the transparency for the background
    
    glClear(GL_COLOR_BUFFER_BIT);
    glPointSize(8);
    glBegin(GL_POLYGON);
    if(background ==0){
        
    glColor4ub(51,153,255,255);
    glVertex2f(10,10);
    glColor4ub(51,153,255,255);
    glVertex2f(-10,10);
    glColor4ub(51,153,255,1);
    glVertex2f(-10,-10);
    glColor4ub(51,153,255,1);
        glVertex2f(10,-10);}
    else{
        glColor4ub(128,255,0,255);
        glVertex2f(10,10);
        glColor4ub(128,255,0,255);
        glVertex2f(-10,10);
        glColor4ub(128,255,0,1);
        glVertex2f(-10,-10);
        glColor4ub(128,255,0,1);
        glVertex2f(10,-10);
    }
    glEnd();
    glFlush();
	
	glPushMatrix();

	//controls transformation
	glScalef(k, k, k);	
	glTranslatef(tx, ty, 0);	
	glRotatef(alpha, 0, 0, 1);
	
	//draw your stuff here
    

    //Blossom
    
    //draw the hair at the back
    
    glPushMatrix();
    
    glColor3ub(255, 128, 0);
    glBegin(GL_POLYGON);
    drawBackHair();
    glEnd();
    
    glColor3ub(0, 0, 0);
    glLineWidth(2);

    glBegin(GL_LINE_STRIP);
    drawBackHair();
    glEnd();
    
    glRotatef(180, 0, 1, 0);
    
    glColor3ub(255, 128, 0);
    glBegin(GL_POLYGON);
    drawBackHair();
    glEnd();
    
    glColor3ub(0, 0, 0);
    glBegin(GL_LINE_STRIP);
    drawBackHair();
    glEnd();
    
    glPopMatrix();
    
    
    //add more details
    glColor3ub(0, 0, 0);
    glLineWidth(1);
    glBegin(GL_LINE_STRIP);
    drawArc(4.3, -3.3, 3, 227, 240, 1, 1);
    glEnd();
    
    glBegin(GL_LINE_STRIP);
    drawArcReverse(-4.3, -3.3, 3, 133, 113, 1, 1);
    glEnd();

    

    
    //draw body
    glLineWidth(3);

    //left leg
    drawOval(-0.5, -5, 0.8, 255, 255, 255, 1, 2.8, 0);
    drawOval(-0.5, -5, 0.8, 1, 1, 1, 1, 2.8, 1);

    
    
    //black left feet
    glColor3ub(0, 0, 0);
    glBegin(GL_POLYGON);
    glVertex2f(-0.9, -6.2);
    drawArc(-0.5, -5, 0.8, 121, 240, 1, 2.8);
    glVertex2f(0.1, -6.2);
    glEnd();
    
    //upper pink
    drawSquare(0, -3, 2.5, 0.6, 255, 153, 204, 0);
    drawSquare(0, -3, 2.5, 0.6, 0, 0, 0, 1);
    
    //lower pink
    drawSquare(0, -4.5, 2.55, 0.9, 255, 153, 204, 0);
    drawSquare(0, -4.5, 2.55, 0.9, 0, 0, 0, 1);
    
    
    //middle black
    drawSquare(0,-3.7,2.6,0.8,0,0,0,0);
    
    //right leg
    drawOval(0.8, -5, 0.7, 255, 255, 255, 1, 1.3, 0);
    drawOval(0.8, -5, 0.7, 1, 1, 1, 1, 1.3, 1);
    
    //black right feet
    glColor3ub(0, 0, 0);
    glBegin(GL_POLYGON);
    glVertex2f(0.1, -5);
    drawArc(0.8, -5, 0.7, 90, 270, 1, 1.3);
    glVertex2f(1.5, -5);
    glEnd();
    
    //small white semicircle
    glColor3ub(255, 255, 255);
    glBegin(GL_POLYGON);
    glVertex2f(0.4, -5.2);
    drawArc(0.8, -5.2, 0.4, 90, 270, 1, 1);
    glVertex2f(1.2, -5.2);
    glEnd();

    glColor3ub(255, 255, 255);
    glBegin(GL_POLYGON);
    glVertex2f(-0.9, -6.5);
    drawArc(-0.5, -6.5, 0.4, 90, 270, 1, 1);
    glVertex2f(-0.1, -6.5);
    glEnd();
    
    //draw the left arm
    glPushMatrix();
    drawArm();
    
    glRotatef(180, 0, 1, 0);
    drawArm();
    
    glPopMatrix();


    




    //draw the bowknot
    
    //draw one half
    drawHalfBowknot();
    
    //draw the other half
    glPushMatrix();
    glRotatef(180, 0, 1, 0);
    drawHalfBowknot();
    glPopMatrix();
    
    //draw the knot of the bowknot
    drawOval(0, 3, 0.5, 230, 0, 0, 1.2, 1, 0);
    glLineWidth(2);
    drawOval(0, 3, 0.5, 0, 0, 0, 1.2, 1, 1);
    
    
    //face
    drawOval(0, 0, 3, 255, 229, 204, 1.2, 1.0,0);
    glLineWidth(5);

    drawOval(0, 0, 3, 0, 0, 0, 1.2, 1.0,1);
    
    if(open==1){
    drawEye();
    
    //draw the other eye
    glPushMatrix();
    glRotatef(180, 0, 1, 0);
    drawEye();
    
        glPopMatrix();
    }
    else{
        //draw the closed eye
        drawClosedEyes();
        
        //draw the other closed eye
        glPushMatrix();
        glRotatef(180, 0, 1, 0);
        drawClosedEyes();
        
        glPopMatrix();
        
    }
    
    //draw the mouth
    glColor3f(0, 0, 0);
    glLineWidth(0.8);
    glBegin(GL_LINE_STRIP);
    drawArc(0, -1.8, 0.4, 75,285,1,1);
    glEnd();
    
 

    //draw the half hair
    
    glPushMatrix();
    
    drawHalfHair();
    
    glRotatef(180, 0, 1, 0);
    
    drawHalfHair();
    
    glPopMatrix();
    
 
    

    

	glPopMatrix();
	glFlush ();
    
}

void reshape (int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);

	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-10, 10, -10, 10, -10, 10);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void init(void)
{
	glClearColor (1.0, 1.0, 1.0, 1.0);
	glShadeModel (GL_SMOOTH);

    

}



void keyboard (unsigned char key, int x, int y)
{
	//keys to control scaling - k
	//keys to control rotation - alpha
	//keys to control translation - tx, ty
	switch (key) {

		case 'a':
			alpha+=10;
			glutPostRedisplay();
		break;

		case 'd':
			alpha-=10;
			glutPostRedisplay();
		break;

		case 'q':
			k+=0.1;
			glutPostRedisplay();
		break;

		case 'e':
			if(k>0.1)
				k-=0.1;
			glutPostRedisplay();
		break;

		case 'z':
			tx-=0.1;
			glutPostRedisplay();
		break;

		case 'c':
			tx+=0.1;
			glutPostRedisplay();
		break;

		case 's':
			ty-=0.1;
			glutPostRedisplay();
		break;

		case 'w':
			ty+=0.1;
			glutPostRedisplay();
		break;
        
        //key to control open the eye
        case 'o':
            open = 1;
            glutPostRedisplay();
        break;
        
        case 'p':
            open = 0;
            glutPostRedisplay();
        break;
            
		case 27:
			exit(0);
		break;

		default:
		break;
	}
}

void mouse(int button,int state,int x,int y){
    
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            if (background==0){
                background=1;
                glutPostRedisplay();
            }
            else{
                background=0;
                glutPostRedisplay();
            }
        }
    
    }

int main(int argc, char **argv)
{
	cout<<"CS3241 Lab 1\n\n";
	cout<<"+++++CONTROL BUTTONS+++++++\n\n";
	cout<<"Scale Up/Down: Q/E\n";
	cout<<"Rotate Clockwise/Counter-clockwise: A/D\n";
	cout<<"Move Up/Down: W/S\n";
	cout<<"Move Left/Right: Z/C\n";
	cout <<"ESC: Quit\n";

	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (600, 600);
	glutInitWindowPosition (50, 50);
	glutCreateWindow (argv[0]);
	init ();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	//glutMouseFunc(mouse);
    glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutMainLoop();

	return 0;
}
