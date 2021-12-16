#include<iostream>
#include"particles.h"

class cannon {
    public:
        cannon(float x, float Z){
            scale_over_time = 0.05f;
            MaxParticles = 225;
            teller=0;
            fired = false;
            explosionStyle = 0;
            upType=0;
            angle = 0;
            light_position[0] = 0.0;
            light_position[1] = 10.0;
            light_position[2] = 0.0;
            light_position[3] = 0.0;
            cannonPosX = x;
            cannonPosZ = Z;
        };
        float cannonPosX, cannonPosZ; 
        int teller;
        int MaxParticles;
        int random[4];
        bool fired;//check if firework was fired

        /** explosionStyle
         * defines how the firework explodes
         * 0: spherical explosion
         * 1: circulair explosion
         * 2: combination
         */
        int explosionStyle; //degines the style of explosion
        
        /** upType
         * defines how the firework ascends
         * 0: straight up
         * 1: wavy up
         * 2: circulating up
         */
        int upType = 0;  
        float scale_over_time;
        float angle;
        float rotationspeed = 1;
        GLfloat light_position[4];
        particle particles[1000];
        void fireCannon(particle particles[], int NumParticles);
        void drawParticles(particle particles[], int NumParticles);
        void explode(particle particles[], int NumParticles, int style);
        void fire(int type, int explosionStyle);
        void wavefire();
        void circularFire();
        //void setUpType(int type);
        void setExplosionStyle(int style);
};

