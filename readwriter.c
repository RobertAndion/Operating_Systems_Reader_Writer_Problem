#include "readwriter.h"

void rwlock_init(rwlock_t *rw)
{ // Initialize the locks and values for the locking structure.
    rw->readers = 0;
    sem_init(&rw->lock, 0, 1);
    sem_init(&rw->writelock, 0, 1);
    sem_init(&rw->reader_waiting, 0, 1);
    sem_init(&rw->writer_waiting, 0, 1);
}

void rwlock_acquire_readlock(rwlock_t *rw)
{
    sem_wait(&rw->reader_waiting); // If readers are waiting keep waiting, else enter next section.
    sem_post(&rw->reader_waiting);

    sem_wait(&rw->writer_waiting); // If a writer is waiting, wait.
    sem_post(&rw->writer_waiting);
    sem_wait(&rw->lock);
    rw->readers++;
    if (rw->readers == 1) // If first reader grab writelock.
        sem_wait(&rw->writelock);
    sem_post(&rw->lock);
}

void rwlock_release_readlock(rwlock_t *rw)
{
    sem_wait(&rw->lock);
    rw->readers--;
    if (rw->readers == 0)
        sem_post(&rw->writelock);
    sem_post(&rw->lock);
}

void rwlock_acquire_writelock(rwlock_t *rw)
{
    sem_wait(&rw->reader_waiting); // If a reader is waiting (I.E. Writer running now, stop the writer to not starve readers.)
    sem_post(&rw->reader_waiting);

    sem_wait(&rw->writer_waiting); // Let readers know there is a writer waiting, stop them from entering.
    sem_wait(&rw->writelock);
    sem_post(&rw->writer_waiting); // Allow readers to enter again.
}

void rwlock_release_writelock(rwlock_t *rw)
{
    sem_post(&rw->writelock);
}