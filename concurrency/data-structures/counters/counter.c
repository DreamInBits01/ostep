#include <stdio.h>
#include <pthread.h>

typedef struct counter_t
{
    int value;
    pthread_mutex_t mutex;
} counter_t;
void init_counter(counter_t *counter)
{
    pthread_mutex_init(&counter->mutex, NULL);
    counter->value = 0;
}
void destroy_counter(counter_t *counter)
{
    pthread_mutex_destroy(&counter->mutex);
}
void increment(counter_t *counter)
{
    pthread_mutex_lock(&counter->mutex);
    counter->value = counter->value + 1;
    pthread_mutex_unlock(&counter->mutex);
}
void decrement(counter_t *counter)
{
    pthread_mutex_lock(&counter->mutex);
    counter->value = counter->value - 1;
    pthread_mutex_unlock(&counter->mutex);
}
void get(counter_t *counter)
{
    pthread_mutex_lock(&counter->mutex);
    int value = counter->value;
    pthread_mutex_unlock(&counter->mutex);
    return value;
}
