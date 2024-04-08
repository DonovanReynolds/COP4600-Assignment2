#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include "rwlocks.h"

typedef struct hash_struct
{
  uint32_t hash;
  char name[50];
  uint32_t salary;
  struct hash_struct *next;
} hashRecord;

uint32_t jenkins_one_at_a_time_hash(const uint8_t* key, size_t length);
hashRecord* makeNode(char* key, uint32_t value,uint32_t hash);
int insert(hashRecord* head,char* key, uint32_t value);
hashRecord* delete(hashRecord* head,char* key);
hashRecord* search(hashRecord* head,char* key);
void printHashDB(hashRecord* head);
void mergeSort(hashRecord** head);
void printNode(hashRecord* head);
void freeHashRecord(hashRecord* head);