#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
/*
The need for such look rises from the fact that many readers will not tamper with the Integrity of the data.
However, one writer is allowed at a time to access the critical section and modify the data.

So the first reader need to aquire the writing lock to prevent any future writing attempts.
The last reader needs to relase the writing lock to allow any future writing attempts

To build such lock, the following parts are needed
-Write mutex (used for writing operations)
-Number of readers to know when to relase the write mutex and when to aquire it
-A mutex used when the number of readers is accessed or modified

*NOTE
Readers might monopolize the lock and never give a chance for writers to take the writing lock.
More sophisticated solutions exists, but on the cost of simplicity.

*/
typedef struct rw_lock
{
    sem_t mutex;
    sem_t write_mutex;
    int readers;
} rw_lock_t;

void rw_lock_init(rw_lock_t *rw_lock)
{
    rw_lock->readers = 0;
    sem_init(&rw_lock->mutex, 0, 1);
    sem_init(&rw_lock->write_mutex, 0, 1);
};
void rw_lock_aquire_write_mutex(rw_lock_t *rw_lock)
{
    sem_wait(&rw_lock->write_mutex);
};
void rw_lock_relase_write_mutex(rw_lock_t *rw_lock)
{
    sem_post(&rw_lock->write_mutex);
}
void rw_read_aquire_read_lock(rw_lock_t *rw_lock)
{
    sem_wait(&rw_lock->mutex);
    rw_lock->readers++;
    if (rw_lock->readers == 1) // First reader aquires write mutex
    {
        rw_lock_aquire_write_mutex(&rw_lock->write_mutex);
    }
    sem_post(&rw_lock->mutex);
}
void rw_read_relase_read_lock(rw_lock_t *rw_lock)
{
    sem_wait(&rw_lock->mutex);
    rw_lock->readers--;
    if (rw_lock->readers == 0) // Last reader releases write mutex
    {
        rw_lock_relase_write_mutex(&rw_lock->write_mutex);
    }
    sem_post(&rw_lock->mutex);
}