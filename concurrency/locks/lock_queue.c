#include <stdio.h>
#include <stdlib.h>
#define _GNU_SOURCE
#include <unistd.h>

/*
the guard is used, basically as a spin-lock around
the flag and queue manipulations the lock is using. This approach thus
doesn’t avoid spin-waiting entirely; a thread might be interrupted while
acquiring or releasing the lock, and thus cause other threads to spin-wait
for this one to run again. However, the time spent spinning is quite lim-
ited (just a few instructions inside the lock and unlock code, instead of the
user-defined critical section), and thus this approach may be reasonable.
*/
void park()
{
    // put caller thread to sleep
}
void unpark(int thread_id)
{
    // run the thread with the provided thread id
}
typedef struct queue_t
{
    int thread_id;
    queue_t *next;
} queue_t;

typedef struct lock_t
{
    int flag;
    int guard;
    queue_t *queue;
} lock_t;
queue_t *queue_add(queue_t **queue, int data)
{
    queue_t *node = malloc(sizeof(queue_t));
    if (node == NULL)
        return NULL;
    if (*queue == NULL)
    {
        *queue = node;
        return node;
    }
    queue_t *current = *queue;
    while (current->next)
    {
        current = current->next;
    }
    current->next = node;
}
int is_queue_empty(queue_t **queue)
{
    if (*queue == NULL)
        return 1;
    else
        return 0;
}
int queue_pop(queue_t **queue)
{
    if ((*queue)->next == NULL)
    {
        int thread_id = (*queue)->thread_id;
        free(*queue);
        return thread_id;
    }
    queue_t *temp = *queue;
    int thread_id = (*queue)->thread_id;
    (*queue) = (*queue)->next;
    free(temp);
    return thread_id;
}
int TestAndSet(int *ptr, int new)
{
    int old = *ptr;
    *ptr = new;
    return old;
};

void lock_int(lock_t *lock)
{
    lock->flag = 0;
    lock->guard = 0;
    lock->queue = NULL;
}
void lock(lock_t *lock)
{
    while (TestAndSet(&lock->guard, 1))
        ; // Spin if the guard is required, if not set it to 1 to aquire it to continue the operation

    // Aquire the flag
    if (lock->flag == 0)
    {
        lock->flag = 1;
        lock->guard = 0;
    }
    else
    {
        /*
            -The flag is already aquired
            -The caller thread must be queued
            -The guard must be resetted
            -The flag state doesn't change when calling the next thread
        */
        queue_add(&lock->queue, gettid());
        lock->guard = 0;
        park();
    }
}
void unlock(lock_t *lock)
{
    while (TestAndSet(&lock->guard, 1))
        ;
    if (is_queue_empty(&lock->queue))
    {
        // The queue is empty, the flag will be zero
        lock->flag = 0;
    }
    else
    {
        // Wake up the next thread
        unpark(queue_pop(&lock->queue));
    }
    lock->guard = 0;
}

/*
park -> puts caller to sleep
unpark(thread_id) -> puts the provided thread_id in a running state

When locking
    -Test guard to see if other threads are modifing the lock
    -if the flag is not held
        -hold the flag
        - set guard to zero
    -else
        -set guard to zero
        -queue the thread
        -park()

When unlocking
    -Test guard to see if other threads are modifing the lock
        -If the queue is empty, reset the flag
        -else get the next item in the queue and unpark it
    -set guard to 0
*/