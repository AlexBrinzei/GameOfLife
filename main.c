#include "header.h"

int main(int argc, const char *argv[])
{

    FILE *file = fopen(argv[1], "rt");
    if (!file)
    {
        printf("Eroare la deschiderea fisierului de intrare!");
        return 1;
    }

    FILE *output = fopen(argv[2], "wt");
    if (!output)
    {
        printf("Eroare la deschiderea fisierului de iesire!");
        fclose(file);
        return 1;
    }

    int t, n, m, k;
    char **mat;
    read_file(file, &t, &n, &m, &k, &mat);

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
    }

    // if(t == 2)
    // {

    // // Declaram stiva care va tine diferentele intre generatii
    //  Generation *stack = NULL;

    //  // Alocam o copie a matricei curente pentru comparatie
    //  char **prev_gen =(char**) malloc(n * sizeof(char*));
    //  for (int i = 0; i < n; i++) {
    //      prev_gen[i] = (char*)malloc(m * sizeof(char));
    //      for (int j = 0; j < m; j++)
    //          prev_gen[i][j] = mat[i][j];
    //  }

    //  for (int gen = 0; gen <= k; gen++) {
    //      // Afisam generatia curenta

    //      if (gen > 0) {
    //          // Comparatie intre generatia anterioara si cea curenta
    //          CellNode *diff = gen_differences(prev_gen, mat, n, m);
    //          push(&stack, diff);
    //      }

    //      // Salvam matricea curenta in prev_gen inainte sa o modificam
    //      for (int i = 0; i < n; i++)
    //          for (int j = 0; j < m; j++)
    //              prev_gen[i][j] = mat[i][j];

    //      // Aplicam regulile jocului pentru urmatoarea generatie
    //      rules(mat, n, m);
    //  }

    //  stack = reverse_stack(stack);
    //  write_stack_to_file(stack, output);

    //  // Curatam memoria
    //  delete_stack(&stack);
    //  free_matrix(mat, n);
    //  free_matrix(prev_gen, n);
    //  fclose(file);
    //  fclose(output);
    // }

    if (t == 2)
    {
        Generation *stack = NULL;
        char **prev_gen = malloc(n * sizeof(char *));
        for (int i = 0; i < n; i++)
            prev_gen[i] = malloc(m * sizeof(char));

        for (int i = 0; i < n; i++)
            memcpy(prev_gen[i], mat[i], m);

        for (int gen = 0; gen <= k; gen++)
        {
            if (gen > 0)
            {
                CellNode *diff = gen_differences(prev_gen, mat, n, m);
                push(&stack, diff);
            }
            for (int i = 0; i < n; i++)
                memcpy(prev_gen[i], mat[i], m);

            rules(mat, n, m);
        }

        stack = reverse_stack(stack);
        write_stack_to_file(stack, output);

        delete_stack(&stack);
        free_matrix(prev_gen, n);
        free_matrix(mat, n);
        fclose(file);
        fclose(output);
    }

    if (t == 3) {
        // Matrice empty cu toate '+' (nimic viu)
        char **empty = malloc(n * sizeof(char*));
        for (int i = 0; i < n; i++) {
            empty[i] = malloc(m);
            memset(empty[i], '+', m);  // toate celulele moarte
        }
    
        Tree *root = build_diff_tree(empty, mat, n, m, 0, k);
        
        // Aplicăm regula B de n ori
        for (int i = 0; i < n; i++) {
            char **matB = apply_rule_B(mat, n, m);
            traverse_tree(root, matB, n, m, 0, k, output);
            free_matrix(matB, n);
        }

        free_tree(root);
        free_matrix(empty, n);
        free_matrix(mat, n);
        fclose(file);
        fclose(output);
        return 0;
    }


    return 0;
}
