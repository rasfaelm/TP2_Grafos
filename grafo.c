#include "grafo.h"

#include <stdio.h>
#include <stdlib.h>
#include <float.h>

/**
 * Cria um grafo com 'ordem' vértices.
 * A matriz será inicializada com zero.
 */
Grafo *criarGrafo(int ordem){
    Grafo *grafo = malloc(sizeof(Grafo));

    if (grafo == NULL) {
        return NULL;
    }

    grafo->ordem = ordem;
    grafo->tamanho = 0;
    grafo->matriz = malloc(ordem * sizeof(double *));

    if (grafo->matriz == NULL) {
        free(grafo);
        return NULL;
    }

    for (int i = 0; i < ordem; i++) {
        grafo->matriz[i] = calloc(ordem, sizeof(double));

        if (grafo->matriz[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(grafo->matriz[j]);
            }

            free(grafo->matriz);
            free(grafo);
            return NULL;
        }
    }

    return grafo;
}

/**
 * Libera toda a memória utilizada pelo grafo.
 */
void liberarGrafo(Grafo *grafo){

    if (grafo == NULL) {
        return;
    }

    if (grafo->matriz != NULL) {
        for (int i = 0; i < grafo->ordem; i++) {
            free(grafo->matriz[i]);
        }

        free(grafo->matriz);
    }

    free(grafo);
}

/**
 * Lê um grafo a partir do arquivo.
 *
 * Formato esperado:
 *
 * 6
 * 1 2 2.5
 * 1 4 4.0
 * ...
 */
Grafo *lerGrafo(const char *nomeArquivo){
    FILE *arquivo;
    int ordem, coluna, linha, tamanho = 0;  
    double peso;
    arquivo = fopen(nomeArquivo, "r");
        if (arquivo == NULL) {
        printf("Erro: Não foi possível abrir o arquivo '%s'.\n", nomeArquivo);
        return NULL;
    }
    fscanf(arquivo,"%d",&ordem);
    Grafo *grafo = criarGrafo(ordem);
    while(fscanf(arquivo,"%d %d %lf",&linha,&coluna,&peso)!=EOF){
        tamanho += 1;
        grafo->matriz[coluna-1][linha-1]=peso;
        grafo->matriz[linha-1][coluna-1]=peso;
        grafo->tamanho++;
    }
    fclose(arquivo);
    return grafo;
}

/* Retorna a ordem do grafo. */
int obterOrdem(const Grafo *grafo){

    if (grafo == NULL) {
        return 0;
    }

    return grafo->ordem;
}

/* Retorna o tamanho do grafo. */
int obterTamanho(const Grafo *grafo){

    if (grafo == NULL) {
        return 0;
    }

    return grafo->tamanho;
}

/* Calcula a densidade do grafo. */
double calcularDensidade(const Grafo *grafo){
    if (grafo == NULL || grafo->ordem < 2) {
        return 0.0;
    }

    /* Densidade de grafo não direcionado: 2m / (n(n-1)) */
    return (2.0 * grafo->tamanho) / ((double)grafo->ordem * (grafo->ordem - 1));
}

/* Lista os vizinhos de um vértice. */
void listarVizinhos(const Grafo *grafo, int vertice){

    if (grafo == NULL) {
        printf("Grafo invalido.\n");
        return;
    }

    if (vertice < 1 || vertice > grafo->ordem) {
        printf("Vertice invalido.\n");
        return;
    }

    vertice--;

    printf("Vizinhos do vertice %d:\n", vertice + 1);

    int encontrou = 0;

    for (int i = 0; i < grafo->ordem; i++) {

        if (grafo->matriz[vertice][i] != 0) {

            printf("%d ", i + 1);

            encontrou = 1;
        }
    }

    if (!encontrou) {
        printf("Nenhum");
    }

    printf("\n");
}

/* Retorna o grau de um vértice. */
int obterGrau(const Grafo *grafo, int vertice) {
    if (grafo == NULL) {
        return 0;
    }

    if (vertice < 1 || vertice > grafo->ordem) {
        return 0;
    }

    vertice--;

    int grau = 0;

    for (int i = 0; i < grafo->ordem; i++) {
        if (grafo->matriz[vertice][i] != 0) {
            grau++;
        }
    }

    return grau;
}

/* Verifica se um vértice é de articulação. */
int ehArticulacao(const Grafo *grafo, int vertice){

    if (grafo == NULL) {
        return 0;
    }

    if (vertice < 1 || vertice > grafo->ordem) {
        return 0;
    }

    vertice--;

    int n = grafo->ordem;

    int *visitado = calloc(n, sizeof(int));
    int *fila = malloc(n * sizeof(int));

    if (visitado == NULL || fila == NULL) {

        free(visitado);
        free(fila);

        return 0;
    }

    int componentesAntes = 0;

    int inicio = 0;
    int fim = 0;

    for (int i = 0; i < n; i++) {

        if (!visitado[i]) {

            componentesAntes++;

            inicio = 0;
            fim = 0;

            fila[fim] = i;
            fim++;

            visitado[i] = 1;

            while (inicio < fim) {

                int atual = fila[inicio];
                inicio++;

                for (int j = 0; j < n; j++) {

                    if (grafo->matriz[atual][j] != 0 &&
                        !visitado[j]) {

                        visitado[j] = 1;

                        fila[fim] = j;
                        fim++;
                    }
                }
            }
        }
    }

    for (int i = 0; i < n; i++) {
        visitado[i] = 0;
    }

    int componentesDepois = 0;

    for (int i = 0; i < n; i++) {

        if (i == vertice) {
            continue;
        }

        if (!visitado[i]) {

            componentesDepois++;

            inicio = 0;
            fim = 0;

            fila[fim] = i;
            fim++;

            visitado[i] = 1;

            while (inicio < fim) {

                int atual = fila[inicio];
                inicio++;

                for (int j = 0; j < n; j++) {

                    if (j == vertice) {
                        continue;
                    }

                    if (grafo->matriz[atual][j] != 0 &&
                        !visitado[j]) {

                        visitado[j] = 1;

                        fila[fim] = j;
                        fim++;
                    }
                }
            }
        }
    }

    free(visitado);
    free(fila);

    if (componentesDepois > componentesAntes) {
        return 1;
    }

    return 0;
}

/* Executa busca em largura (BFS). */
void buscaLargura(const Grafo *grafo, int inicio){
    if (grafo == NULL) {
        printf("Grafo invalido.\n");
        return;
    }

    inicio--;

    if (inicio < 0 || inicio >= grafo->ordem) {
        printf("Vertice inicial invalido.\n");
        return;
    }

    int *visitado = calloc(grafo->ordem, sizeof(int));
    int *pai = malloc(grafo->ordem * sizeof(int)); 
    int *fila = malloc(grafo->ordem * sizeof(int));

    if (visitado == NULL || pai == NULL || fila == NULL) {
        printf("Erro ao alocar memoria.\n");

        free(visitado);
        free(pai);
        free(fila);

        return;
    }

    for (int i = 0; i < grafo->ordem; i++) {
        pai[i] = -1;
    }

    int inicioFila = 0;
    int fimFila = 0;

    fila[fimFila++] = inicio;
    visitado[inicio] = 1;

    printf("\nBUSCA EM LARGURA \n");
    printf("Vertice inicial: %d\n", inicio + 1);
    printf("Sequencia de vertices visitados:\n");

    while (inicioFila < fimFila) {

        int atual = fila[inicioFila++];

        printf("%d ", atual + 1);

        for (int vizinho = 0; vizinho < grafo->ordem; vizinho++) {

            if (grafo->matriz[atual][vizinho] != 0 && !visitado[vizinho]) {
                visitado[vizinho] = 1;
                pai[vizinho] = atual;
                fila[fimFila++] = vizinho;
            }
        }
    }

    printf("\n");
    printf("\nArestas da arvore de busca em largura:\n");

    int encontrouArvore = 0;

    for (int v = 0; v < grafo->ordem; v++) {

        if (pai[v] != -1) {
            printf("(%d, %d)\n", pai[v] + 1, v + 1);
            encontrouArvore = 1;
        }
    }

    if (!encontrouArvore) {
        printf("Nenhuma aresta.\n");
    }

    printf("\nArestas que nao pertencem a arvore BFS:\n");

    int encontrouNaoArvore = 0;

    for (int u = 0; u < grafo->ordem; u++) {

        for (int v = u + 1; v < grafo->ordem; v++) {

            // Verifica se existe uma aresta entre u e v.
            if (grafo->matriz[u][v] != 0) {

                int pertenceArvore = (pai[u] == v || pai[v] == u);

                if (!pertenceArvore) {

                    printf("(%d, %d)\n", u + 1, v + 1);
                    encontrouNaoArvore = 1;
                }
            }
        }
    }

    if (!encontrouNaoArvore) {
        printf("Nenhuma aresta.\n");
    }

    // Verifica se existem vertices que nao foram alcançados.
    printf("\nVertices nao alcancados a partir de %d:\n", inicio + 1);

    int encontrouNaoAlcancado = 0;

    for (int i = 0; i < grafo->ordem; i++) {

        if (!visitado[i]) {
            printf("%d ", i + 1);
            encontrouNaoAlcancado = 1;
        }
    }

    if (!encontrouNaoAlcancado) {
        printf("Nenhum");
    }

    printf("\n");

    // Libera toda a memoria utilizada pela BFS.
    free(visitado);
    free(pai);
    free(fila);
}

/*
 * Função auxiliar (Roy): rotula cada vértice com o índice da sua
 * componente conexa em comp[] e retorna o número de componentes.
 * Escolhe um vértice ainda não rotulado e propaga o rótulo a todos os
 * vértices alcançáveis a partir dele. Custo O(n^2) com matriz.
 */
static int rotularComponentes(const Grafo *grafo, int *comp){
    int n = grafo->ordem, k = 0;
    int *pilha = malloc(n * sizeof(int));

    for (int i = 0; i < n; i++) {
        comp[i] = -1;
    }

    for (int s = 0; s < n; s++) {
        if (comp[s] != -1) {
            continue;
        }

        int topo = 0;
        comp[s] = k;
        pilha[topo++] = s;

        while (topo > 0) {
            int u = pilha[--topo];
            for (int v = 0; v < n; v++) {
                if (grafo->matriz[u][v] != 0.0 && comp[v] == -1) {
                    comp[v] = k;
                    pilha[topo++] = v;
                }
            }
        }
        k++;
    }

    free(pilha);
    return k;
}

/* Retorna a quantidade de componentes conexas. */
int contarComponentesConexas(const Grafo *grafo){
    if (grafo == NULL) {
        return 0;
    }

    int *comp = malloc(grafo->ordem * sizeof(int));
    int k = rotularComponentes(grafo, comp);
    free(comp);
    return k;
}

/* Lista os vértices pertencentes a cada componente conexa. */
void listarComponentesConexas(const Grafo *grafo){
    if (grafo == NULL) {
        return;
    }

    int *comp = malloc(grafo->ordem * sizeof(int));
    int k = rotularComponentes(grafo, comp);

    printf("Numero de componentes conexas: %d\n", k);
    for (int c = 0; c < k; c++) {
        printf("Componente %d: {", c + 1);
        int primeiro = 1;
        for (int v = 0; v < grafo->ordem; v++) {
            if (comp[v] == c) {
                printf("%s%d", primeiro ? "" : ", ", v + 1);
                primeiro = 0;
            }
        }
        printf("}\n");
    }

    free(comp);
}

/* Verifica se o grafo possui ciclo. */
int possuiCiclo(const Grafo *grafo){

    if (grafo == NULL) {
        return 0;
    }

    int n = grafo->ordem;

    int *visitado = calloc(n, sizeof(int));
    int *pai = malloc(n * sizeof(int));
    int *fila = malloc(n * sizeof(int));

    if (visitado == NULL ||pai == NULL ||fila == NULL) {

        free(visitado);
        free(pai);
        free(fila);

        return 0;
    }

    for (int i = 0; i < n; i++) {
        pai[i] = -1;
    }

    for (int i = 0; i < n; i++) {

        if (!visitado[i]) {

            int inicio = 0;
            int fim = 0;

            fila[fim] = i;
            fim++;

            visitado[i] = 1;

            while (inicio < fim) {

                int atual = fila[inicio];
                inicio++;

                for (int j = 0; j < n; j++) {

                    if (grafo->matriz[atual][j] == 0) {
                        continue;
                    }

                    if (!visitado[j]) {

                        visitado[j] = 1;
                        pai[j] = atual;

                        fila[fim] = j;
                        fim++;

                    } else if (pai[atual] != j) {

                        free(visitado);
                        free(pai);
                        free(fila);

                        return 1;
                    }
                }
            }
        }
    }

    free(visitado);
    free(pai);
    free(fila);

    return 0;
}

/* Calcula caminhos mínimos a partir de uma origem. */
void caminhosMinimos(const Grafo *grafo, int origem){

    if (grafo == NULL) {
        printf("Grafo invalido.\n");
        return;
    }

    origem--;

    if (origem < 0 || origem >= grafo->ordem) {
        printf("Vertice de origem invalido.\n");
        return;
    }

    int n = grafo->ordem;

    double *dist = malloc(n * sizeof(double));

    int *anterior = malloc(n * sizeof(int));

    int *visitado = calloc(n, sizeof(int));

    if (dist == NULL || anterior == NULL || visitado == NULL) {

        printf("Erro ao alocar memoria.\n");

        free(dist);
        free(anterior);
        free(visitado);

        return;
    }

    const double INFINITO = 1e308;

    for (int i = 0; i < n; i++) {
        dist[i] = INFINITO;
        anterior[i] = -1;
    }

    dist[origem] = 0.0;

    for (int contador = 0; contador < n; contador++) {

        int atual = -1;
        double menorDistancia = INFINITO;

        for (int i = 0; i < n; i++) {

            if (!visitado[i] && dist[i] < menorDistancia) {
                menorDistancia = dist[i];
                atual = i;
            }
        }

        if (atual == -1) {
            break;
        }

        visitado[atual] = 1;

        for (int vizinho = 0; vizinho < n; vizinho++) {

            if (grafo->matriz[atual][vizinho] != 0 && !visitado[vizinho]) {

                double peso = grafo->matriz[atual][vizinho];

                if (dist[atual] + peso < dist[vizinho]) {
                    dist[vizinho] = dist[atual] + peso;
                    anterior[vizinho] = atual;
                }
            }
        }
    }

    // Mostra os resultados.
    printf("Vertice de origem: %d\n\n", origem + 1);

    for (int destino = 0; destino < n; destino++) {

        printf("Destino: %d\n", destino + 1);

        /*
         * Se a distancia continua infinita, significa que
         * nao existe caminho entre origem e destino.
         */
        if (dist[destino] == INFINITO) {

            printf("Distancia: infinito\n");
            printf("Caminho: inexistente\n");

        } else {

            printf("Distancia: %.2f\n", dist[destino]);
            printf("Caminho: ");
            imprimirCaminho(origem, destino, anterior);
            printf("\n");
        }

        printf("\n");
    }

    free(dist);
    free(anterior);
    free(visitado);
}

/* Imprime um caminho utilizando o vetor de predecessores. */
void imprimirCaminho(int origem, int destino, const int *anterior){

    if (destino == origem) {
        printf("%d", origem + 1);
        return;
    }

    if (anterior[destino] == -1) {
        printf("inexistente");
        return;
    }

    imprimirCaminho(origem, anterior[destino], anterior);

    printf(" -> %d", destino + 1);
}

/* Verifica se 'cobertura' cobre todas as arestas do grafo. */
static int coberturaValida(const Grafo *grafo, const int *cobertura){
    int n = grafo->ordem;

    for (int u = 0; u < n; u++) {
        for (int v = u + 1; v < n; v++) {
            if (grafo->matriz[u][v] != 0 && !cobertura[u] && !cobertura[v]) {
                return 0;
            }
        }
    }

    return 1;
}

/* Imprime o conjunto de vértices marcados em 'cobertura'. */
static void imprimirCobertura(const Grafo *grafo, const int *cobertura, int tamanho){
    printf("Tamanho: %d\n", tamanho);
    printf("Vertices: {");

    int primeiro = 1;

    for (int i = 0; i < grafo->ordem; i++) {
        if (cobertura[i]) {
            printf("%s%d", primeiro ? "" : ", ", i + 1);
            primeiro = 0;
        }
    }

    printf("}\n");
}

/*
 * Algoritmo GULOSO
 * Retorna o tamanho da cobertura, ou -1 se faltar memória.
 */
int coberturaGulosa(const Grafo *grafo, int *cobertura){
    if (grafo == NULL || cobertura == NULL) {
        return 0;
    }

    int n = grafo->ordem;
    int tamanho = 0;

    /* grau[i] = número de arestas AINDA DESCOBERTAS incidentes em i. */
    int *grau = calloc(n, sizeof(int));

    if (grau == NULL) {
        return -1;
    }

    for (int i = 0; i < n; i++) {
        cobertura[i] = 0;

        for (int j = 0; j < n; j++) {
            if (i != j && grafo->matriz[i][j] != 0) {
                grau[i]++;
            }
        }
    }

    while (1) {
        int melhor = -1;

        for (int i = 0; i < n; i++) {
            if (!cobertura[i] && grau[i] > 0 &&
                (melhor == -1 || grau[i] > grau[melhor])) {
                melhor = i;
            }
        }

        /* Nenhuma aresta descoberta: terminou. */
        if (melhor == -1) {
            break;
        }

        cobertura[melhor] = 1;
        tamanho++;

        /* As arestas de 'melhor' agora estão cobertas. */
        for (int j = 0; j < n; j++) {
            if (j != melhor && grafo->matriz[melhor][j] != 0 && !cobertura[j]) {
                grau[j]--;
            }
        }

        grau[melhor] = 0;
    }

    free(grau);
    return tamanho;
}

/*
 * HEURÍSTICA
 * Retorna o tamanho da cobertura.
 */
int coberturaHeuristica(const Grafo *grafo, int *cobertura){
    if (grafo == NULL || cobertura == NULL) {
        return 0;
    }

    int n = grafo->ordem;

    for (int i = 0; i < n; i++) {
        cobertura[i] = 0;
    }

    for (int u = 0; u < n; u++) {
        for (int v = u + 1; v < n; v++) {
            if (grafo->matriz[u][v] != 0 && !cobertura[u] && !cobertura[v]) {
                cobertura[u] = 1;
                cobertura[v] = 1;
            }
        }
    }

    for (int u = 0; u < n; u++) {
        if (!cobertura[u]) {
            continue;
        }

        int redundante = 1;

        for (int v = 0; v < n; v++) {
            if (v != u && grafo->matriz[u][v] != 0 && !cobertura[v]) {
                redundante = 0;
                break;
            }
        }

        if (redundante) {
            cobertura[u] = 0;
        }
    }

    int tamanho = 0;

    for (int i = 0; i < n; i++) {
        tamanho += cobertura[i];
    }

    return tamanho;
}

/* Executa e imprime a cobertura de vértices (gulosa e heurística). */
void coberturaMinima(const Grafo *grafo){
    if (grafo == NULL) {
        printf("Grafo invalido.\n");
        return;
    }

    int *cobGulosa = malloc(grafo->ordem * sizeof(int));
    int *cobHeuristica = malloc(grafo->ordem * sizeof(int));

    if (cobGulosa == NULL || cobHeuristica == NULL) {
        printf("Erro ao alocar memoria.\n");

        free(cobGulosa);
        free(cobHeuristica);

        return;
    }

    int tamGulosa = coberturaGulosa(grafo, cobGulosa);
    int tamHeuristica = coberturaHeuristica(grafo, cobHeuristica);

    if (tamGulosa < 0 || tamHeuristica < 0) {
        printf("Erro ao alocar memoria.\n");

        free(cobGulosa);
        free(cobHeuristica);

        return;
    }

    printf("\nCOBERTURA DE VERTICES\n");

    printf("\n[Algoritmo guloso - maior grau primeiro]\n");
    imprimirCobertura(grafo, cobGulosa, tamGulosa);
    printf("Cobertura valida: %s\n", coberturaValida(grafo, cobGulosa) ? "sim" : "nao");

    printf("\n[Heuristica - emparelhamento maximal + poda]\n");
    imprimirCobertura(grafo, cobHeuristica, tamHeuristica);
    printf("Cobertura valida: %s\n", coberturaValida(grafo, cobHeuristica) ? "sim" : "nao");

    printf("\nMenor cobertura encontrada: %d vertice(s) (%s)\n",
           tamGulosa <= tamHeuristica ? tamGulosa : tamHeuristica,
           tamGulosa < tamHeuristica ? "guloso" :
           tamHeuristica < tamGulosa ? "heuristica" : "empate");

    free(cobGulosa);
    free(cobHeuristica);
}

int existeAresta(Grafo *grafo, int u, int v) {
    return grafo->matriz[u][v] != 0;
}

void removerAresta(Grafo *grafo, int u, int v) {

    if (grafo->matriz[u][v] != 0) {

        grafo->matriz[u][v] = 0;
        grafo->matriz[v][u] = 0;

        grafo->tamanho--;
    }
}

int* HierHolzer(Grafo *grafo, int inicio, int *tamCircuito) {

    int *pilha = malloc((grafo->tamanho + 1) * sizeof(int));
    int *circuito = malloc((grafo->tamanho + 1) * sizeof(int));

    int topo = -1;
    int fim = 0;

    // Começa pelo vértice inicial
    pilha[++topo] = inicio;

    while (topo >= 0) {

        int v = pilha[topo];
        int w = -1;

        // Procura uma aresta ainda não utilizada
        for (int i = 0; i < grafo->ordem; i++) {
            if (existeAresta(grafo, v, i)) {
                w = i;
                break;
            }
        }

        if (w != -1) {

            // Remove a aresta utilizada
            removerAresta(grafo, v, w);

            // Continua o percurso
            pilha[++topo] = w;

        } else {

            // Não existem mais arestas disponíveis em v
            circuito[fim++] = v;

            topo--;
        }
    }

    // O circuito é construído na ordem inversa
    for (int i = 0; i < fim / 2; i++) {
        int aux = circuito[i];
        circuito[i] = circuito[fim - 1 - i];
        circuito[fim - 1 - i] = aux;
    }

    free(pilha);

    *tamCircuito = fim;

    return circuito;
}

void DFS(Grafo *grafo, int v, int *visitado) {

    visitado[v] = 1;

    for (int i = 0; i < grafo->ordem; i++) {

        if (existeAresta(grafo, v, i) && !visitado[i]) {
            DFS(grafo, i, visitado);
        }
    }
}

int ehPonte(Grafo *grafo, int u, int v) {

    if (obterGrau(grafo, u + 1) == 1) {
        return 0;
    }

    int *visitado = calloc(grafo->ordem, sizeof(int));

    // Guarda o peso original
    double peso = grafo->matriz[u][v];

    // Remove temporariamente a aresta
    removerAresta(grafo, u, v);

    // Verifica a conectividade
    DFS(grafo, u, visitado);

    int ponte = !visitado[v];

    // Restaura a aresta com seu peso original
    grafo->matriz[u][v] = peso;
    grafo->matriz[v][u] = peso;

    grafo->tamanho++;

    free(visitado);

    return ponte;
}

int* Fleury(Grafo *grafo, int inicio, int *tamCircuito) {

    int *circuito = malloc((grafo->tamanho + 1) * sizeof(int));

    int atual = inicio;
    int fim = 0;

    circuito[fim++] = atual;

    while (grafo->tamanho > 0) {

        int proximo = -1;

        // Procura uma aresta que não seja ponte
        for (int i = 0; i < grafo->ordem; i++) {

            if (existeAresta(grafo, atual, i)) {

                if (!ehPonte(grafo, atual, i)) {
                    proximo = i;
                    break;
                }

                // Guarda uma ponte como alternativa
                if (proximo == -1) {
                    proximo = i;
                }
            }
        }

        // Percorre e remove a aresta escolhida
        removerAresta(grafo, atual, proximo);

        atual = proximo;

        // Adiciona o novo vértice ao circuito
        circuito[fim++] = atual;
    }

    *tamCircuito = fim;

    return circuito;
}

int escolherInicio(Grafo *grafo) {

    // Primeiro vértice (0-based) que possui alguma aresta
    for (int i = 0; i < grafo->ordem; i++) {
        if (obterGrau(grafo, i + 1) > 0) {
            return i;
        }
    }

    return 0;
}

int possuiCircuitoEuleriano(Grafo *grafo) {

    int inicio = -1;

    // Verifica se todos os vértices possuem grau par
    // (obterGrau recebe o vértice 1-based)
    for (int i = 0; i < grafo->ordem; i++) {

        int grau = obterGrau(grafo, i + 1);

        if (grau % 2 != 0) {
            return 0;
        }

        // Encontra o primeiro vértice com arestas
        if (grau > 0 && inicio == -1) {
            inicio = i;
        }
    }

    // Grafo sem arestas: circuito trivial
    if (inicio == -1) {
        return 1;
    }

    // Verifica se todos os vértices com arestas
    // pertencem à mesma componente conexa
    int *visitado = calloc(grafo->ordem, sizeof(int));

    if (visitado == NULL) {
        return 0;
    }

    DFS(grafo, inicio, visitado);

    for (int i = 0; i < grafo->ordem; i++) {

        if (obterGrau(grafo, i + 1) > 0 && !visitado[i]) {

            free(visitado);
            return 0;
        }
    }

    free(visitado);

    return 1;
}

/* Imprime um circuito (vértices internos 0-based, exibidos 1-based). */
static void imprimirCircuito(const char *titulo, int *circuito, int tam) {

    printf("\n[%s]\n", titulo);

    for (int i = 0; i < tam; i++) {
        printf("%d%s", circuito[i] + 1, i < tam - 1 ? " -> " : "\n");
    }

    printf("Arestas percorridas: %d\n", tam - 1);
}

/* Verifica se existe circuito Euleriano e, se existir,
 * imprime o circuito obtido por Hierholzer e por Fleury. */
void imprimirCircuitoEuleriano(Grafo *grafo) {

    if (grafo == NULL) {
        printf("Grafo invalido.\n");
        return;
    }

    if (!possuiCircuitoEuleriano(grafo)) {
        printf("O grafo NAO possui circuito Euleriano.\n");
        return;
    }

    if (grafo->tamanho == 0) {
        printf("O grafo nao possui arestas (circuito trivial).\n");
        return;
    }

    int inicio = escolherInicio(grafo);

    printf("O grafo possui circuito Euleriano (inicio no vertice %d).\n", inicio + 1);

    // Os algoritmos removem arestas, então cada um usa sua própria cópia
    Grafo *copiaH = copiarGrafo(grafo);
    int tamH = 0;
    int *circuitoH = HierHolzer(copiaH, inicio, &tamH);
    imprimirCircuito("Hierholzer", circuitoH, tamH);
    free(circuitoH);
    liberarGrafo(copiaH);

    Grafo *copiaF = copiarGrafo(grafo);
    int tamF = 0;
    int *circuitoF = Fleury(copiaF, inicio, &tamF);
    imprimirCircuito("Fleury", circuitoF, tamF);
    free(circuitoF);
    liberarGrafo(copiaF);
}

Grafo* copiarGrafo(Grafo *original) {

    Grafo *copia = malloc(sizeof(Grafo));

    copia->ordem = original->ordem;
    copia->tamanho = original->tamanho;

    copia->matriz = malloc(
        copia->ordem * sizeof(double *)
    );

    for (int i = 0; i < copia->ordem; i++) {

        copia->matriz[i] = malloc(
            copia->ordem * sizeof(double)
        );

        for (int j = 0; j < copia->ordem; j++) {
            copia->matriz[i][j] = original->matriz[i][j];
        }
    }

    return copia;
}

/*Union-Find */
typedef struct {
    int *pai, *rank;
} UF;

static int uf_init(UF *uf, int n) {
    uf->pai = malloc(n * sizeof(int));
    uf->rank = calloc(n, sizeof(int));
    if (!uf->pai || !uf->rank) return -1;
    for (int i = 0; i < n; i++) uf->pai[i] = i;
    return 0;
}

static void uf_free(UF *uf) {
    free(uf->pai);
    free(uf->rank);
}

static int uf_find(UF *uf, int x) {
    while (uf->pai[x] != x) {
        uf->pai[x] = uf->pai[uf->pai[x]]; /* compressão de caminho */
        x = uf->pai[x];
    }
    return x;
}

static int uf_union(UF *uf, int a, int b) {
    int ra = uf_find(uf, a), rb = uf_find(uf, b);
    if (ra == rb) return 0;
    if (uf->rank[ra] < uf->rank[rb]) { int t = ra; ra = rb; rb = t; }
    uf->pai[rb] = ra;
    if (uf->rank[ra] == uf->rank[rb]) uf->rank[ra]++;
    return 1;
}

/*Kruskal */
static int cmp_aresta(const void *a, const void *b) {
    double wa = ((const Aresta *)a)->w, wb = ((const Aresta *)b)->w;
    return (wa > wb) - (wa < wb);
}

int kruskal(int n, const Aresta *e, int m, Aresta *agm, double *custo) {
    Aresta *ord = malloc((m > 0 ? m : 1) * sizeof(Aresta));
    UF uf;
    if (!ord) return -1;
    if (uf_init(&uf, n) != 0) { uf_free(&uf); free(ord); return -1; }
    for (int i = 0; i < m; i++) ord[i] = e[i];
    qsort(ord, m, sizeof(Aresta), cmp_aresta);

    int k = 0;
    *custo = 0.0;
    for (int i = 0; i < m && k < n - 1; i++) {
        if (ord[i].u != ord[i].v && uf_union(&uf, ord[i].u, ord[i].v)) {
            agm[k++] = ord[i];
            *custo += ord[i].w;
        }
    }
    free(ord);
    uf_free(&uf);
    return (k == n - 1) ? k : -1;
}

/*Prim */
typedef struct {
    double w;
    int u, v; /* u: vértice já na árvore, v: candidato */
} Item;

typedef struct {
    Item *a;
    int tam;
} Heap;

static void heap_push(Heap *h, Item it) {
    int i = h->tam++;
    while (i > 0) {
        int p = (i - 1) / 2;
        if (h->a[p].w <= it.w) break;
        h->a[i] = h->a[p];
        i = p;
    }
    h->a[i] = it;
}

static Item heap_pop(Heap *h) {
    Item top = h->a[0], last = h->a[--h->tam];
    int i = 0;
    while (1) {
        int f = 2 * i + 1;
        if (f >= h->tam) break;
        if (f + 1 < h->tam && h->a[f + 1].w < h->a[f].w) f++;
        if (h->a[f].w >= last.w) break;
        h->a[i] = h->a[f];
        i = f;
    }
    if (h->tam > 0) h->a[i] = last;
    return top;
}

int prim(int n, const Aresta *e, int m, Aresta *agm, double *custo) {
    int *ini = calloc(n + 1, sizeof(int));
    int *pos = malloc((n + 1) * sizeof(int));
    int *viz = malloc((2 * m + 1) * sizeof(int));
    double *pes = malloc((2 * m + 1) * sizeof(double));
    char *vis = calloc(n > 0 ? n : 1, 1);
    Heap h = { malloc((2 * m + 1) * sizeof(Item)), 0 };
    int k = -1;

    if (!ini || !pos || !viz || !pes || !vis || !h.a) goto fim;

    /* adjacência em CSR */
    for (int i = 0; i < m; i++) { ini[e[i].u + 1]++; ini[e[i].v + 1]++; }
    for (int i = 0; i < n; i++) ini[i + 1] += ini[i];
    for (int i = 0; i <= n; i++) pos[i] = ini[i];
    for (int i = 0; i < m; i++) {
        viz[pos[e[i].u]] = e[i].v; pes[pos[e[i].u]++] = e[i].w;
        viz[pos[e[i].v]] = e[i].u; pes[pos[e[i].v]++] = e[i].w;
    }

    k = 0;
    *custo = 0.0;
    if (n > 0) {
        vis[0] = 1;
        for (int j = ini[0]; j < ini[1]; j++)
            heap_push(&h, (Item){ pes[j], 0, viz[j] });
    }
    while (h.tam > 0 && k < n - 1) {
        Item it = heap_pop(&h);
        if (vis[it.v]) continue;
        vis[it.v] = 1;
        agm[k++] = (Aresta){ it.u, it.v, it.w };
        *custo += it.w;
        for (int j = ini[it.v]; j < ini[it.v + 1]; j++)
            if (!vis[viz[j]])
                heap_push(&h, (Item){ pes[j], it.v, viz[j] });
    }
    if (k != n - 1) k = -1;

fim:
    free(ini); free(pos); free(viz); free(pes); free(vis); free(h.a);
    return k;
}

/* Imprime a AGM obtida por um algoritmo (vértices exibidos de 1 a n). */
static void imprimirAGM(const char *titulo, const Aresta *agm, int k, double custo) {

    printf("\n[%s]\n", titulo);

    for (int i = 0; i < k; i++) {
        printf("%d - %d (%g)\n", agm[i].u + 1, agm[i].v + 1, agm[i].w);
    }

    printf("Arestas: %d | Custo total: %.2f\n", k, custo);
}

/* Calcula e imprime a AGM por Kruskal e por Prim. */
void arvoreGeradoraMinima(const Grafo *grafo) {

    if (grafo == NULL || grafo->ordem < 1) {
        printf("Grafo invalido.\n");
        return;
    }

    int n = grafo->ordem;

    if (contarComponentesConexas(grafo) != 1) {
        printf("O grafo nao e conexo: a arvore geradora minima nao esta definida.\n");
        return;
    }

    // Lista de arestas a partir da matriz (cada aresta uma vez, sem laços)
    int m = 0;

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (grafo->matriz[i][j] != 0) {
                m++;
            }
        }
    }

    Aresta *arestas = malloc((m > 0 ? m : 1) * sizeof(Aresta));
    Aresta *agmK = malloc((n > 1 ? n - 1 : 1) * sizeof(Aresta));
    Aresta *agmP = malloc((n > 1 ? n - 1 : 1) * sizeof(Aresta));

    if (arestas == NULL || agmK == NULL || agmP == NULL) {
        printf("Erro ao alocar memoria.\n");
        free(arestas);
        free(agmK);
        free(agmP);
        return;
    }

    int idx = 0;

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (grafo->matriz[i][j] != 0) {
                arestas[idx].u = i;
                arestas[idx].v = j;
                arestas[idx].w = grafo->matriz[i][j];
                idx++;
            }
        }
    }

    double custoK = 0.0, custoP = 0.0;
    int kK = kruskal(n, arestas, m, agmK, &custoK);
    int kP = prim(n, arestas, m, agmP, &custoP);

    if (kK < 0 || kP < 0) {
        printf("Erro ao calcular a arvore geradora minima.\n");
    } else {
        imprimirAGM("Kruskal", agmK, kK, custoK);
        imprimirAGM("Prim", agmP, kP, custoP);

        double dif = custoK - custoP;

        if (dif < 0) {
            dif = -dif;
        }

        printf("\nCustos iguais: %s\n", dif < 1e-6 ? "sim" : "nao");
    }

    free(arestas);
    free(agmK);
    free(agmP);
}

/*
 * excentricidade, diametro e raio.
 * G: matriz, n: quantidade de vértices, inicio: vértice (índice a partir de 0).
 * Retornam DBL_MAX quando o grafo não é conexo.
 */

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

/* Converte a matriz do Grafo (0 = sem aresta) para o formato de metrica:
 * vetor linear com DBL_MAX onde não há aresta e 0 na diagonal. */
static double *matrizParaVetor(const Grafo *grafo) {

    int n = grafo->ordem;
    double *G = malloc((size_t)n * n * sizeof(double));

    if (G == NULL) {
        return NULL;
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) {
                G[i * n + j] = 0.0;
            } else if (grafo->matriz[i][j] != 0) {
                G[i * n + j] = grafo->matriz[i][j];
            } else {
                G[i * n + j] = DBL_MAX;
            }
        }
    }

    return G;
}

/* Compara duas distâncias com tolerância (somas em ordens diferentes
 * podem diferir por erro de arredondamento). */
static int distanciasIguais(double a, double b) {

    double dif = a - b;

    if (dif < 0) {
        dif = -dif;
    }

    double escala = a < 0 ? -a : a;

    return dif <= 1e-9 * (escala > 1.0 ? escala : 1.0);
}

/* Imprime a excentricidade de cada vértice, o raio (com o centro)
 * e o diâmetro (com os vértices periféricos). */
void metricasDistancia(const Grafo *grafo) {

    if (grafo == NULL || grafo->ordem < 1) {
        printf("Grafo invalido.\n");
        return;
    }

    int n = grafo->ordem;

    if (contarComponentesConexas(grafo) != 1) {
        printf("O grafo nao e conexo: excentricidade, raio e diametro nao estao definidos.\n");
        return;
    }

    double *G = matrizParaVetor(grafo);
    double *ecc = malloc(n * sizeof(double));

    if (G == NULL || ecc == NULL) {
        printf("Erro ao alocar memoria.\n");
        free(G);
        free(ecc);
        return;
    }

    // Calcula a excentricidade de cada vértice uma única vez
    double menor = DBL_MAX, maior = 0.0;

    for (int i = 0; i < n; i++) {
        ecc[i] = excentricidade(G, n, i);

        if (ecc[i] < menor) {
            menor = ecc[i];
        }

        if (ecc[i] > maior) {
            maior = ecc[i];
        }
    }

    printf("\nExcentricidade de cada vertice:\n");

    for (int i = 0; i < n; i++) {
        printf("Vertice %d: %.2f\n", i + 1, ecc[i]);
    }

    printf("\nRaio: %.2f\nCentro (vertices de menor excentricidade):", menor);

    for (int i = 0; i < n; i++) {
        if (distanciasIguais(ecc[i], menor)) {
            printf(" %d", i + 1);
        }
    }

    printf("\n\nDiametro: %.2f\nPeriferia (vertices de maior excentricidade):", maior);

    for (int i = 0; i < n; i++) {
        if (distanciasIguais(ecc[i], maior)) {
            printf(" %d", i + 1);
        }
    }

    printf("\n");

    free(G);
    free(ecc);
}