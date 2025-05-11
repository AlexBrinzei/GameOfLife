#include "header.h"

void read_file(FILE *file, int *t, int *n, int *m, int *k, char ***mat)
{
    // t = task number
    fscanf(file, "%d", t);
    fscanf(file, "%d %d", n, m);
    fscanf(file, "%d", k);

    *mat = (char **)malloc(*n * sizeof(char *));
    if (*mat == NULL)
    {
        printf("Eroare la alocarea dinamica pentru matrice!\n");
        exit(1);
    }

    for (int i = 0; i < *n; i++)
    {
        (*mat)[i] = (char *)malloc(*m * sizeof(char));
        if ((*mat)[i] == NULL)
        {
            printf("Eroare la alocarea dinamica pentru matrice!\n");
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

// int rules(char **mat, int n, int m)
// {
//     char **aux = (char **)malloc(n * sizeof(char *));
//     if (aux == NULL)
//     {
//         printf("Eroare la alocarea matricei auxiliare!\n");
//         exit(1);
//     }

//     for (int i = 0; i < n; i++)
//     {
//         aux[i] = (char *)malloc(m * sizeof(char));
//         if (aux[i] == NULL)
//         {
//             printf("Eroare la alocarea coloanei auxiliare!\n");
//             exit(1);
//         }
//     }

//     for (int i = 0; i < n; i++)
//     {
//         for (int j = 0; j < m; j++)
//         {
//             int neighbours_alive = 0;

//             if (i > 0)
//             {
//                 if (j > 0 && mat[i - 1][j - 1] == 'X')
//                     neighbours_alive++;
//                 if (mat[i - 1][j] == 'X')
//                     neighbours_alive++;
//                 if (j < m - 1 && mat[i - 1][j + 1] == 'X')
//                     neighbours_alive++;
//             }
//             if (j > 0 && mat[i][j - 1] == 'X')
//                 neighbours_alive++;
//             if (j < m - 1 && mat[i][j + 1] == 'X')
//                 neighbours_alive++;
//             if (i < n - 1)
//             {
//                 if (j > 0 && mat[i + 1][j - 1] == 'X')
//                     neighbours_alive++;
//                 if (mat[i + 1][j] == 'X')
//                     neighbours_alive++;
//                 if (j < m - 1 && mat[i + 1][j + 1] == 'X')
//                     neighbours_alive++;
//             }

//             if (mat[i][j] == 'X')
//             {
//                 if (neighbours_alive < 2 || neighbours_alive > 3)
//                 {
//                     aux[i][j] = '+';
//                 }
//                 else
//                 {
//                     aux[i][j] = 'X';
//                 }
//             }
//             else
//             {
//                 if (neighbours_alive == 3)
//                 {
//                     aux[i][j] = 'X';
//                 }
//                 else
//                 {
//                     aux[i][j] = '+';
//                 }
//             }
//         }
//     }

//     for (int i = 0; i < n; i++)
//     {
//         for (int j = 0; j < m; j++)
//         {
//             mat[i][j] = aux[i][j];
//         }
//         free(aux[i]);
//     }
//     free(aux);
//     return 0;
// }

int rules(char **mat, int n, int m) {
    char **aux = malloc(n * sizeof(char*));  // matricea auxiliară pentru a salva rezultatele
    for (int i = 0; i < n; i++) {
        aux[i] = malloc(m);
        for (int j = 0; j < m; j++) {
            int cnt = 0;  // numărăm vecinii vii

            // Parcurgem vecinii pentru fiecare celulă
            for (int di = -1; di <= 1; di++) {
                for (int dj = -1; dj <= 1; dj++) {
                    if (di == 0 && dj == 0) continue;  // să nu numărăm celula curentă
                    int ni = i + di, nj = j + dj;
                    if (ni >= 0 && ni < n && nj >= 0 && nj < m && mat[ni][nj] == 'X')
                        cnt++;  // creștem numărul de vecini vii
                }
            }

            // Aplicăm regulile jocului:
            if (mat[i][j] == 'X') {
                // Celulă vie rămâne vie dacă are 2 sau 3 vecini vii
                aux[i][j] = (cnt == 2 || cnt == 3) ? 'X' : '+';  // rămâne vie sau moare
            } else {
                // Celulă moartă devine vie doar dacă are exact 3 vecini vii
                aux[i][j] = (cnt == 3) ? 'X' : '+';  // devine vie sau rămâne moartă
            }
        }
    }

    // Copiem rezultatele în matricea originală
    for (int i = 0; i < n; i++) {
        memcpy(mat[i], aux[i], m);
        free(aux[i]);
    }
    free(aux);
    return 0;
}





void free_matrix(char **mat, int n)
{
    for (int i = 0; i < n; i++)
    {
        free(mat[i]);
    }
    free(mat);
}

void display(char **mat, int n, int m, FILE *output) {
    for (int i = 0; i < n; i++) {
        fwrite(mat[i], 1, m, output);  // scrie fix m caractere, fără spații
        fputc('\n', output);            // newline după fiecare linie
    }
    fputc('\n', output);               // linie goală între matrici
}



// Functii pentru task 2

// Creează un nod nou de celulă
CellNode *create_cell_node(int row, int col)
{
    CellNode *new_node = (CellNode *)malloc(sizeof(CellNode));
    new_node->row = row;
    new_node->col = col;
    new_node->next = NULL;
    return new_node;
}

// Adaugă o celulă la sfârșitul listei
void add_cell(CellNode **head, int row, int col)
{
    CellNode *new_node = create_cell_node(row, col);
    if (*head == NULL)
    {
        *head = new_node;
    }
    else
    {
        CellNode *temp = *head;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = new_node;
    }
}

// afisare celulele dintr o generatie sub forma (row, col);
void print_cell_list(CellNode *head)
{
    while (head)
    {
        printf("(%d,%d);", head->row, head->col);
        head = head->next;
    }
}

// elibereaza memoria unei liste de celule modificate
void delete_cell_list(CellNode **head)
{
    while (*head)
    {
        CellNode *temp = *head;
        *head = (*head)->next;
        free(temp);
    }
}

// verifica daca stiva de generatii este goala
int isEmpty(Generation *top)
{
    return top == NULL;
}

void push(Generation **top, CellNode *cellList)
{
    Generation *newGen = (Generation *)malloc(sizeof(Generation));
    newGen->cells = cellList;
    newGen->next = *top;
    *top = newGen;
}

CellNode *pop_stack(Generation **top)
{
    if (isEmpty(*top))
        return NULL;

    Generation *temp = *top;
    CellNode *cells = temp->cells;
    *top = temp->next;
    free(temp);
    return cells;
}

void delete_stack(Generation **top)
{
    while (!isEmpty(*top))
    {
        CellNode *cells = pop_stack(top);
        delete_cell_list(&cells);
    }
}

Generation *reverse_stack(Generation *top)
{
    Generation *prev = NULL, *current = top, *next = NULL;
    while (current != NULL)
    {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    return prev;
}

void insert_cell_sorted(CellNode **head, int row, int col)
{
    CellNode *new_node = create_cell_node(row, col);

    // Cazul 1: Lista este goală sau noul nod trebuie inserat înaintea capului
    if (*head == NULL || (row < (*head)->row) || (row == (*head)->row && col < (*head)->col))
    {
        new_node->next = *head;
        *head = new_node;
        return;
    }

    // Căutăm poziția corectă pentru inserare
    CellNode *current = *head;
    while (current->next != NULL &&
           ((current->next->row < row) ||
            (current->next->row == row && current->next->col < col)))
    {
        current = current->next;
    }

    // Inserăm noul nod
    new_node->next = current->next;
    current->next = new_node;
}

CellNode *gen_differences(char **old_gen, char **new_gen, int n, int m)
{
    CellNode *diffs = NULL;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {

            if (old_gen[i][j] != new_gen[i][j])
            {

                insert_cell_sorted(&diffs, i, j);
            }
        }
    }
    return diffs;
}

// scrie stiva in fisier cu indexul fiecarei generatii
void write_stack_to_file(Generation *top, FILE *f)
{
    int gen_id = 1;
    while (top != NULL)
    {
        // 1) indexul generaţiei drept câte două coloane, aliniat la dreapta
        fprintf(f, "%d", gen_id++);

        CellNode *current = top->cells;
        while (current != NULL)
        {
            // 2) fiecare pereche (row,col) tot în câte două coloane,
            //    cu un spațiu înainte ca separator
            fprintf(f, " %d %d",
                    current->row,
                    current->col);
            current = current->next;
        }

        // 3) newline după fiecare linie
        fprintf(f, "\n");
        top = top->next;
    }
}





// === implementare apply_rule_B ===
char **apply_rule_B(char **matrice, int n, int m) {
    char **copie = malloc(n * sizeof(char *));  // Alocăm matricea auxiliară
    for (int i = 0; i < n; i++) {
        copie[i] = malloc(m);  // Alocăm fiecare rând

        for (int j = 0; j < m; j++) {
            int nr_vecini = 0;  // Inițializăm numărul de vecini vii

            // Calculăm numărul de vecini vii pentru fiecare celulă
            for (int di = -1; di <= 1; di++) {
                for (int dj = -1; dj <= 1; dj++) {
                    if (di == 0 && dj == 0) continue;  // Să nu numărăm celula curentă
                    int ni = i + di, nj = j + dj;
                    if (ni >= 0 && ni < n && nj >= 0 && nj < m && matrice[ni][nj] == 'X') {
                        nr_vecini++;  // Creștem numărul de vecini vii
                    }
                }
            }

            // Noua regulă: celula moartă devine vie dacă are exact 2 vecini vii
            if (matrice[i][j] == '+' && nr_vecini == 2) {
                copie[i][j] = 'X';  // Celula moartă devine vie
            } else {
                copie[i][j] = matrice[i][j];  // Altfel, păstrează starea actuală
            }
        }
    }

    return copie;  // Returnează matricea modificată
}








// === utilitară pentru copiere matrix ===
char **copy_matrix(char **mat, int n, int m)
{
    char **c = malloc(n * sizeof(char *));
    if (!c)
        return NULL;
    for (int i = 0; i < n; i++)
    {
        c[i] = malloc(m * sizeof(char));
        memcpy(c[i], mat[i], m * sizeof(char));
    }
    return c;
}

// diffs (folosim gen_differences din Task 2)

// === construcție arbore de diffs ===
// Funcție pentru a construi arborele de diferențe
Tree *build_diff_tree(char **prev, char **cur, int n, int m, int depth, int K) {
    Tree *node = malloc(sizeof(Tree));  // Alocă un nod pentru arbore
    node->diffs = gen_differences(prev, cur, n, m);  // Salvează diferențele între generații
    node->left = node->right = NULL;  // Inițializăm ramurile stânga și dreapta ca NULL

    if (depth < K) {
        // Aplicația regulii B pentru ramura stângă
        char **matB = apply_rule_B(cur, n, m);  // Aplică regula B
        node->left = build_diff_tree(cur, matB, n, m, depth + 1, K);  // Apel recursiv pentru ramura stângă
        free_matrix(matB, n);

        // Aplicația regulii standard pentru ramura dreaptă
        char **matS = copy_matrix(cur, n, m);  // Copiază matricea curentă
        rules(matS, n, m);  // Aplică regulile standard
        node->right = build_diff_tree(cur, matS, n, m, depth + 1, K);  // Apel recursiv pentru ramura dreaptă
        free_matrix(matS, n);
    }

    return node;  // Returnează nodul construit
}




// === parcurgere preordine ===
void traverse_tree(Tree *root, char **cur, int n, int m, int depth, int K, FILE *output) {
    if (!root) return;

    // Afișăm generația curentă
    display(cur, n, m, output);  // Afișează matricea curentă

    // Continuăm să parcurgem arborele pe ramura stângă (cu regula B) și dreapta (cu regula standard)
    if (depth < K) {
        // Regula B pe stânga
        char **matB = apply_rule_B(cur, n, m);
        traverse_tree(root->left, matB, n, m, depth + 1, K, output);  // Recursivitate pe ramura stângă
        free_matrix(matB, n);

        // Regula standard pe dreapta
        char **matS = copy_matrix(cur, n, m);
        rules(matS, n, m);  // Aplică regulile standard
        traverse_tree(root->right, matS, n, m, depth + 1, K, output);  // Recursivitate pe ramura dreaptă
        free_matrix(matS, n);
    }
}






// === eliberare arbore ===
void free_tree(Tree *root)
{
    if (!root)
        return;
    free_tree(root->left);
    free_tree(root->right);
    delete_cell_list(&root->diffs);
    free(root);
}
