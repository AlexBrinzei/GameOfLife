#include "header.h"

int main(int argc, const char *argv[])
{
    // deschide fisierul de intrare in modul text read
    FILE *file = fopen(argv[1], "rt");
    if (!file)
    {
        printf("Eroare la deschiderea fisierului de intrare!");
        return 1;
    }

    // deschide fisierul de iesire in modul text write
    FILE *output = fopen(argv[2], "wt");
    if (!output)
    {
        printf("Eroare la deschiderea fisierului de iesire!");
        fclose(file);
        return 1;
    }
    int t, n, m, k;

    // task 2 bonus

    fscanf(file, "%d %d %d %d", &t, &n, &m, &k);
     if (t == 5)
     {

        // alocam matricea si citim matricea finala
        char **mat = malloc(n * sizeof *mat);
        for (int i = 0; i < n; i++)
        {
            mat[i] = malloc(m * sizeof *mat[i]);
            for (int j = 0; j < m; j++)
            {
                fscanf(file, " %c", &mat[i][j]);
            }
        }

        // citim cele k linii de diferente
        Generation *stack = NULL;
        for (int gen = 1; gen <= k; gen++)
        {
            CellNode *changes = NULL;
            int i, j;
            // fiecare pereche i j este separata de spatiu
            while (fscanf(file, "%d %d", &i, &j) == 2)
            {
                insert_cell_sorted(&changes, i, j);
                int c = fgetc(file);
                if (c == '\n' || c == EOF)
                    break;
                ungetc(c, file);
            }
            push(&stack, changes);
        }

        // reconstruim matricea initiala
        reconstruct_initial_state(mat, &stack);
        display(mat, n, m, output);

        // 5) Cleanup
        delete_stack(&stack);
        for (int i = 0; i < n; i++)
            free(mat[i]);
        free(mat);
        return 0;
    }

    char **mat;
    read_file(file, &t, &n, &m, &k, &mat);
    // afiseaza fiecare generatie aplicand regulile standard
    if (t == 1)
    {
        for (int gen = 0; gen <= k; gen++)
        {
            display(mat, n, m, output);
            rules(mat, n, m);
        }

        fclose(file);
        fclose(output);
        free_matrix(mat, n);
        return 0;
    }

    // task 1 bonus de pe teams cu stiva de liste

    // if (t == 1)
    // {
    //     Generation *stack = NULL;

    //     // matricea initiala
    //     char **gen0 = copy_matrix(mat, n, m);
    //     char **prev = copy_matrix(mat, n, m);

    //     // genereaza K diferente si le salveaza in stiva
    //     for (int gen = 1; gen <= k; gen++)
    //     {
    //         rules(mat, n, m); // obtinem generatia urmatoare
    //         CellNode *diff = gen_differences(prev, mat, n, m);
    //         push(&stack, diff);

    //         // copiem mat in prev pentru pasul urmator
    //         for (int i = 0; i < n; i++)
    //             for (int j = 0; j < m; j++)
    //                 prev[i][j] = mat[i][j];
    //     }

    //     // copia primei generatii
    //     char **cur = copy_matrix(gen0, n, m);

    //     Generation *rev = reverse_stack(stack);

    //     // afisarea primei generatii
    //     display(cur, n, m, output);

    //     // reconstruim fiecare generatie de la 1 la K
    //     while (rev)
    //     {
    //         apply_inverse(cur, rev->cells); // aplicam modificarile generatiei urmatoare
    //         display(cur, n, m, output);     // afisam
    //         rev = rev->next;
    //     }

    //     // eliberam memoria
    //     delete_stack(&stack);
    //     free_matrix(gen0, n);
    //     free_matrix(prev, n);
    //     free_matrix(cur, n);
    //     free_matrix(mat, n);
    //     fclose(file);
    //     fclose(output);
    //     return 0;
    // }

    // daca task-ul este 2: salveaza doar diferentele in stiva si le scrie
    if (t == 2)
    {
        Generation *stack = NULL;
        char **prev_gen = copy_matrix(mat, n, m);

        for (int gen = 0; gen <= k; gen++)
        {
            if (gen > 0)
            {
                CellNode *diff = gen_differences(prev_gen, mat, n, m);
                push(&stack, diff);
            }

            for (int i = 0; i < n; i++)
                for (int j = 0; j < m; j++)
                    prev_gen[i][j] = mat[i][j];

            rules(mat, n, m);
        }

        stack = reverse_stack(stack);
        write_stack_to_file(stack, output);

        delete_stack(&stack);
        free_matrix(prev_gen, n);
        free_matrix(mat, n);
        fclose(file);
        fclose(output);
        return 0;
    }

    // daca task-ul este 3: arbore de diferente si preordine
    if (t == 3)
    {
        char **empty = create_empty_matrix(n, m, '+');
        Tree *root = build_diff_tree(empty, mat, n, m, 0, k);
        traverse_tree(root, mat, n, m, 0, k, output);

        free_tree(root);
        free_matrix(empty, n);
        free_matrix(mat, n);
        fclose(file);
        fclose(output);
        return 0;
    }

    if (t == 4)
    {
        // generam arborele de diferente
        char **empty = create_empty_matrix(n, m, '+');
        Tree *root = build_diff_tree(empty, mat, n, m, 0, k);
        // parcurgem si rezolvam Hamilton pentru fiecare generatie
        traverse_hamilton(root, mat, n, m, 0, k, output);
        // eliberam memoria
        free_tree(root);
        free_matrix(empty, n);
        free_matrix(mat, n);
        fclose(file);
        fclose(output);
        return 0;
    }

    return 0;
}
