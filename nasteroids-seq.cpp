#include <iostream>
#include <fstream>
#include <typeinfo>
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


void init(unsigned int seed, Cuerpo &c, int planeta){
    int width = 200;
    int height = 200;
    double mass = 1000;
    double sdm = 50;

    std::default_random_engine re{seed};
    std::uniform_real_distribution<double> xdist{0.0, std::nextafter(width, std::numeric_limits<double>::max())};
    std::uniform_real_distribution<double> ydist{0.0, std::nextafter(height,std::numeric_limits<double>::max())};
    std::normal_distribution<double> mdist{mass, sdm};
    
    double posX = xdist(re);
    double posY = ydist(re);
    double masa = mdist(re);    
    if (planeta != -1) {
        planeta = planeta % 4;
        switch (planeta){
            case 0:
                posX = 0;
                break;
            case 1:
                posY = 0;
                break;
            case 2:
                posX = width;
                break;
            case 3:
                posY = height;
                break;
        }
    }
    c.set(posX, posY, masa);
}

// Funcion para escribir en un fichero los valores iniciales
void writeInit(int argc, char *argv[], Asteroide asteroides[], Planeta planetas[]){
    ofstream init_file ("init_conf.txt");
    for(int i=1; i<argc; i++ ){
        init_file << argv[i] << " ";
    }
    init_file << endl;
    init_file.close();
}


int main(int argc, char *argv[]){
    if(argc<5){
        cout << "nasteroids-seq: Wrong arguments."<< endl <<"Correct use:"<< endl <<"nasteroids-seq num_asteroides num_iteraciones num_planetas semilla"<<endl;
        return -1;
    }

    int num_asteroides = atoi(argv[1]);     //argv[1]=num_asteroides
    int num_iteraciones = atoi(argv[1]);    //argv[2]=num_iteraciones
    int num_planetas = atoi(argv[1]);       //argv[3]=num_planetas
    int semilla = atoi(argv[1]);            //argv[4]=semilla
    
    ofstream init_file ("init_conf.txt");

    for(int i=1; i<argc; i++ ){
        init_file << argv[i] << " ";
    }
    
    Planeta planetas[num_planetas];
    Asteroide asteroides[num_asteroides];

    for (int i = 0;i<(num_asteroides + num_planetas); i++){
        if (i<num_asteroides){
            init(semilla+i, asteroides[i], -1);
        }
        else {
           init(semilla+i, planetas[i-num_asteroides], i-num_asteroides);
        }
    }

    writeInit(argc, argv, asteroides, planetas);    
    return 0;
}

