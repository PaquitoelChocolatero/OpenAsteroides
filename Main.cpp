#include <iostream>
#include <random>
using namespace std;

class Cuerpo{
    public:
        double posX, posY, masa;

        //void Cuerpo(double x ,double y, double m) : posX(x) , posY(y),  masa(m){};
        
};

class Planeta : public Cuerpo {  

};

class Asteroide : public Cuerpo {

};


void init(unsigned int seed){
    int width = 200;
    int height = 200;
    double mass = 10;
    double sdm = 2;

    std::default_random_engine re{seed};
    std::uniform_real_distribution<double> xdist{0.0, std::nextafter(width, std::numeric_limits<double>::max())};
    std::uniform_real_distribution<double> ydist{0.0, std::nextafter(height,std::numeric_limits<double>::max())};
    std::normal_distribution<double> mdist{mass, sdm};

    cout << xdist(re) << ", " << ydist(re) << ", " << mdist(re) << endl;

}

int main() {
    init(2000);
}
