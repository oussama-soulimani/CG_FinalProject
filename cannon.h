#include<iostream>
#include"particles.h"

class cannon {
    public:
        cannon(){
            scale_over_time = 0.05f;
            MaxParticles = 225;
            teller=0;
        };
        int teller;
        int MaxParticles;
        int random[4];
        float scale_over_time;
        void fireCannon(particle particles[], int NumParticles);
        void drawParticles(particle particles[], int NumParticles);
        void explode(particle particles[], int NumParticles, int style);

};

