#include <iostream>
#include <gtest/gtest.h>
#include "nasteroids-seq.cpp"
using namespace std;

TEST( Test_del_paso_de_argumentos, Sin_parametros){
    int argc = 0;
    char **argv = new char*[argc];
    Datos d;
    try {
        d = parseArgs(argc, argv);
    }catch (int i){
        if (i == 1){
            SUCCEED();
        }
    }
}

TEST( Test_del_paso_de_argumentos, Parametros_no_integer){
    int argc = 5;
    char **argv = new char*[argc];
    argv[1] = "hola";
    
    Datos d;
    try {
        d = parseArgs(argc, argv);
    }catch (int i){
        if (i == 1){
            SUCCEED();
        }
    }
}

int main(int argc, char **argv){
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}

