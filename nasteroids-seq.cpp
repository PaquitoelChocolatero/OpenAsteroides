#include <iostream>
#include <fstream>
#include <typeinfo>
#include <random>
using namespace std;

class Cuerpo{
    public:
        double posx, posy, masa;

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

void simulate(Cuerpo cuerpos[]){
    for(int i=0; i<(sizeof(a)/sizeof(*a)); i++){
        cout << cuerpos[i]::posx << endl;
    }
}

double * atraccion(Cuerpo &c1, Cuerpo &c2){
    double distancia=sqrt( pow( (c1.posx-c2.posx),2)+pow((c1.posx-c2.posx), 2) );
    if(distancia>5){
        double pendiente=(c1.posy-c2.posy)/(c1.posx - c2.posx);
        if(pendiente>1){
            pendiente=1;
        }else if(pendiente<-1){
            pendiente=-1;
        }

        double alpha = atan(pendiente);

        double G = 6.665*pow(10, -11);
        //double fuerzax = (G*c1.masa*c2.masa)/(distancia*distancia)*cos(alpha) ;
        //double fuerzay = (G*c1.masa*c2.masa)/(distancia*distancia)*sen(alpha) ;

        //fuerza[0]=fuerzax, fuerza[1]=fuerzay
        double fuerza[]{
            (G*c1.masa*c2.masa)/(distancia*distancia)*cos(alpha), 
            (G*c1.masa*c2.masa)/(distancia*distancia)*sin(alpha)
            };

        return fuerza ;
    }
  
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