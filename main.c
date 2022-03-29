#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include "readwriter.h"

long reading_writing();
void *reader(void *);
void *writer(void *);
/*
 * This is a custom main.c, it was created before the sample was provided to us.
 */
int main(int argc, char *argv[])
{
  int error;
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
    printf("\n -- Scenario %d -- \n\n", lineNumber);
    myargs_t *args;
    pthread_t p;
    while (*c != '\n' && *c != '\0')
    {
      if (*c == 'r')
      {
        printf("Reader %d entering\n", readerCount);
        args = malloc(sizeof(myargs_t));
        args->rwLock = rwLock;
        args->thread_id = malloc(sizeof(int));
        *args->thread_id = readerCount;
        error = pthread_create(&p, NULL, (void *)reader, (void *)args);
        if (error != 0)
          exit(-1);
        readerCount++;
      }
      else if (*c == 'w')
      {
        printf("Writer %d entering\n", writerCount);
        args = malloc(sizeof(myargs_t));
        args->rwLock = rwLock;
        args->thread_id = malloc(sizeof(int));
        *args->thread_id = writerCount;
        error = pthread_create(&p, NULL, (void *)writer, (void *)args);
        if (error != 0)
          exit(-1);
        writerCount++;
      }
      else
      {
        printf("Improper input file.\n");
        exit(-1);
      }
      c++;
    }
    printf("#########################################\n");
    pthread_join(p, NULL);
    // Force main thread to wait long enough to see it all complete.
    sleep(30);
    lineNumber++;
  }
  printf("Running complete.\n");
  fclose(ptr);
}

void *reader(void *arg)
{ // Function to simulate a reader
  myargs_t *myArgs = (myargs_t *)arg;
  rwlock_acquire_readlock(myArgs->rwLock);
  reading_writing();
  rwlock_release_readlock(myArgs->rwLock);
  printf("Reader %d complete\n", *myArgs->thread_id);
  free(myArgs->thread_id); // Free the int, good practice and no longer needed.
  return NULL;
}

void *writer(void *arg)
{ // Function to simulate a writer
  myargs_t *myArgs = (myargs_t *)arg;
  rwlock_acquire_writelock(myArgs->rwLock);
  reading_writing();
  rwlock_release_writelock(myArgs->rwLock);
  printf("Writer %d complete\n", *myArgs->thread_id);
  free(myArgs->thread_id); // Free the int, good practice and no longer needed.
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
