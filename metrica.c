#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include "metrica.h"

/*
G: matriz de adjacência ponderada.
n: quantidade de vértices.
inicio: vértice inicial (índice começando em 0).
*/

void dijkstra(double *G, int n, int inicio);//serve de resultado para os outros
double excentricidade(double *G, int n, int inicio);
double diametro(double *G, int n);
double raio(double *G, int n);

double excentricidade(double *G, int n, int inicio){
    double *dist = malloc(n * sizeof(double));
    int *marcado = calloc(n, sizeof(int));
    int i, j, prox;
    double maiorDist = 0;

    if(dist == NULL || marcado == NULL){
        free(dist);
        free(marcado);
        return DBL_MAX;
    }
    for(i = 0; i < n; i++)
        dist[i] = DBL_MAX;

    dist[inicio] = 0;

    for(int k = 0; k < n; k++){
        double menor = DBL_MAX;
        prox = -1;

        for(i = 0; i < n; i++){
            if(!marcado[i] && dist[i] < menor){
                menor = dist[i];
                prox = i;
            }
        }
        if(prox == -1)
            break;

        marcado[prox] = 1;

        for(j = 0; j < n; j++){
            if(!marcado[j] && G[prox*n+j] != DBL_MAX){
                if(dist[prox] + G[prox*n+j] < dist[j])
                    dist[j] = dist[prox] + G[prox*n+j];
            }
        }
    }
    for(i = 0; i < n; i++){
        if(dist[i] == DBL_MAX){
            maiorDist = DBL_MAX;
            break;
        }
        if(dist[i] > maiorDist)
            maiorDist = dist[i];
    }
    free(dist);
    free(marcado);
    return maiorDist;
}

double diametro(double *G, int n){
    double maiorDist = 0, ex;

    for(int i = 0; i < n; i++){
        ex = excentricidade(G, n, i);
        if(ex == DBL_MAX)
            return DBL_MAX;
        if(ex > maiorDist)
            maiorDist = ex;
    }

    return maiorDist;
}

double raio(double *G, int n){
    double menorDist = DBL_MAX, ex;

    for(int i = 0; i < n; i++){
        ex = excentricidade(G, n, i);
        if(ex < menorDist)
            menorDist = ex;
    }
    return menorDist;
}

void dijkstra(double *G, int n, int inicio){
    double *dist;
    int *ant, *marcado;
    int i, j, prox;
    dist = malloc(n * sizeof(double));
    ant = malloc(n * sizeof(int));
    marcado = calloc(n, sizeof(int));

    if(dist == NULL || ant == NULL || marcado == NULL){
        printf("Erro de memoria!\n");
        free(dist);
        free(ant);
        free(marcado);
        return;
    }

    for(i = 0; i < n; i++){
        dist[i] = DBL_MAX;
        ant[i] = -1;
    }
    dist[inicio] = 0;
    for(int k = 0; k < n; k++){
        double menor = DBL_MAX;
        prox = -1;

        for(i = 0; i < n; i++){
            if(!marcado[i] && dist[i] < menor){
                menor = dist[i];
                prox = i;
            }
        }

        if(prox == -1)
            break;

        marcado[prox] = 1;

        for(j = 0; j < n; j++){
            if(!marcado[j] && G[prox*n+j] != DBL_MAX){
                if(dist[prox] + G[prox*n+j] < dist[j]){
                    dist[j] = dist[prox] + G[prox*n+j];
                    ant[j] = prox;
                }
            }
        }
    }

    printf("\n===== RESULTADO DIJKSTRA =====\n");

    for(i = 0; i < n; i++){
        if(i == inicio)
            continue;
        printf("\nVertice: %d\n", i+1);
        if(dist[i] == DBL_MAX){
            printf("Nao existe caminho.\n");
            continue;
        }
        printf("Distancia: %.2lf\n", dist[i]);
        int *rota = malloc(n * sizeof(int));
        if(rota == NULL)
            break;
        int qtd = 0;
        int v = i;

        while(v != -1 && qtd < n){
            rota[qtd++] = v;
            v = ant[v];
        }
        printf("Caminho: ");
        for(j = qtd-1; j >= 0; j--){
            printf("%d", rota[j]+1);
            if(j > 0)
                printf(" -> ");
        }
        printf("\n");
        free(rota);
    }
    free(dist);
    free(ant);
    free(marcado);
}