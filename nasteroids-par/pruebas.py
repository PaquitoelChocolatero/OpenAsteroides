import os
import time

#Datos
resultado=0
cuerpos=[250,500,1000]
iteraciones=[50,100,200]
hilos=[1, 2, 4, 8, 10]

os.system("make")
#Por cada hilo
for t in range(5):
    #Cambiamos el numero de hilos
    os.system("export OMP_NUM_THREADS=%i" % (hilos[t]))
    #Por cada configuración de asteroides y planetas
    for i in range(3):
        #Por cada configuración de iteraciones
        for j in range(3):
            #Calculamos el tiempo 10 veces
            for k in range(11):
                #Sacamos con el comando en bash el float resultante en cada bucle
                resultado+=float(os.popen('./nasteroids-par %i %i %i 1999 | cut -d: -f 2 | tr -d " "' % (cuerpos[i], iteraciones[j], cuerpos[i])).read())
                #time.sleep(0.1)
            #Hacemos la media
            resultado=resultado/10
            #Imprimimos el tiempo total de cada configuración
            print("%i %i %i hilos=%i -> %f" % (cuerpos[i], iteraciones[j], cuerpos[i], hilos[t], resultado))
            #Reseteamos la variable
            resultado=0
