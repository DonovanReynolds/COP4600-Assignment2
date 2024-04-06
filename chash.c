#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hashdb.h"

/*
COP 4600 Assignment 2: Concurrent Hash Table
Worked on by:
Donovan Reynolds
Tristan Barber
Tyler Cosby
Rodrigo Santos
*/

hashRecord* hashDBHead = NULL;

uint32_t stringToUINT32(char* string, int index)
{
    if (index == 0)
        return string[index] - '0';
    uint32_t currentNum = string[index] - '0';
    uint32_t recDigits = stringToUINT32(string,index-1);

    return currentNum + recDigits*10;
    
}


void runCommand(char* command,char* name,char* salary)
{
    if(strcmp(command,"insert") == 0)
    {
        if (hashDBHead == NULL)
        {
            hashDBHead = malloc(sizeof(hashRecord));
            hashDBHead->salary = stringToUINT32(salary,strlen(salary)-1);
            strcpy(hashDBHead->name,name);
            hashDBHead->hash = jenkins_one_at_a_time_hash(name,strlen(name));
            hashDBHead->next = NULL;
            return;
        }
        hashDBHead = insert(hashDBHead,name,stringToUINT32(salary,strlen(salary)-1));
    }
    if(strcmp(command,"search") == 0)
    {
        //search(name);
    }
    if(strcmp(command,"delete") == 0)
    {
        //delete(name);
    }
    if (strcmp(command,"print") == 0)
    {
        printf("%u, %s, %u",hashDBHead->hash,hashDBHead->name,hashDBHead->salary);
        //printHashDB(hashDBHead);
    }
}

void readFile()
{
    FILE *input_file = fopen("commands.txt", "r");
    if (input_file == NULL) {
        perror("Error opening file");
        return ;
    }

    int num_threads;
    fscanf(input_file, "threads,%d,0", &num_threads);
    printf("Number of threads: %d\n", num_threads);
    char command[100];
    char name[100];
    char salary[100];
    char line[100];

    while (fgets(line, sizeof(line), input_file) != NULL) {
        // Remove newline character at the end, if present
        size_t len = strlen(line);
        if (line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        char* token = strtok(line, ",");
        if(token == NULL)
            continue;
        strcpy(command,token);
        token = strtok(NULL, ",");
        strcpy(name,token);
        token = strtok(NULL, ",");
        strcpy(salary,token);
        runCommand(command,name,salary);
    }
    
    fclose(input_file);

}



int main() {

    readFile();

    return 0;
}