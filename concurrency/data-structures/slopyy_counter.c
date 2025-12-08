#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#define NUM_CORES 5
/*
    The idea of this counter is to trade accuracy for performance gains.
    The counter consists of a global counter, a global lock, local counters, local locks, and finally a threshold variable that decides the update frequency of the global counter.
    The local counters are incremented, until they reach that threshold, then the global counter is incremented
*/
typedef struct Counter
{
    int global_counter;
    pthread_mutex_t global_mutex;
    int local_counters[NUM_CORES];
    pthread_mutex_t local_mutexes[NUM_CORES];
    int threshhold;
} Counter;

void init_counter(Counter *counter, int threshhold)
{
    counter->global_counter = 0;
    counter->global_counter = threshhold;
    pthread_mutex_init(&counter->global_mutex, NULL);
    for (size_t i = 0; i < NUM_CORES; i++)
    {
        counter->local_counters[i] = 0;
        pthread_mutex_init(&counter->local_mutexes[i], NULL);
    }
};

void update(Counter *counter, int amt, int thread_id)
{
    pthread_mutex_lock(&counter->local_mutexes[thread_id]);
    counter->local_counters[thread_id] += amt;
    if (counter->local_counters[thread_id] >= counter->threshhold)
    {
        pthread_mutex_lock(&counter->global_mutex);
        counter->global_counter += counter->local_counters[thread_id];
        pthread_mutex_unlock(&counter->global_mutex);
    }
    pthread_mutex_unlock(&counter->local_mutexes[thread_id]);
}
int get(Counter *counter)
{
    pthread_mutex_lock(&counter->global_mutex);
    int result;
    result = counter->global_counter;
    pthread_mutex_unlock(&counter->global_mutex);
    return result; // approximation
}
