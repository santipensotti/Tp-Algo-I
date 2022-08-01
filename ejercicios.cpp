#include "ejercicios.h"
#include "auxiliares.h"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <math.h>

using namespace std;

/******++++**************************** EJERCICIO tiempoTotal ***********+++***********************/
tiempo tiempoTotal(viaje v) { // n = |v|
    tiempo t;
    tiempo tMayor;
    tiempo tMenor = obtenerTiempo(v[0]);
    tiempo tActual;
    for (int i= 0; i < v.size(); i ++){
        tActual = obtenerTiempo(v[i]);
        if (tActual > tMayor){
            tMayor = tActual;
        }
        if (tActual< tMenor){
            tMenor = tActual;
        }
    }
    t = tMayor - tMenor;
    return t;
}

// O(n) recorre todos los elementos para asegurarse de tener el mayor y el menor.


/************++*********************** EJERCICIO distanciaTotal ************++*********************/

distancia distanciaTotal(viaje v) {// n = |v|
    distancia d = 0;
    v = ordenarPorTiempo(v);
    for (int i = 1; i< v.size(); i++){
        d = d + distEnKM(obtenerPosicion(v[i]), obtenerPosicion(v[i-1]));
    }

    return d;
}

// es O(n^2 + n), ordena el vector y despues lo recorre, pertenece a O(n^2).

/*****************************+***** EJERCICIO excesoDeVelocidad **********************************/


bool excesoDeVelocidad(viaje v) { // n = |v|

    auto error = 0.001;
    v = ordenarPorTiempo(v);
    int i = 1;
    while(i < v.size() && velocidad(v[i-1],v[i]) < (80+error)){
        i++;
    }
    return i != v.size();
}


// es O(n^2 + n), ordena el vector y despues lo recorre, pertenece a O(n^2).


/************************************ EJERCICIO recorridoNoCubierto
 * *******************************/

vector<gps> recorridoNoCubierto(viaje v, recorrido r, distancia u) { // n =|v|, m = |r|
    vector<gps> resp;
    for (int i = 0; i < r.size(); i++){
        if (!puntoCubiertoViaje(r[i],v,u))
        {
            resp.push_back(r[i]);
        }
    }
    return resp;
}
// O(n*m)
/***************************************** EJERCICIO flota ***************************************/



int flota(vector<viaje> f, tiempo t0, tiempo tf) {
    int resp  = 0;
    for(int i = 0; i<f.size(); i++){
        viaje v = f[i];
        if (viajeEnFranja(v, t0, tf)){
            resp = resp +1;
        }


    }
    return resp;
}

// Sea n = |f| y m = elementos de un vector cualquiera dentro de f.
// En el peor caso la complejidad es O(n * m) recorre todos los elementos de f y los elementos dentro de ellos.
/************************************** EJERCICIO construirGrilla *******************************/




grilla construirGrilla(gps esq1, gps esq2, int n, int m) {
    grilla resp = {};

    double altoCelda = (get<0>(esq1) - get<0>(esq2))/n;
    double anchoCelda = (get<1>(esq2) - get<1>(esq1))/m;

    // i fila, j columna
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {

            tuple<gps,gps,nombre> elem = make_tuple(esqSuperior(altoCelda,
                                                                anchoCelda,j,
                                                                i,esq1),
                                                    esqInferior(altoCelda,
                                                                anchoCelda,j,
                                                                i,esq2,n,m), make_tuple(i,j));
            resp.push_back(elem);

        }

    }
    return resp;
} // O(n*m)

/************************************* EJERCICIO cantidadDeSaltos ******************************/
int cantidadDeSaltos(grilla g, viaje v) { // m = |g|, n = |v|
    int saltos = 0;
    v=ordenarPorTiempo(v);
    for (int i = 0; i < v.size()-1; ++i) {
        bool sonSaltoConsecutivo = sonSalto(obtenerPosicion(v[i]),obtenerPosicion(v[i+1])
                                            ,g);
        if(sonSaltoConsecutivo)saltos+=1;
    }
    return saltos;
}

// Ordenar es O(n^2) + iterar n-1*sonSalto que es O(m) = O(n^2+m)

/************************************* EJERCICIO corregirViaje ******************************/


void corregirViaje(viaje& v, vector<tiempo> errores){// m = |v|, n = |errores|
    for (int i=0 ; i < errores.size();i++){ // O(n)
        gps corregido = corregirError(v, errores[i], errores);//O(n*m)
        for (int j = 0; j< v.size(); j++ ){//O(m)
            if(obtenerTiempo(v[j])== errores[i]){
                get<1>(v[j]) = corregido;
            }
        }
    }

}// O(n^2*m^2)

