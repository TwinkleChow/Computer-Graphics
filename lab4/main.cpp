// CS3241Lab4.cpp : Defines the entry point for the console application.
//#include <cmath>
#include "math.h"
#include <iostream>
#include <fstream>

/* Include header files depending on platform */
#ifdef _WIN32
#include "GL\freeglut.h"
#define M_PI 3.14159
#elif __APPLE__
#include <OpenGL/gl.h>
#include <GLUT/GLUT.h>
#endif

#define MAXPTNO 1000
#define NLINESEGMENT 32
#define NOBJECTONCURVE 8

using namespace std;

// Global variables that you can use
struct Point {
    int x,y;
};

// Storage of control points
int nPt = 0;
Point ptList[MAXPTNO];
Point greyPointList[MAXPTNO];
int greyPt = 0;

Point pos[MAXPTNO];
int posPt =0;
double angleList[MAXPTNO];

// Display options
bool displayControlPoints = true;
bool displayControlLines = true;
bool displayTangentVectors = false;
bool displayObjects = false;
bool C1Continuity = false;
bool back = true;
bool previous = false;
bool change = false;
bool animation = false;
int sec = 0;


void drawRightArrow()
{
    glColor3f(0,1,0);
    glBegin(GL_LINE_STRIP);
    glVertex2f(0,0);
    glVertex2f(100,0);
    glVertex2f(95,5);
    glVertex2f(100,0);
    glVertex2f(95,-5);
    glEnd();
}

int factorial(int k){
    int result =1;
    for(int i=k;i>0;i--){
        result *= i;
        
    }
    return result;
}

void drawCurve(){
    
    int i=0;
    while(i<nPt){
        
        glLineWidth(2);
        glBegin(GL_LINE_STRIP);
        
        glColor3f(0,0,0);
        glVertex2d(ptList[i].x, ptList[i].y);
        
        
        if(i+3<nPt){
            
            for(int k=1;k<NLINESEGMENT;k++){
                
                double t=k/double(NLINESEGMENT);
                double x_pos=0;
                double y_pos=0;
                
                double coef;
                double base;
                
                for(int j=0;j<4;j++){
                    
                    coef = factorial(3)/double((factorial(3-j)*factorial(j)));
                    base = pow((1-t),3-j)*pow(t,j);
                    x_pos += coef*base*ptList[i+j].x;
                    y_pos += coef*base*ptList[i+j].y;
                    
                }
                
                glColor3f(0,0,0);
                glVertex2d(x_pos, y_pos);
                
            }
            glColor3f(0,0,0);
            glVertex2d(ptList[i+3].x, ptList[i+3].y);
            
        }
        i +=3;
        glEnd();
        
        
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


void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    change = false;
    
    if(!previous&&back){
        
        if(C1Continuity){
            
            //last time is reversed to the non C1continuity
            //we need to pop the greyPtList and recalculate
            change = true;
            greyPt=0;
            previous = back;
            back = false;
        }
        
    }
    
    else if(previous&&!back){
        
        //last time is recalculated the second pt
        
        if(!C1Continuity){
            //we have to do the reverse
            change = true;
            previous = back;
            back = true;
            
        }
        
    }
    
    
    for(int i=0;i<nPt;i+=3){
        
        if(C1Continuity){
            
            if(i&&(i+3<nPt)){
                
                if(change){
                    //push the second point as grey point;
                    greyPointList[greyPt].x = ptList[i+1].x;
                    greyPointList[greyPt].y = ptList[i+1].y;
                    
                    greyPt++;
                    
                    
                    //alter the second point
                    ptList[i+1].x = ptList[i].x*2-ptList[i-1].x;
                    ptList[i+1].y = ptList[i].y*2-ptList[i-1].y;
                    
                }
            }
        }
        else{
            
            if(back){
                
                if(i&&(i+3<nPt)&&(i/3-1<greyPt)){
                    //reverse back
                    ptList[i+1].x = greyPointList[(i/3-1)].x;
                    ptList[i+1].y = greyPointList[(i/3-1)].y;
                    
                }
            }
        }
    }
    
    
    drawCurve();
    
    if(displayControlPoints)
    {
        
        glPointSize(5);
        glBegin(GL_POINTS);
        for(int i=0;i<nPt; i++)
        {
            if(i%3!=1){
                glColor3f(0,0,0);
                glVertex2d(ptList[i].x,ptList[i].y);
            }
            
            else{
                if((C1Continuity)&&(i!=1)){
                    glColor3f(1, 0, 0);
                    glVertex2d(ptList[i].x,ptList[i].y);
                    
                }
                
                else{
                    glColor3f(0,0,0);
                    glVertex2d(ptList[i].x,ptList[i].y);
                    
                }
            }
            
            
        }
        
        if(C1Continuity){
            //draw the grey points
            for(int i=0;i<greyPt;i++){
                glColor3f(0.5, 0.5, 0.5);
                glVertex2d(greyPointList[i].x, greyPointList[i].y);
            }
            
        }
        
        glEnd();
        glPointSize(1);
        
    }
    
    if(displayControlLines)
    {
        
        int i=0;
        glLineWidth(2);
        glBegin(GL_LINE_STRIP);
        
        glColor3f(0,1,0);
        
        while(i<nPt){
            
            glVertex2d(ptList[i].x, ptList[i].y);
            
            i++;
        }
        glEnd();
        glLineWidth(1);
    }
    
    if(displayTangentVectors){
        
        
        for(int i=0;i<nPt;i+=3){
            if(i+3<nPt){
                for(int j=0;j<=NOBJECTONCURVE;j++){
                    double t =j/double(NOBJECTONCURVE);
                    double tangent_x = 0;
                    double tangent_y = 0;
                    
                    tangent_x+= 3*pow(1-t,2)*(-1)*ptList[i].x+3*(2*(1-t)*t*(-1)+pow((1-t),2))*ptList[i+1].x+3*((1-t)*2*t-t*t)*ptList[i+2].x+3*t*t*ptList[i+3].x;
                    
                    tangent_y+= 3*pow(1-t,2)*(-1)*ptList[i].y+3*(2*(1-t)*t*(-1)+pow((1-t),2))*ptList[i+1].y+3*((1-t)*2*t-t*t)*ptList[i+2].y+3*t*t*ptList[i+3].y;
                    
                    
                    double x_pos =0;
                    double y_pos =0;
                    for(int k=0;k<4;k++){
                        double coef = factorial(3)/double((factorial(3-k)*factorial(k)));
                        double base = pow((1-t),3-k)*pow(t,k);
                        x_pos += coef*base*ptList[i+k].x;
                        y_pos += coef*base*ptList[i+k].y;
                    }
                    
                    if(t!=1){
                        glVertex2d(x_pos, y_pos);
                    }
                    else{
                        glVertex2d(ptList[i+3].x, ptList[i+3].y);
                    }
                    
                    double cos_value = tangent_y/double(sqrt(pow(tangent_x,2)+pow(tangent_y,2)));
                    double radius = acos(cos_value);
                    double angle = radius *180/double(3.1415926535);
                    
                    glPushMatrix();
                    glTranslated(x_pos, y_pos, 0);
                    glRotated(angle-90, 0, 0, -1);
                    drawRightArrow();
                    glPopMatrix();
                    
                    
                }
            }
            
        }
        
    }
    
    if(displayObjects){
        //recalculate the tangent vector and position
        posPt = 0;
        
        for(int i=0;i<nPt;i+=3){
            if(i+3<nPt){
                for(int j=0;j<=NOBJECTONCURVE;j++){
                    double t =j/double(NOBJECTONCURVE);
                    double tangent_x = 0;
                    double tangent_y = 0;
                    
                    tangent_x+= 3*pow(1-t,2)*(-1)*ptList[i].x+3*(2*(1-t)*t*(-1)+pow((1-t),2))*ptList[i+1].x+3*((1-t)*2*t-t*t)*ptList[i+2].x+3*t*t*ptList[i+3].x;
                    
                    tangent_y+= 3*pow(1-t,2)*(-1)*ptList[i].y+3*(2*(1-t)*t*(-1)+pow((1-t),2))*ptList[i+1].y+3*((1-t)*2*t-t*t)*ptList[i+2].y+3*t*t*ptList[i+3].y;
                    
                    
                    double x_pos =0;
                    double y_pos =0;
                    for(int k=0;k<4;k++){
                        double coef = factorial(3)/double((factorial(3-k)*factorial(k)));
                        double base = pow((1-t),3-k)*pow(t,k);
                        x_pos += coef*base*ptList[i+k].x;
                        y_pos += coef*base*ptList[i+k].y;
                    }
                    
                    if(t!=1){
                        glVertex2d(x_pos, y_pos);
                    }
                    else{
                        glVertex2d(ptList[i+3].x, ptList[i+3].y);
                    }
                    
                    double cos_value = tangent_y/double(sqrt(pow(tangent_x,2)+pow(tangent_y,2)));
                    double radius = acos(cos_value);
                    double angle = radius *180/double(3.1415926535);
                    
                    pos[posPt].x = x_pos;
                    pos[posPt].y = y_pos;
                    angleList[posPt]=angle;
                    
                    posPt++;
                    
                    if((i)&(!j))
                        continue;
                    glPushMatrix();
                    
                    glTranslated(x_pos, y_pos, 0);
                    glRotated(angle-90, 0, 0, -1);
                    glScaled(3, 3, 0);
                    drawClockSun();
                    glPopMatrix();
                    
                    
                }
            }
            
        }
        
        
        
    }
    
    if(animation){
        
        displayObjects = false;
        
        int index = sec%posPt;
        
        glPushMatrix();
        glTranslated(pos[index].x, pos[index].y, 0);
        glRotated(angleList[index]-90, 0, 0, -1);
        glScaled(3, 3, 0);
        drawClockSun();
        glPopMatrix();
        
    
    }
    
    glPopMatrix();
    glutSwapBuffers ();
}

void reshape (int w, int h)
{
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0,w,h,0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
}

void init(void)
{
    glClearColor (1.0,1.0,1.0, 1.0);
}

void readFile()
{
    
    std::ifstream file;
    file.open("savefile.txt");
    file >> nPt;
    
    
    
    if(nPt>MAXPTNO)
    {
        cout << "Error: File contains more than the maximum number of points." << endl;
        nPt = MAXPTNO;
    }
    
    
    for(int i=0;i<nPt;i++)
    {
        file >> ptList[i].x;
        file >> ptList[i].y;
        
    }
    file.close();// is not necessary because the destructor closes the open file by default
}

void writeFile()
{
    std::ofstream file;
    file.open("savefile.txt");
    file << nPt << endl;
    
    for(int i=0;i<nPt;i++)
    {
        file << ptList[i].x << " ";
        file << ptList[i].y << endl;
        
        
    }
    file.close();// is not necessary because the destructor closes the open file by default
}

void idle()
{
    //update animation here
    if(animation){
        sec++;
        
    }
    
    glutPostRedisplay();	//after updating, draw the screen again
}


void keyboard (unsigned char key, int x, int y)
{
    switch (key) {
        case 'r':
        case 'R':
            readFile();
            break;
            
        case 'w':
        case 'W':
            writeFile();
            break;
            
        case 'T':
        case 't':
            displayTangentVectors = !displayTangentVectors;
            break;
            
        case 'o':
        case 'O':
            displayObjects = !displayObjects;
            break;
            
        case 'p':
        case 'P':
            displayControlPoints = !displayControlPoints;
            break;
            
        case 'L':
        case 'l':
            displayControlLines = !displayControlLines;
            break;
            
        case 'C':
        case 'c':
            C1Continuity = !C1Continuity;
            break;
            
        case 'e':
        case 'E':
            // Do something to erase all the control points added
            nPt=0;
            greyPt=0;
            posPt = 0;
            
            break;
        case 'a':
        case 'A':
            //animation
            animation =!animation;
            break;
            
        case 27:
            exit(0);
            break;
            
        default:
            break;
    }
    
    glutPostRedisplay();
}



void mouse(int button, int state, int x, int y)
{
    /*button: GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, or GLUT_RIGHT_BUTTON */
    /*state: GLUT_UP or GLUT_DOWN */
    enum
    {
        MOUSE_LEFT_BUTTON = 0,
        MOUSE_MIDDLE_BUTTON = 1,
        MOUSE_RIGHT_BUTTON = 2,
        MOUSE_SCROLL_UP = 3,
        MOUSE_SCROLL_DOWN = 4
    };
    if((button == MOUSE_LEFT_BUTTON)&&(state == GLUT_UP))
    {
        if(nPt==MAXPTNO)
        {
            cout << "Error: Exceeded the maximum number of points." << endl;
            return;
        }
        ptList[nPt].x=x;
        ptList[nPt].y=y;
        nPt++;
    }
    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    cout<<"CS3241 Lab 4"<< endl<< endl;
    cout << "Left mouse click: Add a control point"<<endl;
    cout << "ESC: Quit" <<endl;
    cout << "P: Toggle displaying control points" <<endl;
    cout << "L: Toggle displaying control lines" <<endl;
    cout << "E: Erase all points (Clear)" << endl;
    cout << "C: Toggle C1 continuity" <<endl;
    cout << "T: Toggle displaying tangent vectors" <<endl;
    cout << "O: Toggle displaying objects" <<endl;
    cout << "R: Read in control points from \"savefile.txt\"" <<endl;
    cout << "W: Write control points to \"savefile.txt\"" <<endl;
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize (600, 600);
    glutInitWindowPosition (50, 50);
    glutCreateWindow ("CS3241 Assignment 4");
    init ();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);
    
    glutMainLoop();
    
    return 0;
}
