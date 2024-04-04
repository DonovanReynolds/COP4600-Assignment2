#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>


typedef struct hash_struct
{
  uint32_t hash;
  char name[50];
  uint32_t salary;
  struct hash_struct *next;
} hashRecord;


int insert(hashRecord* head,char* key, uint32_t value);
int delete(hashRecord* head,char* key);
uint32_t search(hashRecord* head,char* key);
void printHashDB(hashRecord* head);