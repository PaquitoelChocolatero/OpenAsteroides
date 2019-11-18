#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <chrono>
using namespace std;

//Constantes dadas en el enunciado
constexpr float tiempo = 0.1;
constexpr float dmin = 5.0;
constexpr float width = 200.0;
constexpr float height = 200.0;
constexpr int m = 1000;
constexpr int sdm = 50;
constexpr float G=6.67e-5;

//La clase Cuerpo engloba a planetas y asteroides
class Cuerpo{
    public:
        float posx, posy, masa, vx, vy;
        float fuerza[2]; 

        void set(float x ,float y, float m) { posx = x; posy = y; masa = m; vx = 0; vy=0; fuerza[0]=0; fuerza[1]=0;};
};

class Planeta : public Cuerpo {  

};

class Asteroide : public Cuerpo {
    public:
        void Mover();
        void calcularVelocidad();
};

//Calcula la velocidad de un asteroide y anula su fuerza
void Asteroide::calcularVelocidad(){
    //V = V0 + a*t
        vx += (fuerza[0]/masa)*tiempo;
        vy += (fuerza[1]/masa)*tiempo;
        fuerza[0]=0;
        fuerza[1]=0;
}

//Función que mueve al asteroide cambiando su posición, velocidad y tiempo transcurrido
void Asteroide::Mover(){
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
    uniform_real_distribution<float> xdist{0.0, std::nextafter(width, std::numeric_limits<float>::max())};
    uniform_real_distribution<float> ydist{0.0, std::nextafter(height,std::numeric_limits<float>::max())};
    normal_distribution<float> mdist{m, sdm};

    for (int i = 0; i<d.num_asteroides; i++){
        float posx = xdist(re);
        float posy = ydist(re);
        float masa = mdist(re);        
        asteroides[i].set(posx, posy, masa);
    }
    float posx, posy, masa;
    for (int i=0; i<d.num_planetas; i++){
        int lateral = i%4;
        switch (lateral){
            case 0:
                posy = ydist(re);
                masa = mdist(re);
                if(masa<0) masa*=-1;
                planetas[i].set(0, posy, masa);
                break;
            case 1:
                posx = xdist(re);
                masa = mdist(re);
                if(masa<0) masa*=-1;
                planetas[i].set(posx, 0, masa);
                break;
            case 2:
                posy = ydist(re);
                masa = mdist(re);
                if(masa<0) masa*=-1;
                planetas[i].set(width, posy, masa);
                break;
            case 3:
                posx = xdist(re);
                masa = mdist(re);
                if(masa<0) masa*=-1;
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
    float distancia=sqrt( pow( (c1.posx-c2.posx),2)+pow((c1.posx-c2.posx), 2) );
    //Calculos del enunciado
    if(distancia>dmin){
        float pendiente=(c1.posy-c2.posy)/(c1.posx - c2.posx);
        if(pendiente>1){
            pendiente=1;
        }else if(pendiente<-1){
            pendiente=-1;
        }

        float alpha = atan(pendiente);

        float f = (G*c1.masa*c2.masa)/(distancia*distancia);
        if(f>100){
            f=100;
        }
        //fuerza[0]=fuerzax, fuerza[1]=fuerzay
        float fuerza[]{
            (f)*cos(alpha), 
            (f)*sin(alpha)
        };
        
        c1.fuerza[0]+=fuerza[0];
        c1.fuerza[1]+=fuerza[1];
        c2.fuerza[0]-=fuerza[0];
        c2.fuerza[1]-=fuerza[1];
          
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
            cout << "nasteroids-seq: Wrong arguments."<< endl <<"Correct use:"<< endl <<"./nasteroids-seq num_asteroides num_iteraciones num_planetas semilla"<<endl;
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

        //Calculo de la fuerza de atracción entre cada planeta -> O(n_asteroides²-n_asteroides+n_asteroides*n_planetas)
        //Recorremos sólo los asteroides (los planetas son fijos)
        for(int i=0; i<datos.num_asteroides; i++){
            //Se sienten atraídos por todos los cuerpos
            for(int j=i+1; j<(datos.num_asteroides + datos.num_planetas)-1; j++){
                //Si i es mayor o igual que el numero de asteroides es que ambos son planetas y no hace falta calcular la fuerza entre planetas
                if (i<datos.num_asteroides){
                    //Si j es un asteroide
                    if(j<datos.num_asteroides){
                        atraccion(asteroides[i], asteroides[j]);
                    }
                    //Si es un planeta
                    else{
                        atraccion(asteroides[i], planetas[j-datos.num_asteroides]);
                    }
                }
            }
            asteroides[i].calcularVelocidad();
            //Movemos el asteroide de turno 
            asteroides[i].Mover();
        }
        
        for(int i=0; i<datos.num_asteroides; i++){
        
            //Creamos un vector con todos los asteroides con los que colisiona cada asteroide
            vector<Asteroide*> choques;
        
            //Si llegamos al borde colocamos el asteroide 5 posiciones alejado de él e invertimos su velocidad
            if(asteroides[i].posx<=0){
                asteroides[i].posx = dmin;
                asteroides[i].vx *= -1;
            }
            else if(asteroides[i].posx>=width){
                asteroides[i].posx = width-dmin;
                asteroides[i].vx *= -1;
            }
            if(asteroides[i].posy<=0){
                asteroides[i].posy = dmin;
                asteroides[i].vy *= -1;
            }
            else if(asteroides[i].posy>=height){
                asteroides[i].posy = height-dmin;
                asteroides[i].vy *= -1;
            }             

            //Añadimos el asteroide ed turno
            choques.push_back(&asteroides[i]);

            for(int j=i+1; j<datos.num_asteroides; j++){
                //Calculamos la distancia entre los dos asteroides
                float distancia=sqrt( pow( (asteroides[i].posx-asteroides[j].posx),2)+pow((asteroides[i].posx-asteroides[j].posx), 2) );
                //Añadimos el asteroide a la lista de choques si su distancia es menor que dmin
                if (distancia <= dmin) {
                    choques.push_back(&asteroides[j]);
                }
            }

            //Si sólo hay un elemento significa que no choca con nadie, luego no hay que hacer la rutina de rebote
            if(choques.size() != 1){
                //Guardamos los valores del primer asteroide
                float tempx = choques[0]->vx;
                float tempy = choques[0]->vy;
                //Iteramos sobre la lista para intercambiar valores
                for(auto i = choques.begin(); i<choques.end()-1; ++i){
                    auto sig = next(i,1);
                    (*i)->vx = (*sig)->vx;
                    (*i)->vy = (*sig)->vy;
                }
                //Le damos al último elemento el valor del primero
                choques[choques.size()-1]->vx = tempx;
                choques[choques.size()-1]->vy = tempy;
            }
        }
    }

    auto end=chrono::high_resolution_clock::now();

    //Escribimos el resultado final
    ofstream out_file ("output.txt");
     
    for (int i=0; i<datos.num_asteroides; ++i){
        out_file << asteroides[i].posx << " " << asteroides[i].posy << " " << asteroides[i].vx << " " << asteroides[i].vy << " "  << asteroides[i].masa << endl;
    }
    out_file.close();

    chrono::duration<float> elapsed = chrono::duration_cast<chrono::duration<float>>(end-start);

    cout<< "Time elapsed: " <<elapsed.count()<<endl;


    return 0;
}
