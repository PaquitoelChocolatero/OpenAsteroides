#include <iostream>
#include <fstream>
#include <typeinfo>
#include <random>
using namespace std;

//La clase Cuerpo engloba a planetas y asteroides
class Cuerpo{
    public:
        double posx, posy, masa, vx = 0, vy = 0;

        void set(double x ,double y, double m) { posx = x; posy = y; masa = m;};i
};

class Planeta : public Cuerpo {  

};

class Asteroide : public Cuerpo {
    public:
        void Mover(int tiempo);
};

//Función que mueve al asteroide cambiando su posición, velocidad y tiempo transcurrido
void Asteroide::Mover(int tiempo){
    posx = posx + vx*tiempo;
    posy = posy + vy*tiempo;
}

//Constantes dadas en el enunciado
constexpr int tiempo = 0.1;
constexpr double dmin = 5.0;
constexpr int width = 200;
constexpr int height = 200;
constexpr int m = 10000;
constexpr int sdm = 50;
constexpr double G=6.67e-5;

typedef struct {
    Asteroide asteroides[];
    Planeta planetas[];
} cuerpos;

void init(unsigned int seed, Cuerpo &c, int planeta){
    
    std::default_random_engine re{seed};
    std::uniform_real_distribution<double> xdist{0.0, std::nextafter(width, std::numeric_limits<double>::max())};
    std::uniform_real_distribution<double> ydist{0.0, std::nextafter(height,std::numeric_limits<double>::max())};
    std::normal_distribution<double> mdist{m, sdm};
    
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

/*
void simulate(Cuerpo cuerpos[]){
    for(int i=0; i<(sizeof(a)/sizeof(*a)); i++){
        cout << cuerpos[i].posx << endl;
    }
}
*/

//Se calcula la atraccion entre dos cuerpos y se modifican sus velocidades usando esa fuerza.
//      NO SE MODIFICAN LAS POSICIONES: esto es para luego modificar solo las de los asteroides.
//La idea es ejecutar esto para cada par de cuerpos O(n²-n) una vez calculado el tiempo
void atraccion(Cuerpo &c1, Cuerpo &c2){
    double distancia=sqrt( pow( (c1.posx-c2.posx),2)+pow((c1.posx-c2.posx), 2) );
    //Calculos del enunciado
    if(distancia>dmin){
        double pendiente=(c1.posy-c2.posy)/(c1.posx - c2.posx);
        if(pendiente>1){
            pendiente=1;
        }else if(pendiente<-1){
            pendiente=-1;
        }

        double alpha = atan(pendiente);
        
<<<<<<< HEAD
        //Me gustaria tener mas decimales de G
        double G = 6.674*pow(10, -5);
        
=======
>>>>>>> bfc901e3d3e4b1d096091c80ca578c20740d0d9f
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
    }
  
}

int main(int argc, char *argv[]){
    //Si el número de argumentos no es el correcto se imprime el uso correcto de la aplicación    
    if(argc<5){
        cout << "nasteroids-seq: Wrong arguments."<< endl <<"Correct use:"<< endl <<"nasteroids-seq num_asteroides num_iteraciones num_planetas semilla"<<endl;
        return -1;
    }
    
    //Parseo de los datos del enunciado, quizas try and catch
    int num_asteroides = atoi(argv[1]);
    int num_iteraciones = atoi(argv[2]);    
    int num_planetas = atoi(argv[3]);     
    int semilla = atoi(argv[4]);
    
    //Escribimos los argumentos en un archivo
    ofstream init_file ("init_conf.txt");
    for(int i=1; i<argc; i++ ){
        init_file << argv[i] << " ";
    }
    
    //Declaramos los cuerpos
    Planeta planetas[num_planetas];
    Asteroide asteroides[num_asteroides];

    //Inicializacion de cada cuerpo
    for (int i = 0;i<(num_asteroides + num_planetas); i++){
        if (i<num_asteroides){
            init(semilla+i, asteroides[i], -1);
        }
        else {
           init(semilla+i, planetas[i-num_asteroides], i-num_asteroides);
        }
    }
    
    writeInit(argc, argv, asteroides, planetas);   

    //Calculo de la fuerza de atraccion entre cada planeta
    // O(n_asteroides²-n_asteroides+n_asteroides*n_planetas)
    for(int i=0; i<num_asteroides; i++){
        for(int j=i+1; j<num_asteroides; j++){
            //Si i es mayor o igual que el numero de asteroides es que ambos son planetas y no hace falta calcular la fuerza entre planetas
            if (i<num_asteroides){
                if(j<num_asteroides){
                    atraccion(asteroides[i], asteroides[j]);
                } else if(j >= num_planetas){
                    atraccion(asteroides[i], planetas[j]);
                }
            }
        }
    }
    return 0;
}

