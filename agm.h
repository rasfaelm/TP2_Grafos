#ifndef AGM_H
#define AGM_H

typedef struct {
    int u, v;
    double w;
} Aresta;

int kruskal(int n, const Aresta *e, int m, Aresta *agm, double *custo);

int prim(int n, const Aresta *e, int m, Aresta *agm, double *custo);

#endif
