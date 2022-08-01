#include "../ejercicios.h"
#include "../auxiliares.h"
#include "gtest/gtest.h"

using namespace std;


TEST(cantidadDeSaltosTEST, todosSaltosDesordenados){
    grilla g = construirGrilla(puntoGps(12, 12), puntoGps(0.0,21),  3, 3);
    gps p1 = puntoGps(3, 13);
    gps p2 = puntoGps(12,13);
    gps p3 = puntoGps(20,20);
    viaje v = {medicion(20, p1),
               medicion(12, p2),
               medicion(40, p3)}; // no eran cada 20 sec?
    EXPECT_EQ(cantidadDeSaltos(g,v),2);
}


TEST(cantidadDeSaltosTEST, unSalto) {
    grilla g = construirGrilla(puntoGps(12, 12), puntoGps(0.0,21),  3, 3);
    gps p1 = puntoGps(3, 13);
    gps p2 = puntoGps(3,20);
    gps p3 = puntoGps(5,20);
    viaje v = {medicion(1.5, p1),
               medicion(1.6, p2),
               medicion(1.7, p3)};
    EXPECT_EQ(cantidadDeSaltos(g,v),1);

}

TEST(cantidadDeSaltosTEST, sinSaltos) {
    grilla g = construirGrilla(puntoGps(12, 12), puntoGps(0.0,21),  3, 3);
    gps p1 = puntoGps(3, 13);
    gps p2 = puntoGps(3,15);
    gps p3 = puntoGps(3,20);
    viaje v = {medicion(1.5, p1),
               medicion(1.6, p2),
               medicion(1.7, p3)};
    EXPECT_EQ(cantidadDeSaltos(g,v),0);

}

TEST(cantidadDeSaltosTEST, movimientoDiagonalNoProduceSaltos){
    grilla g = construirGrilla(puntoGps(0.0,0.0),desviarPunto(puntoGps(0.0,0.0), 3000, 3000),3,3);

    gps p1 = puntoGps(0, 0);
    gps p2 = desviarPunto(p1, 1000, 1000);
    gps p3 = desviarPunto(p1, 2000, 2000);

    viaje v = {medicion(1.5, p1),
               medicion(1.6, p2),
               medicion(1.7, p3)};

    EXPECT_EQ(cantidadDeSaltos(g,v),0);
}
