#include <iostream>
#include <cmath>
#include <random>
#include <iomanip>
#include <fstream>
#include <chrono>
using namespace std;

#define tiempo 0.1
#define dmin 5.0
#define width 200
#define height 200
#define m 1000
#define sdm 50
#define G 6.674e-5

typedef struct {
    float x;
    float y;
}Fuerza;

typedef struct {
    float posx;
    float posy;
    float masa;
    vector<Fuerza> fuerzas;
    float vx;
    float vy;
}Asteroide;

typedef struct {
    float posx;
    float posy;
    float masa;
}Planeta;

void calculoPosicionInicial(long unsigned int seed, int num_asteroides, int num_planetas, Asteroide *asteroides, Planeta *planetas){
    default_random_engine re{seed};
    uniform_real_distribution<double> xdist{0.0, std::nextafter(width, std::numeric_limits<double>::max())};
    uniform_real_distribution<double> ydist{0.0, std::nextafter(height,std::numeric_limits<double>::max())};
    normal_distribution<double> mdist{m, sdm};

    for (int i = 0; i<num_asteroides; i++){
        asteroides[i].posx = xdist(re);
        asteroides[i].posy = ydist(re);
        asteroides[i].masa = mdist(re);
        asteroides[i].vx = 0;
        asteroides[i].vy = 0;        
    }
    float masa;
    for (int i=0; i<num_planetas; i++){
       
        int lateral = i%4;
        switch (lateral){
            case 0:
                planetas[i].posx = 0;
                planetas[i].posy = ydist(re);
                masa = abs(mdist(re));
                planetas[i].masa = 10*masa;
                break;
            case 1:
                planetas[i].posx = xdist(re);
                planetas[i].posy = 0;
                masa = abs(mdist(re));
                planetas[i].masa = 10*masa;
                break;
            case 2:
                planetas[i].posx = width;
                planetas[i].posy = ydist(re);
                masa = abs(mdist(re));
                planetas[i].masa = 10*masa;
                break;
            case 3:
                planetas[i].posx = xdist(re);
                planetas[i].posy = height;
                masa = abs(mdist(re));
                planetas[i].masa = 10*masa;
                break;
        }      
    }
}

void escribirInit(int num_asteroides, int num_planetas, int num_iteraciones, int semilla, Planeta *planetas, Asteroide *asteroides ){
    ofstream init_file ("init_conf.txt");
    init_file << num_asteroides << " " << num_iteraciones << " " << num_planetas << "  " << semilla << endl;
     
    for (int i=0; i<num_asteroides; ++i){
        init_file << fixed << setprecision(3) << asteroides[i].posx << " " << asteroides[i].posy << " " << asteroides[i].masa << endl;
    }
    for (int i=0; i<num_planetas; ++i){
       init_file << fixed << setprecision(3) << planetas[i].posx << " " << planetas[i].posy << " " << planetas[i].masa << endl;
    }
    init_file.close();

}

int main(int argc, char *argv[]){
    // Leer argumentos 
    int num_asteroides = 0, num_planetas = 0, num_iteraciones = 0, semilla = 0;
    if (argc<5){
        cout << "nasteroids-seq: Wrong arguments."<< endl <<"Correct use:"<< endl <<"./nasteroids-seq num_asteroides num_iteraciones num_planetas semilla"<<endl;
        return -1;
    }else{
        num_asteroides = stoi(argv[1]);
        num_iteraciones = stoi(argv[2]);    
        num_planetas = stoi(argv[3]);     
        semilla = stoi(argv[4]);
    }

    // Array de struct planetas
    Planeta *planetas = new Planeta[num_planetas];
    //Array de struct asteroides
    Asteroide *asteroides = new Asteroide[num_asteroides];

    // Asignacion de las posiciones iniciales
    calculoPosicionInicial(semilla, num_asteroides, num_planetas, asteroides, planetas);
    // Escribimos las posiciones iniciales en un fichero
    escribirInit(num_asteroides, num_planetas, num_iteraciones, semilla, planetas, asteroides );
    
    //Reloj
    //auto start=chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed;
    double it = 0;

    //Para cada iteracion
    for (int i = 0; i< num_iteraciones; ++i){

        auto start=chrono::high_resolution_clock::now();

        //Para cada asteroide calculamos la atraccion con el resto de elementos
        for (int j = 0; j<num_asteroides; ++j){
            float fuerzax=0, fuerzay=0;
            //Declaramos un vector donde alamacenar las fuerzas
            //vector<Fuerza> sumatorio_fuerzas;
            // Para los elementos de tipo asteroide cuya atraccion no ha sido calculada aun (k = j+1)
            for (int k = 0; k<num_asteroides; ++k ){
                 if(k==j){
                    continue;
                }
                // Calculamos la distancia en modulo
                float distancia=sqrt( pow( (asteroides[j].posx-asteroides[k].posx),2)+pow((asteroides[j].posx-asteroides[k].posx), 2) );
                // Si la distancia es mayor que la distancia de rebote
                if (distancia > dmin){
                    // Calculamos la pendiente
                    float pendiente=(asteroides[j].posy-asteroides[k].posy)/(asteroides[j].posx - asteroides[k].posx);
                    // Truncamos si es mayor que 1 o menor que -1
                    if(pendiente>1){
                        pendiente=1;
                    }else if(pendiente<-1){
                        pendiente=-1;
                    }
                    //Calculamos el angulo
                    float alpha = atan(pendiente);

                    if(k>j){
                        fuerzax = (G*asteroides[j].masa*asteroides[k].masa)/(distancia*distancia)*cos(alpha);
                        fuerzay = (G*asteroides[j].masa*asteroides[k].masa)/(distancia*distancia)*sin(alpha);
                    }
                    else{
                        //En otro caso significa que k es menor que j y la fuerza sobre j tendra sentido negativo
                        fuerzax = (G*asteroides[j].masa*asteroides[k].masa)/(distancia*distancia)*cos(alpha)*-1;
                        fuerzay = (G*asteroides[j].masa*asteroides[k].masa)/(distancia*distancia)*sin(alpha)*-1;
                    }
                    //En cualquier caso la fuerza sobre k sera calculada mas adelante    
                    
                    // La fuerza tiene el mismo valor pero sentido contrario para cada uno
                    Fuerza fuerzaj = {fuerzax, fuerzay};

                    // La añadimos al vector de fuerza de cada uno    
                    asteroides[j].fuerzas.push_back(fuerzaj);
                }
            }
            for (int k = 0; k < num_planetas; ++k){
                // Calculamos la distancia en modulo
                float distancia=sqrt( pow( (asteroides[j].posx-planetas[k].posx),2)+pow((asteroides[j].posx-planetas[k].posx), 2) );
                // Si la distancia es mayor que la distancia de rebote
                if (distancia > dmin){
                    // Calculamos la pendiente
                    float pendiente=(asteroides[j].posy-planetas[k].posy)/(asteroides[j].posx - planetas[k].posx);
                    // Truncamos si es mayor que 1 o menor que -1
                    if(pendiente>1){
                        pendiente=1;
                    }else if(pendiente<-1){
                        pendiente=-1;
                    }
                    //Calculamos el angulo
                    float alpha = atan(pendiente);

                    //Calculamos la fuerza entre ambos
                    float fuerzax = (G*asteroides[j].masa*planetas[k].masa)/(distancia*distancia)*cos(alpha);
                    float fuerzay = (G*asteroides[j].masa*planetas[k].masa)/(distancia*distancia)*sin(alpha);
                    
                    // La fuerza tiene el mismo valor pero sentido contrario para cada uno
                    Fuerza fuerzaj = {fuerzax, fuerzay};

                    asteroides[j].fuerzas.push_back(fuerzaj);
                }
            }
        }
        
        // Para cada asteroide calcularemos su fuerza, su aceleracion y velocidad
        for (int j = 0; j< num_asteroides; ++j) {            
            //Obtenemos el sumatorio de las fuerzas
            float sum_fuerzax = 0, sum_fuerzay = 0;
            for (auto fuerza : asteroides[j].fuerzas){
                sum_fuerzax += fuerza.x;
                sum_fuerzay += fuerza.y; 
            }
            //Calculamos la aceleracion
            float aceleracionx = 1/asteroides[j].masa * sum_fuerzax;
            float aceleraciony = 1/asteroides[j].masa * sum_fuerzay;
            //Calculamos la velocidad
            asteroides[j].vx += aceleracionx * tiempo;
            asteroides[j].vy += aceleraciony * tiempo;
            //Modificamos la posicion
           
            //Dejamos el vector vacio para la siguiente iteracion
            asteroides[j].fuerzas.clear(); 
        }

        //Modificamos la posicion
        for(int j=0; j<num_asteroides; j++){
            asteroides[j].posx += asteroides[j].vx * tiempo; 
            asteroides[j].posy += asteroides[j].vy * tiempo;

            // Efecto rebote con los muros
            if (asteroides[j].posx <= 0){
                asteroides[j].posx = dmin;
                asteroides[j].vx *= -1;
            }else if (asteroides[j].posx >= width){
                asteroides[j].posx = width - dmin;
                asteroides[j].vx *= -1;
            }else if (asteroides[j].posy <= 0){
                asteroides[j].posy = dmin;
                asteroides[j].vy *= -1;
            }else if (asteroides[j].posy >= height){
                asteroides[j].posy = height - dmin;
                asteroides[j].vy *= -1;
            }
        }
        //Rebotes entre asteroides 
        for (int j = 0; j< num_asteroides; ++j) {
            for (int k = j+1; k < num_asteroides; ++k){
                //Calculamos la distancia en modulo
                float distancia=sqrt( pow( (asteroides[j].posx-asteroides[k].posx),2)+pow((asteroides[j].posx-asteroides[k].posx), 2) );

                if (distancia >= dmin){
                    float temp = asteroides[j].vx;
                    asteroides[j].vx = asteroides[k].vx;
                    asteroides[k].vx = temp;
                    temp = asteroides[j].vy;
                    asteroides[j].vy = asteroides[k].vy;
                    asteroides[k].vy = temp;

                    //Tras rebotar se calcula el siguiente asteroide
                    break;
                }
            }
        }
        auto end=chrono::high_resolution_clock::now();
        elapsed = chrono::duration_cast<chrono::duration<double>>(end-start);  
        it += elapsed.count();                
    }

    // auto end=chrono::high_resolution_clock::now();
    // chrono::duration<double> elapsed = chrono::duration_cast<chrono::duration<double>>(end-start);
    // cout << elapsed.count() <<endl;

    it /= num_iteraciones;
    cout << it << endl;
   
    //Escribimos el resultado final
    ofstream out_file("output.txt");

    for (int i=0; i<num_asteroides; ++i){
        out_file << fixed << setprecision(3) << asteroides[i].posx << " " << asteroides[i].posy << " " << asteroides[i].vx << " " << asteroides[i].vy << " "  << asteroides[i].masa << endl;
    }
    out_file.close();
}