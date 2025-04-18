#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>




//o celula reprezentata de linie si coloana
typedef struct CellNode
{
    int row, col;
    struct CellNode *next;
}CellNode;

typedef struct Generation
{
    CellNode *cells; //lista de celule modificate
    struct Generation *next;
}Generation;


// Functie de citire din fisier
void read_file(FILE *file, int *t, int *n, int *m, int *k, char ***mat);

// Functie care aplica regulile jocului
void rules(char **mat, int n, int m);

// Functie care afiseaza matricea in fisierul de iesire
void display(char **mat, int n, int m, FILE *output);

// Functie care elibereaza memoria matricei
void free_matrix(char **mat, int n);

//Functie pentru creare celula

CellNode* create_cell_node(int row, int col);
void add_cell(CellNode **head, int row, int col);
void print_cell_list(CellNode *head);
void delete_cell_list(CellNode **head);
int isEmpty(Generation *top);
void push(Generation **top, CellNode *cellList);
void print_stack(Generation *top, FILE *fout);
CellNode* pop_stack(Generation **top);
void delete_stack(Generation **top);

// Functii pentru lucrul cu stiva de generatii
void insert_cell_sorted(CellNode **head, int row, int col);
void write_stack_to_file(Generation *top, FILE* f);
// Functie care returneaza diferentele dintre doua generatii
CellNode* gen_differences(char** old_gen, char** new_gen, int n, int m);
Generation* reverse_stack(Generation *top);

#endif 
