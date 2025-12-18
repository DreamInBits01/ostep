#include <stdio.h>
#include <pthread.h>
/*
Why not signaling only when the value becomes 0 or greater

## Why This Could Work

If `value >= 0` after increment, it means:
- Before increment: `value >= -1`
- After increment: Resources are available (or just became available)
- Any waiting thread should wake up and check

This covers the transition from "no resources" to "resources available."

## The Problem: When Value Starts Negative

Your semaphore can go negative if multiple threads wait before any posts:
Thread A: iwait() → value: 0 → -1 (waits)
Thread B: iwait() → value: -1 → -2 (waits)
Thread C: iwait() → value: -2 → -3 (waits)
Thread D: post()  → value: -3 → -2 (NO SIGNAL with your check!)
Thread E: post()  → value: -2 → -1 (NO SIGNAL with your check!)
Thread F: post()  → value: -1 → 0  (SIGNALS!)

Only the last post signals, but all three threads need to be woken up eventually.
With pthread_cond_signal(), only one thread wakes per signal, so threads A and B stay blocked forever!
*/
typedef struct zemaphore_t
{
    int value;
    pthread_cond_t cond;
    pthread_mutex_t mutex;
} zemaphore_t;

void zemaphore_init(zemaphore_t *zemaphore)
{
    zemaphore->value = 0;
    pthread_cond_init(&zemaphore->cond, NULL);
    pthread_mutex_init(&zemaphore->mutex, NULL);
}
void zemaphore_iwait(zemaphore_t *zemaphore)
{
    pthread_mutex_lock(&zemaphore->mutex);
    // reached zero or less, the code following code that's written after this must wait until the value is greater than 0
    while (0 >= zemaphore->value)
        pthread_cond_wait(&zemaphore->cond, &zemaphore->mutex);
    zemaphore->value--;
    pthread_mutex_unlock(&zemaphore->mutex);
}
void zemaphore_post(zemaphore_t *zemaphore)
{
    pthread_mutex_lock(&zemaphore->mutex);
    zemaphore->value++;
    // signal increasing value
    pthread_cond_signal(&zemaphore->cond);
    pthread_mutex_unlock(&zemaphore->mutex);
}
