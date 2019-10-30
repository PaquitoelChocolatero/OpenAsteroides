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

<<<<<<< HEAD:nasteroids-seq.cpp

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


    init(2000);
    
    return 0;

}
=======
int main() {
    int num_asteroides = 5;
    int num_planetas = 5;
    int semilla = 2000;

    //Cuerpo cuerpos[num_asteroides + num_planetas];
    Planeta planetas[num_planetas];
    Asteroide asteroides[num_asteroides];

    for (int i = 0;i<(num_asteroides + num_planetas); i++){
        if (i<num_asteroides){
            cout << "Inicializado asteroide " << i<<endl;
            init(semilla+i, asteroides[i], -1);
        }
        else {
            cout << "Inicializado planeta" << i-num_asteroides<<endl;
            init(semilla+i, planetas[i-num_asteroides], i-num_asteroides);
        }
    }

     for (int i = 0; i< num_asteroides; i++){
        cout << asteroides[i].posX << ", " <<  asteroides[i].posY << ", " << asteroides[i].masa << endl;
    }
    for (int i = 0; i< num_planetas; i++){
        cout << planetas[i].posX << ", " <<  planetas[i].posY << ", " << planetas[i].masa << endl;
    }
}
>>>>>>> Random:Main.cpp
