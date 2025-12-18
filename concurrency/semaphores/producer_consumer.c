#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#define BUFFER_SIZE 10
volatile int count = 0;
int buffer[BUFFER_SIZE];

sem_t full;
sem_t empty;
sem_t mutex;

/*
Semaphores are atomic counters that can be used to achieve mutual exclusion.
If the counter reached a minus value, the code after it won't execute

In this example, we have three semaphores,
one that indicates the full buckets,
one that indicates the empty buckets,
and a mutex to use around the critical section.

When the producer runs, it decrements the amount of empty slots,
if the producer gets a negative value that means there are zero slots to fill.
On the other hand, if the value is not minus, the producer continue working and produces a value.
Finally, it increments the number of filled slots.

When the consumer runs, it decrements the amount of empty buckets, if it gets a negative value that means there are no values to consume.
On the other hand, if the value is not minus, the consumer continue working and consumes a value.
Finally, it increments the number of empty slots to fill


Avoided deadlock
If the mutex was wrapped around the whole section inside the for loop (including sem_wait and sem_post) a deadlock could arise.
If the consumer runs first with the values (filled = 0, empty = 10), it's going to hold the lock, calls for sem_wait(&full).
The thread is going to be put to sleep without releasing the lock, now the producr can't enter its critical section and a deadlock has occured
The solution is to only wrap the critical section with locks

-Correct
    sem_wait(&full);
    sem_wait(&mutex);
    count--;
    printf("Consumed:%d\n", buffer[count]);
    sem_post(&mutex);
    // increase the empty slots
    sem_post(&empty);
-Wrong
    sem_wait(&mutex);
    sem_wait(&full);
    count--;
    printf("Consumed:%d\n", buffer[count]);
    // increase the empty slots
    sem_post(&empty);
    sem_post(&mutex);
*/
void *producer(void *args)
{
    for (size_t i = 0; i < 20; i++)
    {
        // decrease the empty slots
        sem_wait(&empty);
        sem_wait(&mutex);
        buffer[count] = i;
        printf("Produced:%d\n", buffer[count]);
        count++;
        sem_post(&mutex);
        // increase the full slots
        sem_post(&full);
    }
}
void *consumer(void *args)
{
    for (size_t i = 0; i < 20; i++)
    {
        sem_wait(&full);
        sem_wait(&mutex);
        count--;
        printf("Consumed:%d\n", buffer[count]);
        sem_post(&mutex);
        // increase the empty slots
        sem_post(&empty);
    }
    // while (1)
    // {
    //     sem_wait(&full);
    //     sem_wait(&mutex);
    //     count--;
    //     printf("Consumed:%d\n", buffer[count]);
    //     sem_post(&mutex);
    //     // increase the empty slots
    //     sem_post(&empty);
    // }
}

int main()
{
    sem_init(&empty, 0, BUFFER_SIZE); // 10 empty slots to start with
    sem_init(&full, 0, 0);            // 0 slots full to start with
    sem_init(&mutex, 0, 1);           // 0 slots full to start with
    pthread_t consumer_id;
    pthread_t producer_id;
    pthread_create(&consumer_id, NULL, consumer, NULL);
    pthread_create(&producer_id, NULL, producer, NULL);

    pthread_join(consumer_id, NULL);
    pthread_join(producer_id, NULL);
    return 0;
}