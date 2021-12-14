
#include <math.h>
#include <time.h>
#include "GL/glut.h"
#include"cannon.h"
#include<iostream>
using namespace std;

float posx_init = 0.0, posy_init = 25.0, posz_init = 63.0, orientation_init = 0.0;
float g_orientation = orientation_init; // y axis
float g_posX = posx_init, g_posY = posy_init, g_posZ = posy_init;
float posy = 0, randR, randG, randB;
bool gravity = true;
float firehoogte = 50.0f;
float gravity_force = 10.0f;
bool fired = false;//check if firework was fired
bool running=true;//program running
int upType = 0; //defines how the firework ascends
int explosionStyle=0;
float angle = 0, rotationspeed = 1;
cannon firework;
particle particles[1000];


void update()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef(g_orientation, 0.0, 1.0, 0.0); // rotate in y axis
    glTranslatef(-g_posX, -g_posY, -g_posZ);

    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);
    cout<< g_posX<<endl;
    cout<< g_posY<<endl;
    cout<< g_posZ<<endl;
    cout<< g_orientation<<endl;
    cout<<"********"<<endl;
    glColor3f(1.0, 1.0, 1.0);
    // cannon base
    glBegin(GL_QUADS);
    glVertex3f(-5.0, 0.0, -5.0);
    glVertex3f(-5.0, 0.0, 5.0);
    glVertex3f(5.0, 0.0, 5.0);
    glVertex3f(5.0, 0.0, -5.0);
    glEnd();
    // ground plane
    glBegin(GL_LINE_STRIP);
    glVertex3f(-40.0, 0.0, -40.0);
    glVertex3f(-40.0, 0.0, 40.0);
    glVertex3f(40.0, 0.0, 40.0);
    glVertex3f(40.0, 0.0, -40.0);
    glVertex3f(-40.0, 0.0, -40.0);
    glEnd();

    firework.drawParticles(particles, firework.MaxParticles);
    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'a': // up
        g_posY = g_posY + 1.0;
        break;
    case 'z': // down
        g_posY = g_posY - 1.0;
        break;
    case 'j': // left
        g_orientation = g_orientation - 15.0;
        break;
    case 'l': // right
        g_orientation = g_orientation + 15.0;
        break;
    case 'i': // forwards
        g_posX = g_posX + sin(g_orientation / 180.0 * M_PI);
        g_posZ = g_posZ - cos(g_orientation / 180.0 * M_PI);
        break;
    case 'k': //backwards
        g_posX = g_posX - sin(g_orientation / 180.0 * M_PI);
        g_posZ = g_posZ + cos(g_orientation / 180.0 * M_PI);
        break;
    case 'f': // fire
        posy = 0;
        for(int i =0; i<4; i++){
            firework.random[i]=rand()%firework.MaxParticles;
        }
        firework.fireCannon(particles, firework.MaxParticles);
        fired = true;
        upType=0;
        break;
    case 'w':
        posy = 0;
        firework.fireCannon(particles, firework.MaxParticles);
        fired = true;
        upType = 1;
        break;
    case 's':
        posy = 0;
        firework.fireCannon(particles, firework.MaxParticles);
        fired = true;
        upType = 2;
        rotationspeed = 1;
        angle= 0;
        break;
    case '0':
        explosionStyle=0;
        break;
    case '1':
        explosionStyle=1;
        break;
    case '2':
        explosionStyle=2;
        break;
    case '3':
        explosionStyle=3;
        break;
    case 27: //exit the program
        exit(0);
        break;
    }

    glutPostRedisplay();
}

void timer(int value)
{
    int i;
    static int lastTime;
    int thisTime;
    float time;

    thisTime = glutGet(GLUT_ELAPSED_TIME);
    time = (thisTime - lastTime) / 500.0;
    lastTime = thisTime;
    for (i = 0; i < firework.MaxParticles; i = i + 1)
    {
        //gravity
        if(gravity && fired)
            particles[i].v_y = particles[i].v_y - gravity_force * time;

        //straight up        
        if(particles[i].v_y>0){
            particles[i].y = particles[i].y + particles[i].v_y * time;
            if(upType==1){
                particles[i].x = 2*sin((particles[i].y)/3);
            }else if(upType==2){
                particles[i].x = 5*cos(angle);
                particles[i].z = 5*sin(angle);
            }
        }
        
    }
    if(upType==2){
        angle+=rotationspeed;
        if(rotationspeed>=0)
            rotationspeed-=0.04;
    }

    if(particles[0].v_y<0)
        firework.explode(particles, firework.MaxParticles, explosionStyle);

    glutPostRedisplay();
    glutTimerFunc(50, &timer, 0);
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutCreateWindow("Particle Cannon");
    glutReshapeWindow(1920, 1080);
    glMatrixMode(GL_PROJECTION);
    gluPerspective(120.0, 1.0, 1.0, 1000.0);
    glEnable(GL_DEPTH_TEST);
    glutDisplayFunc(&update);
    glutKeyboardFunc(&keyboard);
    glutTimerFunc(50, &timer, 0);
    glutMainLoop();
    return 0;
}