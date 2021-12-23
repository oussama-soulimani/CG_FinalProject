#include <stdlib.h>
#include "GL/glut.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <time.h>
#include <iostream>
#include"cannon.h"
#include "audio.h"
using namespace std;




        int cannon::getMaxParticles(){
            return MaxParticles;
        }
        bool cannon::getFired(){
            return fired;
        }
        float cannon::getAngle(){
            return angle;
        }
        void cannon::setAngle(float Angle){
            angle = Angle;
        }
        int cannon::getUpType(){
            return upType;
        }
        int cannon::getExplosionStyle(){
            return explosionStyle;
        }
        void cannon::setExplosionStyle(int style){
            explosionStyle = style;
        }
        float cannon::getRotationSpeed(){
            return rotationspeed;
        }
        void cannon::setRotationSpeed(float rotSpeed){
            rotationspeed = rotSpeed;
        }


void cannon::fireCannon(particle particles[], int NumParticles)
{   
    playSound("rising.wav", SDL_MIX_MAXVOLUME);
    playExplosionSound = true;
    float r = 1.0 * (rand() / (float)RAND_MAX);
    float g = 1.0 * (rand() / (float)RAND_MAX);
    float b = 1.0 * (rand() / (float)RAND_MAX);
    for (int i = 0; i < NumParticles; i = i + 1)
    {
        particles[i].width = 0.3 * (rand() / (float)RAND_MAX) + 1.0;
        particles[i].x = cannonPosX;
        particles[i].y = 0.0;
        particles[i].z = cannonPosZ;
        particles[i].v_x = 10.0 * (rand() / (float)RAND_MAX) - 5.0f;
        particles[i].v_y = 30.0f;
        particles[i].v_z = 10.0 * (rand() / (float)RAND_MAX) - 5.0f;
        particles[i].r = r;
        particles[i].g = g;
        particles[i].b = b;
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
        glDisable(GL_LIGHTING);
        glPopMatrix();
    }
}

void cannon::fire(int UpType, int expStyle){
    fireCannon(particles, MaxParticles);
    upType = UpType;
    fired = true;
    angle = 0;
    rotationspeed = 1;
    setExplosionStyle(expStyle);
}
void cannon::explode(particle particles[], int numParticles, int style){
    float x=0, y=0, z=0;
    //generate sphere that gets bigger to simulate explosion
    //light Source
    if(particles[0].width>0){
        if(playExplosionSound){
            if(style==0){
                playSound("exploding0.wav", SDL_MIX_MAXVOLUME);
            }else if(style==1){
                playSound("exploding1.wav", SDL_MIX_MAXVOLUME);
            }else if(style==2){
                playSound("exploding2.wav", SDL_MIX_MAXVOLUME);
            }
            playExplosionSound = false;
        }
        GLfloat mbientLight[]	= {0.2, 0.2, 0.2, 1.0};
        GLfloat diffuseLight[]	= {0.8, 0.8, 0.8, 1.0};
        //Intesity is relative to size of particles
        GLfloat specularLight[]	= {particles[0].r*particles[0].width*5, particles[0].g*particles[0].width*5, particles[0].b*particles[0].width*5, 1.0};
        GLfloat spot_direction[] = { 0.0, -1.0, 0.0 };
        glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 5000);
        glLightfv(GL_LIGHT0, GL_AMBIENT, mbientLight);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
        glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);
        glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 45.0);
        glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
        glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 2.0);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);

    }
    else{
        glDisable(GL_LIGHT0);

    }
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
                    particles[i].v_y=0.0;
                }
            }
        }
    }
    else if(style==1){
        for(int i=0; i<numParticles; i++){
            x = cos(i*2*M_PI/numParticles);
            z = sin(i*2*M_PI/numParticles);
            particles[i].x +=x; 
            particles[i].z +=z;
            if(particles[i].width>0){
                    particles[i].width -=scale_over_time;
                }
            if(particles[i].width<0){
                particles[i].width = 0;
                particles[i].v_y=0.0;

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
            }
            else{
                z = cos(i*2*M_PI/numParticles);
                y = sin(i*2*M_PI/numParticles);
                x=0;
            }
            particles[i].x +=x; 
            particles[i].y +=y;
            particles[i].z +=z;

            if(particles[i].width>0){
                    particles[i].width -=scale_over_time;
                }
            if(particles[i].width<0){
                particles[i].width = 0;
                particles[i].v_y=0.0;
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
