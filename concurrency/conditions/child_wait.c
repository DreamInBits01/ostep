#include <stdio.h>
#include <pthread.h>
volatile int is_child_done = 0;
pthread_mutex_t mutex;
pthread_cond_t child_done;
void *child(void *args)
{
    printf("Child started...\n");
    printf("Child ended...\n");
    pthread_mutex_lock(&mutex);
    is_child_done = 1;
    pthread_cond_signal(&child_done);
    pthread_mutex_unlock(&mutex);
}
void child_join()
{
    pthread_mutex_lock(&mutex);
    while (is_child_done == 0)
        pthread_cond_wait(&child_done, &mutex);
    pthread_mutex_unlock(&mutex);
}
int main()
{
    pthread_t child_id;
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&child_done, NULL);
    printf("Parent started\n");
    pthread_create(&child_id, NULL, child, NULL);
    // Halt any further execution if the child has not changed the state of the condition and signaled that
    child_join();
    printf("Parent ended\n");
    return 0;
}