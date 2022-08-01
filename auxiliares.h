#ifndef TRANSPORTEURBANO_AUXILIARES_H
#define TRANSPORTEURBANO_AUXILIARES_H

#include "definiciones.h"
#include "string"

double obtenerLatitud(gps posicion);
double obtenerLongitud(gps posicion);
gps obtenerPosicion(tuple<tiempo, gps> medicion);
tiempo obtenerTiempo(tuple<tiempo, gps> medicion);
double distEnKM(gps posicion1, gps posicion2);
gps desviarPunto(gps p, double desvioMtsLatitud, double desvioMtsLongitud);
gps puntoGps(double latitud, double longitud);
tuple<tiempo, gps> medicion(tiempo t, gps g);

void guardarGrillaEnArchivo(grilla g, string nombreArchivo);
void guardarRecorridosEnArchivo(vector<recorrido> recorridos, string nombreArchivo);

bool sonSalto(gps v1,gps v2,grilla g);
#endif //TRANSPORTEURBANO_AUXILIARES_H
int tiempoMenor(viaje v, int j) ;
viaje swap(int i, int j , viaje v);
viaje ordenarPorTiempo(viaje v);
double velocidad(tuple<tiempo, gps> p1, tuple<tiempo, gps> p2);
bool puntoCubiertoViaje(gps g, viaje v, distancia u);
bool viajeEnFranja(viaje v,tiempo t0, tiempo tf);
gps esqSuperior(double altoCelda,double anchoCelda,int j,int i, gps esq1);
gps esqInferior(double altoCelda,double anchoCelda,int j,int i, gps esq2,int n,int m);
int distanciaEntreCeldas(nombre c1,nombre c2);
bool celdaEnCoordenada(gps x,gps g1,gps g2);
nombre celdaEnGrilla(gps x,grilla g);
bool sonSalto(gps v1,gps v2,grilla g);

bool noEsError (tiempo t, vector<tiempo> errores);
gps corregirError (viaje v, tiempo error,vector<tiempo> errores );
viaje masCercanos(viaje v, tiempo t,vector<tiempo> errores);
double longitudCorrecta(tuple<tiempo, gps> p1, tuple<tiempo, gps> p2,tuple<tiempo, gps> corregir);
double latitudCorrecta(tuple<tiempo, gps> p1, tuple<tiempo, gps> p2,tuple<tiempo, gps> corregir);
bool sonGrillasIguales(grilla g0, grilla g1);
