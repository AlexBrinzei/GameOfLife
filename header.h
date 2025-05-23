#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXN 100 


// definim o celula cu pozitia ei (linie si coloana)
typedef struct CellNode
{
    int row, col;
    struct CellNode *next;
} CellNode;

// reprezentam o generatie ca o lista de celule modificate
typedef struct Generation
{
    CellNode *cells; // lista de celule active in generatie
    struct Generation *next;
} Generation;

// structura pentru arborele de diferente (task 3)
typedef struct Tree
{
    CellNode *diffs;    // diferentele fata de parinte
    struct Tree *left;  // ramura care aplica regula b
    struct Tree *right; // ramura care aplica regula standard
} Tree;

// aplica regula b pe matrice si returneaza noua matrice generata
char **apply_rule_B(char **mat, int n, int m);

// face o copie a unei matrici
char **copy_matrix(char **mat, int n, int m);

// construieste arborele de diferente intre generatii (pornind de la o matrice initiala goala)
Tree *build_diff_tree(char **prev, char **cur, int n, int m, int depth, int K);

// parcurge arborele in preordine si afiseaza fiecare generatie in fisierul de iesire
void traverse_tree(Tree *root, char **cur, int n, int m, int depth, int K, FILE *output);

// elibereaza memoria ocupata de arbore (inclusiv listele de diferente)
void free_tree(Tree *root);

//creaza matrice initializata cu caracterul fill, in cazul de la task 3 cu '+' pentru celule moarte
char **create_empty_matrix(int n, int m, char fill);

// citeste datele de intrare din fisier
void read_file(FILE *file, int *t, int *n, int *m, int *k, char ***mat);

// aplica regulile jocului pe matrice
int rules(char **mat, int n, int m);

// afiseaza matricea curenta in fisierul de iesire
void display(char **mat, int n, int m, FILE *output);

// elibereaza memoria unei matrici
void free_matrix(char **mat, int n);

// creeaza o celula noua cu pozitia data
CellNode *create_cell_node(int row, int col);

// elibereaza memoria unei liste de celule
void delete_cell_list(CellNode **head);

// verifica daca stiva de generatii este goala
int isEmpty(Generation *top);

// adauga o generatie noua in stiva
void push(Generation **top, CellNode *cellList);

// scoate o generatie din stiva si returneaza lista de celule
CellNode *pop_stack(Generation **top);

// elibereaza toata stiva
void delete_stack(Generation **top);

// insereaza o celula in lista in ordine crescatoare
void insert_cell_sorted(CellNode **head, int row, int col);

// scrie continutul stivei in fisier 
void write_stack_to_file(Generation *top, FILE *f);

// calculeaza diferentele dintre doua generatii
CellNode *gen_differences(char **old_gen, char **new_gen, int n, int m);

// inverseaza ordinea generatiilor din stiva
Generation *reverse_stack(Generation *top);

//bonus task 2
// inverseaza celulele din matrice pe baza unei liste de diferente
void apply_inverse(char **mat, CellNode *changes);

// reconstruieste matricea initiala (generatia 0) din matricea finala si stiva de diferente
void reconstruct_initial_state(char **mat, Generation **stack);



// ================================================
// Task 4: cel mai lung lanț Hamiltonian în graf
// ================================================

// solve_task4_file:
//   N       = număr de noduri în graf
//   adj     = matricea de adiacență NxN
//   coords  = perechi (row,col) pentru fiecare nod global
//   out     = fișierul de ieșire
void solve_task4_file(int N, int adj[][MAXN], int coords[][2], FILE *out);

// traverse_hamilton:
//   parcurge arborele în preordine și la fiecare nod
//   reconstruit curent (cur) în generația depth,
//   construiește graful și rulează Task 4.
void traverse_hamilton(
    struct Tree *root,
    char **cur,
    int n,
    int m,
    int depth,
    int K,
    FILE *out
);
#endif
