#include <stdio.h>
#include <stdlib.h>
#include "header.h"

void read_file(FILE *file, int *n, int *m, int *k, char ***mat)
{
    int test_number;
    fscanf(file, "%d", &test_number);
    fscanf(file, "%d %d", n, m);
    fscanf(file, "%d", k);

    *mat = (char **)malloc(*n * sizeof(char *));
    if (*mat == NULL) {
        printf("Eroare la alocarea dinamica pentru matrice!\n");
        exit(1);
    }

    for (int i = 0; i < *n; i++) {
        (*mat)[i] = (char *)malloc(*m * sizeof(char));
        if ((*mat)[i] == NULL) {
            printf("Eroare la alocarea dinamica pentru matrice!\n");
            exit(1);
        }
    }

    for (int i = 0; i < *n; i++) {
        for (int j = 0; j < *m; j++) {
            fscanf(file, " %c", &((*mat)[i][j]));
        }
    }
}

void rules(char **mat, int n, int m)
{
    char **aux = (char **)malloc(n * sizeof(char *));
    if (aux == NULL) {
        printf("Eroare la alocarea matricei auxiliare!\n");
        exit(1);
    }

    for (int i = 0; i < n; i++) {
        aux[i] = (char *)malloc(m * sizeof(char));
        if (aux[i] == NULL) {
            printf("Eroare la alocarea coloanei auxiliare!\n");
            exit(1);
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            int neighbours_alive = 0;

            if (i > 0) {
                if (j > 0 && mat[i - 1][j - 1] == 'X') neighbours_alive++;
                if (mat[i - 1][j] == 'X') neighbours_alive++;
                if (j < m - 1 && mat[i - 1][j + 1] == 'X') neighbours_alive++;
            }
            if (j > 0 && mat[i][j - 1] == 'X') neighbours_alive++;
            if (j < m - 1 && mat[i][j + 1] == 'X') neighbours_alive++;
            if (i < n - 1) {
                if (j > 0 && mat[i + 1][j - 1] == 'X') neighbours_alive++;
                if (mat[i + 1][j] == 'X') neighbours_alive++;
                if (j < m - 1 && mat[i + 1][j + 1] == 'X') neighbours_alive++;
            }

            if (mat[i][j] == 'X') {
                if (neighbours_alive < 2 || neighbours_alive > 3) {
                    aux[i][j] = '+';
                } else {
                    aux[i][j] = 'X';
                }
            } else {
                if (neighbours_alive == 3) {
                    aux[i][j] = 'X';
                } else {
                    aux[i][j] = '+';
                }
            }
            
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            mat[i][j] = aux[i][j];
        }
        free(aux[i]);
    }
    free(aux);
}

void display(char **mat, int n, int m, FILE *output)
{
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            fprintf(output, "%c", mat[i][j]);
        }
        fprintf(output, "\n");
    }
    fprintf(output, "\n");
}

void free_matrix(char **mat, int n)
{
    for (int i = 0; i < n; i++) {
        free(mat[i]);
    }
    free(mat);
}
