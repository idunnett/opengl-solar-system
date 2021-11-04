//Isaac Dunnett (B00800378)
//CSCI 3136: Intro to Graphics and Animation
//Assignment #2

//An animated solar system scene with a spaceship and various controls and functions

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#else
#include <freeglut.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

const int numStars = 100;
const float windowWidth = 700;
const float windowHeight = 700;
const float pi = 3.14159;

GLfloat theta = 0.0, explodeRadius = 1.0;
GLfloat planetsScale = 0.175;

GLfloat stars[numStars][3];

float starshipVertices[2000][3];
int starshipFaces[2000][3];

bool showRings = false, showStars = false, showCorona = false, showBoosters = true, explode = false;

GLdouble cameraX = 0, cameraY = 1, cameraZ = 5;
GLdouble spaceShipTiltX = 0, spaceShipTiltY = 0;

float randomInRange(float min, float max)
{
    float random = ((float) rand()) / (float) RAND_MAX;
    float range = max - min;
    return (random*range) + min;
}

void initializeRandomStars(void){
    srand(time(NULL));
    for(int i=0; i<numStars; i++){
        stars[i][0] = randomInRange(-4, 4);
        stars[i][1] = randomInRange(-4, 4);
        stars[i][2] = -10;
    }
}

//read in enterprise text file and store values in starshipVertices and starshipFaces
void readStarship(void){
    FILE* file;
    if((file = fopen("enterprise.txt", "r")) == NULL){
       printf("Error opening file\n");
        return;
    }
    char* indicator;
    float num1, num2, num3;
    int v=0;
    int f=0;
    while (fscanf(file, "%s %f %f %f", indicator, &num1, &num2, &num3) == 4)
    {
        if(!strcmp(indicator, "v")){
            starshipVertices[v][0] = num1;
            starshipVertices[v][1] = num2;
            starshipVertices[v][2] = num3;
            v++;
        }
        else if(!strcmp(indicator, "f")){
            starshipFaces[f][0] = (int)num1;
            starshipFaces[f][1] = (int)num2;
            starshipFaces[f][2] = (int)num3;
            f++;
        }
    }
}

//helper function for drawPlanets that draws a sphere for the planet base
void sphere(void)
{
    double x, y, z, thet, phi, c;
    x=y=0;
    z=1;
    glBegin(GL_TRIANGLE_FAN);
    glVertex3d(x,y,z);
    c=3.14159/180.0;
    z = sin(c*80.0);
    for(thet=-180.0; thet<=180.0;thet+=20.0)
    {
        x=sin(c*thet)*cos(c*80.0);
        y=cos(c*thet)*cos(c*80.0);
        glVertex3d(x,y,z);
    }
    glEnd();
    x=y=0;
    z=-1;
    glBegin(GL_TRIANGLE_FAN);
    glVertex3d(x,y,z);
    z=-sin(c*80.0);
    for(thet=-180.0; thet<=180.0;thet+=20.0)
    {
        x=sin(c*thet)*cos(c*80.0);
        y=cos(c*thet)*cos(c*80.0);
        glVertex3d(x,y,z);
    }
    glEnd();
    for(phi=-80.0; phi<=80.0; phi+=20.0)
    {
        glBegin(GL_QUAD_STRIP);
        for(thet=-180.0; thet<=180.0;thet+=20.0)
        {
            x=sin(c*thet)*cos(c*phi);
            y=cos(c*thet)*cos(c*phi);
            z=sin(c*phi);
            glVertex3d(x,y,z);
            
            x=sin(c*thet)*cos(c*(phi+20.0));
            y=cos(c*thet)*cos(c*(phi+20.0));
            z=sin(c*(phi+20.0));
            glVertex3d(x,y,z);
        }
        glEnd();
    }
}

//helper function for drawPlanets to draw a ring around a planet
void handleDrawRing(float distance, int plane){
    if(showRings){
        glColor3f(1, 1, 1);
        glBegin(GL_LINE_STRIP);
        switch (plane) {
            case 0:
                for (int i=0; i <= 360; i++) {
                    glVertex3f(distance * sin(i*pi / 180), 0, distance * cos(i*pi/180));
                }
                break;
            case 1:
                for (int i=0; i <= 360; i++) {
                    glVertex3f(distance * sin(i*pi / 180), distance * cos(i*pi/180), 0);
                }
                break;
            case 2:
                for (int i=0; i <= 360; i++) {
                    glVertex3f(0, distance * sin(i*pi / 180), distance * cos(i*pi/180));
                }
                break;
            default:
                for (int i=0; i <= 360; i++) {
                    glVertex3f(distance * sin(i*pi / 180), 0, distance * cos(i*pi/180));
                }
                break;
        }
        glEnd();
    }
}

//function that draws all of the planets with their rotations
void drawPlanets(void){
    glPushMatrix();
    glScalef(planetsScale, planetsScale, planetsScale);
    glPushMatrix();
    //  SUN
    glColor3f(1, 0.9, 0);
    glScalef(explodeRadius, explodeRadius, explodeRadius);
    sphere();
    glPopMatrix();
    
    //  DARK RED PLANET
    glPushMatrix();
    // Ring
    handleDrawRing(3, 0);
    glColor3f(0.4, 0.2, 0.2);
    glRotatef(theta*2, 0, 1, 0);
    glTranslatef(-3, 0, 0);
    glScalef(0.3, 0.3, 0.3);
    sphere();
    glPopMatrix();
    
    //  PINK PLANET
    glPushMatrix();
    // Ring
    handleDrawRing(5, 0);
    glColor3f(1, 0.5, 0.9);
    glRotatef(theta, 0, 1, 0);
    glTranslatef(5, 0, 0);
    glScalef(0.5, 0.5, 0.5);
    sphere();
    // moon
    handleDrawRing(1.5, 0);
    glColor3f(0.6, 0.8, 0.8);
    glRotatef(theta * 5, 0, 1, 0);
    glTranslatef(1.5, 0, 0);
    glScalef(0.3, 0.3, 0.3);
    sphere();
    glPopMatrix();
    
    //  BLUE PLANET
    glPushMatrix();
    // Ring
    handleDrawRing(7, 0);
    glColor3f(0, 0.5, 1);
    glRotatef(theta*0.5, 0, 1, 0);
    glTranslatef(-7, 0, 0);
    glScalef(0.6, 0.6, 0.6);
    sphere();
    glPushMatrix();
    // moon
    handleDrawRing(1.5, 1);
    glColor3f(0.5, 0.5, 0.5);
    glRotatef(theta * 3, 0, 0, 1);
    glTranslatef(0, 1.5, 0);
    glScalef(0.2, 0.2, 0.2);
    sphere();
    glPopMatrix();
    // moon
    handleDrawRing(2, 2);
    glColor3f(1, 0.8, 1);
    glRotatef(theta * 3, 1, 0, 0);
    glTranslatef(0, 0, 2);
    glScalef(0.25, 0.25, 0.25);
    sphere();
    glPopMatrix();
    
    //  GREEN PLANET
    glPushMatrix();
    // Ring
    handleDrawRing(8.7, 0);
    glColor3f(0.8, 0.9, 0.2);
    glRotatef(theta*1.5, 0, 1, 0);
    glTranslatef(8.7, 0, 0);
    glScalef(0.6, 0.6, 0.6);
    sphere();
    glPopMatrix();
    
    //  ORANGE PLANET
    glPushMatrix();
    // Ring
    handleDrawRing(10, 0);
    glColor3f(1, 0.5, 0);
    glRotatef(theta, 0, 1, 0);
    glTranslatef(-10, 0, 0);
    glScalef(0.2, 0.2, 0.2);
    sphere();
    // moon
    handleDrawRing(2, 0);
    glColor3f(0.9, 0.9, 0.9);
    glRotatef(theta * 3, 0, 1, 0);
    glTranslatef(-2, 0, 0);
    glScalef(0.3, 0.3, 0.3);
    sphere();
    glPopMatrix();
    
    //  WHITE PLANET
    glPushMatrix();
    // Ring
    handleDrawRing(11.5, 0);
    glColor3f(1, 1, 1);
    glRotatef(theta*0.5, 0, 1, 0);
    glTranslatef(11.5, 0, 0);
    glScalef(0.45, 0.45, 0.45);
    sphere();
    glPopMatrix();
    glPopMatrix();
}

//Display callback, clears frame buffer and depth buffer,
//Draws planets, stars & spaceship
void myDisplay(void)
{
    // clear the screen and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // load the identity matrix into the model view matrix
    glLoadIdentity();
    
    // set the camera position
    gluLookAt(cameraX, cameraY, cameraZ, cameraX, cameraY-0.2, cameraZ-1, 0, 1, 0);

    //draw the planets
    drawPlanets();
    
    //draw stars if toggled ('s')
    if(showStars){
        glPushMatrix();
        glTranslatef((GLfloat)cameraX, (GLfloat)cameraY, (GLfloat)cameraZ+4);
        glBegin(GL_POINTS);
        for(int i=0; i<numStars; i++){
            glColor3f((float)rand()/(float)RAND_MAX, (float)rand()/(float)RAND_MAX, (float)rand()/(float)RAND_MAX);
            glVertex3fv(stars[i]);
        }
        glEnd();
        glPopMatrix();
    }
    
    //start of space ship drawing
    glPushMatrix();
    //position it in front and down from camera
    glTranslatef(cameraX, cameraY -1, cameraZ-2.1);
    glScalef(0.6, 0.6, 0.6);
    //handle ship tilt (from up, down, left, right arrow keys)
    if(spaceShipTiltX)
        glRotatef(spaceShipTiltX, 0, 0, 1);
    else if(spaceShipTiltY)
        glRotatef(spaceShipTiltY, 1, 0, 0);
    //always rotate this much
    glRotatef(5, 1, 0, 0);
    //draw space ship
    glBegin(GL_TRIANGLES);
    float color = 1;
    for(int i=0; i<2000; i++){
        if(!starshipFaces[i] || !starshipFaces[i][0] || !starshipFaces[i][1] || !starshipFaces[i][2])
            break;
        glColor3f(color, color, color);
        color -= 0.0003;
        glVertex3fv(starshipVertices[starshipFaces[i][0]]);
        glVertex3fv(starshipVertices[starshipFaces[i][1]]);
        glVertex3fv(starshipVertices[starshipFaces[i][2]]);
    }
    glEnd();
    
    // draw boosters if boosters are toggled on ('b')
    if(showBoosters){
        glLineWidth(3);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBegin(GL_LINES);
        for(int i=0; i<10; i++){
            float x = randomInRange(0.212, 0.26);
            glColor4f(1, 0.9, 0, 1);
            glVertex3f(x, 0.2, 0.7);
            glColor4f(1, 0.1, 0, 0);
            glVertex3f(x, 0.2, 1.2);
        }
        for(int i=0; i<10; i++){
            float x = randomInRange(-0.22, -0.262);
            glColor4f(1, 0.9, 0, 1);
            glVertex3f(x, 0.2, 0.7);
            glColor4f(1, 0.1, 0, 0);
            glVertex3f(x, 0.2, 1.2);
        }
        glEnd();
        glLineWidth(1);
    }
    glPopMatrix();
    
    //draw corona effect if c has been pressed
    if(showCorona){
        glPushMatrix();
        glScalef(0.17, 0.17, 0.17);
        glScalef(explodeRadius, explodeRadius, explodeRadius);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glLineWidth(10);
        for (int i=0; i <= 100; i++) {
            int x = (int)randomInRange(0, 360);
            glBegin(GL_LINES);
            glColor4f(1, 0.9, 0, 1);
            glVertex3f(sin(x*pi / 180), cos(x*pi/180), 0);
            glColor4f(1, 0.1, 0, 0);
            glVertex3f(sin(x*pi / 180)*1.6, cos(x*pi/180)*1.6, 0);
            glEnd();
        }
        glLineWidth(1);
        glPopMatrix();
    }
    
    // swap the drawing buffers
    glutSwapBuffers();
}

//Updates the animation when idle.
void myIdle(void)
{
    // update the rotation around the selected axis
    theta += 1;
    // update explosion if q has been pressed
    if(explode)
        explodeRadius *= 1.05;
    
    // exit program after explosion
    if(explodeRadius > 20)
        exit(0);
    
    // redraw the new state
    glutPostRedisplay();
}

//regular key press functions
void onKeyPress(unsigned char key, int x, int y){
    if(key == 'r')
        showRings = !showRings;
    if (key == 's')
        showStars = !showStars;
    if(key == 'c')
        showCorona = !showCorona;
    if(key == 'b')
        showBoosters = !showBoosters;
    if(key == '+')
        planetsScale += 0.005;
    if(key == '-'){
        if(planetsScale > 0.08)
            planetsScale -= 0.005;
        else
            printf("Minimum size reached.\n");
    }
    if(key == 'q'){
        showCorona = true;
        explode = true;
    }
}
//special key up functions
void onSpecialKeyUp(int key, int x, int y){
    if(key == GLUT_KEY_UP){
        spaceShipTiltY = 0;
    }
    if(key == GLUT_KEY_DOWN){
        spaceShipTiltY = 0;
    }
    if(key == GLUT_KEY_RIGHT){
        spaceShipTiltX = 0;
    }
    if(key == GLUT_KEY_LEFT){
        spaceShipTiltX = 0;
    }
}
//special key press functions
void onSpecialKeyPress(int key, int x, int y){
    if(key == GLUT_KEY_UP){
        spaceShipTiltY = 5;
        cameraY += 0.1;
    }
    if(key == GLUT_KEY_DOWN){
        spaceShipTiltY = -5;
        cameraY -= 0.1;
    }
    if(key == GLUT_KEY_RIGHT){
        spaceShipTiltX = -5;
        cameraX += 0.1;
    }
    if(key == GLUT_KEY_LEFT){
        spaceShipTiltX = 5;
        cameraX -= 0.1;
    }
    if(key == GLUT_KEY_PAGE_UP)
        cameraZ -= 0.1;
    if(key == GLUT_KEY_PAGE_DOWN)
        cameraZ += 0.1;
}


//Initializes the OpenGL rendering context for display.
void initializeGL(void)
{
    // enable depth testing
    glEnable(GL_DEPTH_TEST);
    
    // set background color to be black
    glClearColor(0,0,0,1.0);
    
    // change into projection mode so that we can change the camera properties
    glMatrixMode(GL_PROJECTION);
    
    glPointSize(2);
    
    // load the identity matrix into the projection matrix
    glLoadIdentity();
    
    // gluPerspective(fovy, aspect, near, far)
    gluPerspective(45, (float)windowWidth/(float)windowHeight, 0.1, 20);
    
    // change into model-view mode so that we can change the object positions
    glMatrixMode(GL_MODELVIEW);
}

//Prints the scene and camera controls
void printInstructions(void){
    printf("\nScene Controls\n-----------------\n\n");
    printf("r: toggle rings\n");
    printf("s: toggle stars\n");
    printf("c: toggle the sun's corona\n");
    printf("b: toggle boosters\n");
    printf("+: increase size of solar system\n");
    printf("-: decrease size of solar system\n");
    printf("q: explode the sun and quit\n\n");
    printf("Camera Controls\n-----------------\n\n");
    printf("Up\t\tArrow:\tmove up\n");
    printf("Down\tArrow:\tmove down\n");
    printf("Right\tArrow:\tmove right\n");
    printf("Left\tArrow:\tmove left\n");
    printf("PAGE\tUP:\t\tforward\n");
    printf("PAGE\tDOWN:\tbackward\n\n");
}

//Sets up the openGL rendering context and the windowing
//system, then begins the display loop.
int main(int argc, char** argv)
{
    // initialize the toolkit
    glutInit(&argc, argv);
    // set display mode
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    // set window size
    glutInitWindowSize(windowWidth,windowHeight);
    // set window position on screen
    glutInitWindowPosition(400, 100);
    // open the screen window
    glutCreateWindow("Solar System");
    
    // register redraw function
    glutDisplayFunc(myDisplay);
    // register the idle function
    glutIdleFunc(myIdle);
    // register the key functions
    glutKeyboardFunc(onKeyPress);
    glutSpecialUpFunc(onSpecialKeyUp);
    glutSpecialFunc(onSpecialKeyPress);
    
    //initialize the rendering context
    initializeGL();
    // generate random star positions
    initializeRandomStars();
    
    readStarship();
    printInstructions();
    // go into a perpetual loop
    glutMainLoop();
}
