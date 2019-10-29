#include <iostream>
#include <fstream>
#include <typeinfo>
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



int main(int argc, char *argv[]){
    if(argc<5){
        cout << "nasteroids-seq: Wrong arguments."<< endl <<"Correct use:"<< endl <<"nasteroids-seq num_asteroides num_iteraciones num_planetas semilla"<<endl;
        return -1;
    }

    int num_asteroides = atoi(argv[1]);     //argv[1]=num_asteroides
    int num_iteraciones = atoi(argv[1]);    //argv[2]=num_iteraciones
    int num_planetas = atoi(argv[1]);       //arg[3]=num_planetas
    int semilla = atoi(argv[1]);            //arg[4]=semilla
    
    ofstream init_file ("init_conf.txt");

    for(int i=1; i<argc; i++ ){
        init_file << argv[i] << " ";
    }
    init_file << endl;
    init_file.close();


    init(semilla);
    
    return 0;
}