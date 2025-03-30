#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//functie de citire din fisier
void read_file(FILE *file, int *n, int *m, int *k, char ***mat)
{
    int test_number;
    fscanf(file, "%d", &test_number);// se citeste, dar nu se foloseste in program 
    fscanf(file, "%d %d", n, m);// citirea numarului de linii si coloane
    fscanf(file, "%d", k);//citirea numarului de generatii

    *mat = (char **)malloc(*n * sizeof(char *));
    if (*mat == NULL)
    {
        printf("Eroare la alocarea dinamica pentru matrice!");
        exit(1);
    }

    for (int i = 0; i < *n; i++)
    {
        (*mat)[i] = (char *)malloc(*m * sizeof(char));
        if ((*mat)[i] == NULL)
        {
            printf("Eroare la alocarea dinamica pentru matrice!");
            exit(1);
        }
    }

    for (int i = 0; i < *n; i++)
    {
        for (int j = 0; j < *m; j++)
        {
            fscanf(file, " %c", &((*mat)[i][j]));
        }
    }
}


//functie de implementare a regulilor
void rules(char **mat, int n, int m)
{
  //initializare matrice auxiliara pentru retinerea matricei modificate
    char **aux = (char **)malloc(n * sizeof(char *));
    if (aux == NULL)
    {
        printf("Eroare la alocarea matricei!");
        exit(1);
    }

    for (int i = 0; i < n; i++)
    {
        aux[i] = (char *)malloc(m * sizeof(char));
        if (aux[i] == NULL)
        {
            printf("Eroare la alocarea coloanei!");
            exit(1);
        }
    }


    //if urile sunt folosite pornind de la o matrice 3x3 dar care poate fi scalat la orice dimensiune
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            int neighbours_alive = 0;

            if (i > 0) {
                if (j > 0 && mat[i - 1][j - 1] == 'X') 
                neighbours_alive++;
                if (mat[i - 1][j] == 'X') 
                neighbours_alive++;
                if (j < m - 1 && mat[i - 1][j + 1] == 'X') 
                neighbours_alive++;
            }
            if (j > 0 && mat[i][j - 1] == 'X') 
            neighbours_alive++;
            if (j < m - 1 && mat[i][j + 1] == 'X') 
            neighbours_alive++;
            if (i < n - 1) {
                if (j > 0 && mat[i + 1][j - 1] == 'X') 
                neighbours_alive++;
                if (mat[i + 1][j] == 'X') 
                neighbours_alive++;
                if (j < m - 1 && mat[i + 1][j + 1] == 'X') 
                neighbours_alive++;
            }

            if (mat[i][j] == 'X') {
                if (neighbours_alive < 2 || neighbours_alive > 3)
                    aux[i][j] = '+';
                else
                    aux[i][j] = 'X';
            } else {
                if (neighbours_alive == 3)
                    aux[i][j] = 'X';
                else
                    aux[i][j] = '+';
            }
        }
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            mat[i][j] = aux[i][j];
        }
        free(aux[i]);
    }
    free(aux);
}


//functie de afisare matricei
void display(char **mat, int n, int m, FILE *output)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            fprintf(output, "%c", mat[i][j]);
        }
        fprintf(output, "\n");
    }
    fprintf(output, "\n");
}

int main(int argc, const char *argv[])
{
    FILE *file = fopen(argv[1], "rt");
    if (!file)
    {
        printf("Eroare la citirea fisierului!\n");
        exit(1);
    }

    FILE *output = fopen(argv[2], "wt");
    if (!output)
    {
        printf("Eroare la scrierea fisierului de iesire!\n");
        exit(1);
    }

    int n, m, k;
    char **mat;
    read_file(file, &n, &m, &k, &mat);


    //afisarea si aplicarea regulilor pentru numarul generatiilor dorite
    for (int gen = 0; gen <= k; gen++)
    {
        display(mat, n, m, output);
        rules(mat, n, m);
    }

    fclose(file);
    fclose(output);


    //eliberarea memoriei matricei
    for (int i = 0; i < n; i++)
    {
        free(mat[i]);
    }
    free(mat);

    return 0;
}