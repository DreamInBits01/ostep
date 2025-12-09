#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#define NUM_CORES 5
/*
    The idea of this counter is to trade accuracy for performance gains.
    The counter consists of a global counter, a global lock, local counters, local locks, and finally a threshold variable that decides the update frequency of the global counter.
    The local counters are incremented, until they reach that threshold, then the global counter is incremented
*/
typedef struct Counter
{
    unsigned global_counter;
    pthread_mutex_t global_mutex;
    unsigned local_counters[NUM_CORES];
    pthread_mutex_t local_mutexes[NUM_CORES];
    unsigned threshhold;
} Counter;

void init_counter(Counter *counter, int threshhold)
{
    counter->global_counter = 0;
    counter->threshhold = threshhold;
    pthread_mutex_init(&counter->global_mutex, NULL);
    for (size_t i = 0; i < NUM_CORES; i++)
    {
        counter->local_counters[i] = 0;
        pthread_mutex_init(&counter->local_mutexes[i], NULL);
    }
};
/*
Grab a local counter
1-Increase the local counter
2-If it's equal or more to the threshhold, update global counter, and reset the local counter
3-release the lock
*/
void update(Counter *counter, int amt, int thread_id)
{
    pthread_mutex_lock(&counter->local_mutexes[thread_id]);
    counter->local_counters[thread_id] += amt;
    if (counter->local_counters[thread_id] >= counter->threshhold)
    {
        pthread_mutex_lock(&counter->global_mutex);
        counter->global_counter += counter->local_counters[thread_id];
        pthread_mutex_unlock(&counter->global_mutex);
        counter->local_counters[thread_id] = 0;
    }
    pthread_mutex_unlock(&counter->local_mutexes[thread_id]);
}
unsigned get(Counter *counter)
{
    pthread_mutex_lock(&counter->global_mutex);
    unsigned result;
    result = counter->global_counter;
    pthread_mutex_unlock(&counter->global_mutex);
    return result; // approximation
}

int main()
{
    struct timeval start_time;
    struct timeval end_time;
    gettimeofday(&start_time, NULL);
    Counter counter;
    init_counter(&counter, 5);
    for (size_t i = 0; i < 100; i++)
    {
        update(&counter, 1, 0);
        update(&counter, 1, 0);
        update(&counter, 1, 1);
        update(&counter, 1, 1);
        update(&counter, 1, 2);
        update(&counter, 1, 3);
        update(&counter, 1, 4);
    }
    gettimeofday(&end_time, NULL);
    printf("Result:%u\n", get(&counter));
    long unsigned elapsed = (end_time.tv_sec * 1000000 + end_time.tv_usec) - (start_time.tv_sec * 1000000 + start_time.tv_usec);
    printf("Took MS:%ld", elapsed);
    return 0;
}