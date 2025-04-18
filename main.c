#include "header.h"

int main(int argc, const char *argv[])
{

    FILE *file = fopen(argv[1], "rt");
    if (!file) {
        printf("Eroare la deschiderea fisierului de intrare!");
        return 1;
    }

    FILE *output = fopen(argv[2], "wt");
    if (!output) {
        printf("Eroare la deschiderea fisierului de iesire!");
        fclose(file);
        return 1;
    }

    int t, n, m, k;
    char **mat;
    read_file(file, &t, &n, &m, &k, &mat);



    if(t == 1)
    {
        for (int gen = 0; gen <= k; gen++) {
            display(mat, n, m, output);
            rules(mat, n, m);
        }
    
        fclose(file);
        fclose(output);
        free_matrix(mat, n);
    }



    if(t == 2)
    {

    // Declaram stiva care va tine diferentele intre generatii
     Generation *stack = NULL;

     // Alocam o copie a matricei curente pentru comparatie
     char **prev_gen =(char**) malloc(n * sizeof(char*));
     for (int i = 0; i < n; i++) {
         prev_gen[i] = (char*)malloc(m * sizeof(char));
         for (int j = 0; j < m; j++)
             prev_gen[i][j] = mat[i][j];
     }
 
     for (int gen = 0; gen <= k; gen++) {
         // Afisam generatia curenta

         
 
         if (gen > 0) {
             // Comparatie intre generatia anterioara si cea curenta
             CellNode *diff = gen_differences(prev_gen, mat, n, m);
             push(&stack, diff);
         }
 
         // Salvam matricea curenta in prev_gen inainte sa o modificam
         for (int i = 0; i < n; i++)
             for (int j = 0; j < m; j++)
                 prev_gen[i][j] = mat[i][j];
 
         // Aplicam regulile jocului pentru urmatoarea generatie
         rules(mat, n, m);
     }
 
     stack = reverse_stack(stack);
     write_stack_to_file(stack, output);
     
 
     // Curatam memoria
     delete_stack(&stack);
     free_matrix(mat, n);
     free_matrix(prev_gen, n);
     fclose(file);
     fclose(output);
    }
 
     return 0;
 }
