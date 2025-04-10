#include <stdio.h>
#include <stdlib.h>
#include "header.h"

int main(int argc, const char *argv[])
{

    FILE *file = fopen(argv[1], "rt");
    if (!file) {
        printf("Eroare la deschiderea fisierului de intrare!\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "wt");
    if (!output) {
        printf("Eroare la deschiderea fisierului de iesire!\n");
        fclose(file);
        return 1;
    }

    int n, m, k;
    char **mat;

    read_file(file, &n, &m, &k, &mat);

    for (int gen = 0; gen <= k; gen++) {
        display(mat, n, m, output);
        rules(mat, n, m);
    }

    fclose(file);
    fclose(output);
    free_matrix(mat, n);

    return 0;
}
