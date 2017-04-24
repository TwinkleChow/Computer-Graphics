// CS3241Lab2.cpp : Defines the entry point for the console application.
#include <cmath>
#include <iostream>
#include <vector>
#include <sys/time.h>

/* Include header files depending on platform */
#ifdef _WIN32
	#include "glut.h"
	#define M_PI 3.14159
#elif __APPLE__
	#include <OpenGL/gl.h>
	#include <GLUT/GLUT.h>
#endif

using namespace std;

#define numStars 50
#define numPlanets 9

class planet
{
public:
	float distFromRef;
	float angularSpeed;
	GLfloat color[3];
	float size;
	float angle;

	planet()
	{
		distFromRef = 0;
		angularSpeed = 0;
		color[0] = color[1] = color[2] = 0;
		size = 0;
		angle = 0;
	}
    //construtor
    planet(float dFR,float aS,GLfloat color0,GLfloat color1,GLfloat color2,float si,float ang){
        distFromRef = dFR;
        angularSpeed = aS;
        color[0]=color0;
        color[1]=color1;
        color[2]=color2;
        size = si;
        angle = ang;
    }
};

float alpha = 0.0, k=1;
float tx = 0.0, ty=0.0;
planet planetList[numPlanets];



//planet creation

planet* sun = new planet(0,0.25,255,153,51,1,0);

planet * moon = new planet(-0.5,0.7,224,224,224,0.1,0);

planet * earth = new planet(4,0.25,153,255,255,0.25,0);

planet * mars = new planet(-4,0.15,255,255,153,0.4,0);

planet * mercury = new planet (9,0.1,0,153,76,0.3,0);

planet * jupiter = new planet (-0.5,0.8,255,255,255,0.1,0);

planet * venus = new planet (4.5,0.1,204,0,0,0.8,0);

planet * saturn = new planet (7,0.1,160,160,160,0.6,0);

planet * neptune = new planet(1,0.5,127,0,255,0.2,0);

planet * icon = new planet(-0.5,0.8,255,255,255,0.1,0);

int mynum = 10;
planet* myPlanetList [] = {sun,moon,earth,mars,mercury,jupiter,venus,saturn,neptune,icon};



vector<float> starPosx;
vector<float> starPosy;
vector<float> shine;

struct tm * time_stored_info;
float stored_sec;


int Clock=0; //indicator of whether the clock mode is open
bool up = true;//indicator of the motion of the icon
int height=6;

int starTail=30; //num of stars following venus


//some basic functions borrowed from lab 1
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

void drawOval(double cx,double cy,float r,float red,float green,float blue,float a,float b,int mode,int transparent){
    glColor4ub(red, green, blue,255);
    
    if(mode ==0){
        glBegin(GL_POLYGON);}
    else if(mode ==1){
        glBegin(GL_LINE_LOOP);
    }
    
    int i;
    for(i=0;i<361;i++){

        if(i==20){
            if(transparent==-2){
            glColor4ub(red, green, blue, 100);
            }
            else if(transparent!=-1){
                glColor4ub(red, green, blue, transparent);
            }
        }

            

        glVertex2f(cx+sin(2.0*M_PI*i/360.0)*r*a,cy+cos(2.0*M_PI*i/360.0)*r*b);
    }
    glEnd();
}

void drawPlanet(planet* myPlanet){
    glRotatef(myPlanet->angle, 0, 0, 1);
    glTranslated(myPlanet->distFromRef, 0, 0);
    drawOval(0, 0, myPlanet->size, myPlanet->color[0], myPlanet->color[1], myPlanet->color[2], 1, 1, 0,-2);

}

void drawEye(){
    //eyes
    
    //outermost white one
//    drawOval(-1.9, 0, 1.5, 1, 1, 1, 1, 1, 1);
    drawOval(-1.9, 0.7, 1.5, 255, 255, 255, 1, 1, 0,-1);
    
//    //pink middle
//    drawOval(-1.77, 0, 1.35, 255, 153, 204, 1.05, 1.1, 0);
    //black middle
    glPushMatrix();
    glScalef(0.6, 0.7, 0.6);
    drawOval(-2.4, 0.6, 1.2, 51, 25, 0, 1, 1, 0,-1);
//    //white innermost
//    drawOval(-1, 0.2, 0.5, 255, 255, 255, 1, 1, 0);
    glPopMatrix();
}


void drawClockSun(){
    //draw the flame
    drawOval(0, 2.8, 1, 255,153,51, 1, 2, 0,-1);
    glPushMatrix();
    for(int i=1;i<4;i++){
        glRotatef(90, 0, 0, 1);
        drawOval(0, 2.8, 1, 255,153,51, 1, 2, 0,-1);
        
    }
    glPopMatrix();
    
    glPushMatrix();
    glRotatef(45, 0, 0, 1);
    drawOval(0, 3.5, 1, 255,153,51, 0.8, 1.2, 0,-1);
    
    for(int i=1;i<4;i++){
        glRotatef(90, 0, 0, 1);
        drawOval(0, 3.5, 1, 255,153,51, 0.8, 1.2, 0,-1);
        
    }
    glPopMatrix();
    
    
    //face
    drawOval(0, 0, 3, 255, 230, 7, 1.2, 1.0,0,-1);
    
//    glLineWidth(5);
//    drawOval(0, 0, 3, 0, 0, 0, 1.2, 1.0,1);
    
    //scale the eyes to smaller
    glPushMatrix();
    glScalef(0.7, 0.8, 0.7);
        drawEye();
        
        //draw the other eye
        glPushMatrix();
        glRotatef(180, 0, 1, 0);
        drawEye();
        
        glPopMatrix();
    glPopMatrix();

//    //draw the mouth
//    glColor3b(51, 25, 0);
//    glLineWidth(5);
//    glBegin(GL_LINE_STRIP);
//    drawArc(0, -0.7, 0.6, 140,220,1,1);
//    glEnd();
//    


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
	glClearColor (0.0, 0.0, 0.3, 1.0);
	glShadeModel (GL_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    //initialize the vectors
    starPosx.clear();
    starPosy.clear();
    shine.clear();
    

    
    //initialize the random star position
    for(int i=0;i<numStars;i++){
        starPosx.push_back(rand()%200/double(10)-10);
        starPosy.push_back(rand()%200/double(10)-10);
  
            //initialize whether to transparency the star or not
            shine.push_back(rand()%256);

    }
    
    
    time_t time_stored = time (NULL);
    time_stored_info = localtime(&time_stored);
    stored_sec=float(time_stored_info->tm_sec);
    
    
    
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glPushMatrix();

	//controls transformation
	glScalef(k, k, k);	
	glTranslatef(tx, ty, 0);	
	glRotatef(alpha, 0, 0, 1);

	//draw stuff here!
    
    if(Clock==0){
        //draw the star on the background
        
        //microsecond
        struct timeval tp;
        gettimeofday(&tp, NULL);
        

            for(int i=0;i<numStars;i++){
                
                if(!(tp.tv_usec % 50)){
                    
                    //update the transparency
                for(int i=0;i<numStars;i++){
                    //update the transparency
                    shine[i]=rand()%256;
                    
                    
                }
            }
                drawOval(starPosx[i], starPosy[i], 0.04, 255, 255, 255, 1, 1, 0,shine[i]);
                
            }

        
    //draw the sun
    glPushMatrix();
    drawPlanet(sun);
        
    //mars orbits the sun
    //elliptical orbit
    glPushMatrix();
        if((int(mars->angle)%180)<60){
            mars->distFromRef=-4-2*((60-int(mars->angle)%180)/double(60));
        }
        else if((int(mars->angle)%180)>120){
            mars->distFromRef=-4-2*((int(mars->angle)%180-120)/double(60));
            
        }
    drawPlanet(mars);
    glPopMatrix();
    
    //mercury orbits the sun
    glPushMatrix();
    drawPlanet(mercury);
        
    
    //jupiter orbits mercury
    glPushMatrix();
    drawPlanet(jupiter);
    glPopMatrix();
        glPopMatrix();
        
    //earth orbits the sun
    
    glPushMatrix();
    drawPlanet(earth);
    //moon orbits the earth
    
    glPushMatrix();
    drawPlanet(moon);
    glPopMatrix();
    glPopMatrix();
    
    //venus orbits the sun
    glPushMatrix();
    drawPlanet(venus);
        
        //icon bumps into venus
        glPushMatrix();
        glScaled(0.1, 0.1, 0.1);
        glRotated(270, 0, 0, 1);
        glTranslatef(0, 10, 0);
        
        if(height<20&&height>0){
            
            if(up){
                height++;
            }
            else{
                height--;
            }
        }
        else if(height>=20){
            up=false;
            height--;
        }
        else if(height<=0){
            up=true;
            height++;
        }
        glTranslatef(0,height,0);
        
        drawClockSun();
        
        //starTail follows venus
        glTranslatef(0, 1, 0);
        
        for(int i=0;i<starTail/2;i++){
            
            glPushMatrix();
            glTranslatef(0.5-rand()%10/double(10), 0, 0);
            drawOval(0, 2-rand()%20/double(10), 0.05, 255, 255, 255, 1, 1, 0, -1);
            glPopMatrix();
        }
        
        for(int i=0;i<starTail/2;i++){
            
            glPushMatrix();
            glTranslatef(1-rand()%20/double(10), 0, 0);
            drawOval(0, abs(1/double(1-rand()%20/double(10)))*(rand()%10/double(10)), 0.05, 255, 255, 255, 1, 1, 0, -1);
            glPopMatrix();
        }
        


        
        
        glPopMatrix();
        glPopMatrix();

        
        //saturn orbits the sun
        glPushMatrix();
        drawPlanet(saturn);
        
        //naptune obits saturn
        glPushMatrix();
        drawPlanet(neptune);
        glPopMatrix();
        
        glPopMatrix();

    }
    else{
        //draw the clock
        drawClockSun();
        glLineWidth(5);
        
        time_t current_time = time (NULL);
        struct tm * timeinfo = localtime(&current_time);
        
        
        //microsecond
        struct timeval tp;
        gettimeofday(&tp, NULL);
        
      
        //second
        glPushMatrix();
        double angle_sec = 360-((float)timeinfo->tm_sec+tp.tv_usec/double(1000000))/60*360;
        glRotatef(angle_sec,0,0,1);
        drawOval(0, 3, 0.3, 178 , 102, 255, 1, 1, 0, -2);
        glBegin(GL_LINES);
        glVertex2f(0, 0);
        glVertex2f(0, 3);
        glEnd();
        
        glPopMatrix();
        
        //minute
        glPushMatrix();
        double angle_min = 360-(float)timeinfo->tm_min/60*360;
        glRotatef(angle_min,0,0,1);
        drawOval(0, 2, 0.2, 102 , 0, 51, 1, 1, 0, -2);
        
        glBegin(GL_LINES);
        glVertex2f(0, 0);
        glVertex2f(0, 2);
        glEnd();

        glPopMatrix();

        //hour
        glPushMatrix();
        double angle_hour = 360-((float)timeinfo->tm_hour/12*360+(float)timeinfo->tm_min/60*30);
        glRotatef(angle_hour,0,0,1);
        drawOval(0, 1.5, 0.2, 0 , 0, 0, 1, 1, 0, -2);
        
        glBegin(GL_LINES);
        glVertex2f(0, 0);
        glVertex2f(0, 1.5);
        glEnd();

        glPopMatrix();


    }
    

	glPopMatrix();
	glFlush ();
}

void idle()
{
	//update animation here

    if(Clock==0){
    for(int i=0;i<mynum;i++){
        if(i!=0){
        if(i%2){
            myPlanetList[i]->angle=myPlanetList[i]->angle+myPlanetList[i]->angularSpeed;
            }
        else{
            myPlanetList[i]->angle=myPlanetList[i]->angle-myPlanetList[i]->angularSpeed;}
        }
    }



        
    }


	glutPostRedisplay();	//after updating, draw the screen again
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
        
        case 't':
            if(Clock==0){
                Clock=1;
            }
            else{
                Clock=0;
            }
            break;
		
		case 27:
			exit(0);
		break;

		default:
		break;
	}
}

int main(int argc, char **argv)
{
	cout<<"CS3241 Lab 2\n\n";
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
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);	
	//glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutMainLoop();

	return 0;
}
