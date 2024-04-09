#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "rwlocks.h"

rwlock_t mutex;

extern FILE* outputFile;
extern int lockAquired;
extern int lockReleased;


void rwlock_init(rwlock_t *lock) {
    lock->readers = 0;
    Sem_init(&lock->lock, 1); 
    Sem_init(&lock->writelock, 1); 
}

void rwlock_acquire_readlock(rwlock_t *lock) {
    fputs("READLOCK ACQUIRED\n",outputFile);
    lockAquired++;
    Sem_wait(&lock->lock);
    lock->readers++;
    if (lock->readers == 1)
	Sem_wait(&lock->writelock);
    Sem_post(&lock->lock);
}

void rwlock_release_readlock(rwlock_t *lock) {
    fputs("READLOCK RELEASED\n",outputFile);
    lockReleased++;
    Sem_wait(&lock->lock);
    lock->readers--;
    if (lock->readers == 0)
	Sem_post(&lock->writelock);
    Sem_post(&lock->lock);
}

void rwlock_acquire_writelock(rwlock_t *lock) {
    fputs("WRITELOCK ACQUIRED\n",outputFile);
    lockAquired++;
    Sem_wait(&lock->writelock);
}

void rwlock_release_writelock(rwlock_t *lock) {
    fputs("WRITELOCK RELEASED\n",outputFile);
    lockReleased++;
    Sem_post(&lock->writelock);
}

int read_loops;
int write_loops;
int counter = 0;



