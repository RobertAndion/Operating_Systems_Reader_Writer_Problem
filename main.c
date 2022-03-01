#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include "readwriter.h"

long reading_writing();
void *reader(void *);
void *writer(void *);

int main(int argc, char *argv[])
{
    // Initialize the random number generator
    time_t t;
    srand((unsigned)time(&t));
    // Initialize Read Write Lock Data Structure
    rwlock_t *rwLock = malloc(sizeof(rwlock_t));
    if (rwLock == NULL)
        exit(-1);
    rwlock_init(rwLock);
    // Set up file read in routine
    ssize_t read;
    size_t len;
    char *line = NULL;
    FILE *ptr = fopen("scenarios.txt", "r");
    if (ptr == NULL)
        exit(-1);
    int lineNumber = 1;
    while ((read = getline(&line, &len, ptr)) != -1)
    {
        int readerCount = 0;
        int writerCount = 0;
        char *c = line;
        printf("Scenario %d\n", lineNumber);
        while (*c != '\n' && *c != '\0')
        {
            pthread_t p;

            if (*c == 'r')
            {
                pthread_create(&p, NULL, (void *)reader, (void *)rwLock);
                readerCount++;
            }
            else if (*c == 'w')
            {
                pthread_create(&p, NULL, (void *)writer, (void *)rwLock);
                writerCount++;
            }
            else
            {
                printf("Improper input file.\n");
                exit(-1);
            }
            c++;
        }
        // Force main thread to wait long enough to see it all complete.
        sleep(30);
        lineNumber++;
    }
    printf("Running complete.\n");
    fclose(ptr);
}

void *reader(void *arg)
{ // Function to simulate a read
    rwlock_t *myArgs = (rwlock_t *)arg;
    rwlock_acquire_readlock(myArgs);
    reading_writing();
    rwlock_release_readlock(myArgs);
    printf("Reader complete\n");
    return NULL;
}

void *writer(void *arg)
{ // Function to simulate a write
    rwlock_t *myArgs = (rwlock_t *)arg;
    rwlock_acquire_writelock(myArgs);
    reading_writing();
    rwlock_release_writelock(myArgs);
    printf("Writer complete\n");
    return NULL;
}

long reading_writing() // Function to simulate IO time or data structure access time.
{
    long x = 0;
    int T, i, j;
    T = rand() % 100000;
    for (i = 0; i < T; i++)
        for (j = 0; j < T; j++)
            x = i * j;
    return x;
}