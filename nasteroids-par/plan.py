import os
import time

#Datos
resultado=[0, 0, 0]
cuerpos=[250,500,1000]
iteraciones=[50,100,200]
hilos=[4, 8]

os.system("make")
#Por cada hilo
for t in range(2):
    #Cambiamos el numero de hilos
    os.system("export OMP_NUM_THREADS=%i" % (hilos[t]))
    #Por cada configuración de asteroides y planetas
    for i in range(3):
        #Por cada configuración de iteraciones
        for j in range(3):
            #Calculamos el tiempo 10 veces
            for k in range(11):
                #Sacamos con el comando en bash el float resultante en cada bucle
                resultado[0]+=(float(os.popen('./nasteroids-par_dynamic %i %i %i 1999 | cut -d: -f 2 | tr -d " "' % (cuerpos[i], iteraciones[j], cuerpos[i])).read()))
                        #time.sleep(0.1)
                #Sacamos con el comando en bash el float resultante en cada bucle
                resultado[1]+=(float(os.popen('./nasteroids-par_static %i %i %i 1999 | cut -d: -f 2 | tr -d " "' % (cuerpos[i], iteraciones[j], cuerpos[i])).read()))
                        #time.sleep(0.1)
                #Sacamos con el comando en bash el float resultante en cada bucle
                resultado[2]+=(float(os.popen('./nasteroids-par_guided %i %i %i 1999 | cut -d: -f 2 | tr -d " "' % (cuerpos[i], iteraciones[j], cuerpos[i])).read()))
                        #time.sleep(0.1)
            
            
            #Hacemos la media
            resultado[0]=resultado[0]/10
            resultado[1]=resultado[1]/10
            resultado[2]=resultado[2]/10

            #Imprimimos el tiempo total de cada configuración
            print("%i %i %i hilos=%i -> %f" % (cuerpos[i], iteraciones[j], cuerpos[i], hilos[t], resultado[0]), " dinamic")
            print("%i %i %i hilos=%i -> %f" % (cuerpos[i], iteraciones[j], cuerpos[i], hilos[t], resultado[1]), " static")
            print("%i %i %i hilos=%i -> %f" % (cuerpos[i], iteraciones[j], cuerpos[i], hilos[t], resultado[2]), " guided")

            #Reseteamos la variable
            resultado=[0,0,0]
