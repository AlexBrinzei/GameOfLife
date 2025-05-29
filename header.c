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

// afiseaza matricea in fisier, fara spatii intre caractere
void display(char **mat, int n, int m, FILE *output)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            fputc(mat[i][j], output); // NU %d, NU 0/1, doar caracter
        }
        fputc('\n', output);
    }
    fputc('\n', output);
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
                if (j > 0 && mat[i - 1][j - 1] == 'X')
                    neighbours_alive++;
                if (mat[i - 1][j] == 'X')
                    neighbours_alive++;
                if (j < m - 1 && mat[i - 1][j + 1] == 'X')
                    neighbours_alive++;
            }
            // celula stanga si dreapta
            if (j > 0 && mat[i][j - 1] == 'X')
                neighbours_alive++;
            if (j < m - 1 && mat[i][j + 1] == 'X')
                neighbours_alive++;
            // verificam randul de sub
            if (i < n - 1)
            {
                if (j > 0 && mat[i + 1][j - 1] == 'X')
                    neighbours_alive++;
                if (mat[i + 1][j] == 'X')
                    neighbours_alive++;
                if (j < m - 1 && mat[i + 1][j + 1] == 'X')
                    neighbours_alive++;
            }

            // regulile
            if (mat[i][j] == 'X')
            {
                // subpopulare sau suprapopulare
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

// elibereaza memoria alocata pentru matrice
void free_matrix(char **mat, int n)
{
    for (int i = 0; i < n; i++)
        free(mat[i]);
    free(mat);
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

        if (mat[i][j] == 'X')
            mat[i][j] = '+';
        else
            mat[i][j] = 'X';

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

// task 4

// compara doua drumuri dupa coordonate
int is_smaller(int a[][2], int b[][2], int length)
{
    for (int i = 0; i < length; i++)
    {
        if (a[i][0] != b[i][0])
            return a[i][0] < b[i][0];
        if (a[i][1] != b[i][1])
            return a[i][1] < b[i][1];
    }
    return 0;
}

// dfs cu backtracking pentru a construi lantul cel mai lung (idee din curs 9, DFS recursiv)
void dfs_hamilton(
    int node, //index de la drum
    int depth, // lungimea curenta a drumului
    int total, // numarul total de noduri din componenta
    int adj[100][100], //matrice de adiacenta 
    int visited[100], //vector pentru nodurile deja incluse in graf
    int path[100], // stocare temporara a indicilor nodurilor
    int coords[100][2], // coordonatele nodurilor
    int *best_len, // lungimea celui mai bun drum gasit la un moment dat
    int best_coords[100][2], // coordonatele celui mai bun drum gasit pana la un moment dat
    int nodes[100] //vector pentru indexii nodurilor
) {
    path[depth - 1] = node;// adaugam nodul 

    if (depth > *best_len) {
        *best_len = depth;// actualizam best_len la depth
        for (int i = 0; i < depth; i++) {
            best_coords[i][0] = coords[path[i]][0];//copiez path la best_coords
            best_coords[i][1] = coords[path[i]][1];
        }
    } else if (depth == *best_len) {// daca sunt egale le adauga la best_coords dupa ce le compara
        int temp[100][2];
        for (int i = 0; i < depth; i++) {
            temp[i][0] = coords[path[i]][0];
            temp[i][1] = coords[path[i]][1];
        }
        if (is_smaller(temp, best_coords, depth)) {
            for (int i = 0; i < depth; i++) {
                best_coords[i][0] = temp[i][0];
                best_coords[i][1] = temp[i][1];
            }
        }
    }

    if (*best_len == total) return;// marcheaza ca a gasit cel mai lung lant

    for (int next = 0; next < total; next++) {
        // in graf pentru fiecare vecin next nevizitat, il marchez vizitat si apelez recursiv
        if (!visited[next] && adj[nodes[node]][nodes[next]]) {
            visited[next] = 1;
            dfs_hamilton(next, depth + 1, total, adj, visited, path, coords, best_len, best_coords, nodes);
            visited[next] = 0;// dupa apel marchez nevizitat
        }
    }
}


// marcheaza fiecare componenta conexa folosind dfs cu stiva (idee din curs 9)
void find_components(int n, int adj[100][100], int comp[100], int *comp_count) {
    for (int i = 0; i < n; i++) 
        comp[i] = -1;
    *comp_count = 0;

    for (int i = 0; i < n; i++) {
        if (comp[i] != -1) continue;

        int stack[100], top = 0;
        stack[top++] = i;
        comp[i] = *comp_count;

        // dfs iterativ: parcurgem componenta
        while (top > 0) {
            int node = stack[--top];
            for (int j = 0; j < n; j++) {
                if (adj[node][j] && comp[j] == -1) {
                    comp[j] = *comp_count;
                    stack[top++] = j;
                }
            }
        }

        (*comp_count)++;// se incrementeaza de fiecare data cand incepe o componenta noua
    }
}

// verifica daca o componenta poate avea drum hamiltonian (idee din curs 8)
int is_valid_component(int size, int adj[100][100], int nodes[100]) {
    int deg1 = 0, isolated = 0;

    for (int i = 0; i < size; i++) {
        int deg = 0;
        for (int j = 0; j < size; j++) {
            if (adj[nodes[i]][nodes[j]]) deg++;
        }

        if (deg == 0) isolated = 1;
        if (deg == 1) deg1++;
    }

    // conditia din curs: maxim doua noduri de grad 1, niciun nod izolat
    return !(size > 1 && (isolated || deg1 > 2));
}

// incearca fiecare nod din componenta ca punct de start pentru dfs_hamilton
void find_best_from_all(int size, int nodes[100], int adj[100][100], int coords[100][2], int *best_len, int best_coords[100][2]) {
    int visited[100] = {0}, path[100];

    int temp_coords[100][2];
    for (int i = 0; i < size; i++) {
        temp_coords[i][0] = coords[nodes[i]][0];
        temp_coords[i][1] = coords[nodes[i]][1];
    }

    for (int i = 0; i < size; i++) {
        visited[i] = 1;

        // apelam dfs cu logica ca indexii sunt relativi (0..size-1), dar accesam adj[nodes[i]][nodes[j]]
        dfs_hamilton(i, 1, size, adj, visited, path, temp_coords, best_len, best_coords, nodes);

        visited[i] = 0;
        if (*best_len == size) break;
    }
}


// afiseaza rezultatul final: lungimea drumului si coordonatele
void print_result(int n, int adj[100][100], int coords[100][2], int path[100][2], int path_len, FILE *out) {
    if (path_len <= 0) {
        // verificam daca sunt noduri complet izolate
        int found = 0;
        for (int i = 0; i < n && !found; i++)
            for (int j = 0; j < n; j++)
                if (adj[i][j]) found = 1;

        if (!found && n == 1) {
            fprintf(out, "0\n(%d,%d)\n", coords[0][0] + 1, coords[0][1] + 1);
        } else {
            fprintf(out, "-1\n");
        }
    } else {
        fprintf(out, "%d\n", path_len - 1);
        for (int i = 0; i < path_len; i++) {
            if (i + 1 < path_len)
                fprintf(out, "(%d,%d) ", path[i][0], path[i][1]);
            else
                fprintf(out, "(%d,%d)\n", path[i][0], path[i][1]);
        }
    }
}

// functia principala care cauta cel mai lung drum hamiltonian in oricare componenta
void solve_task4_file(int n, int adj[100][100], int coords[100][2], FILE *out) {
    int comp[100], comp_count;
    find_components(n, adj, comp, &comp_count);

    int max_len = -1;
    int best_path[100][2];
    int best_path_len = 0;

    for (int c = 0; c < comp_count; c++) {
        int nodes[100], count = 0;

        // extragem nodurile din componenta curenta
        for (int i = 0; i < n; i++)
            if (comp[i] == c)
                nodes[count++] = i;

        if (count == 0) continue;
        if (!is_valid_component(count, adj, nodes)) continue;

        int temp_coords[100][2], temp_len = 0;

        // gasim cel mai bun drum din componenta
        find_best_from_all(count, nodes, adj, coords, &temp_len, temp_coords);
        if (temp_len < count) continue;

        // actualizam drumul daca este mai lung sau mai mic 
        if (temp_len > max_len) {
            max_len = temp_len;
            best_path_len = temp_len;
            for (int i = 0; i < temp_len; i++) {
                best_path[i][0] = temp_coords[i][0];
                best_path[i][1] = temp_coords[i][1];
            }
        } else if (temp_len == max_len) {
            if (is_smaller(temp_coords, best_path, temp_len)) {
                for (int i = 0; i < temp_len; i++) {
                    best_path[i][0] = temp_coords[i][0];
                    best_path[i][1] = temp_coords[i][1];
                }
            }
        }
    }

    print_result(n, adj, coords, best_path, best_path_len, out);
}


// reconstruieste din matricea curenta graful și apeleaza solve_task4_file
void traverse_hamilton(Tree *root, char **cur, int n, int m, int depth, int K, FILE *out)
{
    if (!root)
        return;

    int coords[100][2];
    int N = 0;

    // colectam coordonatele celulelor vii (idee curs 8: indexare directa pe matrice)
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            if (cur[i][j] == 'X')
            {
                coords[N][0] = i;
                coords[N][1] = j;
                N++;
            }
        }
    }

    int adj[100][100];

    // construim matricea de adiacenta comparand direct coordonatele (idee din curs 8, pag 18)
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (i == j)
            {
                adj[i][j] = 0;
            }
            else
            {
                if ((coords[i][0] - coords[j][0]) >= -1 && (coords[i][0] - coords[j][0]) <= 1 && (coords[i][1] - coords[j][1]) >= -1 && (coords[i][1] - coords[j][1]) <= 1)
                {
                    adj[i][j] = 1;
                }
                else
                {
                    adj[i][j] = 0;
                }
            }
        }
    }

    // apelam functia principala care cauta drumul hamiltonian
    solve_task4_file(N, adj, coords, out);

    // aplicam regulile doar daca nu am ajuns la adancimea maxima
    if (depth < K)
    {
        char **matB = apply_rule_B(cur, n, m);
        traverse_hamilton(root->left, matB, n, m, depth + 1, K, out);
        free_matrix(matB, n);

        char **matS = copy_matrix(cur, n, m);
        rules(matS, n, m);
        traverse_hamilton(root->right, matS, n, m, depth + 1, K, out);
        free_matrix(matS, n);
    }
}
