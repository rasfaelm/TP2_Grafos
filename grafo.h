#ifndef GRAFO_H
#define GRAFO_H

typedef struct {
    int ordem;          // Número de vértices
    int tamanho;        // Número de arestas
    double **matriz;    // Matriz de adjacência/pesos
} Grafo;

void removerAresta(Grafo *grafo, int u, int v);
int existeAresta(Grafo *grafo, int u, int v);
void DFS(Grafo *grafo, int v, int *visitado);
int ehPonte(Grafo *grafo, int u, int v);
int escolherInicio(Grafo *grafo);
int possuiCircuitoEuleriano(Grafo *grafo);
Grafo* copiarGrafo(Grafo *original);

int* HierHolzer(Grafo *grafo, int inicio, int *tamCircuito);
int* Fleury(Grafo *grafo, int inicio, int *tamCircuito);

/* Criação e destruição */
Grafo *criarGrafo(int ordem);
void liberarGrafo(Grafo *grafo);

/* Entrada de dados */
Grafo *lerGrafo(const char *nomeArquivo);

/* Informações básicas */
int obterOrdem(const Grafo *grafo);
int obterTamanho(const Grafo *grafo);
double calcularDensidade(const Grafo *grafo);

/* Vizinhança e grau */
void listarVizinhos(const Grafo *grafo, int vertice);
int obterGrau(const Grafo *grafo, int vertice);

/* Vértices de articulação */
int ehArticulacao(const Grafo *grafo, int vertice);

/* Busca em largura */
void buscaLargura(const Grafo *grafo, int inicio);

/* Componentes conexas */
int contarComponentesConexas(const Grafo *grafo);
void listarComponentesConexas(const Grafo *grafo);

/* Ciclos */
int possuiCiclo(const Grafo *grafo);

/* Caminhos mínimos */
void caminhosMinimos(const Grafo *grafo, int origem);
void imprimirCaminho(int origem, int destino, const int *anterior);

/* Cobertura de vértices */
int coberturaGulosa(const Grafo *grafo, int *cobertura);
int coberturaHeuristica(const Grafo *grafo, int *cobertura);
void coberturaMinima(const Grafo *grafo);

#endif