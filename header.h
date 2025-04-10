#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>

// Functie de citire din fisier
void read_file(FILE *file, int *n, int *m, int *k, char ***mat);

// Functie care aplica regulile jocului
void rules(char **mat, int n, int m);

// Functie care afiseaza matricea in fisierul de iesire
void display(char **mat, int n, int m, FILE *output);

// Functie care elibereaza memoria matricei
void free_matrix(char **mat, int n);

#endif 
