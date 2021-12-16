
#include <math.h>
#include <time.h>
#include "GL/glut.h"
#include"cannon.h"
#include<iostream>
using namespace std;

float posx_init = 0.0, posy_init = 25.0, posz_init = 54.0, orientation_init = 0.0;
float g_orientation = orientation_init; // y axis
float g_posX = posx_init, g_posY = posy_init, g_posZ = posz_init;
bool gravity = true;
float gravity_force = 10.0f;


particle particles2[1000];
cannon firework(-40.0, 0.0);
cannon currentFirework = firework;
int num = 0;

void renderGround(){
    glBegin(GL_QUADS);
    glNormal3f(0.0, 1.0, 0.0);
    glColor3f(0.0f, 0.0f, 0.0f);
    //Ground material
    GLfloat matDiffuse[] = {0.01, 0.01, 0.01, 1.0};
	GLfloat matSpecular[] = {1, 1, 1, 1.0};
	glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, 60.0);

    //split ground to small small squares
    const GLfloat part = 0.1;
    for (int i = -400; i < 400; ++i) {
        for (int j = -400; j < 400; ++j) {
            glVertex3f(j*part, 0, i*part );
            glVertex3f((j+1)*part, 0, i*part);
            glVertex3f((j+1)*part, 0, (i+1)*part);
            glVertex3f(j*part, 0, (i+1)*part);
        }
    }
	glEnd();
	glFlush();
}

void update()
{
    glMatrixMode(GL_MODELVIEW);   
    glLoadIdentity();
    glRotatef(g_orientation, 0.0, 1.0, 0.0); // rotate in y axis
    glTranslatef(-g_posX, -g_posY, -g_posZ);
    
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);

    //Ground
    renderGround();

    currentFirework.drawParticles(currentFirework.particles, currentFirework.MaxParticles);
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
        for(int i =0; i<4; i++){
            currentFirework.random[i]=rand()%currentFirework.MaxParticles;
        }
        currentFirework.fire(0, 0);
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
    for (i = 0; i < currentFirework.MaxParticles; i = i + 1)
    {
        //gravity
        if(gravity && currentFirework.fired)
            currentFirework.particles[i].v_y = currentFirework.particles[i].v_y - gravity_force * time;

        //straight up        
        if(currentFirework.particles[i].v_y>0){
            currentFirework.particles[i].y = currentFirework.particles[i].y + currentFirework.particles[i].v_y * time;
            particles2[i].y += currentFirework.particles[i].v_y * time;
            if(currentFirework.upType==1){ //wavy
                currentFirework.particles[i].x += 2*sin((currentFirework.particles[i].y)/3);
            }else if(currentFirework.upType==2){//circular
                currentFirework.particles[i].x += 5*cos(currentFirework.angle);
                currentFirework.particles[i].z += 5*sin(currentFirework.angle);
            }
        }
        
    }
    if(currentFirework.upType==2){
        currentFirework.angle+=currentFirework.rotationspeed;
        if(currentFirework.rotationspeed>=0)
            currentFirework.rotationspeed-=0.04;
    }

    if(currentFirework.particles[0].v_y<0){
        currentFirework.explode(currentFirework.particles, currentFirework.MaxParticles, currentFirework.explosionStyle);
        if(currentFirework.particles[0].width<=0){
            int s1 = rand()%3;
            int s2 = rand()%3;                
            cannon firework((rand()%81)-40, (rand()%81)-40);
            currentFirework = firework;
            currentFirework.fire(s1, s2);
        }
    }
    glutPostRedisplay();
    glutTimerFunc(50, &timer, 0);
}

int main(int argc, char *argv[])
{
    cout<<"\n\n*****************************"<<endl;
    cout<<"Name: Oussama Soulimani"<<endl;
    cout<<"S2439379"<<endl;
    cout<<"*****************************\n"<<endl;
    cout<<"Press f to start the show and esc to stop it."<<endl;
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