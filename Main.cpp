#include <iostream>
#include <random>
using namespace std;

class Cuerpo{
    public:
        double posX, posY, masa;

        void set(double x ,double y, double m) { posX = x; posY = y; masa = m;};
        
};

class Planeta : public Cuerpo {  

};

class Asteroide : public Cuerpo {

};


void init(unsigned int seed, Cuerpo &c){
    int width = 200;
    int height = 200;
    double mass = 1000;
    double sdm = 50;

    std::default_random_engine re{seed};
    std::uniform_real_distribution<double> xdist{0.0, std::nextafter(width, std::numeric_limits<double>::max())};
    std::uniform_real_distribution<double> ydist{0.0, std::nextafter(height,std::numeric_limits<double>::max())};
    std::normal_distribution<double> mdist{mass, sdm};

   
    c.set(xdist(re), ydist(re), mdist(re));

}

int main() {
    int num_asteroides = 5;
    int num_planetas = 1;
    int semilla = 2000;

    Cuerpo cuerpos[num_asteroides + num_planetas];
    Planeta planetas[num_planetas];
    Asteroide asteroides[num_asteroides];

    for (int i = 0;i<sizeof(cuerpos); i++){
        if (i<num_asteroides){
            init(semilla+i, asteroides[i]);
        }
        else {
            init(semilla+i, planetas[i-num_asteroides]);
        }
    }

    for (int i = 0; i<sizeof(num_asteroides); i++){
        cout << asteroides[i].posX << ", " <<  asteroides[i].posY << ", " << asteroides[i].masa << endl;
    }

   

}
