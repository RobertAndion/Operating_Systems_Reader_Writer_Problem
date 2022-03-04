#ifndef __READWRITER__H__
#define __READWRITER__H__

#include <semaphore.h>
#include <stdio.h>
#include "stdlib.h"
// Code from figure 31.13 was used to start this project.
typedef struct _rwlock_t
{
    sem_t lock;      // Protects rwlock_t itself
    sem_t writelock; // Stops writers to allow readers
    sem_t reader_waiting;
    sem_t writer_waiting;
    int readers;
} rwlock_t;

typedef struct _myargs_t
{
    int *thread_id;
    rwlock_t *rwLock;
} myargs_t;

void rwlock_init(rwlock_t *);
void rwlock_acquire_readlock(rwlock_t *);
void rwlock_release_readlock(rwlock_t *);
void rwlock_acquire_writelock(rwlock_t *);
void rwlock_release_writelock(rwlock_t *);

#endif