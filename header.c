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