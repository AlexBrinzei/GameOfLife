#include "header.h"

int main(int argc, const char *argv[])
{
    // deschide fisierul de intrare in modul text read
    FILE *file = fopen(argv[1], "rt");
    if (!file)
    {
        // afiseaza eroare daca nu s-a putut deschide fisierul de intrare
        printf("Eroare la deschiderea fisierului de intrare!");
        return 1;
    }

    // deschide fisierul de iesire in modul text write
    FILE *output = fopen(argv[2], "wt");
    if (!output)
    {
        // afiseaza eroare daca nu s-a putut deschide fisierul de iesire
        printf("Eroare la deschiderea fisierului de iesire!");
        fclose(file);
        return 1;
    }

    // citeste parametrii t, n, m, k si matricea initiala din fisier
    int t, n, m, k;
    char **mat;
    read_file(file, &t, &n, &m, &k, &mat);

    // daca task-ul este 1, afiseaza fiecare generatie inclusiv initiala
    if (t == 1)
    {
        // pentru gen de la 0 la k
        for (int gen = 0; gen <= k; gen++)
        {
            // afiseaza matricea curenta
            display(mat, n, m, output);
            // aplica regulile standard pentru urmatoarea generatie
            rules(mat, n, m);
        }

        // inchide fisierele si elibereaza matricea
        fclose(file);
        fclose(output);
        free_matrix(mat, n);
        return 0;
    }

    // daca task-ul este 2, construim o stiva cu diferentele intre generatii
    if (t == 2)
{
    // initializare stiva goala
    Generation *stack = NULL;

    // alocam copia pentru generatia anterioara
    char **prev_gen = malloc(n * sizeof(char *));
    if (!prev_gen) exit(1);
    for (int i = 0; i < n; i++) {
        prev_gen[i] = malloc(m * sizeof(char));
        if (!prev_gen[i]) exit(1);
    }

    // copiere initiala a matricei, element cu element
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            prev_gen[i][j] = mat[i][j];
        }
    }

    for (int gen = 0; gen <= k; gen++)
    {
        if (gen > 0)
        {
            // calculeaza diferentele fata de generatia anterioara
            CellNode *diff = gen_differences(prev_gen, mat, n, m);
            // adauga lista de diferente in stiva
            push(&stack, diff);
        }

        // actualizeaza prev_gen pentru comparatie, element cu element
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                prev_gen[i][j] = mat[i][j];
            }
        }

        // aplica regulile standard pentru generatia urmatoare
        rules(mat, n, m);
    }

    // inverseaza ordinea generatiilor in stiva
    stack = reverse_stack(stack);
    // scrie continutul stivei in fisier
    write_stack_to_file(stack, output);

    // elibereaza memoria utilizata
    delete_stack(&stack);
    free_matrix(prev_gen, n);
    free_matrix(mat, n);
    fclose(file);
    fclose(output);
    return 0;
}


    // daca task-ul este 3, construim un arbore de diferente pana la adancimea k
    if (t == 3) {
        // creeaza matrice goala cu toate celulele moarte 
        char **empty = create_empty_matrix(n, m, '+');

        // construieste arborele de diferente pornind de la matricea empty si cea initiala
        Tree *root = build_diff_tree(empty, mat, n, m, 0, k);

        // parcurge arborele in preordine si afiseaza fiecare generatie
        traverse_tree(root, mat, n, m, 0, k, output);

        // elibereaza memoria de la arborele si matricele folosite
        free_tree(root);
        free_matrix(empty, n);
        free_matrix(mat, n);
        fclose(file);
        fclose(output);
        return 0;
    }

    return 0;
}
