#include "header.h"

// citeste din fisier task-ul, dimensiunile si matricea initiala
void read_file(FILE *file, int *t, int *n, int *m, int *k, char ***mat)
{
    // citim numarul task-ului
    fscanf(file, "%d", t);
    // citim numarul de linii si de coloane
    fscanf(file, "%d %d", n, m);
    // citim numarul de pasi k
    fscanf(file, "%d", k);

    // alocam vectorul de pointeri pentru fiecare linie
    *mat = malloc(*n * sizeof(char *));
    if (*mat == NULL)
    {
        printf("eroare la alocarea dinamica pentru matrice!\n");
        exit(1);
    }

    // alocam fiecare linie a matricei
    for (int i = 0; i < *n; i++)
    {
        (*mat)[i] = malloc(*m * sizeof(char));
        if ((*mat)[i] == NULL)
        {
            printf("eroare la alocarea dinamica pentru matrice!\n");
            exit(1);
        }
    }

    // citim caracter cu caracter matricea
    for (int i = 0; i < *n; i++)
    {
        for (int j = 0; j < *m; j++)
        {
            fscanf(file, " %c", &((*mat)[i][j]));
        }
    }
}

// aplica regulile standard ale jocului si scrie rezultatul inapoi in mat
int rules(char **mat, int n, int m)
{
    // alocam o matrice auxiliara pentru calcul
    char **aux = malloc(n * sizeof(char *));
    if (aux == NULL)
    {
        printf("eroare la alocarea matricei auxiliare!\n");
        exit(1);
    }
    for (int i = 0; i < n; i++)
    {
        aux[i] = malloc(m * sizeof(char));
        if (aux[i] == NULL)
        {
            printf("eroare la alocarea coloanei auxiliare!\n");
            exit(1);
        }
    }

    // iteram fiecare celula si calculam vecinii vii
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            int neighbours_alive = 0;

            // verificam randul de deasupra
            if (i > 0)
            {
                if (j > 0 && mat[i - 1][j - 1] == 'X') neighbours_alive++;
                if (mat[i - 1][j] == 'X')               neighbours_alive++;
                if (j < m - 1 && mat[i - 1][j + 1] == 'X') neighbours_alive++;
            }
            // celula stanga si dreapta
            if (j > 0 && mat[i][j - 1] == 'X')       neighbours_alive++;
            if (j < m - 1 && mat[i][j + 1] == 'X')   neighbours_alive++;
            // verificam randul de sub
            if (i < n - 1)
            {
                if (j > 0 && mat[i + 1][j - 1] == 'X') neighbours_alive++;
                if (mat[i + 1][j] == 'X')               neighbours_alive++;
                if (j < m - 1 && mat[i + 1][j + 1] == 'X') neighbours_alive++;
            }

            // regulile
            if (mat[i][j] == 'X')
            {
                //subpopulare sau suprapopulare
                if (neighbours_alive < 2 || neighbours_alive > 3)
                    aux[i][j] = '+';
                else
                    aux[i][j] = 'X';
            }
            else
            {
                // invie daca are exact 3 vecini
                if (neighbours_alive == 3)
                    aux[i][j] = 'X';
                else
                    aux[i][j] = '+';
            }
        }
    }

    // copiem inapoi rezultatele in matricea initiala si eliberam aux
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            mat[i][j] = aux[i][j];
        }
        free(aux[i]);
    }
    free(aux);

    return 0;
}

// elibereaza memoria alocata pentru matrice
void free_matrix(char **mat, int n)
{
    for (int i = 0; i < n; i++)
        free(mat[i]);
    free(mat);
}

// afiseaza matricea in fisier, fara spatii intre caractere
void display(char **mat, int n, int m, FILE *output)
{
    for (int i = 0; i < n; i++)
    {
        fwrite(mat[i], 1, m, output);
        fputc('\n', output);
    }
    fputc('\n', output); // linie goala intre afisari
}

// creeaza un nod nou pentru lista de celule modificate
CellNode *create_cell_node(int row, int col)
{
    CellNode *new_node = malloc(sizeof(CellNode));
    new_node->row  = row;
    new_node->col  = col;
    new_node->next = NULL;
    return new_node;
}


// elibereaza nodurile din lista de celule
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

// adauga o noua generatie in varful stivei
void push(Generation **top, CellNode *cellList)
{
    Generation *newGen = malloc(sizeof(Generation));
    newGen->cells = cellList;
    newGen->next  = *top;
    *top = newGen;
}

// scoate ultima generatie si returneaza lista de celule
CellNode *pop_stack(Generation **top)
{
    if (isEmpty(*top)) return NULL;
    Generation *temp = *top;
    CellNode *cells = temp->cells;
    *top = temp->next;
    free(temp);
    return cells;
}

// elibereaza intreaga stiva de generatii
void delete_stack(Generation **top)
{
    while (!isEmpty(*top))
    {
        CellNode *cells = pop_stack(top);
        delete_cell_list(&cells);
    }
}

// inverseaza ordinea elementelor din stiva
Generation *reverse_stack(Generation *top)
{
    Generation *prev = NULL, *cur = top, *next = NULL;
    while (cur)
    {
        next = cur->next;
        cur->next = prev;
        prev = cur;
        cur = next;
    }
    return prev;
}

// insereaza un nod in lista sortata dupa (row,col)
void insert_cell_sorted(CellNode **head, int row, int col)
{
    CellNode *new_node = create_cell_node(row, col);

    // daca e cap de lista sau mai mic decat primul
    if (*head == NULL ||
        row  < (*head)->row ||
        (row == (*head)->row && col < (*head)->col))
    {
        new_node->next = *head;
        *head = new_node;
        return;
    }

    // cautam pozitia corecta
    CellNode *cur = *head;
    while (cur->next &&
           (cur->next->row  < row ||
            (cur->next->row == row && cur->next->col < col)))
    {
        cur = cur->next;
    }
    new_node->next = cur->next;
    cur->next      = new_node;
}

// genereaza lista de diferente intre doua matrici
CellNode *gen_differences(char **old_gen, char **new_gen, int n, int m)
{
    CellNode *diffs = NULL;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            if (old_gen[i][j] != new_gen[i][j])
                insert_cell_sorted(&diffs, i, j);
        }
    }
    return diffs;
}

// scrie in fisier continutul stivei cu index de generatie si coordonate
void write_stack_to_file(Generation *top, FILE *f)
{
    int gen_id = 1;
    while (top)
    {
        // afisam id-ul generatiei
        fprintf(f, "%d", gen_id++);

        // afisam fiecare pereche (row,col)
        CellNode *cur = top->cells;
        while (cur)
        {
            fprintf(f, " %d %d", cur->row, cur->col);
            cur = cur->next;
        }

        fprintf(f, "\n");
        top = top->next;
    }
}

// aplica regula b: o celula moarta invie daca are exact 2 vecini
char **apply_rule_B(char **mat, int n, int m)
{
    // alocam matrice auxiliara
    char **copy = malloc(n * sizeof(char *));
    if (!copy) exit(1);
    for (int i = 0; i < n; i++) {
        copy[i] = malloc(m * sizeof(char));
        if (!copy[i]) exit(1);
    }

    // parcurgem fiecare celula
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            int cnt = 0;

            // randul de deasupra
            if (i > 0) {
                if (j > 0 && mat[i-1][j-1] == 'X') cnt++;
                if (    mat[i-1][j]   == 'X') cnt++;
                if (j < m-1 && mat[i-1][j+1] == 'X') cnt++;
            }
            // stanga si dreapta
            if (j > 0   && mat[i][j-1] == 'X') cnt++;
            if (j < m-1 && mat[i][j+1] == 'X') cnt++;
            // randul de sub
            if (i < n-1) {
                if (j > 0 && mat[i+1][j-1] == 'X') cnt++;
                if (    mat[i+1][j]   == 'X') cnt++;
                if (j < m-1 && mat[i+1][j+1] == 'X') cnt++;
            }

            // aplicam regula b
            if (mat[i][j] == '+') {
                // celula moarta invie daca are exact 2 vecini vii
                if (cnt == 2)
                    copy[i][j] = 'X';
                else
                    copy[i][j] = '+';
            } else {
                // daca e vie, ramane vie
                copy[i][j] = 'X';
            }
        }
    }

    return copy;
}


// copiaza matricea si returneaza un pointer nou
char **copy_matrix(char **mat, int n, int m)
{
    // alocam vectorul de pointeri pentru cele n linii
    char **c = malloc(n * sizeof(char *));
    if (!c) return NULL;

    for (int i = 0; i < n; i++)
    {
        // alocam fiecare linie
        c[i] = malloc(m * sizeof(char));
        if (!c[i]) {
            for (int k = 0; k < i; k++)
                free(c[k]);
            free(c);
            return NULL;
        }
        for (int j = 0; j < m; j++)
        {
            c[i][j] = mat[i][j];
        }
    }

    return c;
}


// construieste recursiv arborele de diferente
Tree *build_diff_tree(char **prev, char **cur, int n, int m, int depth, int K)
{
    Tree *node = malloc(sizeof(Tree));
    node->diffs = gen_differences(prev, cur, n, m);
    node->left  = NULL;
    node->right = NULL;

    if (depth < K)
    {
        // regula b pe ramura stanga
        char **matB = apply_rule_B(cur, n, m);
        node->left = build_diff_tree(cur, matB, n, m, depth + 1, K);
        free_matrix(matB, n);

        // regula standard pe ramura dreapta
        char **matS = copy_matrix(cur, n, m);
        rules(matS, n, m);
        node->right = build_diff_tree(cur, matS, n, m, depth + 1, K);
        free_matrix(matS, n);
    }
    return node;
}

// parcurge arborele in preordine si afiseaza fiecare generatia
void traverse_tree(Tree *root, char **cur, int n, int m, int depth, int K, FILE *output)
{
    if (!root) return;

    // afisam matricea curenta
    display(cur, n, m, output);

    if (depth < K)
    {
        // stanga: regula b
        char **matB = apply_rule_B(cur, n, m);
        traverse_tree(root->left, matB, n, m, depth + 1, K, output);
        free_matrix(matB, n);

        // dreapta: reguli standard
        char **matS = copy_matrix(cur, n, m);
        rules(matS, n, m);
        traverse_tree(root->right, matS, n, m, depth + 1, K, output);
        free_matrix(matS, n);
    }
}

// elibereaza tot arborele cu diferente si listele de celule
void free_tree(Tree *root)
{
    if (!root) return;
    free_tree(root->left);
    free_tree(root->right);
    delete_cell_list(&root->diffs);
    free(root);
}



// creeaza o matrice cu caracterul fill
char **create_empty_matrix(int n, int m, char fill)
{
    char **mat = malloc(n * sizeof(char *));
    if (!mat) return NULL;

    for (int i = 0; i < n; i++) {
        mat[i] = malloc(m * sizeof(char));
        if (!mat[i]) {
            for (int k = 0; k < i; k++)
                free(mat[k]);
            free(mat);
            return NULL;
        }
        // initializam manual fiecare element cu fill
        for (int j = 0; j < m; j++) {
            mat[i][j] = fill;
        }
    }
    return mat;
}

//functii bonus task2
// inverseaza celulele din matrice pe baza unei liste de diferente
void apply_inverse(char **mat, CellNode *changes)
{
    while (changes)
    {
        int i = changes->row;
        int j = changes->col;
        mat[i][j] = (mat[i][j] == 'X') ? '+' : 'X'; // inversam starea
        changes = changes->next;
    }
}

// reconstruieste matricea initiala (generatia 0) din matricea finala si stiva de diferente
void reconstruct_initial_state(char **mat, Generation **stack)
{
    while (!isEmpty(*stack))
    {
        CellNode *changes = pop_stack(stack); // scoatem ultima generatie
        apply_inverse(mat, changes);          // aplicam inversul diferentei
        delete_cell_list(&changes);           // eliberam memoria listei
    }
}


// //task 4
// static int best_len;
// static int path_stack[100];
// static int best_path[100];
// static int gN;
// static int (*gadj)[100];
// static int visited_local[100];

// // DFS backtracking pe graful global gN/gadj
// static void dfs_bt(int u, int depth) {
//     path_stack[depth-1] = u;
//     if (depth > best_len) {
//         best_len = depth;
//         // copiem doar prefixul
//         int i;
//         for (i = 0; i < depth; i++) best_path[i] = path_stack[i];
//     }
//     // dacă am găsit lungime maximă, oprim
//     if (best_len == gN) return;
//     // parcurgem vecinii în ordine crescătoare a indexului
//     for (u = 0; u < gN; u++) {
//         if (gadj[path_stack[depth-1]][u] && !visited_local[u]) {
//             visited_local[u] = 1;
//             dfs_bt(u, depth+1);
//             visited_local[u] = 0;
//         }
//     }
// }

// initializează gN/gadj și pornește DFS de pe fiecare nod
// void solve_task4_file(int N, int adj[][100], FILE *out) {
//     gN = N;
//     gadj = adj;
//     best_len = 0;
//     int i;
//     for (i = 0; i < N; i++) visited_local[i] = 0;
//     for (i = 0; i < N; i++) {
//         visited_local[i] = 1;
//         dfs_bt(i, 1);
//         visited_local[i] = 0;
//         if (best_len == N) break;
//     }
//     // scriem rezultatul
//     if (best_len <= 0) {
//         fprintf(out, "-1\n");
//     } else {
//         fprintf(out, "%d\n", best_len);
//         for (i = 0; i < best_len; i++) {
//             int u = best_path[i];
//             // afișăm ca (linie,coloană) = (u,u)
//             fprintf(out, "(%d,%d) ", u, u);
//         }
//         fprintf(out, "\n");
//     }
// }


//aproape afiseaza bine 
// void display_changes(CellNode *changes, int gen_id, FILE *out)
// {
//     // 1. tipărim id-ul generației
//     fprintf(out, "%d\n", gen_id);

//     // 2. pentru fiecare nod din lista de changes
//     for (CellNode *cur = changes; cur; cur = cur->next) {
//         fprintf(out, "(%d,%d) ", cur->row, cur->col);
//     }

//     // 3. terminăm linia
//     fprintf(out, "\n");
// }


// // ----------------------------------------------
// // 2) Traverse Hamilton dar apelând acum display_changes
// // ----------------------------------------------
// void traverse_hamilton(
//     Tree    *root,
//     char   **cur,
//     int      n,
//     int      m,
//     int      depth,  // vom folosi depth ca gen_id
//     int      k,
//     FILE    *out
// ) {
//     if (!root) return;

//     // 1) afișăm diferențele de la acest nod, cu "gen_id" = depth
//     display_changes(root->diffs, depth, out);

//     // 2) dacă nu am ajuns la adâncimea K, continuăm
//     if (depth < k) {
//         // regula B
//         char **matB = apply_rule_B(cur, n, m);
//         traverse_hamilton(root->left,  matB, n, m, depth+1, k, out);
//         free_matrix(matB, n);

//         // regula standard
//         char **matS = copy_matrix(cur, n, m);
//         rules(matS, n, m);
//         traverse_hamilton(root->right, matS, n, m, depth+1, k, out);
//         free_matrix(matS, n);
//     }
// }

// Helper DFS pentru Hamiltonian (backtracking cu tie-break lexicografic)
static int best_len;
static int path_stack[MAXN];
static int best_path[MAXN];
static int gN;
static int gadj[MAXN][MAXN];
static int visited_local[MAXN];
static int best_coords[MAXN][2], temp_coords[MAXN][2];

// compara două lanțuri de coordonate lexicografic
static int lex_less(int a[][2], int b[][2], int len) {
    for (int i = 0; i < len; i++) {
        if (a[i][0] != b[i][0]) return a[i][0] < b[i][0];
        if (a[i][1] != b[i][1]) return a[i][1] < b[i][1];
    }
    return 0;
}


static void dfs_bt(int u, int depth) {
    path_stack[depth-1] = u;
    if (depth > best_len) {
        best_len = depth;
        memcpy(best_path, path_stack, depth * sizeof(int));
        // salvăm și coordonatele
        for (int i = 0; i < depth; i++)
            memcpy(best_coords[i], temp_coords[path_stack[i]], 2 * sizeof(int));
    } else if (depth == best_len) {
        // tie-break lexicografic
        int candidate[MAXN][2];
        for (int i = 0; i < depth; i++)
            memcpy(candidate[i], temp_coords[path_stack[i]], 2 * sizeof(int));
        if (lex_less(candidate, best_coords, depth)) {
            for (int i = 0; i < depth; i++)
                memcpy(best_coords[i], candidate[i], 2 * sizeof(int));
        }
    }
    if (best_len == gN) return;  // optim prăjit

    for (int v = 0; v < gN; v++) {
        if (gadj[u][v] && !visited_local[v]) {
            visited_local[v] = 1;
            dfs_bt(v, depth+1);
            visited_local[v] = 0;
        }
    }
}

// void solve_task4_file(int N, int adj[][MAXN], int coords[][2], FILE *out) {
//     // 1) găsim componente conexe prin DFS simplu
//     int comp_id[MAXN] = {0}, comp_cnt = 0;
//     int seen[MAXN] = {0}, stack[MAXN], top;
//     for (int i = 0; i < N; i++) {
//         if (!seen[i]) {
//             top = 0; stack[top++] = i;
//             seen[i] = 1;
//             comp_id[i] = comp_cnt;
//             while (top) {
//                 int u = stack[--top];
//                 for (int v = 0; v < N; v++) if (adj[u][v] && !seen[v]) {
//                     seen[v] = 1;
//                     comp_id[v] = comp_cnt;
//                     stack[top++] = v;
//                 }
//             }
//             comp_cnt++;
//         }
//     }

//     // 2) pentru fiecare componentă, extragem subgraf și coordonate
//     int best_overall = -1, best_comp = -1;
//     int best_output[MAXN][2], best_output_len = 0;
//     for (int c = 0; c < comp_cnt; c++) {
//         // colecționăm nodurile componentei
//         int idx = 0, map_local[MAXN];
//         for (int i = 0; i < N; i++)
//             if (comp_id[i] == c) map_local[i] = idx++;
//         gN = idx;
//         if (gN == 0) continue;

//         // construim matrice locală și coordonate locale
//         memset(gadj, 0, sizeof(gadj));
//         for (int i = 0; i < N; i++) if (comp_id[i] == c) {
//             int u = map_local[i];
//             memcpy(temp_coords[u], coords[i], 2*sizeof(int));
//             for (int j = 0; j < N; j++) if (comp_id[j] == c && adj[i][j]) {
//                 int v = map_local[j];
//                 gadj[u][v] = 1;
//             }
//         }

//         // Verificăm gradul nodurilor pentru excluderea componentelor imposibile
//         int degree[MAXN] = {0};
//         int deg_1_count = 0;
//         int isolated_node = 0;
//         for (int u = 0; u < gN; u++) {
//             int deg = 0;
//             for (int v = 0; v < gN; v++)
//                 deg += gadj[u][v];
//             degree[u] = deg;
//             if (deg == 1) deg_1_count++;
//             if (deg == 0) isolated_node = 1;
//         }
//         if (isolated_node || deg_1_count > 2) {
//             // Componenta nu poate avea lanț Hamiltonian complet
//             continue;
//         }

//         // backtracking pe fiecare nod
//         best_len = 0;
//         memset(visited_local, 0, sizeof(visited_local));
//         for (int i = 0; i < gN; i++) {
//             visited_local[i] = 1;
//             dfs_bt(i, 1);
//             visited_local[i] = 0;
//             if (best_len == gN) break;
//         }

//         // comparăm cu soluția globală
//         if (best_len > best_overall) {
//             best_overall = best_len;
//             best_comp = c;
//             best_output_len = best_len;
//             memcpy(best_output, best_coords, best_len*sizeof(best_coords[0]));
//         } else if (best_len == best_overall && best_len > 0) {
//             if (lex_less(best_coords, best_output, best_len)) {
//                 best_output_len = best_len;
//                 memcpy(best_output, best_coords, best_len*sizeof(best_coords[0]));
//             }
//         }
//     }

//     // 3) tipărim rezultatul
//     if (best_overall <= 0)
//     {
//         if (N == 1)
//         {
//             // cazul: o singură celulă vie = lanț de 1 nod, 0 muchii
//             fprintf(out, "0\n");
//             fprintf(out, "(%d,%d)\n", coords[0][0] + 1, coords[0][1] + 1);
//         }
//         else
//         {
//             // poate fi mai multe componente dar fără lanț complet
//             int all_isolated = 1;
//             for (int i = 0; i < N; i++)
//             {
//                 int has_neighbor = 0;
//                 for (int j = 0; j < N; j++)
//                 {
//                     if (adj[i][j])
//                     {
//                         has_neighbor = 1;
//                         break;
//                     }
//                 }
//                 if (has_neighbor)
//                 {
//                     all_isolated = 0;
//                     break;
//                 }
//             }

//             if (all_isolated && N == 1)
//             {
//                 fprintf(out, "0\n");
//                 fprintf(out, "(%d,%d)\n", coords[0][0] + 1, coords[0][1] + 1);
//             }
//             else
//             {
//                 fprintf(out, "-1\n");
//             }
//         }
//     }
//     else
//     {
//         // lungimea e numărul de muchii (noduri - 1)
//         fprintf(out, "%d\n", best_overall - 1);
//         for (int i = 0; i < best_output_len; i++)
//         {
//             fprintf(out, "(%d,%d)%c",
//                     best_output[i][0],
//                     best_output[i][1],
//                     (i + 1 < best_output_len ? ' ' : '\n'));
//         }
//     }
// }

void solve_task4_file(int N, int adj[][MAXN], int coords[][2], FILE *out) {
    int comp_id[MAXN], comp_cnt = 0;
    for (int i = 0; i < N; i++) comp_id[i] = -1;

    for (int i = 0; i < N; i++) {
        if (comp_id[i] != -1) continue;

        int has_edge = 0;
        for (int j = 0; j < N; j++) {
            if (adj[i][j]) {
                has_edge = 1;
                break;
            }
        }

        if (!has_edge) {
            comp_id[i] = comp_cnt++;
            continue;
        }

        int stack[MAXN], top = 0;
        stack[top++] = i;
        comp_id[i] = comp_cnt;
        while (top) {
            int u = stack[--top];
            for (int v = 0; v < N; v++) {
                if (adj[u][v] && comp_id[v] == -1) {
                    comp_id[v] = comp_cnt;
                    stack[top++] = v;
                }
            }
        }
        comp_cnt++;
    }

    int best_overall = -1;
    int best_output[MAXN][2], best_output_len = 0;

    for (int c = 0; c < comp_cnt; c++) {
        int map_global_to_local[MAXN], map_local_to_global[MAXN], idx = 0;
        for (int i = 0; i < N; i++) {
            if (comp_id[i] == c) {
                map_global_to_local[i] = idx;
                map_local_to_global[idx] = i;
                idx++;
            }
        }

        gN = idx;
        if (gN == 0) continue;

        memset(gadj, 0, sizeof(gadj));
        for (int i = 0; i < gN; i++) {
            int gi = map_local_to_global[i];
            temp_coords[i][0] = coords[gi][0];
            temp_coords[i][1] = coords[gi][1];

            for (int j = 0; j < gN; j++) {
                int gj = map_local_to_global[j];
                if (adj[gi][gj]) {
                    gadj[i][j] = 1;
                }
            }
        }

        int isolated = 0, deg1 = 0;
        for (int u = 0; u < gN; u++) {
            int deg = 0;
            for (int v = 0; v < gN; v++) deg += gadj[u][v];
            if (deg == 0) isolated = 1;
            if (deg == 1) deg1++;
        }

        if (gN > 1 && (isolated || deg1 > 2)) continue;

        best_len = 0;
        memset(visited_local, 0, sizeof(visited_local));
        for (int i = 0; i < gN; i++) {
            visited_local[i] = 1;
            dfs_bt(i, 1);
            visited_local[i] = 0;
            if (best_len == gN) break;
        }
        if (best_len < gN) continue;  // ignoră dacă lanțul nu e complet Hamiltonian


        if (best_len > best_overall) {
            best_overall = best_len;
            best_output_len = best_len;
            memcpy(best_output, best_coords, sizeof(best_coords));
        } else if (best_len == best_overall) {
            if (lex_less(best_coords, best_output, best_len)) {
                best_output_len = best_len;
                memcpy(best_output, best_coords, sizeof(best_coords));
            }
        }
    }

    if (best_overall <= 0) {
        if (N == 1) {
            fprintf(out, "0\n");
            fprintf(out, "(%d,%d)\n", coords[0][0] + 1, coords[0][1] + 1);
        } else {
            int all_isolated = 1;
            for (int i = 0; i < N; i++) {
                int has_neighbor = 0;
                for (int j = 0; j < N; j++) {
                    if (adj[i][j]) {
                        has_neighbor = 1;
                        break;
                    }
                }
                if (has_neighbor) {
                    all_isolated = 0;
                    break;
                }
            }

            if (all_isolated && N == 1) {
                fprintf(out, "0\n");
                fprintf(out, "(%d,%d)\n", coords[0][0] + 1, coords[0][1] + 1);
            } else {
                fprintf(out, "-1\n");
            }
        }
    } else {
        fprintf(out, "%d\n", best_overall - 1);
        for (int i = 0; i < best_output_len; i++) {
            fprintf(out, "(%d,%d)%c",
                best_output[i][0],
                best_output[i][1],
                (i + 1 < best_output_len ? ' ' : '\n'));
        }
    }
}




// traverse_hamilton: reconstruiește din matricea curentă graful și apelează solve_task4_file
void traverse_hamilton(
    Tree *root,
    char **cur,
    int n,
    int m,
    int depth,
    int K,
    FILE *out
) {
    if (!root) return;

    // 1) colectăm coordonatele celulelor vii 'X'
    int coords[MAXN][2], id_map[100][100], N = 0;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            id_map[i][j] = -1;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (cur[i][j] == 'X') {
                id_map[i][j] = N;
                coords[N][0] = i;
                coords[N][1] = j;
                N++;
            }
        }
    }

    // 2) construim matricea de adiacență locale
    static int adj_loc[MAXN][MAXN];
    memset(adj_loc, 0, sizeof(adj_loc));
    for (int i = 0; i < n; i++) for (int j = 0; j < m; j++) if (id_map[i][j] >= 0) {
        int u = id_map[i][j];
        for (int di = -1; di <= 1; di++) for (int dj = -1; dj <= 1; dj++) {
            if (di==0 && dj==0) continue;
            int ni = i+di, nj = j+dj;
            if (ni>=0 && ni<n && nj>=0 && nj<m && id_map[ni][nj]>=0) {
                int v = id_map[ni][nj];
                adj_loc[u][v] = 1;
            }
        }
    }

    // 3) apelăm solver pe acest graf
    solve_task4_file(N, adj_loc, coords, out);

    // 4) recursivitate
    if (depth < K) {
        // regula B
        char **matB = apply_rule_B(cur, n, m);
        traverse_hamilton(root->left, matB, n, m, depth+1, K, out);
        free_matrix(matB, n);
        // regula standard
        char **matS = copy_matrix(cur, n, m);
        rules(matS, n, m);
        traverse_hamilton(root->right, matS, n, m, depth+1, K, out);
        free_matrix(matS, n);
    }
}