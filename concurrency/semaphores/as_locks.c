#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
sem_t mutex;
/*
Semaphores are atomic counters that can be used to achieve mutual exclusion.
If the counter reached 0, the code after it won't execute


Tracing
-initial counter value is 1
-Thread 1 runs
    -Decreases the counter, it becomes 0 so the thread continue running
    -completes the critical section

-Thread 2 runs (while thread 1 is running)
    -The counter is zero, decreasing it results in a minus value so thread 2 will wait until thread 1 is done

-Thread 1
    -Increments the counter it became 1 now
    -Exits

-Thread 2 runs (thread 1 completes)
    -Decreases the counter, it becomes 0 so the thread continue running
    -It enters the critical section
    -Completes and increments the semahpore
*/
void *worker(void *args)
{
    int *counter = (int *)args;
    sem_wait(&mutex); // decrements the semaphore counter
    printf("Enter critical section\n");
    for (size_t i = 0; i < 5; i++)
    {
        *counter = *counter + 1;
    }
    printf("Done with critical section\n");
    sem_post(&mutex); // incerments thee semaphore counter
}
int main()
{
    pthread_t thread_1;
    pthread_t thread_2;
    sem_init(&mutex, 0, 1);
    int counter = 0;
    pthread_create(&thread_1, NULL, worker, &counter);
    pthread_create(&thread_2, NULL, worker, &counter);

    pthread_join(thread_1, NULL);
    pthread_join(thread_2, NULL);
    printf("Counter:%d", counter);
};