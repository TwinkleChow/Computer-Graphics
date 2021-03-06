// CS3241Lab5.cpp
#include <cmath>
#include <iostream>
#include "vector3D.h"
#include <chrono>
#include <float.h>

/* Include header files depending on platform */
#ifdef _WIN32
#include "GL\freeglut.h"
#define M_PI 3.14159
#elif __APPLE__
#include <OpenGL/gl.h>
#include <GLUT/GLUT.h>
#endif

using namespace std;

#define WINWIDTH 600
#define WINHEIGHT 400
#define NUM_OBJECTS 4
#define MAX_RT_LEVEL 50
#define NUM_SCENE 2

float* pixelBuffer = new float[WINWIDTH * WINHEIGHT * 3];

class Ray { // a ray that start with "start" and going in the direction "dir"
public:
    Vector3 start, dir;
};

class RtObject {
    
public:
    virtual double intersectWithRay(Ray, Vector3& pos, Vector3& normal) = 0; // return a -ve if there is no intersection. Otherwise, return the smallest postive value of t
    
    // Materials Properties
    double ambiantReflection[3];
    double diffusetReflection[3];
    double specularReflection[3];
    double speN = 300;
    
    
    
};
class Sphere : public RtObject {
    
    Vector3 center_;
    double r_;
public:
    Sphere(Vector3 c, double r) { center_ = c; r_ = r; };
    Sphere() {};
    void set(Vector3 c, double r) { center_ = c; r_ = r; };
    double intersectWithRay(Ray, Vector3& pos, Vector3& normal);
};

class Hyperboloid: public RtObject{

    Vector3 center_;
    double d_;

public:
    Hyperboloid(Vector3 c,double d){ center_ =c;d_ =d;};
    Hyperboloid(){};
    void set(Vector3 c,double d){ center_ = c;d_ = d;};
    double intersectWithRay(Ray, Vector3& pos, Vector3&normal);
};

RtObject **objList; // The list of all objects in the scene


// Global Variables
// Camera Settings
Vector3 cameraPos(0, 0, -500);

// assume the the following two vectors are normalised
Vector3 lookAtDir(0, 0, 1);
Vector3 upVector(0, 1, 0);
Vector3 leftVector(1, 0, 0);
float focalLen = 500;

// Light Settings

Vector3 lightPos(900, 1000, -1500);
double ambiantLight[3] = { 0.4,0.4,0.4 };
double diffusetLight[3] = { 0.7,0.7, 0.7 };
double specularLight[3] = { 0.5,0.5, 0.5 };


double bgColor[3] = { 0.1,0.1,0.4 };

int sceneNo = 0;
/*==============================
 Modify Section Here
 Hint: Add additional methods to help you in solving equations
 ==============================*/


double Sphere::intersectWithRay(Ray r, Vector3& intersection, Vector3& normal)
// return a -ve if there is no intersection. Otherwise, return the smallest postive value of t
{// Step 1
    
    double a;
    double b;
    double c;
    
    /*
     α =  D·D
     β = 2 D·( r0  - R0 )
     γ = ( r0  - R0 )·( r0  - R0 ) - q 2
     =  R0 2 + r0 2 - 2 r0 · R0   - q 2*/
    
    a = dot_prod(r.dir, r.dir);
    b = dot_prod(r.dir,r.start-center_)*2;
    c = dot_prod(center_,center_)+dot_prod(r.start,r.start)-dot_prod(r.start,center_)*2-r_*r_;
    
    double delta = b*b-4*a*c;
    if (delta<=0){
        
        return -1;
    }
    else{
        
        double t1 = (-b+sqrt(delta))/(2*a);
        double t2 =(-b-sqrt(delta))/(2*a);
        
        if(t1>t2){
            double tempt = t1;
            t1 = t2;
            t2 = tempt;
        }
        
        //t1<=t2
        if(t1>0){
            //0<t1<=t2
            intersection = r.start+r.dir*t1;
            normal = intersection-center_;
            normal.normalize();
            
            return t1;
        }
        else if(t2>0){
            //t1<=0<t2
            intersection = r.start+r.dir*t2;
            normal = intersection-center_;
            normal.normalize();
            
            return t2;
            
        }
        else{
            //t1<=t2<=0;
            return -1;
        }
        
        
    }
    
}

double Hyperboloid::intersectWithRay(Ray r, Vector3& intersection, Vector3& normal)
// return a -ve if there is no intersection. Otherwise, return the smallest postive value of t
{// Step 1

    double a;
    double b;
    double c;
    
    /*
     equation of hyperboloid : (x-a)^2+(y-b)^2-(z-c)^2=d
     */
    
    a = r.dir.x[0]*r.dir.x[0]+r.dir.x[1]*r.dir.x[1]-r.dir.x[2]*r.dir.x[2];
    b = r.start.x[0] * r.dir.x[0]*2+r.start.x[1] * r.dir.x[1]*2-r.start.x[2] * r.dir.x[2]*2-2*center_.x[0]*r.dir.x[0]-2*center_.x[1]*r.dir.x[1]+2*center_.x[2]*r.dir.x[2];
    c = r.start.x[0]*r.start.x[0]+r.start.x[1]*r.start.x[1]-r.start.x[2]*r.start.x[2]-d_+center_.x[0]*center_.x[0]+center_.x[1]*center_.x[1]-center_.x[2]*center_.x[2]-2*center_.x[0]*r.start.x[0]-2*center_.x[1]*r.start.x[1]+2*center_.x[2]*r.start.x[2];
    
    double delta = b*b-4*a*c;
    if (delta<=0){
        
        return -1;
    }
    else{
        
        double t1 = (-b+sqrt(delta))/(2*a);
        double t2 =(-b-sqrt(delta))/(2*a);
        
        if(t1>t2){
            double tempt = t1;
            t1 = t2;
            t2 = tempt;
        }
        
        //t1<=t2
        if(t1>0){
            //0<t1<=t2
            intersection = r.start+r.dir*t1;
            if((intersection.x[2]<center_.x[2]+d_)&&(intersection.x[2]>center_.x[2]-d_)){
            
            normal = intersection-center_;
            normal.normalize();
            
                return t1;}
            else{
                return -1;
            
            }
        }
        else if(t2>0){
            //t1<=0<t2
            intersection = r.start+r.dir*t2;
            
            if((intersection.x[2]<center_.x[2]+d_)&&(intersection.x[2]>center_.x[2]-d_)){
            normal = intersection-center_;
            normal.normalize();
          
                return t2;}
               else{
                   return -1;
               }
            
        }
        else{
            //t1<=t2<=0;
            return -1;
        }
        
        
    }
    
}



void addAnotherScene() {
    //Step 5: Modify objList here
    
    ((Sphere*)objList[0])->set(Vector3(50, 80, 100), 70);
    delete objList[1];
    objList[1] = new Hyperboloid(Vector3(130,-80,-80),30);
    ((Sphere*)objList[2])->set(Vector3(-130, -80, -80), 25);
    ((Sphere*)objList[3])->set(Vector3(0, -200, 250), 200);
    
    objList[0]->ambiantReflection[0] = 0.1;
    objList[0]->ambiantReflection[1] = 0.4;
    objList[0]->ambiantReflection[2] = 0.4;
    objList[0]->diffusetReflection[0] = 0;
    objList[0]->diffusetReflection[1] = 1;
    objList[0]->diffusetReflection[2] = 1;
    objList[0]->specularReflection[0] = 0.2;
    objList[0]->specularReflection[1] = 0.4;
    objList[0]->specularReflection[2] = 0.4;
    objList[0]->speN = 300;
    
    objList[1]->ambiantReflection[0] = 0.6;
    objList[1]->ambiantReflection[1] = 0.6;
    objList[1]->ambiantReflection[2] = 0.2;
    objList[1]->diffusetReflection[0] = 1;
    objList[1]->diffusetReflection[1] = 1;
    objList[1]->diffusetReflection[2] = 0;
    objList[1]->specularReflection[0] = 0.6;
    objList[1]->specularReflection[1] = 0.4;
    objList[1]->specularReflection[2] = 0.8;
    objList[1]->speN = 50;
    
    objList[2]->ambiantReflection[0] = 0.1;
    objList[2]->ambiantReflection[1] = 0.6;
    objList[2]->ambiantReflection[2] = 0.1;
    objList[2]->diffusetReflection[0] = 0.1;
    objList[2]->diffusetReflection[1] = 1;
    objList[2]->diffusetReflection[2] = 0.1;
    objList[2]->specularReflection[0] = 0.3;
    objList[2]->specularReflection[1] = 0.7;
    objList[2]->specularReflection[2] = 0.3;
    objList[2]->speN = 650;
    
    objList[3]->ambiantReflection[0] = 0.3;
    objList[3]->ambiantReflection[1] = 0.3;
    objList[3]->ambiantReflection[2] = 0.3;
    objList[3]->diffusetReflection[0] = 0.7;
    objList[3]->diffusetReflection[1] = 0.7;
    objList[3]->diffusetReflection[2] = 0.7;
    objList[3]->specularReflection[0] = 0.6;
    objList[3]->specularReflection[1] = 0.6;
    objList[3]->specularReflection[2] = 0.6;
    objList[3]->speN = 650;
    
    
    
}
void rayTrace(Ray ray, double& r, double& g, double& b, int fromObj = -1 ,int level = 0)
{
    // Step 4
    // Checks if it reaches MAX_RT_LEVEL
    if(level==MAX_RT_LEVEL){
        return;
    }
    
    
    int goBackGround = 1, i = 0;
    double inColor[3];
    
    Vector3 intersection, normal;
    Vector3 lightV;
    Vector3 viewV;
    Vector3 lightReflectionV;
    Vector3 rayReflectionV;
    
    Ray newRay;
    double mint = DBL_MAX, t;
    int index = -1;
    
    for (i = 0; i < NUM_OBJECTS; i++)
        //Remove above loop comment after step 1
    {
        if(i!=fromObj){
            if ((t = objList[i]->intersectWithRay(ray, intersection, normal)) > 0)
            {
                if(t<mint){
                    mint = t;
                    index = i;
                }
            }
        }
    }
    
    if(index!=-1){
        
        goBackGround = 0;
        
        // Step 2
        r = objList[index]->ambiantReflection[0]*ambiantLight[0];
        g = objList[index]->ambiantReflection[1]*ambiantLight[1];
        b = objList[index]->ambiantReflection[2]*ambiantLight[2];
        
        t = objList[index]->intersectWithRay(ray, intersection, normal);
        
        //calculate whether there's shadow
        Ray lightRay;
        lightRay.start = lightPos;
        lightRay.dir = intersection - lightPos;
        lightRay.dir.normalize();
        
        Vector3 lightintersection;
        Vector3 lightnormal;
        double myDistance;
        double otherDistance;
        bool shadow = false;
        
        myDistance =objList[index]->intersectWithRay(lightRay, lightintersection, lightnormal);
        
        for (int i=0;i<NUM_OBJECTS;i++){
            if(i!=index){
                otherDistance =objList[i]->intersectWithRay(lightRay, lightintersection, lightnormal);
                if ((otherDistance<myDistance)&&(otherDistance>0)){
                    shadow = true;
                    break;
                }
            }
        }
        
        // Step 3
        if(!shadow){
            
            lightV= lightPos-intersection;
            lightV.normalize();
            
            
            double dotprod =dot_prod(normal, lightV);
            //diffuse
            r += objList[index]->diffusetReflection[0]*diffusetLight[0]*dotprod;
            g += objList[index]->diffusetReflection[1]*diffusetLight[1]*dotprod;
            b += objList[index]->diffusetReflection[2]*diffusetLight[2]*dotprod;
            
            //specular
            
            viewV = ray.start - intersection;
            viewV.normalize();
            lightReflectionV = normal*dot_prod(normal, lightV)*2-lightV;
            lightReflectionV.normalize();
            
            double dotprodwithN = pow(dot_prod(lightReflectionV, viewV),objList[index]->speN);
            
            r += objList[index]->specularReflection[0]*specularLight[0]*dotprodwithN;
            g += objList[index]->specularReflection[1]*specularLight[1]*dotprodwithN;
            b += objList[index]->specularReflection[2]*specularLight[2]*dotprodwithN;
            
            //reflection from other object
            rayReflectionV = normal*dot_prod(normal, viewV)*2-viewV;
            rayReflectionV.normalize();
            newRay.start = intersection;
            newRay.dir = rayReflectionV;
            newRay.dir.normalize();
            
            rayTrace(newRay,inColor[0],inColor[1], inColor[2],index,++level);
            
            r += objList[index]->specularReflection[0]*inColor[0];
            g += objList[index]->specularReflection[1]*inColor[1];
            b += objList[index]->specularReflection[2]*inColor[2];
            
        }
    }
    
    if (goBackGround)
    {
        r = bgColor[0];
        g = bgColor[1];
        b = bgColor[2];
    }
    
    
    
}
/*==============================
 End of Section,
 IT IS NOT RECOMMENDED TO CHANGE ANYTHING BELOW
 ==============================*/



void drawInPixelBuffer(int x, int y, double r, double g, double b)
{
    pixelBuffer[(y*WINWIDTH + x) * 3] = (float)r;
    pixelBuffer[(y*WINWIDTH + x) * 3 + 1] = (float)g;
    pixelBuffer[(y*WINWIDTH + x) * 3 + 2] = (float)b;
}

void renderScene()
{
    int x, y;
    Ray ray;
    double r, g, b;
    
    cout << "Rendering Scene " << sceneNo << " with resolution " << WINWIDTH << "x" << WINHEIGHT << "........... ";
    long long int time1 = chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now().time_since_epoch()).count(); // marking the starting time
    
    ray.start = cameraPos;
    
    Vector3 vpCenter = cameraPos + lookAtDir * focalLen;  // viewplane center
    Vector3 startingPt = vpCenter + leftVector * (-WINWIDTH / 2.0) + upVector * (-WINHEIGHT / 2.0);
    Vector3 currPt;
    
    for(x=0;x<WINWIDTH;x++)
        for (y = 0; y < WINHEIGHT; y++)
        {
            currPt = startingPt + leftVector*x + upVector*y;
            ray.dir = currPt-cameraPos;
            ray.dir.normalize();
            rayTrace(ray, r, g, b);
            drawInPixelBuffer(x, y, r, g, b);
        }
    
    long long int time2 = chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now().time_since_epoch()).count(); // marking the ending time
    
    cout << "Done! \nRendering time = " << time2 - time1 << "ms" << endl << endl;
}


void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_DOUBLEBUFFER);
    glDrawPixels(WINWIDTH, WINHEIGHT, GL_RGB, GL_FLOAT, pixelBuffer);
    glutSwapBuffers();
    glFlush();
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    glOrtho(-10, 10, -10, 10, -10, 10);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


void setScene(int i = 0)
{
    if (i > NUM_SCENE)
    {
        cout << "Warning: Invalid Scene Number" << endl;
        return;
    }
    
    if (i == 0)
    {
        delete objList[1];
        objList[1] = new Sphere(Vector3(130, -80, -80), 100);
        
        ((Sphere*)objList[0])->set(Vector3(-130, 80, 120), 100);
//        ((Sphere*)objList[1])->set(Vector3(130, -80, -80), 100);
        ((Sphere*)objList[2])->set(Vector3(-130, -80, -80), 100);
        ((Sphere*)objList[3])->set(Vector3(130, 80, 120), 100);
        
        objList[0]->ambiantReflection[0] = 0.1;
        objList[0]->ambiantReflection[1] = 0.4;
        objList[0]->ambiantReflection[2] = 0.4;
        objList[0]->diffusetReflection[0] = 0;
        objList[0]->diffusetReflection[1] = 1;
        objList[0]->diffusetReflection[2] = 1;
        objList[0]->specularReflection[0] = 0.2;
        objList[0]->specularReflection[1] = 0.4;
        objList[0]->specularReflection[2] = 0.4;
        objList[0]->speN = 300;
        
        objList[1]->ambiantReflection[0] = 0.6;
        objList[1]->ambiantReflection[1] = 0.6;
        objList[1]->ambiantReflection[2] = 0.2;
        objList[1]->diffusetReflection[0] = 1;
        objList[1]->diffusetReflection[1] = 1;
        objList[1]->diffusetReflection[2] = 0;
        objList[1]->specularReflection[0] = 0.0;
        objList[1]->specularReflection[1] = 0.0;
        objList[1]->specularReflection[2] = 0.0;
        objList[1]->speN = 50;
        
        objList[2]->ambiantReflection[0] = 0.1;
        objList[2]->ambiantReflection[1] = 0.6;
        objList[2]->ambiantReflection[2] = 0.1;
        objList[2]->diffusetReflection[0] = 0.1;
        objList[2]->diffusetReflection[1] = 1;
        objList[2]->diffusetReflection[2] = 0.1;
        objList[2]->specularReflection[0] = 0.3;
        objList[2]->specularReflection[1] = 0.7;
        objList[2]->specularReflection[2] = 0.3;
        objList[2]->speN = 650;
        
        objList[3]->ambiantReflection[0] = 0.3;
        objList[3]->ambiantReflection[1] = 0.3;
        objList[3]->ambiantReflection[2] = 0.3;
        objList[3]->diffusetReflection[0] = 0.7;
        objList[3]->diffusetReflection[1] = 0.7;
        objList[3]->diffusetReflection[2] = 0.7;
        objList[3]->specularReflection[0] = 0.6;
        objList[3]->specularReflection[1] = 0.6;
        objList[3]->specularReflection[2] = 0.6;
        objList[3]->speN = 650;
        
    }
    
    if (i == 1)
    {
        
        // Step 5
        addAnotherScene();
    }
}

void keyboard(unsigned char key, int x, int y)
{
    //keys to control scaling - k
    //keys to control rotation - alpha
    //keys to control translation - tx, ty
    switch (key) {
        case 's':
        case 'S':
            sceneNo = (sceneNo + 1) % NUM_SCENE;
            setScene(sceneNo);
            renderScene();
            glutPostRedisplay();
            break;
        case 'q':
        case 'Q':
            exit(0);
            
        default:
            break;
    }
}

int main(int argc, char **argv)
{
    
    
    cout << "<<CS3241 Lab 5>>\n\n" << endl;
    cout << "S to go to next scene" << endl;
    cout << "Q to quit" << endl;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINWIDTH, WINHEIGHT);
    
    glutCreateWindow("CS3241 Lab 5: Ray Tracing");
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    
    glutKeyboardFunc(keyboard);
    
    objList = new RtObject*[NUM_OBJECTS];
    
    // create four spheres
    objList[0] = new Sphere(Vector3(-130, 80, 120), 100);
    objList[1] = new Sphere(Vector3(130, -80, -80), 100);
    objList[2] = new Sphere(Vector3(-130, -80, -80), 100);
    objList[3] = new Sphere(Vector3(130, 80, 120), 100);
    
    setScene(0);
    
    setScene(sceneNo);
    renderScene();
    
    glutMainLoop();
    
    for (int i = 0; i < NUM_OBJECTS; i++)
        delete objList[i];
    delete[] objList;
    
    delete[] pixelBuffer;
    
    return 0;
}
