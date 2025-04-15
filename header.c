#include "header.h"



void read_file(FILE *file, int *t, int *n, int *m, int *k, char ***mat)
{
    //t = task number
    fscanf(file, "%d", t);
    fscanf(file, "%d %d", n, m);
    fscanf(file, "%d", k);

    *mat = (char **)malloc(*n * sizeof(char *));
    if (*mat == NULL) {
        printf("Eroare la alocarea dinamica pentru matrice!\n");
        exit(1);
    }

    for (int i = 0; i < *n; i++) {
        (*mat)[i] = (char *)malloc(*m * sizeof(char));
        if ((*mat)[i] == NULL) {
            printf("Eroare la alocarea dinamica pentru matrice!\n");
            exit(1);
        }
    }

    for (int i = 0; i < *n; i++) {
        for (int j = 0; j < *m; j++) {
            fscanf(file, " %c", &((*mat)[i][j]));
        }
    }
}

void rules(char **mat, int n, int m)
{
    char **aux = (char **)malloc(n * sizeof(char *));
    if (aux == NULL) {
        printf("Eroare la alocarea matricei auxiliare!\n");
        exit(1);
    }

    for (int i = 0; i < n; i++) {
        aux[i] = (char *)malloc(m * sizeof(char));
        if (aux[i] == NULL) {
            printf("Eroare la alocarea coloanei auxiliare!\n");
            exit(1);
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            int neighbours_alive = 0;

            if (i > 0) {
                if (j > 0 && mat[i - 1][j - 1] == 'X') neighbours_alive++;
                if (mat[i - 1][j] == 'X') neighbours_alive++;
                if (j < m - 1 && mat[i - 1][j + 1] == 'X') neighbours_alive++;
            }
            if (j > 0 && mat[i][j - 1] == 'X') neighbours_alive++;
            if (j < m - 1 && mat[i][j + 1] == 'X') neighbours_alive++;
            if (i < n - 1) {
                if (j > 0 && mat[i + 1][j - 1] == 'X') neighbours_alive++;
                if (mat[i + 1][j] == 'X') neighbours_alive++;
                if (j < m - 1 && mat[i + 1][j + 1] == 'X') neighbours_alive++;
            }

            if (mat[i][j] == 'X') {
                if (neighbours_alive < 2 || neighbours_alive > 3) {
                    aux[i][j] = '+';
                } else {
                    aux[i][j] = 'X';
                }
            } else {
                if (neighbours_alive == 3) {
                    aux[i][j] = 'X';
                } else {
                    aux[i][j] = '+';
                }
            }
            
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            mat[i][j] = aux[i][j];
        }
        free(aux[i]);
    }
    free(aux);
}
void free_matrix(char **mat, int n)
{
    for (int i = 0; i < n; i++) {
        free(mat[i]);
    }
    free(mat);
}


void display(char **mat, int n, int m, FILE *output)
{
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            fprintf(output, "%c", mat[i][j]);
        }
        fprintf(output, "\n");
    }
    fprintf(output, "\n");
}

//Functii pentru task 2


// Creează un nod nou de celulă
CellNode* create_cell_node(int row, int col) {
    CellNode *new_node = (CellNode*) malloc(sizeof(CellNode));
    new_node->row = row;
    new_node->col = col;
    new_node->next = NULL;
    return new_node;
}

// Adaugă o celulă la sfârșitul listei
void add_cell(CellNode **head, int row, int col) {
    CellNode *new_node = create_cell_node(row, col);
    if (*head == NULL) {
        *head = new_node;
    } else {
        CellNode *temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = new_node;
    }
}


//afisare celulele dintr o generatie sub forma (row, col);
void print_cell_list(CellNode *head) {
    while(head) {
        printf("(%d,%d);", head->row, head->col);
        head = head->next;
    }
}


//elibereaza memoria unei liste de celule modificate
void delete_cell_list(CellNode **head) {
    while(*head) {
        CellNode *temp = *head;
        *head = (*head)->next;
        free(temp);
    }
}


//verifica daca stiva de generatii este goala
int isEmpty(Generation *top)
{
    return top == NULL;
}



void push(Generation **top, CellNode *cellList) {
    Generation *newGen = (Generation*) malloc(sizeof(Generation));
    newGen->cells = cellList;
    newGen->next = *top;
    *top = newGen;
}






CellNode* pop_stack(Generation **top) {
    if (isEmpty(*top)) 
        return NULL;

    Generation *temp = *top;
    CellNode *cells = temp->cells;
    *top = temp->next;
    free(temp);
    return cells;
}

void delete_stack(Generation **top) {
    while (!isEmpty(*top)) {
        CellNode *cells = pop_stack(top);
        delete_cell_list(&cells);
    }
}

Generation* reverse_stack(Generation *top) {
    Generation *prev = NULL, *current = top, *next = NULL;
    while (current != NULL) {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    return prev;
}


void insert_cell_sorted(CellNode **head, int row, int col) {
    CellNode *new_node = create_cell_node(row, col);
   
    
    // Cazul 1: Lista este goală sau noul nod trebuie inserat înaintea capului
    if (*head == NULL || (row < (*head)->row) || (row == (*head)->row && col < (*head)->col)) {
        new_node->next = *head;
        *head = new_node;
        return;
    }

    // Căutăm poziția corectă pentru inserare
    CellNode *current = *head;
    while (current->next != NULL && 
          ((current->next->row < row) || 
           (current->next->row == row && current->next->col < col))) {
        current = current->next;
    }

    // Inserăm noul nod
    new_node->next = current->next;
    current->next = new_node;
}





CellNode* gen_differences(char **old_gen, char **new_gen, int n, int m) {
    CellNode *diffs = NULL;
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            
            
            if(old_gen[i][j] != new_gen[i][j]) {
                
                    insert_cell_sorted(&diffs, i, j); 
                
                
                
            }
        }
    }
    return diffs;
}



//scrie stiva in fisier cu indexul fiecarei generatii 
void write_stack_to_file(Generation *top, FILE *f) {
    int gen_id = 1;
    while (top != NULL) {
        fprintf(f, "%d ", gen_id++);
        CellNode *current = top->cells;
        while (current != NULL) {
            fprintf(f, "%d %d ", current->row, current->col);
            current = current->next;
        }
        fprintf(f, "\n"); // generație nouă pe linie nouă
        top = top->next;
    }
}


