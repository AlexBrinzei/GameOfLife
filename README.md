# README

## Descriere generala

Acest proiect implementeaza extensii pentru Game of Life, conform cerintelor din cursul de Proiectarea Algoritmilor. Programul gestioneaza generatii de celule vii si moarte si ofera 4 taskuri distincte, care simuleaza evolutia jocului, comprima diferentele, construieste un arbore si cauta drumuri Hamiltoniene in generatii.

Pentru specificatiile complete:  
https://site-pa.netlify.app/proiecte/game_of_life/

## Rulare si compilare

### Cerinte
- compilator C standard (ex: gcc)
- sistem POSIX (Linux/macOS/WSL)
- fisiere: main.c, header.c, header.h

### Compilare
```
gcc -o game main.c header.c
```

### Executie
```
./game input.txt output.txt
```
- input.txt contine task-ul, dimensiunile, numarul de pasi si matricea initiala
- output.txt va contine rezultatul cerut

## Structura proiectului

| Fisier        | Rol                                                                 |
|---------------|----------------------------------------------------------------------|
| main.c        | gestioneaza fluxul programului: citire input, apeluri, output       |
| header.c      | contine toate functiile principale de procesare si logica           |
| header.h      | declaratii de functii si structuri (Tree, Generation, CellNode etc) |

## Functionalitati / Taskuri

### Task 1: Simulare Game of Life
- Afiseaza toate cele k+1 generatii obtinute prin aplicarea regulilor standard.
- Complexitate: O(k * n * m)

### Task 2: Diferente intre generatii
- Calculeaza diferentele dintre generatii si le salveaza intr-o stiva.
- Outputul contine doar celulele modificate.
- Complexitate: O(k * n * m)

### Task 3: Arbore de diferente
- Construieste un arbore binar al generatiilor, pe k nivele, folosind reguli standard si regula B.
- Parcurge acest arbore in preordine si afiseaza fiecare generatie.
- Numar noduri arbore: 2^k
- Complexitate: O(2^k * n * m)

### Task 4: Drum Hamiltonian
- Pentru fiecare generatie din arbore, construieste un graf si cauta cel mai lung drum Hamiltonian (cu backtracking).
- Alege cel mai lung drum, iar daca sunt mai multe, cel lexicografic minim.
- Complexitate worst case: O(N!) per componenta (dar optimizat cu verificari prealabile)

## Structuri de date

- CellNode – lista de celule active (noduri)
- Generation – stiva cu diferente intre generatii
- Tree – arbore binar cu diferente intre generatii (regula B si standard)
- char **mat – matricea generala de celule

## Functii importante

| Functie               | Descriere                                                                 | Complexitate           |
|-----------------------|---------------------------------------------------------------------------|------------------------|
| read_file             | citeste inputul din fisier                                                | O(n * m)               |
| rules                 | aplica regulile standard Game of Life                                     | O(n * m)               |
| apply_rule_B          | regula alternativa: invie celulele moarte cu 2 vecini                     | O(n * m)               |
| gen_differences       | construieste lista de celule modificate intre doua generatii              | O(n * m)               |
| build_diff_tree       | construieste arborele pe baza regulilor                                   | O(2^k * n * m)         |
| dfs_hamilton          | cauta cel mai lung drum Hamiltonian cu backtracking                       | O(N!) worst case       |
| solve_task4_file      | gestioneaza grafurile si drumurile pe fiecare componenta                  | O(N! + N^2) approx.    |
| find_components       | gaseste componentele conexe cu DFS                                        | O(N^2)                 |
| is_valid_component    | verifica rapid daca o componenta poate avea drum Hamiltonian              | O(N^2)                 |
| find_best_from_all    | incearca toate nodurile din componenta ca start pentru dfs_hamilton       | O(N * N!) worst case   |

## Alte observatii

- programul evita calculele inutile folosind validari prealabile (is_valid_component)
- toate structurile sunt gestionate manual, fara memory leaks
- codul este modular si permite extinderea usoara
- output-ul este consistent, usor de validat automat
