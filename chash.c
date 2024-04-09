#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "hashdb.h"

extern rwlock_t mutex;
/*
COP 4600 Assignment 2: Concurrent Hash Table
Worked on by:
Donovan Reynolds
Tristan Barber
Tyler Cosby
Rodrigo Santos
*/

hashRecord* hashDBHead = NULL;

char command[100];
char name[100];
char salary[100];
char** allCommands;
int sizeOfArray;
int num_threads;
pthread_t* allThreads;
int lockAquired = 0;
int lockReleased = 0;

FILE* outputFile;

uint32_t stringToUINT32(char* string, int index)
{
    if (index == 0)
        return string[index] - '0';
    uint32_t currentNum = string[index] - '0';
    uint32_t recDigits = stringToUINT32(string,index-1);

    return currentNum + recDigits*10;
    
}



void* runCommand(void* args)
{
    
    if(strcmp(command,"insert") == 0)
    {
        if (hashDBHead == NULL)
        {
            rwlock_acquire_writelock(&mutex);
            uint32_t hash = jenkins_one_at_a_time_hash((uint8_t*)name,strlen(name));
            hashDBHead = makeNode(name,stringToUINT32(salary,strlen(salary)-1),hash);
            rwlock_release_writelock(&mutex);
            return;
        }
        insert(hashDBHead,name,stringToUINT32(salary,strlen(salary)-1));
        
    }
    if(strcmp(command,"search") == 0)
    {
        hashRecord* searchResult = search(hashDBHead,name);
        if (searchResult == NULL)
            fputs("No result found...\n",outputFile);
        else
            fprintf(outputFile,"%s was found with a salary of %u", name,searchResult->salary);
    }
    if(strcmp(command,"delete") == 0)
    {
        hashDBHead = delete(hashDBHead,name);
    }
    if (strcmp(command,"print") == 0)
    {
        mergeSort(&hashDBHead);
        printHashDB(hashDBHead);
        
    }
    
}


void readFileMultiThread()
{


    FILE *input_file = fopen("commands.txt", "r");
    if (input_file == NULL) {
        perror("Error opening file");
        return ;
    }

   
    fscanf(input_file, "threads,%d,0", &num_threads);

    allCommands = (char**)malloc(sizeof(char*)*(num_threads));
    allThreads = (pthread_t*)malloc(sizeof(pthread_t)*num_threads);

    char line[100];
    fgets(line, sizeof(line), input_file);
    for (int i = 0; i < num_threads;i++) {
        
        fgets(line, sizeof(line), input_file);
        // Remove newline character at the end, if present
        char curCommand[100];
        strcpy(curCommand,"");
        size_t len = strlen(line);
        if (line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }

        strcat(curCommand,line);



        allCommands[sizeOfArray] = malloc(sizeof(curCommand));
        strcpy(allCommands[sizeOfArray],curCommand);
        sizeOfArray++;
    }
    fclose(input_file);
    

}



int main() {

    rwlock_init(&mutex);
    
    sizeOfArray = 0;

    readFileMultiThread();
    
    outputFile = fopen("output.txt","w");
    int j = 0;
    for (j = 0; j < sizeOfArray;j++)
    {
            
        char* token = strtok(allCommands[j], ",");
        if(token == NULL)
            continue;

        strcpy(command,token);
        token = strtok(NULL, ",");
        strcpy(name,token);
        token = strtok(NULL, ",");
        strcpy(salary,token);

        Pthread_create(&allThreads[j],NULL,runCommand,NULL);
        Pthread_join(allThreads[j],NULL);

        free(allCommands[j]);
    }
    free(allCommands);
    free(allThreads);
    
    fprintf(outputFile,"Number of lock acquisitions: %d\n",lockAquired);
    fprintf(outputFile,"Number of lock releases: %d\n",lockReleased);

    fprintf(outputFile,"Final Table:\n");
    mergeSort(&hashDBHead);
    printHashDB(hashDBHead);
    fclose(outputFile);
    
    return 0;

}
