#ifndef NASTEROIDS_H    // To make sure you don't declare the function more than once by including the header multiple times.
#define NASTEROIDS_H

#include <iostream>
#include <fstream>
#include <string>
#include <random>
using namespace std;


class Cuerpo{
    public:
        double posx, posy, masa, vx = 0, vy = 0;

        void set(double x ,double y, double m);
};

class Planeta : public Cuerpo {  

};

class Asteroide : public Cuerpo {
    public:
        void Mover();
};

typedef struct {
    int num_asteroides;
    int num_iteraciones;
    int num_planetas; 
    int semilla;
}Datos;


void init(unsigned int seed, Cuerpo &c, int planeta);
void writeInit(Datos d, Asteroide asteroides[], Planeta planetas[]);
void atraccion(Cuerpo &c1, Cuerpo &c2);
Datos parseArgs(int argc, char *argv[]);

#endif