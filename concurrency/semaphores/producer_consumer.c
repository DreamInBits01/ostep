#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#define BUFFER_SIZE 10
volatile int count = 0;
int buffer[BUFFER_SIZE];

sem_t full;
sem_t empty;
sem_t mutex;
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