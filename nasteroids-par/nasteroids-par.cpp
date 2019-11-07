#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <chrono>
#include <omp.h>

using namespace std;

//Constantes dadas en el enunciado
constexpr double tiempo = 0.1;
constexpr double dmin = 5.0;
constexpr int width = 200;
constexpr int height = 200;
constexpr int m = 10000;
constexpr int sdm = 50;
constexpr double G=6.67e-5;

//La clase Cuerpo engloba a planetas y asteroides
class Cuerpo{
    public:
        double posx, posy, masa, vx, vy;

        void set(double x ,double y, double m) { posx = x; posy = y; masa = m; vx = 0; vy=0;};
};

class Planeta : public Cuerpo {  

};

class Asteroide : public Cuerpo {
    public:
        void Mover();
};

//Función que mueve al asteroide cambiando su posición, velocidad y tiempo transcurrido
void Asteroide::Mover(){
    //Si llegamos al borde se invierte la velocidad de esa coordenada (rebota)
    if(posx<=0 || posx>=width) vx *= -1;
    if(posy<=0 || posy>=height) vy *= -1;
    //Movemos el asteroide
    posx = posx + vx*tiempo;
    posy = posy + vy*tiempo;
}


typedef struct {
    int num_asteroides;
    int num_iteraciones;
    int num_planetas; 
    int semilla;
}Datos;

void init(Datos d, Asteroide *asteroides, Planeta *planetas){
    unsigned long int seed = d.semilla;
    default_random_engine re{seed};
    uniform_real_distribution<double> xdist{0.0, std::nextafter(width, std::numeric_limits<double>::max())};
    uniform_real_distribution<double> ydist{0.0, std::nextafter(height,std::numeric_limits<double>::max())};
    normal_distribution<double> mdist{m, sdm};

    for (int i = 0; i<d.num_asteroides; i++){
        double posx = xdist(re);
        double posy = ydist(re);
        double masa = mdist(re);        
        asteroides[i].set(posx, posy, masa);
    }
    double posx, posy, masa;
    for (int i=0; i<d.num_planetas; i++){
        int lateral = i%4;
        switch (lateral){
            case 0:
                posy = ydist(re);
                masa = mdist(re);
                planetas[i].set(0, posy, masa);
                break;
            case 1:
                posx = xdist(re);
                masa = mdist(re);
                planetas[i].set(posx, 0, masa);
                break;
            case 2:
                posy = ydist(re);
                masa = mdist(re);
                planetas[i].set(width, posy, masa);
                break;
            case 3:
                posx = xdist(re);
                masa = mdist(re);
                planetas[i].set(posx, height, masa);
                break;
        }      
    }
}

// Funcion para escribir en un fichero los valores iniciales
void writeInit(Datos d, Asteroide asteroides[], Planeta planetas[]){
    ofstream init_file ("init_conf.txt");
    init_file << d.num_asteroides << " " << d.num_iteraciones << " " << d.num_planetas << " " << d.semilla << endl;
     
    for (int i=0; i<d.num_asteroides; ++i){
        init_file << asteroides[i].posx << " " << asteroides[i].posy << " " << asteroides[i].masa << endl;
    }
    for (int i=0; i<d.num_planetas; ++i){
       init_file << planetas[i].posx << " " << planetas[i].posy << " " << planetas[i].masa << endl;
    }
    init_file.close();
}


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

Datos parseArgs(int argc, char *argv[]){
     //Si el número de argumentos no es el correcto se imprime el uso correcto de la aplicación 
    if (argc<5){
        throw int(1);
    }
    Datos datos;
    try{
        datos.num_asteroides = stoi(argv[1]);
        datos.num_iteraciones = stoi(argv[2]);    
        datos.num_planetas = stoi(argv[3]);     
        datos.semilla = stoi(argv[4]);
    }catch(invalid_argument &e){
        throw int(1);
    }
    return datos;   
}

int main(int argc, char *argv[]){
      
    Datos datos;
    try{
        datos = parseArgs(argc, argv);
    }catch(int i){
        if (i==1){
            cout << "nasteroids-par: Wrong arguments."<< endl <<"Correct use:"<< endl <<"./nasteroids-seq num_asteroides num_iteraciones num_planetas semilla"<<endl;
            return -1;
        }
    }
    
   //Declaramos los cuerpos
    Planeta *planetas = new Planeta[datos.num_planetas];
    Asteroide *asteroides = new Asteroide[datos.num_asteroides];

    init(datos, asteroides, planetas);
  
    
    writeInit(datos, asteroides, planetas);   

    auto start=chrono::high_resolution_clock::now();

    //Bucle principal de la simulación
    for(int it = 0;it<datos.num_iteraciones;it++){

        //Calculo de la fuerza de atracción entre cada cuerpo -> O(n_asteroides²-n_asteroides+n_asteroides*n_planetas)
        
        //Cada hilo recorre una porcion del array de cuerpos
        #pragma omp parallel for
        
            //Recorremos sólo los asteroides (los planetas son fijos)
            for(int i=0; i<datos.num_asteroides; i++){
                //Se sienten atraídos por todos los cuerpos

                //Cada hilo calcula la atraccion de un rango de cuerpos distintos
                #pragma omp parallel for
                
                    for(int j=i+1; j<(datos.num_asteroides + datos.num_planetas)-1; j++){
                        //Si i es mayor o igual que el numero de asteroides es que ambos son planetas y no hace falta calcular la fuerza entre planetas
                        if (i<datos.num_asteroides){
                            //Si j es un asteroide
                            if(j<datos.num_asteroides){
                                atraccion(asteroides[i], asteroides[j]);
                            }
                            //Si es un planeta
                            else{
                                atraccion(asteroides[i], planetas[j]);
                            }
                        }
                    }

                //Movemos el asteroide de turno 
                asteroides[i].Mover();
                
            }

        
    }

    auto end=chrono::high_resolution_clock::now();

    //Escribimos el resultado final
    ofstream out_file ("output.txt");
     
    for (int i=0; i<datos.num_asteroides; ++i){
        out_file << asteroides[i].posx << " " << asteroides[i].posy << " " << asteroides[i].vx << " " << asteroides[i].vy << " "  << asteroides[i].masa << endl;
    }
    out_file.close();

    chrono::duration<double> elapsed = chrono::duration_cast<chrono::duration<double>>(end-start);

    cout<< "Time elapsed: " <<elapsed.count()<<endl;

    return 0;
}
