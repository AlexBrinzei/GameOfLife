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

    // citeste parametrii t, n, m, k si matricea initiala din fisier
    int t, n, m, k;
    char **mat;
    read_file(file, &t, &n, &m, &k, &mat);

    // daca task-ul este 1: afiseaza fiecare generatie aplicand regulile standard
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
     
        if (t == 4) {
        // generăm arborele de diferențe
        char **empty = create_empty_matrix(n, m, '+');
        Tree *root   = build_diff_tree(empty, mat, n, m, 0, k);
        // parcurgem și rezolvăm Hamilton pentru fiecare generație
        traverse_hamilton(root, mat, n, m, 0, k, output);
        // eliberăm
        free_tree(root);
        free_matrix(empty, n);
        free_matrix(mat, n);
        fclose(file);
        fclose(output);
        return 0;
    }


    // // BONUS: daca task-ul este 4 => reconstruim generatia 0 din matricea K + stiva
    // if (t == 4)
    // {
    //     // initializam stiva
    //     Generation *stack = NULL;

    //     // citim K linii cu diferentele dintre generatii si le punem in stiva
    //     for (int gen = 1; gen <= k; gen++)
    //     {
            

    //         CellNode *changes = NULL;
    //         int l, c;
    //         // citim perechile pana la sfarsitul liniei
    //         while (fscanf(file, "%d %d", &l, &c) == 2)
    //         {
    //             insert_cell_sorted(&changes, l, c);
    //             int ch = fgetc(file);
    //             if (ch == '\n' || ch == EOF) break;
    //             else ungetc(ch, file);
    //         }

    //         // impingem in stiva
    //         push(&stack, changes);
    //     }

    //     // reconstruim matricea initiala din matricea K si diferentele din stiva
    //     reconstruct_initial_state(mat, &stack);

    //     // afisam rezultatul in fisierul de iesire
    //     display(mat, n, m, output);

    //     free_matrix(mat, n);
    //     fclose(file);
    //     fclose(output);
    //     return 0;
    // }

   
    
    return 0;
}
