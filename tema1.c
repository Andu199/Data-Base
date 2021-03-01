/* BOLDIȘOR Dragoș-Alexandru - 312CB */

#include <stdio.h>
#include "utils.h"

int test(char *buffer) /* testează dacă este vorba de comanda de șterge a unui
tabel sau de ștergere a unei linii */ 
{
    unsigned int i, nrcuv = 0;
    for(i = 0; i < strlen(buffer); ++i)
        if(buffer[i] == 32)
            nrcuv++;
    return nrcuv;
}

void task(char *buffer, t_db **d) /* verifică buffer-ul și apelează funcția
specifică comenzii. */
{
    char *p, aux[MAX_CMD_LEN];
    strcpy(aux, buffer);
    p = strtok(aux, " ");
    if(strcmp(p, "INIT_DB") == 0)
    {
        p = strtok(NULL, " ");
        (*d) = initDB(p);
    }
    else if(strcmp(p, "DELETE_DB") == 0)
    {
        deleteDB(d);
        exit(EXIT_SUCCESS);
    }
    else if(strcmp(p, "CREATE") == 0)
        createNewTable(*d, buffer);
    else if(strcmp(p, "DELETE") == 0)
    {
        p = strtok(NULL, " ");
        if(test(buffer) == 1){
            deleteTable(*d, p);
        }
        else
            Print_or_Delete_Lines(2, *d, buffer);
    }
    else if(strcmp(p, "PRINT_DB") == 0)
        printDB(*d);
    else if(strcmp(p, "PRINT") == 0)
    {
        p = strtok(NULL, " ");
        printTable(*d, p, 0);
    }
    else if(strcmp(p, "ADD") == 0)
        addLine(*d, buffer);
    else if(strcmp(p, "CLEAR") == 0)
    {
        p = strtok(NULL, " ");
        clearLines(*d, p);
    }
    else if(strcmp(p, "SEARCH") == 0)
        Print_or_Delete_Lines(1, *d, buffer);
    else
        printf("Unknown command: \"%s\".\n", buffer); // nu recunoaște comanda
}

int main()
{
    t_db *data_base = NULL;
    char buffer[MAX_CMD_LEN];
    while(1)
    {
        fgets(buffer, MAX_CMD_LEN, stdin);
        if(buffer[strlen(buffer) - 1] == '\n')
            buffer[strlen(buffer) - 1] = '\0';
        task(buffer, &data_base);
    }
    return 0;
}