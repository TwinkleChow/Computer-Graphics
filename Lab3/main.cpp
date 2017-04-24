// CS3241Lab3.cpp : Defines the entry point for the console application.
//#include <cmath>
#include "math.h"
#include <iostream>
#include <fstream>

#ifdef _WIN32
#include "Windows.h"
#include "glut.h"
#define M_PI 3.14159
#elif __APPLE__
#include <OpenGL/gl.h>
#include <GLUT/GLUT.h>
#endif

// global variable

bool m_Smooth = false;
bool m_Highlight = false;
GLfloat angle = 0;   /* in degrees */
GLfloat angle2 = 0;   /* in degrees */
GLfloat zoom = 1.0;
GLfloat field_of_view = 40.0;
GLfloat x_translation = 0.0;

int mouseButton = 0;
int moving, startx, starty;

#define NO_OBJECT 4;
int current_object = 0;

double fovy = 40.0;
double Znear = 1.0;
double Zfar = 80.0;
int upVector = 1;

double eyeX = 0;
double eyeY = 0;
double eyeZ = 0;
double centerX = 0;
double centerY = 0;
double centerZ = -1;
double upX = 0;
double upY = 1;
double upZ = 0;


bool Animation_Material = false;
bool Animation_Camera = false;

int Animation_M = 0;
double Animation_C = 0;


using namespace std;

void setupLighting()
{
    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);
    
    // Lights, material properties
    GLfloat	ambientProperties[] = { 0.7f, 0.7f, 0.7f, 1.0f };
    GLfloat	diffuseProperties[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat	specularProperties[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat lightPosition[] = { -100.0f, 100.0f, 100.0f, 1.0f };
    
    glClearDepth(1.0);
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientProperties);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseProperties);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularProperties);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    
    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 0.0);
    
    // Default : lighting
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    
}

void drawQuarterSphere(double r) {
    int n = 20;
    int i, j;
    for (i = 0; i<n / 2; i++)
        for (j = 0; j<n / 2; j++)
            if (m_Smooth)
            {
                glBegin(GL_POLYGON);
                
                // the normal of each vertex is actaully its own coordinates normalized for a sphere
                
                // your normal here
                glNormal3d(r*sin(i*M_PI / n)*cos(j*M_PI / n), r*cos(i*M_PI / n)*cos(j*M_PI / n), r*sin(j*M_PI / n));
                
                
                glVertex3d(r*sin(i*M_PI / n)*cos(j*M_PI / n), r*cos(i*M_PI / n)*cos(j*M_PI / n), r*sin(j*M_PI / n));
                
                // your normal here
                glNormal3d(r*sin((i + 1)*M_PI / n)*cos(j*M_PI / n), r*cos((i + 1)*M_PI / n)*cos(j*M_PI / n), r*sin(j*M_PI / n));
                
                
                glVertex3d(r*sin((i + 1)*M_PI / n)*cos(j*M_PI / n), r*cos((i + 1)*M_PI / n)*cos(j*M_PI / n), r*sin(j*M_PI / n));
                // your normal here
                glNormal3d(r*sin((i + 1)*M_PI / n)*cos((j + 1)*M_PI / n), r*cos((i + 1)*M_PI / n)*cos((j + 1)*M_PI / n), r*sin((j + 1)*M_PI / n));
                
                
                glVertex3d(r*sin((i + 1)*M_PI / n)*cos((j + 1)*M_PI / n), r*cos((i + 1)*M_PI / n)*cos((j + 1)*M_PI / n), r*sin((j + 1)*M_PI / n));
                // your normal here
                glNormal3d(r*sin(i*M_PI / n)*cos((j + 1)*M_PI / n), r*cos(i*M_PI / n)*cos((j + 1)*M_PI / n), r*sin((j + 1)*M_PI / n));
                
                glVertex3d(r*sin(i*M_PI / n)*cos((j + 1)*M_PI / n), r*cos(i*M_PI / n)*cos((j + 1)*M_PI / n), r*sin((j + 1)*M_PI / n));
                glEnd();
            }
            else {
                glBegin(GL_POLYGON);
                // Explanation: the normal of the whole polygon is the coordinate of the center of the polygon for a sphere
                glNormal3d(sin((i + 0.5)*M_PI / n)*cos((j + 0.5)*M_PI / n), cos((i + 0.5)*M_PI / n)*cos((j + 0.5)*M_PI / n), sin((j + 0.5)*M_PI / n));
                glVertex3d(r*sin(i*M_PI / n)*cos(j*M_PI / n), r*cos(i*M_PI / n)*cos(j*M_PI / n), r*sin(j*M_PI / n));
                glVertex3d(r*sin((i + 1)*M_PI / n)*cos(j*M_PI / n), r*cos((i + 1)*M_PI / n)*cos(j*M_PI / n), r*sin(j*M_PI / n));
                glVertex3d(r*sin((i + 1)*M_PI / n)*cos((j + 1)*M_PI / n), r*cos((i + 1)*M_PI / n)*cos((j + 1)*M_PI / n), r*sin((j + 1)*M_PI / n));
                glVertex3d(r*sin(i*M_PI / n)*cos((j + 1)*M_PI / n), r*cos(i*M_PI / n)*cos((j + 1)*M_PI / n), r*sin((j + 1)*M_PI / n));
                glEnd();
            }
    
    
}


void drawSphere(double r)
{
    //blue sphere
    
    float no_mat[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    float mat_ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
    float mat_diffuse[] = { 0.1f, 0.5f, 0.8f, 1.0f };
    float mat_emission[] = { 0.3f, 0.2f, 0.2f, 0.0f };
    float mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float shininess = 100;
    float no_shininess = 0.0f;
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
    
    if (m_Highlight)
    {
        // your codes for highlight here
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialf(GL_FRONT, GL_SHININESS, shininess);
        
    }
    else {
        glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
        glMaterialf(GL_FRONT, GL_SHININESS, no_shininess);
    }
    
    glPushMatrix();
    
    for (int j = 0; j<2; j++) {
        
        for (int i = 0; i<4; i++) {
            glPushMatrix();
            glRotated(90 * i, 0, 0, 1);
            drawQuarterSphere(r);
            glPopMatrix();
            
        }
        glRotated(180, 0, 1, 0);
    }
    
    glPopMatrix();
}

void drawPinkSphere(double r)
{
    //blue sphere
    float no_mat[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    float mat_ambient[] = { 1.0f, 0.6f, 0.8f, 1.0f };
    float mat_diffuse[] = { 0.1f, 0.5f, 0.8f, 1.0f };
    float mat_emission[] = { 0.3f, 0.2f, 0.2f, 0.0f };
    float mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float shininess = 100;
    float no_shininess = 0.0f;
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
    
    if (m_Highlight)
    {
        // your codes for highlight here
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialf(GL_FRONT, GL_SHININESS, shininess);
        
    }
    else {
        glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
        glMaterialf(GL_FRONT, GL_SHININESS, no_shininess);
    }
    
    glPushMatrix();
    
    for (int j = 0; j<2; j++) {
        
        for (int i = 0; i<4; i++) {
            glPushMatrix();
            glRotated(90 * i, 0, 0, 1);
            drawQuarterSphere(r);
            glPopMatrix();
            
        }
        glRotated(180, 0, 1, 0);
    }
    
    glPopMatrix();
}

void drawCompositeSphere(double r, double d) {
    
    float no_mat[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    
    float mat_ambient[] = { 1.0f, 0.6f, 0.1f, 0.5f };
    float mat_diffuse[] = { 0.1f, 0.5f, 0.8f, 1.0f };
    float mat_emission[] = { 0.3f, 0.2f, 0.2f, 0.0f };
    float mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float shininess = 100;
    float no_shininess = 0.0f;
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
    
    if (m_Highlight)
    {
        // your codes for highlight here
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialf(GL_FRONT, GL_SHININESS, shininess);
        
    }
    else {
        glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
        glMaterialf(GL_FRONT, GL_SHININESS, no_shininess);
    }
    
    glPushMatrix();
    
    for (int j = 0; j<2; j++) {
        for (int i = 0; i<4; i++) {
            glPushMatrix();
            glRotated(90 * i, 1, 0, 0);
            glTranslated(d, d, d);
            
            drawQuarterSphere(r);
            glPopMatrix();
            
        }
        glRotated(180, 0, 1, 0);
    }
    glPopMatrix();
    
}


void drawHp(double r, double a, double b) {
    
    float no_mat[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    float mat_ambient[] = { 0.5f, 0.3f, 0.8f, 1.0f };
    float mat_diffuse[] = { 0.5f, 0.5f, 0.8f, 1.0f };
    float mat_emission[] = { 0.3f, 0.2f, 0.2f, 0.0f };
    float mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float shininess = 100;
    float no_shininess = 0.0f;
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
    
    if (m_Highlight)
    {
        // your codes for highlight here
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialf(GL_FRONT, GL_SHININESS, shininess);
        
    }
    else {
        glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
        glMaterialf(GL_FRONT, GL_SHININESS, no_shininess);
    }
    
    
    int i, j;
    int n = 20;
    for (i = 0; i<n; i++)
        for (j = 0; j<2 * n; j++)
            if (m_Smooth)
            {
                glBegin(GL_POLYGON);
                
                double b_sq = pow(b, 2);
                double a_sq = pow(a, 2);
                
                
                double x = r*sin(i*M_PI / n)*cos(j*M_PI / n);
                double y = r*sin(i*M_PI / n)*sin(j*M_PI / n);
                
                glNormal3d(-2 * x / a_sq, 2 * y / b_sq, -1);
                glVertex3d(x, y, pow(y, 2) / b_sq - pow(x, 2) / a_sq);
                
                x = r*sin((i + 1)*M_PI / n)*cos(j*M_PI / n);
                y = r*sin((i + 1)*M_PI / n)*sin(j*M_PI / n);
                
                glNormal3d(-2 * x / a_sq, 2 * y / b_sq, -1);
                glVertex3d(x, y, pow(y, 2) / b_sq - pow(x, 2) / a_sq);
                
                x = r*sin((i + 1)*M_PI / n)*cos((j + 1)*M_PI / n);
                y = r*sin((i + 1)*M_PI / n)*sin((j + 1)*M_PI / n);
                
                glNormal3d(-2 * x / a_sq, 2 * y / b_sq, -1);
                glVertex3d(x, y, pow(y, 2) / b_sq - pow(x, 2) / a_sq);
                
                x = r*sin(i*M_PI / n)*cos((j + 1)*M_PI / n);
                y = r*sin(i*M_PI / n)*sin((j + 1)*M_PI / n);
                
                glNormal3d(-2 * x / a_sq, 2 * y / b_sq, -1);
                glVertex3d(x, y, pow(y, 2) / b_sq - pow(x, 2) / a_sq);
                glEnd();
            }
    
            else {
                glBegin(GL_POLYGON);
                // Explanation: the normal of the whole polygon is the coordinate of the center of the polygon for a sphere
                
                double b_sq = pow(b, 2);
                double a_sq = pow(a, 2);
                
                
                double x = sin((i + 0.5)*M_PI / n)*cos((j + 0.5)*M_PI / n);
                double y = sin((i + 0.5)*M_PI / n)*sin((j + 0.5)*M_PI / n);
                
                glNormal3d(-2 * x / a_sq, 2 * y / b_sq, -1);
                
                x = r*sin(i*M_PI / n)*cos(j*M_PI / n);
                y = r*sin(i*M_PI / n)*sin(j*M_PI / n);
                
                glVertex3d(x, y, pow(y, 2) / b_sq - pow(x, 2) / a_sq);
                
                x = r*sin((i + 1)*M_PI / n)*cos(j*M_PI / n);
                y = r*sin((i + 1)*M_PI / n)*sin(j*M_PI / n);
                
                glVertex3d(x, y, pow(y, 2) / b_sq - pow(x, 2) / a_sq);
                
                x = r*sin((i + 1)*M_PI / n)*cos((j + 1)*M_PI / n);
                y = r*sin((i + 1)*M_PI / n)*sin((j + 1)*M_PI / n);
                
                glVertex3d(x, y, pow(y, 2) / b_sq - pow(x, 2) / a_sq);
                
                x = r*sin(i*M_PI / n)*cos((j + 1)*M_PI / n);
                y = r*sin(i*M_PI / n)*sin((j + 1)*M_PI / n);
                
                
                glVertex3d(x, y, pow(y, 2) / b_sq - pow(x, 2) / a_sq);
                glEnd();
            }
    
}

void drawCone(double r, double a, double b, double s) {
    
    float no_mat[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    float mat_ambient[] = { 1.0f, 0.5f, 1.0f, 1.0f };
    float mat_diffuse[] = { 0.5f, 0.5f, 0.8f, 1.0f };
    float mat_emission[] = { 0.3f, 0.2f, 0.2f, 0.0f };
    float mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float shininess = 100;
    float no_shininess = 0.0f;
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
    
    if (m_Highlight)
    {
        // your codes for highlight here
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialf(GL_FRONT, GL_SHININESS, shininess);
        
    }
    else {
        glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
        glMaterialf(GL_FRONT, GL_SHININESS, no_shininess);
    }
    
    
    int i, j;
    int n = 20;
    for (i = 0; i<n; i++)
        for (j = 0; j<2 * n; j++)
            if (m_Smooth)
            {
                glBegin(GL_POLYGON);
                double b_sq = pow(b, 2);
                double a_sq = pow(a, 2);
                
                
                double x = r*sin(i*M_PI / n)*cos(j*M_PI / n);
                double y = r*sin(i*M_PI / n)*sin(j*M_PI / n);
                
                glNormal3d(2 * x / a_sq, 2 * y / b_sq, -s);
                glVertex3d(x, y, (pow(y, 2) / b_sq + pow(x, 2) / a_sq)*s);
                
                x = r*sin((i + 1)*M_PI / n)*cos(j*M_PI / n);
                y = r*sin((i + 1)*M_PI / n)*sin(j*M_PI / n);
                
                glNormal3d(2 * x / a_sq, 2 * y / b_sq, -s);
                glVertex3d(x, y, (pow(y, 2) / b_sq + pow(x, 2) / a_sq)*s);
                
                x = r*sin((i + 1)*M_PI / n)*cos((j + 1)*M_PI / n);
                y = r*sin((i + 1)*M_PI / n)*sin((j + 1)*M_PI / n);
                
                glNormal3d(2 * x / a_sq, 2 * y / b_sq, -s);
                glVertex3d(x, y, (pow(y, 2) / b_sq + pow(x, 2) / a_sq)*s);
                
                x = r*sin(i*M_PI / n)*cos((j + 1)*M_PI / n);
                y = r*sin(i*M_PI / n)*sin((j + 1)*M_PI / n);
                
                glNormal3d(2 * x / a_sq, 2 * y / b_sq, -s);
                glVertex3d(x, y, (pow(y, 2) / b_sq + pow(x, 2) / a_sq)*s);
                glEnd();
            }
    
            else {
                glBegin(GL_POLYGON);
                // Explanation: the normal of the whole polygon is the coordinate of the center of the polygon for a sphere
                
                double b_sq = pow(b, 2);
                double a_sq = pow(a, 2);
                
                
                double x = sin((i + 0.5)*M_PI / n)*cos((j + 0.5)*M_PI / n);
                double y = sin((i + 0.5)*M_PI / n)*sin((j + 0.5)*M_PI / n);
                
                glNormal3d(2 * x / a_sq, 2 * y / b_sq, -s);
                
                x = r*sin(i*M_PI / n)*cos(j*M_PI / n);
                y = r*sin(i*M_PI / n)*sin(j*M_PI / n);
                
                glVertex3d(x, y, (pow(y, 2) / b_sq + pow(x, 2) / a_sq)*s);
                
                x = r*sin((i + 1)*M_PI / n)*cos(j*M_PI / n);
                y = r*sin((i + 1)*M_PI / n)*sin(j*M_PI / n);
                
                glVertex3d(x, y, (pow(y, 2) / b_sq + pow(x, 2) / a_sq)*s);
                
                x = r*sin((i + 1)*M_PI / n)*cos((j + 1)*M_PI / n);
                y = r*sin((i + 1)*M_PI / n)*sin((j + 1)*M_PI / n);
                
                glVertex3d(x, y, (pow(y, 2) / b_sq + pow(x, 2) / a_sq)*s);
                
                x = r*sin(i*M_PI / n)*cos((j + 1)*M_PI / n);
                y = r*sin(i*M_PI / n)*sin((j + 1)*M_PI / n);
                
                
                glVertex3d(x, y, (pow(y, 2) / b_sq + pow(x, 2) / a_sq)*s);
                glEnd();
            }
    
}





void display(void)
{//Add Projection tool and Camera Movement somewhere here
    
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fovy,/* aspect ratio */ 1.0, Znear, Zfar);
    
    //camera animation
    if (Animation_Camera) {
        
        eyeZ = -6 + 5 * cos(Animation_C);
        eyeY = 0;
        eyeX = 5 * sin(Animation_C);
        centerX = 0;
        centerY = 0;
        centerZ = -6;
        upX = 0;
        upY = 1;
        upZ = 0;
        
    }
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    int upVector = 1;
    gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ);
    
    
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslatef(0, 0, -6);
    glRotatef(angle2, 1.0, 0.0, 0.0);
    glRotatef(angle, 0.0, 1.0, 0.0);
    
    glScalef(zoom, zoom, zoom);
    
    switch (current_object) {
        case 0:
            
            if (Animation_Material) {
                //material animation
                if (Animation_M % 10) {
                    drawSphere(1);
                }
                else {
                    drawPinkSphere(1);
                }
            }
            else {
                drawSphere(1);
            }
            
            
            break;
        case 1:
            // draw your second primitive object here
            drawHp(1, 1, 1);
            
            break;
        case 2:
            // draw your first composite object here
            glPushMatrix();
            for (int i = 1; i<5; i++) {
                glRotated(72, 0, 0, 1);
                drawCompositeSphere(0.2*i, 0.1*i);
            }
            glPopMatrix();
            drawHp(0.2, 0.5, 0.5);
            break;
        case 3:
            
            // draw your second composite object here
            drawPinkSphere(0.5);
            //draw surrounding cones
            
            for (int j = 0; j<3; j++) {
                
                glPushMatrix();
                glRotated(60 * j, 0, 0, 1);
                for (int i = 0; i<6; i++) {
                    glPushMatrix();
                    glRotated(60 * i, 0, 1, 0);
                    glTranslated(0, 0, -1);
                    drawCone(0.1, 1, 1, 50);
                    glPopMatrix();
                }
                glPopMatrix();
            }
            
            break;
        default:
            break;
    };
    glPopMatrix();
    glutSwapBuffers();
}


void resetCamera() {
    /*
     //fill in values below.
     zoom = 1.0;
     angle =
     angle2 =
     zoom =
     field_of_view =
     x_translation =
     // include resetting of gluPerspective & gluLookAt.
     */
    
    angle = 0;   /* in degrees */
    angle2 = 0;   /* in degrees */
    zoom = 1.0;
    field_of_view = 40.0;
    x_translation = 0.0;
    
    fovy = 40.0;
    Znear = 1.0;
    Zfar = 80.0;
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fovy,/* aspect ratio */ 1.0, Znear, Zfar);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    upVector = 1;
    eyeX = 0;
    eyeY = 0;
    eyeZ = 0;
    centerX = 0;
    centerY = 0;
    centerZ = -1;
    upX = 0;
    upY = 1;
    upZ = 0;
    
    gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ);
    
}

void setCameraBestAngle() {
    /*
     //fill in values below
     zoom =
     angle =
     angle2 =
     zoom =
     field_of_view =
     x_translation =
     //TIPS: Adjust gluLookAt function to change camera position
     */
    
    angle = 0;   /* in degrees */
    angle2 = 0;   /* in degrees */
    zoom = 1.0;
    field_of_view = 40.0;
    x_translation = 0.0;
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fovy,/* aspect ratio */ 1.0, Znear, Zfar);
    
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    eyeX = 1;
    eyeY = 2;
    eyeZ = 1;
    centerX = 1;
    centerY = 1;
    centerZ = -1;
    upX = 0;
    upY = 1;
    upZ = 0;
    gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ);
    
    return;
}

void keyboard(unsigned char key, int x, int y)
{//add additional commands here to change Field of View and movement
    switch (key) {
        case 'p':
        case 'P':
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;
        case 'w':
        case 'W':
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            break;
        case 'v':
        case 'V':
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
            break;
        case 's':
        case 'S':
            m_Smooth = !m_Smooth;
            break;
        case 'h':
        case 'H':
            m_Highlight = !m_Highlight;
            break;
            
        case 'n':
            // -Insert code to adjust Near-
            Znear--;
            break;
        case 'N':
            // -Insert code to adjust Near-
            Znear++;
            break;
            
        case 'f':
            // -Insert code to adjust Far-
            Zfar--;
            break;
            
        case 'F':
            // -Insert code to adjust Far-
            Zfar++;
            break;
            
        case 'o':
            // -Insert code to adjust Fovy-
            fovy -= 3;
            break;
            
        case 'O':
            // -Insert code to adjust Fovy-
            fovy += 3;
            break;
            
        case 'r':
            resetCamera();
            break;
            
        case 'R':
            setCameraBestAngle();
            break;
        case 'm':
            Animation_Material = !Animation_Material;
            break;
        case 'c':
            Animation_Camera = !Animation_Camera;
            break;
            
        case '1':
        case '2':
        case '3':
        case '4':
            current_object = key - '1';
            break;
            
        case 27:
            exit(0);
            break;
            
        default:
            break;
    }
    
    glutPostRedisplay();
}



void
mouse(int button, int state, int x, int y)
{
    if (state == GLUT_DOWN) {
        mouseButton = button;
        moving = 1;
        startx = x;
        starty = y;
    }
    if (state == GLUT_UP) {
        mouseButton = button;
        moving = 0;
    }
}

void motion(int x, int y)
{
    if (moving) {
        if (mouseButton == GLUT_LEFT_BUTTON)
        {
            angle = angle + (x - startx);
            angle2 = angle2 + (y - starty);
        }
        else zoom += ((y - starty)*0.01);
        startx = x;
        starty = y;
        glutPostRedisplay();
    }
    
}

void idle() {
    //update animation here
    if (Animation_Camera) {
        Animation_C += 0.01;
    }
    if (Animation_Material) {
        Animation_M++;
    }
    glutPostRedisplay();
    
    
}


int main(int argc, char **argv)
{
    cout << "CS3241 Lab 3" << endl << endl;
    
    cout << "1-4: Draw different objects" << endl;
    cout << "S: Toggle Smooth Shading" << endl;
    cout << "H: Toggle Highlight" << endl;
    cout << "W: Draw Wireframe" << endl;
    cout << "P: Draw Polygon" << endl;
    cout << "V: Draw Vertices" << endl;
    cout << "n, N: Reduce or increase the distance of the near plane from the camera" << endl;
    cout << "f, F: Reduce or increase the distance of the far plane from the camera" << endl;
    cout << "o, O: Reduce or increase the distance of the povy plane from the camera" << endl;
    cout << "r: Reset camera to the initial parameters when the program starts" << endl;
    cout << "R: Change camera to another setting that is has the best viewing angle for your object" << endl;
    cout << "ESC: Quit" << endl << endl;
    
    
    cout << "Left mouse click and drag: rotate the object" << endl;
    cout << "Right mouse click and drag: zooming" << endl;
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("CS3241 Assignment 3");
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glMatrixMode(GL_PROJECTION);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyboard);
    setupLighting();
    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    
    
    
    glutMainLoop();
    
    return 0;
}
