#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#define BUFFER_SIZE 10

/*
Without two conditions,
we might have the data full and signal that we want to consume it.
Depending on the condition queue, we might awaken a producer instead of a consumer.

On the other side,
if we have consumed all data, we need to awake a producer thread,
again depending on the queue of the condition,
we might awaken the other consumer thread instead of a producer thread.

*/
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t not_full = PTHREAD_COND_INITIALIZER;
pthread_cond_t not_empty = PTHREAD_COND_INITIALIZER;

// Count of data
int count = 0;
int buffer[BUFFER_SIZE];

void *producer(void *args)
{
    for (int i = 0; i < 20; i++)
    {
        pthread_mutex_lock(&lock);
        // Wait while buffer is full
        while (count == BUFFER_SIZE)
            /*
            If the count is full, it means that the producer can't do any more work
            so it needs to be put into sleep via pthread_cond_wait call
            When it gets a signal that the buffer is not full, it resumes producing
            */
            pthread_cond_wait(&not_full, &lock); // releases mutex, and sleeps
        buffer[count] = i;
        count++;

        printf("Producer produced:%d\n", i);
        // Signal that the buffer is not empty
        pthread_cond_signal(&not_empty);
        pthread_mutex_unlock(&lock);
    }
}
void *consumer(void *args)
{
    int item;
    for (int i = 0; i < 20; i++)
    {
        pthread_mutex_lock(&lock);
        // Stops when there are no items in the buffer
        while (count == 0)
            /*
                If the count is empty, the thread is going to sleep until it gets a signal
                that the count is not empty
            */
            // wait for the signal that the buffer is not empty
            pthread_cond_wait(&not_empty, &lock);

        count--;
        printf("Consumed item:%d\n", i);
        pthread_cond_signal(&not_full);
        pthread_mutex_unlock(&lock);
        /* code */
    }
}

int main()
{
    pthread_t producer_thread;
    pthread_t consumer_thread;

    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    return 0;
}