#include <algorithm>
#include "auxiliares.h"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <cmath>
using namespace std;

const double PI = 3.14;
double RADIOTIERRA = 6378;

double obtenerLatitud(gps posicion) {
    return get<0>(posicion);
}

double obtenerLongitud(gps posicion) {
    return get<1>(posicion);
}

gps obtenerPosicion(tuple<tiempo, gps> medicion) {
    return get<1>(medicion);
}

tiempo obtenerTiempo(tuple<tiempo, gps> medicion) {
    return get<0>(medicion);
}
double distEnKM(gps posicion1, gps posicion2) {
    double latitud1 = obtenerLatitud(posicion1);
    double latitud2 = obtenerLatitud(posicion2);
    double longitud1 = obtenerLongitud(posicion1);
    double longitud2 = obtenerLongitud(posicion2);

    // obtengo la distancia
    double distanciaLatitud = (latitud2 - latitud1) * PI / 180.0;
    double distanciaLongitud = (longitud2 - longitud1) * PI / 180.0;

    // paso las latitudes a radianes
    latitud1 = (latitud1) * PI / 180.0;
    latitud2 = (latitud2) * PI / 180.0;

    // aplico la formula
    double a = pow(sin(distanciaLatitud / 2), 2) +
               pow(sin(distanciaLongitud / 2), 2) *
               cos(latitud1) * cos(latitud2);

    double c = 2 * asin(sqrt(a));
    return RADIOTIERRA * c;
}

gps desviarPunto(gps p, double desvioMtsLatitud, double desvioMtsLongitud){
    double lat = obtenerLatitud(p);
    double lon = obtenerLongitud(p);

    double dx = desvioMtsLatitud / 1000;
    double dy = desvioMtsLongitud / 1000;
    double new_latitude = lat - (dx / RADIOTIERRA) * (180 / PI);
    double new_longitude = lon + (dy / RADIOTIERRA) * (180 / PI) / cos(lat * PI / 180);
    return puntoGps(new_latitude, new_longitude);

}


gps puntoGps(double latitud, double longitud) {
    return make_tuple(latitud, longitud);
}

tuple<tiempo, gps> medicion(tiempo t, gps g) {
    return make_tuple(t, g);
}

void guardarGrillaEnArchivo(grilla g, string nombreArchivo){
    ofstream myfile;
    float esq1_lat, esq2_lat, esq1_lng, esq2_lng;
    int name_0, name_1;

    myfile.open(nombreArchivo);
    myfile << std::fixed << std::setprecision(5);
    for(int i = 0; i < g.size(); i++){
        esq1_lat = get<0>(get<0>(g[i]));
        esq1_lng = get<1>(get<0>(g[i]));

        esq2_lat = get<0>(get<1>(g[i]));
        esq2_lng = get<1>(get<1>(g[i]));

        name_0 = get<0>(get<2>(g[i]));
        name_1 = get<1>(get<2>(g[i]));

        myfile << esq1_lat << "\t"
               << esq1_lng << "\t"
               << esq2_lat << "\t"
               << esq2_lng << "\t"
               << "(" << name_0 << "," << name_1 << ")"
               << endl;

    }
    myfile.close();

}

void guardarRecorridosEnArchivo(vector<recorrido> recorridos, string nombreArchivo){
    ofstream myfile;
    float lat, lng;

    myfile.open(nombreArchivo);
    myfile << std::fixed << std::setprecision(5);
    for(int i = 0; i < recorridos.size(); i++){
        for(int k = 0; k < recorridos[i].size(); k++){
            lat= get<0>(recorridos[i][k]);
            lng= get<1>(recorridos[i][k]);

            myfile << i << "\t"
                   << lat << "\t"
                   << lng << endl;
        }
    }
    myfile.close();

}

//////////////////////////

int tiempoMenor(viaje v, int j) {
    int tMenor = j;
    for (int i = j; i <  v.size(); i++){
        tiempo tActual = obtenerTiempo(v[i]);
        if (tActual< obtenerTiempo(v[tMenor])){
            tMenor = i;
        }
    }
    return tMenor;
}
// O(n) recorre todo el vector


viaje swap(int i, int j , viaje v){
    tuple<tiempo, gps> x = v[i];
    tuple<tiempo, gps> y = v[j];
    v[i] = y;
    v[j] = x;
    return v;
} // O(1)

viaje ordenarPorTiempo(viaje v){
    for (int i = 0; i < v.size() -1 ; i++){
        int tMenor =  tiempoMenor(v, i);
        v = swap(i, tMenor,v);
    }
    return v;
}
// es O(n^2) recorre todos los elementos y usa la funcion tiempoMenor que es O(n).
// n = |v|


/////////////////////

double velocidad(tuple<tiempo, gps> p1, tuple<tiempo, gps> p2){
    double distancia = distEnKM(obtenerPosicion(p2), obtenerPosicion(p1));
    tiempo tmp = (obtenerTiempo(p2)- obtenerTiempo(p1)) / 3600;
    double vel = distancia / tmp;
    return  vel;

} // O(1)


/////////////////////////

bool puntoCubiertoViaje(gps g, viaje v, distancia u){
    int i = 0;
    while (i < v.size() && (distEnKM(g, obtenerPosicion(v[i])) > u )){
        i++;
    }
    return i!= v.size();
} // O(n); n = |v|




//////////////////////////

bool viajeEnFranja(viaje v,tiempo t0, tiempo tf){
    int i = 0;
    while(i < v.size() && !(obtenerTiempo(v[i]) >= t0 && obtenerTiempo(v[i]) <= tf)){
        i++;
    }
    return i != v.size();
}
// es O(n) en el peor caso recorre todo el vector

////////////////////////////

gps esqSuperior(double altoCelda,double anchoCelda,int j,int i, gps esq1) {
    return puntoGps(obtenerLatitud(esq1) - altoCelda * (i-1),
                    obtenerLongitud(esq1) + anchoCelda * (j-1));


}//O(1)

gps esqInferior(double altoCelda,double anchoCelda,int j,int i, gps
esq2, int n,int m) {

    return puntoGps(obtenerLatitud(esq2) + altoCelda * (n - i),
                    obtenerLongitud(esq2) - anchoCelda * (m - j));

}//O(1)


/////////////////////////

int distanciaEntreCeldas(nombre c1,nombre c2){
    return sqrt(pow(get<0>(c1)- get<0>(c2),2)+pow(get<1>(c1)- get<1>(c2),2)) >= 2 ;
} // O(1)

bool celdaEnCoordenada(gps x,gps g1,gps g2){
    return (obtenerLatitud(g1) >= obtenerLatitud(x) &&  obtenerLatitud(x) > obtenerLatitud(g2)) &&
    (obtenerLongitud(g1) <= obtenerLongitud(x) && obtenerLongitud(x) < obtenerLongitud(g2));
} // O(1)

// Pre viaje en grilla

nombre obtenerNombreGps(gps x,grilla g){
    int i = 0;
    nombre res;
    while(i<g.size()) {
    if(celdaEnCoordenada(x,get<0>(g[i]), get<1>(g[i]))){
        res = get<2>(g[i]);
        i = g.size();
    }
        i++;
    }
    return res;
} // O(n); n = |g|


bool sonSalto(gps v1,gps v2,grilla g){
    nombre celdav1 = obtenerNombreGps(v1,g);
    nombre celdav2 = obtenerNombreGps(v2,g);
    if(distanciaEntreCeldas(celdav1,celdav2)){
        return true;
    }
    return false;
} // O(m+m) = O(m); m = |g|

////////////////
bool noEsError (tiempo t, vector<tiempo> errores){
    int i=0;
    while (i< errores.size() && t != errores[i]){
        i ++;
    }
    return i == errores.size();
} //  O(n), n = |errores|

double latitudCorrecta(tuple<tiempo, gps> p1, tuple<tiempo, gps> p2,tuple<tiempo, gps> corregir){
    double lat ;
    lat = ((obtenerLatitud(obtenerPosicion(p2)) - obtenerLatitud(obtenerPosicion(p1))) *
           (obtenerTiempo(corregir)- obtenerTiempo(p1))) /
          (obtenerTiempo(p2) - obtenerTiempo(p1)) + obtenerLatitud(obtenerPosicion(p1));
    return  lat;

} // O(1)

double longitudCorrecta(tuple<tiempo, gps> p1, tuple<tiempo, gps> p2,tuple<tiempo, gps> corregir){
    double lat ;
    lat = ((obtenerLongitud(obtenerPosicion(p2)) - obtenerLongitud(obtenerPosicion(p1))) *
           (obtenerTiempo(corregir)- obtenerTiempo(p1))) /
          (obtenerTiempo(p2) - obtenerTiempo(p1)) + obtenerLongitud(obtenerPosicion(p1));
    return  lat;

} // O(1)



viaje masCercanos(viaje v, tiempo t,vector<tiempo> errores){ //m = |v|, n =|errores|

    tuple<tiempo, gps> puntoMasCerca ;
    tuple<tiempo, gps> puntoCerca ;
    viaje puntoMasCercanos;
    for (int i = 0; i< v.size();i++){
        if (abs(obtenerTiempo(v[i])- t) < abs(obtenerTiempo(puntoMasCerca)-t) &&
            noEsError(obtenerTiempo(v[i]), errores)){
            puntoMasCerca = v[i];
        }
    } // O(n*m)
    puntoMasCercanos.push_back(puntoMasCerca);
    for (int i = 0; i< v.size();i++){
        if (v[i] != puntoMasCerca && abs(obtenerTiempo(v[i])- t) < abs(obtenerTiempo(puntoCerca)-t) &&
            noEsError(obtenerTiempo(v[i]), errores)){
            puntoCerca = v[i];
        }
    } // O(n*m)
    puntoMasCercanos.push_back(puntoCerca);
    return puntoMasCercanos;
}// O(n*m)


gps corregirError (viaje v, tiempo error,vector<tiempo> errores ){
    int i= 0;
    while (i<v.size() && obtenerTiempo(v[i]) != error){i++;} // O(m), m = |v|
    viaje puntosCercanos = masCercanos(v, obtenerTiempo(v[i]), errores);
    // O(m*n)
    double latitud = latitudCorrecta(puntosCercanos[0],puntosCercanos[1], v[i]);
    double  longitud = longitudCorrecta(puntosCercanos[0],puntosCercanos[1], v[i]);
    gps corregido = obtenerPosicion(v[i]);
    get<0>(corregido) = latitud;
    get<1>(corregido) = longitud;
    return  corregido;
} // O(n*m)

int cantApariciones(tuple<gps,gps,nombre> celda, grilla g){
    int apariciones = 0;
    for (int i = 0; i<g.size();i++){
        if (celda == g[i])
            apariciones ++  ;
    }
    return apariciones;
}

bool sonGrillasIguales(grilla g0, grilla g1){
   int i = 0;
    while (i< g1.size() && (cantApariciones(g0[i],g0) == cantApariciones(g0[i],g1))){
        i++;
    }

    return i == g1.size();
}
