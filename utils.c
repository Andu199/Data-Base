/* BOLDIȘOR Dragoș-Alexandru - 312CB */

#include "utils.h"

/* Ștergere linii din tabel pentru fiecare tip */

void clearIntLines(t_table *t)
{
    t_intLine *line = (t_intLine *) t->lines, *auxl;
    t_intCell *cell, *auxc;
    while(line != NULL)
    {
        cell = line->cells;
        while(cell != NULL)
        {
            auxc = cell;
            cell = cell->next;
            free(auxc);
        }
        auxl = line;
        line = line->next;
        free(auxl);
    }
    t->lines = NULL;
}

void clearFloatLines(t_table *t)
{
    t_floatLine *line = (t_floatLine *) t->lines, *auxl;
    t_floatCell *cell, *auxc;
    while(line != NULL)
    {
        cell = line->cells;
        while(cell != NULL)
        {
            auxc = cell;
            cell = cell->next;
            free(auxc);
        }
        auxl = line;
        line = line->next;
        free(auxl);
    }
    t->lines = NULL;
}

void clearStringLines(t_table *t)
{
    t_stringLine *line = (t_stringLine *) t->lines, *auxl;
    t_stringCell *cell, *auxc;
    while(line != NULL)
    {
        cell = line->cells;
        while(cell != NULL)
        {
            auxc = cell;
            cell = cell->next;
            free(auxc->value);
            free(auxc);
        }
        auxl = line;
        line = line->next;
        free(auxl);
    }
    t->lines = NULL;
}

/*   ---------   */

t_db *initDB(char *name) // inițializare bază de date
{
    t_db *d;
    d = (t_db *) malloc(sizeof(t_db));
    if(!d)
    {
        fprintf(stderr, "ERROR!");
        return NULL;
    }
    strcpy(d->name, name);
    d->tables = NULL;
    return d;
}

void deleteTable(t_db *d, char *name) /* ștergere tabel - folosit pentru a
localiza tabelul în baza de date și scoaterea acestuia din lista de tabele,
urmând ca funcțiile pentru ștergere de linii specifice pentru fiecare tip să
fie apelate în funcție de caz. Ștergea coloanelor se face la final separat. */
{
    t_table *t, *ant, *aux = NULL;
    if(!d->tables)
    {
        printf("Table \"%s\" not found in database.\n", name);
        return;
    }
    if(strcmp(d->tables->name, name) == 0)
    {
        aux = d->tables;
        d->tables = d->tables->next;
    }
    for(ant = NULL, t = d->tables; t != NULL; ant = t, t = t->next)
    {
        if(strcmp(t->name, name) == 0)
        {
            aux = t;
            ant->next = t->next;
            break;
        }
    }
    if(!aux)
    {
        printf("Table \"%s\" not found in database.\n", name);
        return;
    }
    if(aux->type == INT)
        clearIntLines(aux);
    else if(aux->type == FLOAT)
        clearFloatLines(aux);
    else
        clearStringLines(aux);
    t_column *auxcol, *col = aux->columns;
    while(col != NULL)
    {
        auxcol = col;
        col = col->next;
        free(auxcol);
    }
    free(aux);
}

void deleteDB(t_db **d) // ștergere bază de date
{
    if((*d)->tables)
    {
        t_table *t, *aux;
        t = (*d)->tables;
        while(t != NULL)
        {
            aux = t;
            t = t->next;
            if(aux->type == INT)
                clearIntLines(aux);
            else if(aux->type == FLOAT)
                clearFloatLines(aux);
            else
                clearStringLines(aux);
            t_column *auxcol, *col = aux->columns;
            while(col != NULL)
            {
                auxcol = col;
                col = col->next;
                free(auxcol);
            }
            free(aux);
        }
    }
    free(*d);
    *d = NULL;
}

t_table *findTable(t_db *d, char *name) /* funcție auxiliară de găsit tabelul
după nume într-o bază de date */
{
    t_table *t = d->tables;
    while(t != NULL)
    {
        if(strcmp(t->name, name) == 0)
            break;
        t = t->next;
    }
    return t;
}

int findColumn(t_table *t, char *name) /* funcție auxiliară care întoarce
poziția pe care se află coloana cu numele primit ca parametru în tabelul t
sau 0 dacă nu există coloana în tabel */
{
    int poz = 0;
    t_column *col = t->columns;
    while(col)
    {
        poz++;
        if(strcmp(col->name, name) == 0)
            return poz;
        col = col->next;
    }
    return 0;
}

int findRelation(char *string) /* recunoaște relația și întoarce un cod pentru
fiecare tip */
{
    if(strcmp(string, "<") == 0)
        return 1;
    else if(strcmp(string, "<=") == 0)
        return 2;
    else if(strcmp(string, "==") == 0)
        return 3;
    else if(strcmp(string, "!=") == 0)
        return 4;
    else if(strcmp(string, ">=") == 0)
        return 5;
    else if(strcmp(string, ">") == 0)
        return 6;
    return 0;
}

int Digits(int nr) // întoarce numărul de cifre din componența întregului
{
    if(nr == 0)
        return 1;
    int count = 0;
    while(nr)
    {
        nr /= 10;
        count++;
    }
    return count;
}

void createNewTable(t_db *d, char *buffer) /* crează un tabel nou luând
informațiile necesare din buffer-ul dat ca parametru */
{
    t_table *aux = malloc(sizeof(t_table));
    if(!aux)
    {
        fprintf(stderr, "ERROR!");
        return;
    }
    char *token;
    token = strtok(buffer, " ");
    token = strtok(NULL, " ");
    t_table *t = findTable(d, token);
    if(t)
    {
        printf("Table \"%s\" already exists.\n", token);
        free(aux);
        return;
    }
    strcpy(aux->name, token);
    token = strtok(NULL, " ");
    if(strcmp(token, "INT") == 0)
        aux->type = INT;
    else if(strcmp(token, "FLOAT") == 0)
        aux->type = FLOAT;
    else if(strcmp(token, "STRING") == 0)
        aux->type = STRING;
    else
    {
        printf("Unknown data type: \"%s\".\n", token);
        free(aux);
        return;
    }
    aux->columns = NULL;
    aux->lines = NULL;
    aux->next = NULL;
    t = d->tables;
    if(t != NULL){
        for(; t->next != NULL; t = t->next)
        {}
        t->next = aux;
    }
    else
        d->tables = aux;
    token = strtok(NULL, " ");
    t_column *vf = NULL;
    while(token != NULL)
    {
        t_column *colaux = malloc(sizeof(t_column));
        if(!colaux)
        {
            while(aux->columns != NULL)
            {
                vf = aux->columns;
                aux->columns = aux->columns->next;
                free(vf);
            }
            free(aux);
            fprintf(stderr, "ERROR!");
            return;
        }
        strcpy(colaux->name, token);
        colaux->next = NULL;
        if(aux->columns == NULL){
            aux->columns = colaux;
            vf = aux->columns;
        }
        else
        {
            vf->next = colaux;
            vf = vf->next;
        }      
        token = strtok(NULL, " ");
    }
}

/* Afișare linii din tabelă pentru fiecare tip */

void printIntLine(t_intCell *cell)
{
    int dif, i;
    while(cell != NULL)
    {
        dif = MAX_COLUMN_NAME_LEN - Digits(cell->value) + 1; /* numărul de
        spații cu care trebuie completată linia pentru afișare */
        printf("%d", cell->value);
        for(i = 0; i < dif; ++i)
            printf(" ");
        cell = cell->next;
    }
    printf("\n");
}

void printIntTable(t_table *t)
{
    t_intLine *line = (t_intLine *)t->lines;
    while(line != NULL)
    {
        printIntLine(line->cells);
        line = line->next;
    }
}

void printFloatLine(t_floatCell *cell)
{
    int dif, i, aux;
    while(cell != NULL)
    {
        aux = (int) cell->value; // transformare în int pentru funcția Digits
        dif = MAX_COLUMN_NAME_LEN - Digits(aux) - 6; /* față de cazul unui
        întreg, pentru a afișa un float se folosesc în plus 7 caractere: un
        punct și 6 zecimale. */
        printf("%f", cell->value);
        for(i = 0; i < dif; ++i)
            printf(" ");
        cell = cell->next;
    }
    printf("\n");
}

void printFloatTable(t_table *t)
{
    t_floatLine *line = (t_floatLine *)t->lines;
    while(line != NULL)
    {
        printFloatLine(line->cells);
        line = line->next;
    }
}

void printStringLine(t_stringCell *cell)
{
    int dif, i;
    while(cell != NULL)
    {
        dif = MAX_COLUMN_NAME_LEN - strlen(cell->value) + 1;
        printf("%s", cell->value);
        for(i = 0; i < dif; ++i)
            printf(" ");
        cell = cell->next;
    }
    printf("\n");
}

void printStringTable(t_table *t)
{
    t_stringLine *line = (t_stringLine *)t->lines;
    while(line != NULL)
    {
        printStringLine(line->cells);
        line = line->next;
    }
}

/*   ---------   */

void printTable(t_db *d, char *name, int with) /* afișare tabel. El este găsit
cu funcția findTable, i se afișează numele și coloanele, apoi se apelează una
din funcțiile specifice. Întregul "with" poate avea valoarea 0 (situație în
care se afișează și liniile) sau 1 (situație în care se afișează doar
coloanele). */
{
    t_table *t = findTable(d, name);
    if(!t)
    {
        printf("Table \"%s\" not found in database.\n", name);
        return;
    }
    printf("TABLE: %s\n", name);
    int i = 0, j = 0;
    t_column *aux = t->columns;
    int nrcol = 0, dif = 0;
    while(aux != NULL)
    {
        nrcol++;
        dif = MAX_COLUMN_NAME_LEN - strlen(aux->name) + 1;
        printf("%s", aux->name);
        for(i = 0; i < dif; ++i)
            printf(" ");
        aux = aux->next;
    }
    printf("\n");
    for(i = 0; i < nrcol; ++i)
    {
        for(j = 0; j < MAX_COLUMN_NAME_LEN; ++j)
            printf("-");
        printf(" ");
    }
    printf("\n");
    if(with == 0){
    if(t->type == INT)
        printIntTable(t);
    else if (t->type == FLOAT)
        printFloatTable(t);
    else
        printStringTable(t);
    printf("\n");
    }
}

void printDB(t_db *d) // afișează întreaga bază de date
{
    printf("DATABASE: %s\n\n", d->name);
    t_table *t = d->tables;
    while(t != NULL)
    {
        printTable(d, t->name, 0);
        t = t->next;
    }
}

/* Alocare celulă pentru fiecare tip */ 

t_intCell *alocIntCell(char *value)
{
    int nr = atoi(value);
    t_intCell *cell = (t_intCell *) malloc(sizeof(t_intCell));
    if(!cell)
        return NULL;
    cell->value = nr;
    cell->next = NULL;
    return cell;
}

t_floatCell *alocFloatCell(char *value)
{
    float nr = atof(value);
    t_floatCell *cell = (t_floatCell *) malloc(sizeof(t_floatCell));
    if(!cell)
        return NULL;
    cell->value = nr;
    cell->next = NULL;
    return cell;
}

t_stringCell *alocStringCell(char *value)
{
    t_stringCell *cell = (t_stringCell *) malloc(sizeof(t_stringCell));
    if(!cell)
        return NULL;
    cell->value = (char *) malloc(MAX_COLUMN_NAME_LEN * sizeof(char));
    if(!cell->value)
    {
        free(cell);
        return NULL;
    }
    strcpy(cell->value, value);
    cell->next = NULL;
    return cell;   
}

/*   ---------   */

/* Adăugare linie în tabel pentru fiecare tip */

void addIntLine(t_table *t, char *buffer)
{
    t_intLine *line = (t_intLine *)t->lines, *auxl;
    auxl = (t_intLine *) malloc(sizeof(t_intLine));
    if(!auxl)
    {
        fprintf(stderr, "ERROR!");
        return;
    }
    auxl->next = NULL;
    auxl->cells = NULL;
    t_intCell *cell, *ant = NULL;
    if(!line)
        t->lines = (void *) auxl;
    else
    {
        while(line->next != NULL)
            line = line->next;
        line->next = auxl;        
    }
    char *token = strtok(buffer, " ");
    token = strtok(NULL, " ");
    token = strtok(NULL, " ");
    while(token != NULL)
    {
        cell = alocIntCell(token);
        if(!cell)
        {
            fprintf(stderr, "ERROR!");
            return;
        }
        if(!ant)
            auxl->cells = cell;
        else
            ant->next = cell;
        ant = cell;
        token = strtok(NULL, " ");
    }
}

void addFloatLine(t_table *t, char *buffer)
{
    t_floatLine *line = (t_floatLine *)t->lines, *auxl;
    auxl = (t_floatLine *) malloc(sizeof(t_floatLine));
    if(!auxl)
    {
        fprintf(stderr, "ERROR!");
        return;
    }
    auxl->next = NULL;
    auxl->cells = NULL;
    t_floatCell *cell, *ant = NULL;
    if(!line)
        t->lines = (void *) auxl;
    else
    {
        while(line->next != NULL)
            line = line->next;
        line->next = auxl;        
    }
    char *token = strtok(buffer, " ");
    token = strtok(NULL, " ");
    token = strtok(NULL, " ");
    while(token != NULL)
    {
        cell = alocFloatCell(token);
        if(!cell)
        {
            fprintf(stderr, "ERROR!");
            return;
        }
        if(!ant)
            auxl->cells = cell;
        else
            ant->next = cell;
        ant = cell;
        token = strtok(NULL, " ");
    }
}

void addStringLine(t_table *t, char *buffer)
{
    t_stringLine *line = (t_stringLine *)t->lines, *auxl;
    auxl = (t_stringLine *) malloc(sizeof(t_stringLine));
    if(!auxl)
    {
        fprintf(stderr, "ERROR!");
        return;
    }
    auxl->next = NULL;
    auxl->cells = NULL;
    t_stringCell *cell, *ant = NULL;
    if(!line)
        t->lines = (void *) auxl;
    else
    {
        while(line->next != NULL)
            line = line->next;
        line->next = auxl;        
    }
    char *token = strtok(buffer, " ");
    token = strtok(NULL, " ");
    token = strtok(NULL, " ");
    while(token != NULL)
    {
        cell = alocStringCell(token);
        if(!cell)
        {
            fprintf(stderr, "ERROR!");
            return;
        }
        if(!ant)
            auxl->cells = cell;
        else
            ant->next = cell;
        ant = cell;
        token = strtok(NULL, " ");
    }
}

/*   ---------   */

void addLine(t_db *d, char *buffer) /* funcție care caută tabelul și apelează
funcția specifică tipului */
{
    char aux[MAX_CMD_LEN], *token;
    strcpy(aux, buffer);
    token = strtok(aux, " ");
    token = strtok(NULL, " ");
    t_table *t = findTable(d, token);
    if(!t)
    {
        printf("Table \"%s\" not found in database.\n", token);
        return;
    }
    if(t->type == INT)
        addIntLine(t, buffer);
    else if(t->type == FLOAT)
        addFloatLine(t, buffer);
    else
        addStringLine(t, buffer);
}

void clearLines(t_db *d, char *name) /* funcție de ștergere de linii dintr-un
tabel; caută tabelul și apelează funcțiile specifice */
{
    t_table *t = findTable(d, name);
    if(!t)
    {
        printf("Table \"%s\" not found in database.\n", name);
        return;
    }
    if(t->type == INT)
        clearIntLines(t);
    else if(t->type == FLOAT)
        clearFloatLines(t);
    else
        clearStringLines(t);
}

/* Ștergere linie pentru fiecare tip */

void deleteIntLine(t_intLine *ant, t_table *t)
{
    t_intCell *cell, *aux;
    t_intLine *auxl;
    if(!ant)
    {
        cell = ((t_intLine *)(t->lines))->cells;
        auxl = (t_intLine *)t->lines;
        t->lines = (void *)auxl->next;
    }
    else
    {
        cell = ant->next->cells;
        auxl = ant->next;
        ant->next = auxl->next;
    }
    while(cell)
    {
        aux = cell;
        cell = cell->next;
        free(aux);
    }
    free(auxl);
}

void deleteFloatLine(t_floatLine *ant, t_table *t)
{
    t_floatCell *cell, *aux;
    t_floatLine *auxl;
    if(!ant)
    {
        cell = ((t_floatLine *)(t->lines))->cells;
        auxl = (t_floatLine *)t->lines;
        t->lines = (void *)auxl->next;
    }
    else
    {
        cell = ant->next->cells;
        auxl = ant->next;
        ant->next = auxl->next;
    }
    while(cell)
    {
        aux = cell;
        cell = cell->next;
        free(aux);
    }
    free(auxl);
}

void deleteStringLine(t_stringLine *ant, t_table *t)
{
    t_stringCell *cell, *aux;
    t_stringLine *auxl;
    if(!ant)
    {
        cell = ((t_stringLine *)(t->lines))->cells;
        auxl = (t_stringLine *)t->lines;
        t->lines = (void *)auxl->next;
    }
    else
    {
        cell = ant->next->cells;
        auxl = ant->next;
        ant->next = auxl->next;
    }
    while(cell)
    {
        aux = cell;
        cell = cell->next;
        free(aux->value);
        free(aux);
    }
    free(auxl);
}

/*   ---------   */

/* Funcții pentru fiecare tip care șterg sau afișează liniile ce îndeplinesc o
anumită condiție dată: au ca parametrii task care are valoarea 1 pentru
afișare și 2 pentru ștergere, t - tabelul, poz - poziția pe care se află celula
ce va fi testată, rel - codificarea pentru relație, val - un string ce reține
valoarea de referință */

void PoR_intLine(int task, t_table *t, int poz, int rel, char *val)
{
    t_intLine *line = (t_intLine *) t->lines, *ant = NULL; 
    t_intCell *cell;
    int i, valnr = atoi(val), deleted;
    while(line != NULL)
    {
        deleted = 0;
        cell = line->cells;
        for(i = 0; i < poz; ++i) // parcurgere până la celula ce va fi testată
            cell = cell->next;
        switch(rel) // cazul pentru relație
        {
            case 1:
                if(cell->value < valnr){
                    if(task == 1)
                        printIntLine(line->cells);
                    else{
                        deleteIntLine(ant, t); /* dăm ca argument anteriorul
                        pentru a face ștergerea din listă  */
                        deleted = 1;
                    } 
                }
                break;
            case 2:
                if(cell->value <= valnr){
                    if(task == 1)
                        printIntLine(line->cells);
                    else{
                        deleteIntLine(ant, t);
                        deleted = 1;
                    }
                }
                break;
            case 3:
                if(cell->value == valnr){
                    if(task == 1)
                        printIntLine(line->cells);
                    else{
                        deleteIntLine(ant, t);
                        deleted = 1;
                    }
                }
                break;
            case 4:
                if(cell->value != valnr){
                    if(task == 1)
                        printIntLine(line->cells);
                    else{
                        deleteIntLine(ant, t);
                        deleted = 1;
                    }
                }
                break;
            case 5:
                if(cell->value >= valnr){
                    if(task == 1)
                        printIntLine(line->cells);
                    else{
                        deleteIntLine(ant, t);
                        deleted = 1;
                    }
                }
                break;
            case 6:
                if(cell->value > valnr){
                    if(task == 1)
                        printIntLine(line->cells);
                    else{
                        deleteIntLine(ant, t);
                        deleted = 1;
                    }
                }
                break;
        }
        if(task == 2 && deleted == 1)
        {
            if(!ant)
                line = (t_intLine *)t->lines;
            else
                line = ant->next;
        }
        else
        {
            ant = line;
            line = line->next;   
        }
    }
}

void PoR_floatLine(int task, t_table *t, int poz, int rel, char *val)
{
    t_floatLine *line = (t_floatLine *) t->lines, *ant = NULL;
    t_floatCell *cell;
    int i, deleted;
    float valnr = atof(val);
    while(line != NULL)
    {
        deleted = 0;
        cell = line->cells;
        for(i = 0; i < poz; ++i)
            cell = cell->next;
        switch(rel)
        {
            case 1:
                if(cell->value < valnr){
                    if(task == 1)
                        printFloatLine(line->cells);
                     else{
                        deleteFloatLine(ant, t);
                        deleted = 1;
                    }
                }
                break;
            case 2:
                if(cell->value <= valnr){
                    if(task == 1)
                        printFloatLine(line->cells);
                    else{
                        deleteFloatLine(ant, t);
                        deleted = 1;
                    }
                }
                break;
            case 3:
                if(cell->value == valnr){
                    if(task == 1)
                        printFloatLine(line->cells);
                    else{
                        deleteFloatLine(ant, t);
                        deleted = 1;
                    }
                }
                break;
            case 4:
                if(cell->value != valnr){
                    if(task == 1)
                        printFloatLine(line->cells);
                    else{
                        deleteFloatLine(ant, t);
                        deleted = 1;
                    }
                }
                break;
            case 5:
                if(cell->value >= valnr){
                    if(task == 1)
                        printFloatLine(line->cells);
                    else{
                        deleteFloatLine(ant, t);
                        deleted = 1;
                    }
                }
                break;
            case 6:
                if(cell->value > valnr){
                    if(task == 1)
                        printFloatLine(line->cells);
                    else{
                        deleteFloatLine(ant, t);
                        deleted = 1;
                    }
                }
                break;
        }
        if(task == 2 && deleted == 1)
        {
            if(!ant)
                line = (t_floatLine *)t->lines;
            else
                line = ant->next;
        }
        else
        {
            ant = line;
            line = line->next;   
        }
    }
}

void PoR_stringLine(int task, t_table *t, int poz, int rel, char *val)
{
    t_stringLine *line = (t_stringLine *) t->lines, *ant = NULL;
    t_stringCell *cell;
    int i, deleted;
    while(line != NULL)
    {
        deleted = 0;
        cell = line->cells;
        for(i = 0; i < poz; ++i)
            cell = cell->next;
        switch(rel)
        {
            case 1:
                if(strcmp(cell->value, val) < 0){
                    if(task == 1)
                        printStringLine(line->cells);
                    else{
                        deleteStringLine(ant, t);
                        deleted = 1;
                    }
                }
                break;
            case 2:
                if(strcmp(cell->value, val) <= 0){
                    if(task == 1)
                        printStringLine(line->cells);
                    else{
                        deleteStringLine(ant, t);
                        deleted = 1;
                    }
                }
                break;
            case 3:
                if(strcmp(cell->value, val) == 0){
                    if(task == 1)
                        printStringLine(line->cells);
                    else{
                        deleteStringLine(ant, t);
                        deleted = 1;
                    }
                }
                break;
            case 4:
                if(strcmp(cell->value, val) != 0){
                    if(task == 1)
                        printStringLine(line->cells);
                    else{
                        deleteStringLine(ant, t);
                        deleted = 1;
                    }
                }
                break;
            case 5:
                if(strcmp(cell->value, val) >= 0){
                    if(task == 1)
                        printStringLine(line->cells);
                    else{
                        deleteStringLine(ant, t);
                        deleted = 1;
                    }
                }
                break;
            case 6:
                if(strcmp(cell->value, val) > 0){
                    if(task == 1)
                        printStringLine(line->cells);
                    else{
                        deleteStringLine(ant, t);
                        deleted = 1;
                    }
                }
                break;
        }
        if(task == 2 && deleted == 1)
        {
            if(!ant)
                line = (t_stringLine *)t->lines;
            else
                line = ant->next;
        }
        else
        {
            ant = line;
            line = line->next;   
        }
    }
}

/*   ---------   */

void Print_or_Delete_Lines(int task, t_db *d, char *buffer) /* Funcție de
afișare/ștergere a unei linii ce respectă o anumită condiție; primește ca
argument task - care va fi 1 sau 2 în funcție de cerință, d - baza de date și
buffer-ul în care sunt păstrate informațiile. Apelează funcția corespunzătoare
în funcție de tip. */
{
    char *token = strtok(buffer, " ");
    token = strtok(NULL, " ");
    t_table *t = findTable(d, token);
    if(!t)
    {
        printf("Table \"%s\" not found in database.\n", token);
        return;
    }
    token = strtok(NULL, " ");
    int poz = findColumn(t, token);
    if(poz == 0)
    {
        printf("Table \"%s\" does not contain column \"%s\".\n", t->name,
        token);
        return;
    }
    poz--;
    token = strtok(NULL, " ");
    int rel = findRelation(token);
    token = strtok(NULL, " ");
    if(task == 1)
        printTable(d, t->name, 1);
    if(t->type == INT)
        PoR_intLine(task, t, poz, rel, token);
    else if(t->type == FLOAT)
        PoR_floatLine(task, t, poz, rel, token);
    else
        PoR_stringLine(task, t, poz, rel, token);
    if(task == 1)
        printf("\n");
}
