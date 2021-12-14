#include <stdlib.h>
#include "GL/glut.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <time.h>
#include <iostream>
#include"cannon.h"

using namespace std;



void cannon::fireCannon(particle particles[], int NumParticles)
{
    for (int i = 0; i < NumParticles; i = i + 1)
    {
        particles[i].width = 0.3 * (rand() / (float)RAND_MAX) + 1.0;
        particles[i].x = 0.0;
        particles[i].y = 0.0;
        particles[i].z = 0.0;
        particles[i].v_x = 10.0 * (rand() / (float)RAND_MAX) - 5.0f;
        particles[i].v_y = 30.0f;
        particles[i].v_z = 10.0 * (rand() / (float)RAND_MAX) - 5.0f;
        particles[i].r = 1.0 * (rand() / (float)RAND_MAX);
        particles[i].g = 1.0 * (rand() / (float)RAND_MAX);
        particles[i].b = 1.0 * (rand() / (float)RAND_MAX);
    }
    glutGet(GLUT_ELAPSED_TIME);
}


void cannon::drawParticles(particle particles[], int NumParticles)
{

    for (int i = 0; i < NumParticles; i = i + 1)
    {

        glPushMatrix();
        glTranslatef(particles[i].x, particles[i].y, particles[i].z);
        glScalef(particles[i].width, particles[i].width, particles[i].width);
        glColor3f(particles[i].r, particles[i].g, particles[i].b);
        glBegin(GL_TRIANGLE_STRIP);
        // triangle 1
        glVertex3f(-0.5, 0.0, 0.5); // A
        glVertex3f(0.0, 0.0, -0.5); // B
        glVertex3f(0.0, 1.0, 0.0);  // top
        // triangle 2
        glVertex3f(0.5, 0.0, 0.5); // C
        // triangle 3
        glVertex3f(-0.5, 0.0, 0.5); // A again
        // triangle 4 (bottom)
        glVertex3f(0.0, 0.0, -0.5); // B again
        glEnd();
        glPopMatrix();
    }
}
void cannon::explode(particle particles[], int numParticles, int style){
    float x=0, y=0, z=0;
    //generate sphere that gets bigger to simulate explosion
    float circleAngle;
    if(style==0){
        for(int i=0; i<sqrt(numParticles); i++){
            circleAngle = i*2 * M_PI/sqrt(numParticles);
            for(int j=0; j<sqrt(numParticles); j++){
                x = cos(j*2*M_PI/sqrt(numParticles))*cos(circleAngle);
                y = sin(j*2*M_PI/sqrt(numParticles))*cos(circleAngle);
                z = sin(circleAngle);
                int index = j+i*(int)sqrt(numParticles);
                particles[index].x +=x; 
                particles[index].y +=y;
                particles[index].z +=z;
                if(particles[index].width>0){
                    particles[index].width -=scale_over_time;
                }
                if(particles[index].width<0){
                    particles[index].width = 0;
                }
            }
        }
    }
    else if(style==1){
        for(int i=0; i<numParticles; i++){
            x = cos(i*2*M_PI/numParticles);
            y = sin(i*2*M_PI/numParticles);
            particles[i].x +=x; 
            particles[i].y +=y;
            if(particles[i].width>0){
                    particles[i].width -=scale_over_time;
                }
            if(particles[i].width<0){
                particles[i].width = 0;
            }
        }
    }   
    else if(style==2){
        for(int i=0; i<numParticles; i++){
            x = cos(i*2*M_PI/numParticles);
            z = sin(i*2*M_PI/numParticles);
            if(i%3==0){
                y = x;
            }else if(i%3==1){
                y=-x;
            }else{
                y=-z;
            }
            particles[i].x +=x; 
            particles[i].y +=y;
            particles[i].z +=z;

            if(particles[i].width>0){
                    particles[i].width -=scale_over_time+0.05;
                }
            if(particles[i].width<0){
                particles[i].width = 0;
            }
        }
    }else if(style==3){
        particle particles2[25];
        for(int i=0; i<sqrt(numParticles); i++){
            circleAngle = i*2 * M_PI/sqrt(numParticles);
            for(int j=0; j<sqrt(numParticles); j++){
                x = cos(j*2*M_PI/sqrt(numParticles))*cos(circleAngle);
                y = sin(j*2*M_PI/sqrt(numParticles))*cos(circleAngle);
                z = sin(circleAngle);
                int index = j+i*(int)sqrt(numParticles);
                particles[index].x +=x; 
                particles[index].y +=y;
                particles[index].z +=z;

                
                if(particles[index].width>0){
                    if(index!=random[0] && index!=random[1] && index!=random[2] && index!=random[3]){
                        particles[index].width -=scale_over_time;
                    }else{
                        particles[index].width -=scale_over_time-0.02;
                    }
                }
                    if(particles[index].width<0){
                        particles[index].width = 0;
                        cout<<"yes"<<endl;
                        if(index==random[0] && index==random[1] && index==random[2] && index==random[3]){
                            for(int k=0; k<25; k++){
                                particles2[k].x = particles[index].x;
                                particles2[k].y= particles[index].y;
                                particles2[k].z = particles[index].z;
                            }
                            explode(particles2, 25, 1);
                        }
                    }
            }
        }
    }

}
