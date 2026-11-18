#ifndef METRICA_H
#define METRICA_H

/*
G: matriz de adjacência ponderada.
n: quantidade de vértices.
inicio: vértice inicial (índice começando em 0).
*/

void dijkstra(double *G, int n, int inicio);//serve de resultado para os outros
double excentricidade(double *G, int n, int inicio);
double diametro(double *G, int n);
double raio(double *G, int n);

#endif