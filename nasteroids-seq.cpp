#include <iostream>
#include <fstream>
#include <typeinfo>
#include <random>
using namespace std;

class Cuerpo{
    public:
        double posx, posy, masa, vx = 0, vy = 0;

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

void simulate(Cuerpo cuerpos[]){
    for(int i=0; i<(sizeof(a)/sizeof(*a)); i++){
        cout << cuerpos[i]::posx << endl;
    }
}

//Se calcula la atraccion entre dos cuerpos y se modifican sus velocidades usando esa fuerza.
//La idea es ejecutar esto para cada par de cuerpos O(n²-n) una vez calculado el tiempo
void atraccion(Cuerpo &c1, Cuerpo &c2, tiempo){
    double distancia=sqrt( pow( (c1.posx-c2.posx),2)+pow((c1.posx-c2.posx), 2) );
    //Calculos del enunciado
    if(distancia>5){
        double pendiente=(c1.posy-c2.posy)/(c1.posx - c2.posx);
        if(pendiente>1){
            pendiente=1;
        }else if(pendiente<-1){
            pendiente=-1;
        }

        double alpha = atan(pendiente);
        
        //Me gustaria tener mas decimales de G
        double G = 6.665*pow(10, -11);

        //double fuerzax = (G*c1.masa*c2.masa)/(distancia*distancia)*cos(alpha) ;
        //double fuerzay = (G*c1.masa*c2.masa)/(distancia*distancia)*sen(alpha) ;

        //fuerza[0]=fuerzax, fuerza[1]=fuerzay
        double fuerza[]{
            (G*c1.masa*c2.masa)/(distancia*distancia)*cos(alpha), 
            (G*c1.masa*c2.masa)/(distancia*distancia)*sin(alpha)
            };

        //V = V0 + a*t
        c1.vx += (fuerza[0]/c1.masa)*tiempo;
        c1.vy += (fuerza[1]/c1.masa)*tiempo;
        c2.vx -= (fuerza[0]/c2.masa)*tiempo;
        c2.vy -= (fuerza[1]/c2.masa)*tiempo;

        
        //return fuerza ;
    }
  
}

int main(int argc, char *argv[]){
    
    if(argc<5){
        cout << "nasteroids-seq: Wrong arguments."<< endl <<"Correct use:"<< endl <<"nasteroids-seq num_asteroides num_iteraciones num_planetas semilla"<<endl;
        return -1;
    }
    
    //Parseo de los datos del enunciado, quizas try and catch
    int num_asteroides = atoi(argv[1]);     
    int num_iteraciones = atoi(argv[1]);    
    int num_planetas = atoi(argv[1]);       
    int semilla = atoi(argv[1]);     
    
    //Escribimos los argumentos en un archivo
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

