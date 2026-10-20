/*
 * Projeto 03: Bubble Sort Tradicional vs. Bubble Sort com Interrupção Prematura (Flag)
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 1000
#define REPETICOES 1000   /* repete para obter tempos mensuráveis */

/* Contadores globais de operações (preenchidos pelas funções) */
static long long comparacoes = 0;
static long long trocas = 0;

/* ---------------------------------------------------------------
 * 1) Bubble Sort padrão: laços aninhados fixos, executa
 *    rigidamente todas as iterações, mesmo com o vetor já ordenado.
 * --------------------------------------------------------------- */
void bubble_sort_padrao(int v[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            comparacoes++;
            if (v[j] > v[j + 1]) {
                int tmp = v[j];
                v[j] = v[j + 1];
                v[j + 1] = tmp;
                trocas++;
            }
        }
    }
}

/* ---------------------------------------------------------------
 * 2) Bubble Sort com flag: encerra se uma varredura completa
 *    e termina sem nenhuma troca.
 * --------------------------------------------------------------- */
void bubble_sort_flag(int v[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int houve_troca = 0;                 /* flag de controle */
        for (int j = 0; j < n - 1 - i; j++) {
            comparacoes++;
            if (v[j] > v[j + 1]) {
                int tmp = v[j];
                v[j] = v[j + 1];
                v[j + 1] = tmp;
                trocas++;
                houve_troca = 1;
            }
        }
        if (!houve_troca)                    /* nenhuma troca => já ordenado */
            break;
    }
}

/* ---------------------------------------------------------------
 * Auxiliares
 * --------------------------------------------------------------- */
void gera_ordenado(int v[], int n)   { for (int i = 0; i < n; i++) v[i] = i + 1; }
void gera_decrescente(int v[], int n){ for (int i = 0; i < n; i++) v[i] = n - i; }

int esta_ordenado(const int v[], int n) {
    for (int i = 0; i < n - 1; i++)
        if (v[i] > v[i + 1]) return 0;
    return 1;
}

typedef void (*fn_sort)(int[], int);
typedef void (*fn_gera)(int[], int);

void experimento(const char *nome_cenario, fn_gera gera,
                 const char *nome_algo, fn_sort sort) {
    int v[N];

    /* 1 execução para contar operações */
    gera(v, N);
    comparacoes = 0;
    trocas = 0;
    sort(v, N);
    long long comp = comparacoes, tr = trocas;
    int ok = esta_ordenado(v, N);

    /* várias execuções para medir tempo */
    clock_t ini = clock();
    for (int r = 0; r < REPETICOES; r++) {
        gera(v, N);
        sort(v, N);
    }
    clock_t fim = clock();
    double ms = 1000.0 * (double)(fim - ini) / CLOCKS_PER_SEC / REPETICOES;

    printf("%-12s | %-14s | %10lld | %10lld | %9.4f ms | %s\n",
           nome_cenario, nome_algo, comp, tr, ms, ok ? "OK" : "ERRO");
}

int main(void) {
    printf("n = %d  (tempo = media de %d execucoes)\n\n", N, REPETICOES);
    printf("%-12s | %-14s | %10s | %10s | %12s | Ordenou?\n",
           "Cenario", "Algoritmo", "Compar.", "Trocas", "Tempo");
    printf("-------------+----------------+------------+------------+--------------+---------\n");

    experimento("Ordenado",    gera_ordenado,    "Padrao", bubble_sort_padrao);
    experimento("Ordenado",    gera_ordenado,    "Com flag", bubble_sort_flag);
    experimento("Decrescente", gera_decrescente, "Padrao", bubble_sort_padrao);
    experimento("Decrescente", gera_decrescente, "Com flag", bubble_sort_flag);

    printf("\nReferencias teoricas (n = %d):\n", N);
    printf("  n(n-1)/2 = %d comparacoes (pior caso / versao padrao)\n", N * (N - 1) / 2);
    printf("  n-1      = %d comparacoes (melhor caso com flag)\n", N - 1);
    return 0;
}
