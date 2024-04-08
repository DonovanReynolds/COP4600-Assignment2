#include "hashdb.h"

extern rwlock_t mutex;
extern FILE* outputFile;
//GPT WROTE THIS MERGE SORT

hashRecord* merge(hashRecord* left, hashRecord* right) {
    hashRecord* result = NULL;

    if (left == NULL)
        return right;
    else if (right == NULL)
        return left;

    if (left->hash <= right->hash) {
        result = left;
        result->next = merge(left->next, right);
    } else {
        result = right;
        result->next = merge(left, right->next);
    }

    return result;
}

//GPT WROTE THIS SPLIT FUNCTION
void split(hashRecord* source, hashRecord** left, hashRecord** right) {
    hashRecord* fast;
    hashRecord* slow;

    if (source == NULL || source->next == NULL) {
        *left = source;
        *right = NULL;
    } else {
        slow = source;
        fast = source->next;

        while (fast != NULL) {
            fast = fast->next;
            if (fast != NULL) {
                slow = slow->next;
                fast = fast->next;
            }
        }

        *left = source;
        *right = slow->next;
        slow->next = NULL;
    }
}

//GPT WROTE THIS MERGE SORT
void mergeSort(hashRecord** head) {
    hashRecord* current = *head;
    hashRecord* left;
    hashRecord* right;

    if (current == NULL || current->next == NULL)
        return;

    split(current, &left, &right);

    mergeSort(&left);
    mergeSort(&right);

    *head = merge(left, right);
}

hashRecord* makeNode(char* key, uint32_t value,uint32_t hash)
{


    hashRecord* newRecord = (hashRecord*)malloc(sizeof(hashRecord));
    newRecord->salary = value;
    strcpy(newRecord->name,key);
    newRecord->hash = hash;
    newRecord->next = NULL;
    return newRecord;

}

uint32_t jenkins_one_at_a_time_hash(const uint8_t* key, size_t length) {
  size_t i = 0;
  uint32_t hash = 0;
  while (i != length) {
    hash += key[i++];
    hash += hash << 10;
    hash ^= hash >> 6;
  }
  hash += hash << 3;
  hash ^= hash >> 11;
  hash += hash << 15;
  return hash;
}

int insert(hashRecord* head, char* key, uint32_t value)
{
    //Compute the hash
    uint32_t hash = jenkins_one_at_a_time_hash((uint8_t*)key,strlen(key));
    
    //Acquire the write lock
    rwlock_acquire_writelock(&mutex);
    hashRecord* temp = head;
    while(temp->next != NULL)
    {
        if (temp->hash == hash)
        {
            temp->salary = value;
            rwlock_release_writelock(&mutex);
        }
        temp = temp->next;
    }

    hashRecord* newRecord = makeNode(key,value,hash);
    //Otherwise make new node
    temp->next = newRecord;

    
    //Release write lock 
    rwlock_release_writelock(&mutex);

    //Return 1
    return 1; 
}
hashRecord* delete(hashRecord* head,char* key)
{
    //Compute hash
    uint32_t hash = jenkins_one_at_a_time_hash((uint8_t*)key,strlen(key));
    //Obtain write lock
    

    //Search for the hash
    hashRecord* searchResult = search(head,key);
    rwlock_acquire_writelock(&mutex);
    //If key is found remove from list free mem

    //Update to the head of the list if the delete is the first element
    if (searchResult == head)
    {
        hashRecord* temp = head->next;
        free(head);
        //Release write lock
        rwlock_release_writelock(&mutex);
        return temp;
    }

    if (searchResult != NULL)
    {
        hashRecord* temp = head;
        while(temp->next != searchResult)
            temp = temp->next;
        temp->next = temp->next->next;
    }
    //Otherwise do nothing
    //Release write lock
    rwlock_release_writelock(&mutex);
    return head;
}
hashRecord* search(hashRecord* head,char* key)
{
    //Compute hash
    uint32_t hash = jenkins_one_at_a_time_hash((uint8_t*)key,strlen(key));
    //Obtain read lock
    rwlock_acquire_readlock(&mutex);
    //Search the list
    hashRecord* temp = head;
    if(temp->hash == hash)
    {
        rwlock_release_readlock(&mutex);
        return temp;
    }
    while(temp != NULL)
    {
        //If found return the value
        if(temp->hash == hash)
        {
            rwlock_release_readlock(&mutex);
            return temp;
        }
        temp = temp->next;
    }
    
    //Otherwise return NULL
    rwlock_release_readlock(&mutex);
    return NULL;
}


void printHashDB(hashRecord* head)
{
    rwlock_acquire_readlock(&mutex);
    hashRecord* temp = head;
    if (head == NULL)
        return
    printNode(head);
    while(temp->next != NULL)
    {
        temp = temp->next;
        printNode(temp);
    }
    rwlock_release_readlock(&mutex);

}

void freeHashRecord(hashRecord* head)
{
    if (head == NULL)
        return;
    freeHashRecord(head->next);
    free(head);
}

void printNode(hashRecord* head)
{

    fprintf(outputFile,"%u,%s,%u\n",head->hash,head->name,head->salary);
}