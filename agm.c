#include <stdlib.h>
#include "agm.h"

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
