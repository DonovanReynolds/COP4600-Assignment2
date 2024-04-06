#include "hashdb.h"

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
    uint32_t hash = jenkins_one_at_a_time_hash(key,strlen(key));

    //Acquire the write lock

    
    while(head->next != NULL)
    {
        //If hash found update
        if(head->hash == hash)
        {
            head->salary = value;
            return 1;
        }
        head = head->next;
    }
    //Otherwise make new node
    hashRecord* newRecord = malloc(sizeof(hashRecord));
    newRecord->salary = value;
    strcpy(newRecord->name,key);
    newRecord->hash = hash;
    newRecord->next = NULL;
    head->next = newRecord;
    //Release write lock 


    //return 1 if hash found 0 if create new
    return 0; 
}
int delete(hashRecord* head,char* key)
{
    //Compute hash
    //Obtain write lock
    //Search for the hash
    //If key is found remove from list free mem
    //Otherwise do nothing
    //Release write lock
    //return 1 if deleted 0 if nothing 
    return 0;
}
uint32_t search(hashRecord* head,char* key)
{
    //Compute hash
    //Obtain read lock
    //Search the list
    //If found return the value
    //Otherwise return NULL
    return 0;
}
void printHashDB(hashRecord* head)
{

}
